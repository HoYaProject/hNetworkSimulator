/*******************************************************************************
 * @file		platform/socket/socket.c
 * @brief		Socket module.
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
#include "socket.h"

/* Private Variables ---------------------------------------------------------*/
static tSOCKET	tSocket = { 0, 0, NULL };

static int	txsock;
static int	rxusock;
static int	rxbsock;

/* Private Function Prototypes -----------------------------------------------*/
static eSOCKET_STATUS SOCKET_CreateUDPSocket(int* const sock);
static eSOCKET_STATUS SOCKET_BindSocket(const int sock, const int addr);
static eSOCKET_STATUS SOCKET_SetSocketOption(const int sock, const int opt, int val);
static eSOCKET_STATUS SOCKET_Receive(const int sock);

/* APIs ----------------------------------------------------------------------*/
eSOCKET_STATUS SOCKET_Create(const int addr, rxhandler handler) {
	if (tSocket.uaddr != 0) {
		LOG_System("ERROR: Already created.");
		return eSOCKET_STATUS_FAIL_CREATE;
	}
	if (handler == NULL) {
		LOG_System("ERROR: No handler.");
		return eSOCKET_STATUS_FAIL_CREATE;
	}

	tSocket.uaddr = SOCKET_UNICAST_ADDR + addr;
	tSocket.baddr = SOCKET_BROADCAST_ADDR;
	tSocket.handler = handler;

	eSOCKET_STATUS	sta;
	if ((sta = SOCKET_CreateUDPSocket(&txsock)) < 0)
		return sta;
	if ((sta = SOCKET_CreateUDPSocket(&rxusock)) < 0)
		return sta;
	if ((sta = SOCKET_CreateUDPSocket(&rxbsock)) < 0)
		return sta;

	if ((sta = SOCKET_SetSocketOption(rxbsock, SO_REUSEADDR, 1)) < 0)
		return sta;

	if ((sta = SOCKET_BindSocket(rxusock, tSocket.uaddr)) < 0)
		return sta;
	if ((sta = SOCKET_BindSocket(rxbsock, tSocket.baddr)) < 0)
		return sta;

	return eSOCKET_STATUS_SUCCESS;
}

eSOCKET_STATUS SOCKET_Run(void) {
	if (tSocket.uaddr == 0) {
		return eSOCKET_STATUS_NO_ADDR;
	}

	SOCKET_Receive(rxusock);
	SOCKET_Receive(rxbsock);

	return eSOCKET_STATUS_SUCCESS;
}

eSOCKET_STATUS SOCKET_SendUnicast(const char* const dip, const int daddr, const int len, const void* const data) {
	eSOCKET_STATUS	sta;
	if ((sta = SOCKET_SetSocketOption(txsock, SO_BROADCAST, 0)) < 0)
		return sta;

	struct sockaddr_in	dst_addr;
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_addr.s_addr = inet_addr(dip);
	dst_addr.sin_port = htons(SOCKET_UNICAST_ADDR + daddr);
	if (sendto(txsock, data, len, 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr)) < 0) {
		perror("sendto()");
		return eSOCKET_STATUS_FAIL_SEND;
	}
	return eSOCKET_STATUS_SUCCESS;
}

eSOCKET_STATUS SOCKET_SendBroadcast(const int len, const void* const data) {
	eSOCKET_STATUS	sta;
	if ((sta = SOCKET_SetSocketOption(txsock, SO_BROADCAST, 1)) < 0)
		return sta;

	struct sockaddr_in	dst_addr;
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	dst_addr.sin_port = htons(tSocket.baddr);
	if (sendto(txsock, data, len, 0, (struct sockaddr*)&dst_addr, sizeof(dst_addr)) < 0) {
		perror("sendto()");
		return eSOCKET_STATUS_FAIL_SEND;
	}
	return eSOCKET_STATUS_SUCCESS;
}

eSOCKET_STATUS SOCKET_Destroy(void) {
	if (tSocket.uaddr != 0) {
		close(txsock);
		close(rxusock);
		close(rxbsock);
	}
	return eSOCKET_STATUS_SUCCESS;
}

eSOCKET_STATUS SOCKET_GetMyAddress(int* const addr) {
	if (tSocket.uaddr != 0) {
		*addr = tSocket.uaddr - SOCKET_UNICAST_ADDR;
		return eSOCKET_STATUS_SUCCESS;
	}
	return eSOCKET_STATUS_NO_ADDR;
}

/* Private Functions ---------------------------------------------------------*/
static eSOCKET_STATUS SOCKET_CreateUDPSocket(int* const sock) {
	*sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*sock < 0) {
		LOG_System("ERROR: Can't create tx socket.");
		perror("socket()");
		return eSOCKET_STATUS_FAIL_SOCKET;
	}
	return eSOCKET_STATUS_SUCCESS;
}

static eSOCKET_STATUS SOCKET_BindSocket(const int sock, const int addr) {
	struct sockaddr_in	rxaddr;
	memset(&rxaddr, 0, sizeof(rxaddr));
	rxaddr.sin_family = AF_INET;
	rxaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rxaddr.sin_port = htons(addr);
	if (bind(sock, (struct sockaddr*)&rxaddr, sizeof(rxaddr)) < 0) {
		perror("bind()");
		return eSOCKET_STATUS_FAIL_BIND;
	}

	return eSOCKET_STATUS_SUCCESS;
}

static eSOCKET_STATUS SOCKET_SetSocketOption(const int sock, const int opt, int val) {
	if (setsockopt(sock, SOL_SOCKET, opt, &val, sizeof(val)) < 0) {
		perror("setsockopt()");
		return eSOCKET_STATUS_FAIL_SOCKOPT;
	}
	return eSOCKET_STATUS_SUCCESS;
}

static eSOCKET_STATUS SOCKET_Receive(const int sock) {
	int maxfds = sock + 1;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(sock, &readfds);
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 10;
	int ret = select(maxfds, &readfds, NULL, NULL, &tv);
	if (ret < 0) {
		return eSOCKET_STATUS_FAIL_SELECT;
	}
	else if (ret == 0) {
		return eSOCKET_STATUS_TIMEOUT;
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
			tSocket.handler(&rxlen, rxbuf);
		}
	}

	return eSOCKET_STATUS_SUCCESS;
}
