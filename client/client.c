/*******************************************************************************
 * @file		client/client.c
 * @brief		This is a TCP client.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
 * @version	v1.0
 * @note
 *	-2018.01.25	Created.
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

/* Definitions ---------------------------------------------------------------*/
#define BUFSIZE		256

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	if (argc != 3)
		fprintf(stderr, "Usage: <Server Address> <Port>");

	char* sip = argv[1];
	in_port_t sport = atoi(argv[2]);

	int csock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(sip);
	saddr.sin_port = htons(sport);

	if (connect(csock, (struct sockaddr*)&saddr, sizeof(saddr)) == -1) {
		printf("Can't connect\n");
		close(csock);
		return -1;
	}

	char txbuf[BUFSIZE];
	scanf("%s", txbuf);
	txbuf[strlen(txbuf)] = '\0';
	write(csock, txbuf, strlen(txbuf) + 1);

	int read_len;
	char rxbuf[BUFSIZE];
	if ((read_len = read(csock, rxbuf, sizeof(rxbuf))) < 0) {
		return -1;
	}

	rxbuf[read_len] = '\0';
	printf("Received data: %s\n", rxbuf);

	close(csock);
}
