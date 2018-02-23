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
// System
#include "log.h"
#include "node.h"

/* Private Variables ---------------------------------------------------------*/
static tNODE	tNode = { 0, };

static int	txsock;
static int	rxusock;
static int	rxbsock;

static fd_set	ureadfds;
static int		umaxfds;
static fd_set	breadfds;
static int		bmaxfds;

/* Private Function Prototypes -----------------------------------------------*/
static eNODE_STATUS NODE_RecvUnicast(void);
static eNODE_STATUS NODE_RecvBroadcast(void);

/* APIs ----------------------------------------------------------------------*/
eNODE_STATUS NODE_Create(int addr) {
	if (tNode.uaddr != 0) {
		LOG_System("ERROR: Already created.\r\n");
		return eNODE_STATUS_FAIL_CREATE;
	}
	tNode.uaddr = NODE_UNICAST_ADDR + addr;
	tNode.baddr = NODE_BROADCAST_ADDR;

	txsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (txsock < 0) {
		LOG_System("ERROR: Can't create tx socket.\r\n");
		perror("socket()");
		return eNODE_STATUS_FAIL_SOCKET;
	}

	rxusock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (rxusock < 0) {
		LOG_System("ERROR: Can't create rx socket.\r\n");
		perror("socket()");
		return eNODE_STATUS_FAIL_SOCKET;
	}

	struct sockaddr_in	rxuaddr;
	memset(&rxuaddr, 0, sizeof(rxuaddr));
	rxuaddr.sin_family = AF_INET;
	rxuaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rxuaddr.sin_port = htons(tNode.uaddr);
	if (bind(rxusock, (struct sockaddr*)&rxuaddr, sizeof(rxuaddr)) < 0) {
		perror("bind()");
		return eNODE_STATUS_FAIL_BIND;
	}

	rxbsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (rxbsock < 0) {
		LOG_System("ERROR: Can't create rx socket.\r\n");
		perror("socket()");
		return eNODE_STATUS_FAIL_SOCKET;
	}

	if (setsockopt(rxbsock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
		perror("setsockopt()");
		return eNODE_STATUS_FAIL_SOCKOPT;
	}

	struct sockaddr_in	rxbaddr;
	memset(&rxbaddr, 0, sizeof(rxbaddr));
	rxbaddr.sin_family = AF_INET;
	rxbaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rxbaddr.sin_port = htons(NODE_BROADCAST_ADDR);
	if (bind(rxbsock, (struct sockaddr*)&rxbaddr, sizeof(rxbaddr)) < 0) {
		perror("bind()");
		return eNODE_STATUS_FAIL_BIND;
	}

	umaxfds = rxusock + 1;
	FD_ZERO(&ureadfds);
	FD_SET(rxusock, &ureadfds);
	bmaxfds = rxbsock + 1;
	FD_ZERO(&breadfds);
	FD_SET(rxbsock, &breadfds);

	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_Run(void) {
	NODE_RecvUnicast();
	NODE_RecvBroadcast();

	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_SendUnicast(char* dip, int daddr, int len, char* data) {
	if (setsockopt(txsock, SOL_SOCKET, SO_BROADCAST, &(int){0}, sizeof(int)) < 0) {
		perror("setsockopt()");
		return eNODE_STATUS_FAIL_SOCKOPT;
	}

	struct sockaddr_in	saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(dip);
	saddr.sin_port = htons(NODE_UNICAST_ADDR + daddr);
	if (sendto(txsock, data, len, 0, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
		perror("sendto()");
		return eNODE_STATUS_FAIL_SEND;
	}
	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS NODE_SendBroadcast(int len, char* data) {
	if (setsockopt(txsock, SOL_SOCKET, SO_BROADCAST, &(int){1}, sizeof(int)) < 0) {
		perror("setsockopt()");
		return eNODE_STATUS_FAIL_SOCKOPT;
	}

	struct sockaddr_in	saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	saddr.sin_port = htons(tNode.baddr);
	if (sendto(txsock, data, len, 0, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
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
		return eNODE_STATUS_SUCCESS;
	}
	return eNODE_STATUS_FAIL_CLOSE;
}

/* Private Functions ---------------------------------------------------------*/
static eNODE_STATUS NODE_RecvUnicast(void) {
	if (tNode.uaddr == 0) {
		return eNODE_STATUS_NO_ADDR;
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	fd_set	fds = ureadfds;
	FD_SET(rxusock, &fds);
	int ret = select(umaxfds, &fds, NULL, NULL, &tv);
	if (ret < 0) {
		perror("select()");
		return eNODE_STATUS_FAIL_SELECT;
	}
	else if (ret == 0) {
		return eNODE_STATUS_TIMEOUT;
	}

	if (FD_ISSET(rxusock, &fds)) {
		struct sockaddr_in	caddr;
		socklen_t caddrlen = sizeof(caddr);
		char rxbuf[256];
		int rxlen = recvfrom(rxusock, rxbuf, 256, 0, (struct sockaddr*)&caddr, &caddrlen);
		if (rxlen < 0) {
			perror("recvfrom()");
		}
		else {
			rxbuf[rxlen++] = '\0';
			puts(rxbuf);
		}
	}

	return eNODE_STATUS_SUCCESS;
}

static eNODE_STATUS NODE_RecvBroadcast(void) {
	if (tNode.baddr == 0) {
		return eNODE_STATUS_NO_ADDR;
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	fd_set	fds = breadfds;
	FD_SET(rxusock, &fds);
	int ret = select(bmaxfds, &fds, NULL, NULL, &tv);
	if (ret < 0) {
		perror("select()");
		return eNODE_STATUS_FAIL_SELECT;
	}
	else if (ret == 0) {
		return eNODE_STATUS_TIMEOUT;
	}

	if (FD_ISSET(rxbsock, &fds)) {
		struct sockaddr_in	caddr;
		socklen_t caddrlen = sizeof(caddr);
		char rxbuf[256];
		int rxlen = recvfrom(rxbsock, rxbuf, 256, 0, (struct sockaddr*)&caddr, &caddrlen);
		if (rxlen < 0) {
			perror("recvfrom()");
		}
		else {
			rxbuf[rxlen++] = '\0';
			puts(rxbuf);
		}
	}

	return eNODE_STATUS_SUCCESS;
}
