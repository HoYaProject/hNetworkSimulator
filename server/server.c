/*******************************************************************************
 * @file		server/server.c
 * @brief		This is a TCP Server.
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
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	if (argc != 2)
		fprintf(stderr, "Usage: <Port>");

	in_port_t sport = atoi(argv[1]);

	int ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(sport);

	if (bind(ssock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
		fprintf(stderr, "Error: bind failed");
		return -1;
	}

	if (listen(ssock, 5) < 0) {
		fprintf(stderr, "Error: listen failed");
		return -1;
	}

	while (1) {
		struct sockaddr_in caddr;
		socklen_t caddr_len = sizeof(caddr);
		int csock = accept(ssock, (struct sockaddr*)&caddr, &caddr_len);

		char buf[] = "Hello World!\n";
		write(csock, buf, sizeof(buf));

		close(csock);
	}
	close(ssock);
}
