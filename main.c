/*******************************************************************************
 * @file		main.c
 * @brief		This is a main of network simulator.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.07	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// System - Util
#include "hDef.h"
#include "log.h"
#include "cli.h"
#include "ll.h"
// System - Platform
#include "timer.h"
#include "socket.h"
// System - Stack
#include "api.h"

/* Private Function Prototypes -----------------------------------------------*/
// Command
static void CMD_Help(int argc, char* argv[]);
static void CMD_Start(int argc, char* argv[]);
static void CMD_SendRequest(int argc, char* argv[]);
static void CMD_SendNotification(int argc, char* argv[]);
static void CMD_Exit(int argc, char* argv[]);

/* Private Variables ---------------------------------------------------------*/
tCLI_COMMAND	tCmd[] = {
	{"Help", "        Display menu", CMD_Help},
	{"Start", "       Start a node\r\n             - <addr (uint8)>", CMD_Start},
	{"Req", "         Send request\r\n             - <addr (uint8)>", CMD_SendRequest},
	{"Ntf", "         Send notification", CMD_SendNotification},
	{"Exit", "        Exit program", CMD_Exit},
	{NULL, NULL, NULL}
};

/* Main Function -------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	TIMER_CreateSystemTimer(1);

	CLI_Init(tCmd);
	CLI_DisplayMenu();

	while (1) {
		CLI_GetCommand();
		SOCKET_Run();

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
	if (SOCKET_Create(addr, API_ReceivePacket) < 0) {
		LOG_User("Fail: Create a socket interface.");
		return ;
	}
	if (API_Init(addr) < 0) {
		LOG_User("Fail: Init stack.");
		return ;
	}

	LOG_User("Success: Create a node.");
}

static void CMD_SendRequest(int argc, char* argv[]) {
	if (argc != 1) {
		LOG_User("Fail: Wrong argument.");
		return ;
	}

	API_MakeReq(atoi(argv[0]));
	API_SendPacket();

	LOG_User("Success: Send a request.");
}

static void CMD_SendNotification(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	API_MakeNtf();
	API_SendPacket();

	LOG_User("Success: Send a notification.");
}

static void CMD_Exit(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	if (SOCKET_Destroy() < 0) {
		LOG_User("Fail: Close a node.");
	}

	LOG_User("Exit program.");
	exit(0);
}
