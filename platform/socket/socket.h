/*******************************************************************************
 * @file		platform/socket/socket.h
 * @brief		SOCKET module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.14	Created.
 ******************************************************************************/

#ifndef __HoYa_SOCKET_H__
#define __HoYa_SOCKET_H__

/* Definitions ---------------------------------------------------------------*/
#define SOCKET_UNICAST_ADDR			7000
#define SOCKET_BROADCAST_ADDR		8000

/* Definitions ---------------------------------------------------------------*/
typedef void (*rxhandler)(const int* const, const void* const);

/* Enumerations --------------------------------------------------------------*/
typedef enum _eSOCKET_STATUS {
	eSOCKET_STATUS_SUCCESS			= 0,
	eSOCKET_STATUS_FAIL_CREATE	= -1,
	eSOCKET_STATUS_FAIL_SOCKET	= -2,
	eSOCKET_STATUS_FAIL_SOCKOPT	= -3,
	eSOCKET_STATUS_FAIL_BIND		= -4,
	eSOCKET_STATUS_NO_ADDR			= -5,
	eSOCKET_STATUS_FAIL_SELECT	= -6,
	eSOCKET_STATUS_TIMEOUT			= -7,
	eSOCKET_STATUS_FAIL_SEND		= -8,
	eSOCKET_STATUS_FAIL_CLOSE		= -9,
} eSOCKET_STATUS;

/* Structures ----------------------------------------------------------------*/
typedef struct _tSOCKET {
	int				uaddr;
	int				baddr;
	rxhandler	handler;
} tSOCKET;

/* API Prototypes ------------------------------------------------------------*/
eSOCKET_STATUS SOCKET_Create(const int addr, rxhandler handler);
eSOCKET_STATUS SOCKET_Run(void);
eSOCKET_STATUS SOCKET_SendUnicast(const char* const dip, const int daddr, const int len, const void* const data);
eSOCKET_STATUS SOCKET_SendBroadcast(const int len, const void* const data);
eSOCKET_STATUS SOCKET_Destroy(void);
eSOCKET_STATUS SOCKET_GetMyAddress(int* const addr);

#endif
