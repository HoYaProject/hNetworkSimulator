/*******************************************************************************
 * @file		node/node.c
 * @brief		NODE module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.14	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
// System
#include "log.h"
#include "node.h"

/* Private Variables ---------------------------------------------------------*/
static tNODE	tNode = { 0, 0, NULL };

static int	txsock;
static int	rxusock;
static int	rxbsock;

/* Private Function Prototypes -----------------------------------------------*/
static eNODE_STATUS NODE_CreateUDPSocket(int* const sock);
static eNODE_STATUS NODE_BindSocket(const int sock, const int addr);
static eNODE_STATUS NODE_SetSocketOption(const int sock, const int opt, int val);
static eNODE_STATUS NODE_Receive(const int sock);

/* APIs ----------------------------------------------------------------------*/
eNODE_STATUS NODE_Create(const int addr, rxhandler handler) {
	if (tNode.uaddr != 0) {
		LOG_System("ERROR: Already created.");
		return eNODE_STATUS_FAIL_CREATE;
	}
	if (handler == NULL) {
		LOG_System("ERROR: No handler.");
		return eNODE_STATUS_FAIL_CREATE;
	}

	tNode.uaddr = NODE_UNICAST_ADDR + addr;
	tNode.baddr = NODE_BROADCAST_ADDR;
	tNode.handler = handler;

	eNODE_STATUS	sta;
	if ((sta = NODE_CreateUDPSocket(&txsock)) < 0)
		return sta;
	if ((sta = NODE_CreateUDPSocket(&rxusock)) < 0)
		return sta;
	if ((sta = NODE_CreateUDPSocket(&rxbsock)) < 0)
		return sta;

	if ((sta = NODE_SetSocketOption(rxbsock, SO_REUSEADDR, 1)) < 0)
		return sta;

	if ((sta = NODE_BindSocket(rxusock, tNode.uaddr)) < 0)
		return sta;
	if ((sta = NODE_BindSocket(rxbsock, tNode.baddr)) < 0)
		return sta;

	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_Run(void) {
	if (tNode.uaddr == 0) {
		return eNODE_STATUS_NO_ADDR;
	}

	NODE_Receive(rxusock);
	NODE_Receive(rxbsock);

	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_SendUnicast(const char* const dip, const int daddr, const int len, const void* const data) {
	eNODE_STATUS	sta;
	if ((sta = NODE_SetSocketOption(txsock, SO_BROADCAST, 0)) < 0)
		return sta;

	struct sockaddr_in	dst_addr;
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_addr.s_addr = inet_addr(dip);
	dst_addr.sin_port = htons(NODE_UNICAST_ADDR + daddr);
	if (sendto(txsock, data, len, 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr)) < 0) {
		perror("sendto()");
		return eNODE_STATUS_FAIL_SEND;
	}
	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_SendBroadcast(const int len, const void* const data) {
	eNODE_STATUS	sta;
	if ((sta = NODE_SetSocketOption(txsock, SO_BROADCAST, 1)) < 0)
		return sta;

	struct sockaddr_in	dst_addr;
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	dst_addr.sin_port = htons(tNode.baddr);
	if (sendto(txsock, data, len, 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr)) < 0) {
		perror("sendto()");
		return eNODE_STATUS_FAIL_SEND;
	}
	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_Destroy(void) {
	if (tNode.uaddr != 0) {
		close(txsock);
		close(rxusock);
		close(rxbsock);
	}
	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_GetMyAddress(int* const addr) {
	if (tNode.uaddr != 0) {
		*addr = tNode.uaddr - NODE_UNICAST_ADDR;
		return eNODE_STATUS_SUCCESS;
	}
	return eNODE_STATUS_NO_ADDR;
}

/* Private Functions ---------------------------------------------------------*/
static eNODE_STATUS NODE_CreateUDPSocket(int* const sock) {
	*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*sock < 0) {
		LOG_System("ERROR: Can't create tx socket.");
		perror("socket()");
		return eNODE_STATUS_FAIL_SOCKET;
	}
	return eNODE_STATUS_SUCCESS;
}

static eNODE_STATUS NODE_BindSocket(const int sock, const int addr) {
	struct sockaddr_in	rxaddr;
	memset(&rxaddr, 0, sizeof(rxaddr));
	rxaddr.sin_family = AF_INET;
	rxaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rxaddr.sin_port = htons(addr);
	if (bind(sock, (struct sockaddr*)&rxaddr, sizeof(rxaddr)) < 0) {
		perror("bind()");
		return eNODE_STATUS_FAIL_BIND;
	}

	return eNODE_STATUS_SUCCESS;
}

static eNODE_STATUS NODE_SetSocketOption(const int sock, const int opt, int val) {
	if (setsockopt(sock, SOL_SOCKET, opt, &val, sizeof(val)) < 0) {
		perror("setsockopt()");
		return eNODE_STATUS_FAIL_SOCKOPT;
	}
	return eNODE_STATUS_SUCCESS;
}

static eNODE_STATUS NODE_Receive(const int sock) {
	int maxfds = sock + 1;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	int ret = select(maxfds, &readfds, NULL, NULL, &tv);
	if (ret < 0) {
		return eNODE_STATUS_FAIL_SELECT;
	}
	else if (ret == 0) {
		return eNODE_STATUS_TIMEOUT;
	}

	if (FD_ISSET(sock, &readfds)) {
		struct sockaddr_in	caddr;
		socklen_t caddrlen = sizeof(caddr);
		char rxbuf[256];
		int rxlen = recvfrom(sock, rxbuf, 256, 0, (struct sockaddr*)&caddr, &caddrlen);
		if (rxlen < 0) {
			perror("recvfrom()");
		}
		else {
			rxbuf[rxlen++] = '\0';
			puts(rxbuf);
			tNode.handler(&rxlen, rxbuf);
		}
	}

	return eNODE_STATUS_SUCCESS;
}
