/*******************************************************************************
 * @file		utils/log/log.c
 * @brief		Display log.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
 * @version	v1.0
 * @note
 *	- 2018.01.26	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdio.h>
#include <stdarg.h>
// System
#include "log.h"

/* APIs ----------------------------------------------------------------------*/
void Log_Print(const eLOG_LEVEL lv, const char* fmt, ...) {
	va_list	args;
	int	len = 0;
	char	str[256];
	char*	pstr = str;

	va_start(args, fmt);

	switch (lv) {
		case eLOG_LEVEL_NONE:
			break;

		case eLOG_LEVEL_USER:
			len += sprintf(str + len, "[USR] ");
			break;

		case eLOG_LEVEL_SYSTEM:
			len += sprintf(str + len, "[STM] ");
			break;

		case eLOG_LEVEL_DEBUG:
			len += sprintf(str + len, "[DBG] ");
			break;

		default:
			break;
	}

	vsprintf(str + len, fmt, args);
	va_end(args);

	puts(pstr);
}
