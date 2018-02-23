/*******************************************************************************
 * @file		main.c
 * @brief		This is a main of network simulator.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
 * @version	v1.0
 * @note
 *	- 2018.02.07	Created.
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

/* Private Function Prototypes -----------------------------------------------*/
static void CMD_Help(int argc, char* argv[]);
static void CMD_Start(int argc, char* argv[]);
static void CMD_SendUnicast(int argc, char* argv[]);
static void CMD_SendBroadcast(int argc, char* argv[]);
static void CMD_Exit(int argc, char* argv[]);

/* Private Variables ---------------------------------------------------------*/
tCOMMAND	tCmd[] = {
	{"help", "        Display menu", CMD_Help},
	{"start", "       Start a node\r\n             - <addr (uint8)>", CMD_Start},
	{"Uni", "         Send data on unicast\r\n             - <addr (uint8), data>", CMD_SendUnicast},
	{"Broad", "       Send data on broadcast\r\n             - <data>", CMD_SendBroadcast},
	{"exit", "        Exit program", CMD_Exit},
	{NULL, NULL, NULL}
};

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	CLI_Init();
	CLI_AddCommand(tCmd);
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
		LOG_User("Fail: Wrong argument.\n");
		return;
	}

	int addr = atoi(argv[0]);
	if (NODE_Create(addr) < 0) {
		LOG_User("Fail: Create a node.\n");
		return;
	}
	LOG_User("Success: Create a node.\n");
}

static void CMD_SendUnicast(int argc, char* argv[]) {
	if (argc != 2) {
		LOG_User("Fail: Wrong argument.\n");
		return ;
	}

	char ip[] = "127.0.0.1";
	int addr = atoi(argv[0]);
	if (NODE_SendUnicast(ip, addr, strlen(argv[1]), argv[1]) < 0) {
		LOG_User("Fail: Send unicast.\n");
	}
}

static void CMD_SendBroadcast(int argc, char* argv[]) {
	if (argc != 1) {
		LOG_User("Fail: Wrong argument.\n");
		return ;
	}

	if (NODE_SendBroadcast(strlen(argv[0]), argv[0]) < 0) {
		LOG_User("Fail: Send broadcast.\n");
	}
}

static void CMD_Exit(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	if (NODE_Destroy() < 0) {
		LOG_User("Fail: Close a node.\n");
	}

	LOG_User("Exit program.\n");
	exit(0);
}
