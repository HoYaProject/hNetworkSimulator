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

/* Enumerations --------------------------------------------------------------*/
// Status
typedef enum _eAPI_STATUS {
	eAPI_STATUS_SUCCESS		= 0,
	eAPI_STATUS_NOT_INIT	= -1,
	eAPI_STATUS_NO_ADDR		= -2,
	eAPI_STATUS_FAIL_SEND	= -3
} eAPI_STATUS;

// Opcode
typedef enum _eAPI_TYPE {
	eAPI_TYPE_REQ	= 0x00,
	eAPI_TYPE_RSP,
	eAPI_TYPE_NTF,
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

/* Macro Functions -----------------------------------------------------------*/
#define API_MakeReq(dst)	\
	API_MakePacket((dst), eAPI_TYPE_REQ)
#define API_MakeRsp(dst)	\
	API_MakePacket((dst), eAPI_TYPE_RSP)
#define API_MakeNtf()			\
	API_MakePacket(0xffff, eAPI_TYPE_NTF)

/* API Prototypes ------------------------------------------------------------*/
eAPI_STATUS API_Init(const uint16_t addr);
eAPI_STATUS	API_MakePacket(const uint16_t dst, const uint8_t type);
eAPI_STATUS API_SendPacket(void);
void API_ReceivePacket(const int* const len, const void* const data);

#endif
