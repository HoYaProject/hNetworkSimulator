/*******************************************************************************
 * @file		server/server.c
 * @brief		This is a multiplexing TCP Server.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
 * @version	v1.0
 * @note
 *	- 2018.01.25	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
// System
#include "server_conf.h"
#include "socket_def.h"

/* Private Variables ---------------------------------------------------------*/
static int	clist[MAX_CLIENT];

/* Private Function Prototypes -----------------------------------------------*/
static int pushClient(int csock);
static int popClient(int csock);

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Error: Wrong arguments\r\n");
		printf("Usage: %s <Port>\r\n", argv[0]);
		return -1;
	}

	int ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(atoi(argv[1]));

	if (bind(ssock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
		perror("bind()");
		return -2;
	}

	if (listen(ssock, MAX_CLIENT) == -1) {
		perror("listen()");
		return -3;
	}

	for (int i = 0; i < MAX_CLIENT; i++) {
		clist[i] = INVALID_SOCKET;
	}

	while (1) {
		int max = ssock;
		fd_set	readfds;
		FD_ZERO(&readfds);
		FD_SET(ssock, &readfds);
		for (int i = 0; i < MAX_CLIENT; i++) {
			if (clist[i] != INVALID_SOCKET) {
				FD_SET(clist[i], &readfds);
				if (clist[i] > max)
					max = clist[i];
			}
		}
		++max;

		if (select(max, &readfds, NULL, NULL, NULL) < 0) {
			perror("select()");
			return -4;
		}

		if (FD_ISSET(ssock, &readfds)) {
			int csock;
			struct sockaddr_in caddr;
			socklen_t calen = sizeof(caddr);
			if ((csock = accept(ssock, (struct sockaddr*)&caddr, &calen)) > 0) {
					int rsp = pushClient(csock);
					if (rsp < 0) {
						char txbuf[] = "No more client\r\n";
						write(csock, txbuf, strlen(txbuf));
						close(csock);
					}
					else {
						char txbuf[] = "Welcome";
						write(csock, txbuf, strlen(txbuf));
					}
			}
		}

		for (int i = 0; i < MAX_CLIENT; i++) {
			if ((clist[i] != INVALID_SOCKET) && FD_ISSET(clist[i], &readfds)) {
				char rxbuf[BUF_SIZE];
				int rxlen;
				memset(rxbuf, 0, sizeof(rxbuf));
				if ((rxlen = read(clist[i], rxbuf, sizeof(rxbuf))) > 0) {
					for (int j = 0; j < MAX_CLIENT; j++) {
						if (clist[j] != INVALID_SOCKET)
							write(clist[j], rxbuf, rxlen);
					}
					if (strstr(rxbuf, "exit") != NULL) {
						popClient(clist[i]);
						break;
					}
				}
			}
		}
	}
}

static int pushClient(int csock) {
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (clist[i] == INVALID_SOCKET) {
			clist[i] = csock;
			return i;
		}
	}
	return -1;
}

static int popClient(int csock) {
	close(csock);
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (csock == clist[i]) {
			clist[i] = INVALID_SOCKET;
			break;
		}
	}
	return 0;
}
