/*******************************************************************************
 * @file		utils/cli/cli.h
 * @brief		Command line interface module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.07	Created.
 ******************************************************************************/

#ifndef __HoYa_COMMAND_LINE_INTERFACE_H__
#define __HoYa_COMMAND_LINE_INTERFACE_H__

/* Structures ----------------------------------------------------------------*/
typedef struct _tCLI_COMMANDS {
	char*	cmd;
	char*	desc;

	void (*pfn)(int, char**);
} tCLI_COMMAND, *ptCLI_COMMAND;

/* API Prototypes ------------------------------------------------------------*/
void CLI_Init(const ptCLI_COMMAND cmd);
void CLI_DisplayMenu(void);
void CLI_GetCommand(void);

#endif
