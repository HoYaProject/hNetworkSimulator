/*******************************************************************************
 * @file		client/client.c
 * @brief		This is a UDP client.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
 * @version	v1.0
 * @note
 *	- 2018.01.25	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard Header
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "socket_def.h"

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	if (argc != 3)
		fprintf(stderr, "Usage: %s <Server Address> <Port>\r\n", argv[0]);

	int csock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(argv[1]);
	saddr.sin_port = htons(atoi(argv[2]));

	char nick[20];
	printf("Input nickname: ");
	scanf("%s", nick);

	if (connect(csock, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
		perror("connect()");
		return -1;
	}

	int maxfd = csock + 1;
	while (1) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(STDIN_FILENO, &readfds);
		FD_SET(csock, &readfds);

		if (select(maxfd, &readfds, NULL, NULL, NULL) < 0) {
			perror("select()");
			return -2;
		}

		char rxbuf[BUF_SIZE];
		int rxlen;
		memset(rxbuf, 0, sizeof(rxbuf));

		if (FD_ISSET(csock, &readfds)) {
			memset(rxbuf, 0, sizeof(rxbuf));
			if ((rxlen = read(csock, rxbuf, sizeof(rxbuf))) > 0) {
				write(STDOUT_FILENO, rxbuf, rxlen);
			}
		}

		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			char txbuf[BUF_SIZE];
			memset(rxbuf, 0, sizeof(rxbuf));
			if ((rxlen = read(STDIN_FILENO, rxbuf, sizeof(rxbuf))) > 0) {
				sprintf(txbuf, "[%s] %s", nick, rxbuf);
				write(csock, txbuf, strlen(txbuf));

				char buf[] = "exit";
				if (!strncmp(rxbuf, buf, strlen(buf))) {
					break;
				}
			}
		}
	}
	close(csock);
}
