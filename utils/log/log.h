/*******************************************************************************
 * @file		utils/log/log.h
 * @brief		Log module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.01.25	Created.
 ******************************************************************************/

#ifndef __HoYa_LOG_H__
#define __HoYa_LOG_H__

/* Include Headers -----------------------------------------------------------*/
#include "log_conf.h"

/* Definitions ---------------------------------------------------------------*/
#define LOG_LEVEL_NONE		0x00
#define LOG_LEVEL_USER		0x01
#define LOG_LEVEL_SYSTEM	0x02
#define LOG_LEVEL_DEBUG		0x03

/* Enumerations --------------------------------------------------------------*/
typedef enum _eLOG_LEVEL {
	eLOG_LEVEL_NONE		=	LOG_LEVEL_NONE,
	eLOG_LEVEL_USER		= LOG_LEVEL_USER,
	eLOG_LEVEL_SYSTEM	= LOG_LEVEL_SYSTEM,
	eLOG_LEVEL_DEBUG	= LOG_LEVEL_DEBUG,
} eLOG_LEVEL;

/* Macros --------------------------------------------------------------------*/
#define LOG(fmt, ...)						Log_Print(eLOG_LEVEL_NONE, fmt, ##__VA_ARGS__)

#if _CONFIG_LOG_LEVEL >= LOG_LEVEL_USER
	#define	LOG_User(fmt, ...)		Log_Print(eLOG_LEVEL_USER, fmt, ##__VA_ARGS__)
#else
	#define LOG_User(fmt, ...)
#endif

#if _CONFIG_LOG_LEVEL >= LOG_LEVEL_SYSTEM
	#define	LOG_System(fmt, ...)	Log_Print(eLOG_LEVEL_SYSTEM, fmt, ##__VA_ARGS__)
#else
	#define LOG_System(fmt, ...)
#endif

#if _CONFIG_LOG_LEVEL >= LOG_LEVEL_DEBUG
	#define	LOG_Debug(fmt, ...)		Log_Print(eLOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
	#define LOG_Debug(fmt, ...)
#endif

#define LOG_Dump(len, data)			Log_Dump((len), (data))

/* API Prototypes ------------------------------------------------------------*/
void Log_Print(const eLOG_LEVEL lv, const char* const fmt, ...);
void Log_Dump(const int len, const char* const data);

#endif
