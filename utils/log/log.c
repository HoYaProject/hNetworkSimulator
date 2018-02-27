/*******************************************************************************
 * @file		utils/log/log.c
 * @brief		Log module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.01.26	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
// System
#include "log.h"

/* APIs ----------------------------------------------------------------------*/
void Log_Print(const eLOG_LEVEL lv, const char* const fmt, ...) {
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

void Log_Dump(const int len, const char* const data) {
	int i;
	uint8_t	buf[17];

	printf("=== Dump ============================================================\n");
	for (i = 0; i < len; ++i) {
		if ((i % 16) == 0) {
			if (i != 0)
				printf("    %s\n", buf);
		}
		printf(" %02x", (uint8_t)data[i]);

		if ((data[i] < 0x20) || (0x7e < data[i]))
			buf[i % 16] = '.';
		else
			buf[i % 16] = data[i];
		buf[(i % 16) + 1] = '\0';
	}

	while ((i % 16) != 0) {
		printf("   ");
		++i;
	}
	printf("    %s\n", buf);
	printf("=====================================================================\n");
}
