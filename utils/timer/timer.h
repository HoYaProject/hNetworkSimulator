/*******************************************************************************
 * @file		utils/timer/timer.h
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

/* Enumerations --------------------------------------------------------------*/
typedef enum _eTIMER_STATUS {
	eTIMER_STATUS_SUCCESS			= 0,
	eTIMER_STATUS_FAIL_CREATE	= -1,
} eTIMER_STATUS;

/* API Prototypes ------------------------------------------------------------*/
eTIMER_STATUS Timer_Create1(const int msec, void (*pfn)(int));
eTIMER_STATUS Timer_Create2(timer_t* tid, int msec, void (*pfn)(int, siginfo_t*, void*));

#endif
