/*******************************************************************************
 * @file		stack/api.h
 * @brief		Network stack.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.23	Created.
 ******************************************************************************/

#ifndef __HoYa_STACK_H__
#define __HoYa_STACK_H__

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdint.h>
// System
#include "ll.h"

/* Definitions ---------------------------------------------------------------*/
typedef void (*apiHandler)(void);

/* Enumerations --------------------------------------------------------------*/
// Status
typedef enum _eAPI_STATUS {
	eAPI_STATUS_SUCCESS					= 0,
	eAPI_STATUS_FAIL_INIT_SOCK	= -1,
	eAPI_STATUS_FAIL_INIT_LL		= -2,
	eAPI_STATUS_NOT_INIT				= -3,
	eAPI_STATUS_FAIL_MAKE				= -4,
	eAPI_STATUS_FAIL_SEND				= -5,
	eAPI_STATUS_FAIL_SHUTDOWN		= -6
} eAPI_STATUS;

// Opcode
typedef enum _eAPI_TYPE {
	eAPI_TYPE_REQ	= 0x00,
	eAPI_TYPE_RSP,
	eAPI_TYPE_NTF,
	eAPI_TYPE_SIZE
} eAPI_TYPE;

/* Structures ----------------------------------------------------------------*/
// Device Info
typedef struct _tDEV_INFO {
	uint16_t	addr;
} tDEV_INFO;

// Command
typedef struct _tCOMMAND {
	uint8_t	type;
} tCOMMAND;

// Packet
typedef struct _tPACKET {
	uint16_t	src;
	uint16_t	dst;
	tCOMMAND	tCmd;
} tPACKET, *ptPACKET;

// Packet List
typedef struct _tPKT_LIST {
	tLL				node;
	uint8_t		send;
	uint32_t	time;
	tPACKET		pkt;
} tPKT_LIST, *ptPKT_LIST;

/* Macro Functions -----------------------------------------------------------*/
#define API_SRC(p)		(p)->pkt.src
#define API_DST(p)		(p)->pkt.dst

#define API_MakeReq(dst)	\
	API_MakePacket((dst), eAPI_TYPE_REQ)
#define API_MakeRsp(dst)	\
	API_MakePacket((dst), eAPI_TYPE_RSP)
#define API_MakeNtf()			\
	API_MakePacket(0xffff, eAPI_TYPE_NTF)

/* API Prototypes ------------------------------------------------------------*/
eAPI_STATUS API_Init(const uint16_t addr);
eAPI_STATUS API_AddHandler(const uint8_t type, apiHandler handler);
eAPI_STATUS	API_MakePacket(const uint16_t dst, const uint8_t type);
eAPI_STATUS API_Process(void);
eAPI_STATUS API_Shutdown(void);

#endif
