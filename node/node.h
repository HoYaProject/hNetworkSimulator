/*******************************************************************************
 * @file		node/node.h
 * @brief		Node module.
 * @author	llHoYall <hoya128@gmail.com>
 *******************************************************************************
 * @version	v1.0
 * @note
 *	- 2018.02.14	Created.
 ******************************************************************************/

#ifndef __HoYa_NODE_H__
#define __HoYa_NODE_H__

/* Enumerations --------------------------------------------------------------*/
typedef enum _eNODE_STATUS {
	eNODE_STATUS_SUCCESS			= 0,
	eNODE_STATUS_FAIL_CREATE	= -1,
	eNODE_STATUS_FAIL_SOCKET	= -2,
	eNODE_STATUS_FAIL_BIND		= -3,
} eNODE_STATUS;

/* Structures ----------------------------------------------------------------*/
typedef struct _tNODE {
	int	addr;
} tNODE;

/* API Prototypes ------------------------------------------------------------*/
eNODE_STATUS Node_Create(int addr);
eNODE_STATUS Node_Destroy(void);

#endif
