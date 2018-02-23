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
typedef struct _tCOMMANDS {
	char*	cmd;
	char*	desc;

	void (*pfn)(int, char**);
} tCOMMAND, *ptCOMMAND;

/* API Prototypes ------------------------------------------------------------*/
void CLI_Init(void);
void CLI_DisplayMenu(void);
void CLI_GetCommand(void);
void CLI_AddCommand(ptCOMMAND cmd);

#endif
