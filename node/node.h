/*******************************************************************************
 * @file		node/node.h
 * @brief		NODE module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.14	Created.
 ******************************************************************************/

#ifndef __HoYa_NODE_H__
#define __HoYa_NODE_H__

/* Definitions ---------------------------------------------------------------*/
#define NODE_UNICAST_ADDR			7000
#define NODE_BROADCAST_ADDR		8000

/* Definitions ---------------------------------------------------------------*/
typedef void (*rxhandler)(const int* const, const void* const);

/* Enumerations --------------------------------------------------------------*/
typedef enum _eNODE_STATUS {
	eNODE_STATUS_SUCCESS			= 0,
	eNODE_STATUS_FAIL_CREATE	= -1,
	eNODE_STATUS_FAIL_SOCKET	= -2,
	eNODE_STATUS_FAIL_SOCKOPT	= -3,
	eNODE_STATUS_FAIL_BIND		= -4,
	eNODE_STATUS_NO_ADDR			= -5,
	eNODE_STATUS_FAIL_SELECT	= -6,
	eNODE_STATUS_TIMEOUT			= -7,
	eNODE_STATUS_FAIL_SEND		= -8,
	eNODE_STATUS_FAIL_CLOSE		= -9,
} eNODE_STATUS;

/* Structures ----------------------------------------------------------------*/
typedef struct _tNODE {
	int				uaddr;
	int				baddr;
	rxhandler	handler;
} tNODE;

/* API Prototypes ------------------------------------------------------------*/
eNODE_STATUS NODE_Create(const int addr, rxhandler handler);
eNODE_STATUS NODE_Run(void);
eNODE_STATUS NODE_SendUnicast(const char* const dip, const int daddr, const int len, const void* const data);
eNODE_STATUS NODE_SendBroadcast(const int len, const void* const data);
eNODE_STATUS NODE_Destroy(void);
eNODE_STATUS NODE_GetMyAddress(int* const addr);

#endif
