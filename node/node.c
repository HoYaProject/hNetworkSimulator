/*******************************************************************************
 * @file		node/node.c
 * @brief		Node module.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
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
// System
#include "log.h"
#include "node.h"

/* Private Variables ---------------------------------------------------------*/
static tNODE	tNode = { 0, };
static int	txsock;
static int	rxsock;

/* APIs ----------------------------------------------------------------------*/
eNODE_STATUS Node_Create(int addr) {
	if (tNode.addr != 0) {
		LOG_System("ERROR: Already created.\r\n");
		return eNODE_STATUS_FAIL_CREATE;
	}
	tNode.addr = addr;

	txsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (txsock < 0) {
		LOG_System("ERROR: Can't create tx socket.\r\n");
		perror("socket()");
		return eNODE_STATUS_FAIL_SOCKET;
	}

	rxsock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (rxsock < 0) {
		LOG_System("ERROR: Can't create rx socket.\r\n");
		perror("socket()");
		return eNODE_STATUS_FAIL_SOCKET;
	}

	struct sockaddr_in	rxaddr;
	memset(&rxaddr, 0, sizeof(rxaddr));
	rxaddr.sin_family = AF_INET;
	rxaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	rxaddr.sin_port = htons(tNode.addr);

	if (bind(rxsock, (struct sockaddr*)&rxaddr, sizeof(rxaddr)) < 0) {
		perror("bind()");
		return eNODE_STATUS_FAIL_BIND;
	}

	return eNODE_STATUS_SUCCESS;
}

eNODE_STATUS Node_Destroy(void) {
	close(txsock);
	close(rxsock);
	return eNODE_STATUS_SUCCESS;
}
