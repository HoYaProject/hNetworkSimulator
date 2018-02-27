/*******************************************************************************
 * @file		main.c
 * @brief		This is a main of network simulator.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.07	Created.
 *******************************************************************************
 * @TODO
 *	- TIMER: Refactoring.
 *	- CLI: Remove add command function.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// System
#include "hDef.h"
#include "log.h"
#include "cli.h"
#include "node.h"
#include "packet.h"

/* Private Function Prototypes -----------------------------------------------*/
static void CMD_Help(int argc, char* argv[]);
static void CMD_Start(int argc, char* argv[]);
static void CMD_SendRequest(int argc, char* argv[]);
static void CMD_SendNotification(int argc, char* argv[]);
static void CMD_Exit(int argc, char* argv[]);

static void Receive(const int* const len, const void* const data);

/* Private Variables ---------------------------------------------------------*/
tCOMMAND	tCmd[] = {
	{"Help", "        Display menu", CMD_Help},
	{"Start", "       Start a node\r\n             - <addr (uint8)>", CMD_Start},
	{"Req", "         Send request\r\n             - <addr (uint8), data>", CMD_SendRequest},
	{"Ntf", "         Send notification\r\n             - <data>", CMD_SendNotification},
	{"Exit", "        Exit program", CMD_Exit},
	{NULL, NULL, NULL}
};

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	CLI_Init(tCmd);
	CLI_DisplayMenu();

	while (1) {
		CLI_GetCommand();
		NODE_Run();

		usleep(100);
	}

	return 0;
}

/* Private Functions ---------------------------------------------------------*/
static void CMD_Help(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	CLI_DisplayMenu();
}

static void CMD_Start(int argc, char* argv[]) {
	if (argc != 1) {
		LOG_User("Fail: Wrong argument.");
		return;
	}

	int addr = atoi(argv[0]);
	if (NODE_Create(addr, Receive) < 0) {
		LOG_User("Fail: Create a node.");
		return;
	}
	LOG_User("Success: Create a node.");
}

static void CMD_SendRequest(int argc, char* argv[]) {
	if (argc != 2) {
		LOG_User("Fail: Wrong argument.");
		return ;
	}

	int src;
	if (NODE_GetMyAddress(&src) < 0) {
		LOG_User("Fail: Create a node first.");
		return;
	}
	tPACKET	tpkt;
	PKT_MakeReq(src, atoi(argv[0]), strlen(argv[1]), (uint8_t*)&argv[1][0], &tpkt);

	char ip[] = "127.0.0.1";
	int addr = tpkt.dst;
	if (NODE_SendUnicast(ip, addr, sizeof(tpkt), (char*)&tpkt) < 0) {
		LOG_User("Fail: Send unicast.");
	}
}

static void CMD_SendNotification(int argc, char* argv[]) {
	if (argc != 1) {
		LOG_User("Fail: Wrong argument.");
		return ;
	}

	int src;
	if (NODE_GetMyAddress(&src) < 0) {
		LOG_User("Fail: Create a node first.");
		return ;
	}
	tPACKET tpkt;
	PKT_MakeNtf(src, strlen(argv[0]), (uint8_t*)&argv[0][0], &tpkt);

	if (NODE_SendBroadcast(sizeof(tpkt), (char*)&tpkt) < 0) {
		LOG_User("Fail: Send broadcast.");
	}
}

static void CMD_Exit(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	if (NODE_Destroy() < 0) {
		LOG_User("Fail: Close a node.");
	}

	LOG_User("Exit program.");
	exit(0);
}

static void Receive(const int* const len, const void* const data) {
	LOG_Dump(*len, (char*)data);
}
