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
#include <unistd.h>
// System
#include "hDef.h"
#include "cli.h"

/* Private Function Prototypes -----------------------------------------------*/
static void CMD_Help(int argc, char* argv[]);
static void CMD_Exit(int argc, char* argv[]);

/* Private Variables ---------------------------------------------------------*/
tCOMMAND	tCmd[] = {
	{"help", "        Display menu", CMD_Help},
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

static void CMD_Exit(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	exit(0);
}
