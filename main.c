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
static void CMD_Exit(int argc, char* argv[]);

/* Private Variables ---------------------------------------------------------*/
tCOMMAND	tCmd[] = {
	{"help", "        Display menu", CMD_Help},
	{"start", "       Start a node\r\n             <addr (uint8)>", CMD_Start},
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
		LOG_Debug("ERROR: This command has only 1 argument.\r\n");
		return;
	}

	int val = 7000 + atoi(argv[0]);
	if (Node_Create(val) < 0) {
		LOG_User("Failed to create a node\r\n");
		return;
	}
}

static void CMD_Exit(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	Node_Destroy();

	exit(0);
}
