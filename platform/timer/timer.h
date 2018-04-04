/*******************************************************************************
 * @file		platform/timer/timer.h
 * @brief		Timer module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.20	Created.
 ******************************************************************************/

#ifndef __HoYa_TIMER_H__
#define __HoYa_TIMER_H__

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <signal.h>
#include <time.h>

/* Definitions ---------------------------------------------------------------*/
// Handler
typedef void (*tmhandler)(int, siginfo_t*, void*);

/* Enumerations --------------------------------------------------------------*/
typedef enum _eTIMER_STATUS {
	eTIMER_STATUS_SUCCESS			= 0,
	eTIMER_STATUS_FAIL_CREATE	= -1,
} eTIMER_STATUS;

/* API Prototypes ------------------------------------------------------------*/
eTIMER_STATUS TIMER_CreateSystemTimer(const int msec);
eTIMER_STATUS TIMER_GetTime(int* const time);

eTIMER_STATUS TIMER_CreateUserTimer(timer_t* tid, const int msec, tmhandler handler);

#endif
