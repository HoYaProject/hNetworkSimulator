/*******************************************************************************
 * @file		packet/packet.h
 * @brief		Packet module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.23	Created.
 ******************************************************************************/

#ifndef __HoYa_PACKET_H__
#define __HoYa_PACKET_H__

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdint.h>

/* Enumerations --------------------------------------------------------------*/
// Status
typedef enum _ePKT_STATUS {
	ePKT_STATUS_SUCCESS		= 0,
} ePKT_STATUS;

// Opcode
typedef enum _ePKT_OPCODE {
	ePKT_OPCODE_REQ	= 0x00,
	ePKT_OPCODE_RSP,
	ePKT_OPCODE_NTF,
} ePKT_OPCODE;

/* Structures ----------------------------------------------------------------*/
// Command
typedef struct _tPKT_COMMAND {
	uint8_t	opcode;
	uint8_t	payload[8];
} tPKT_COMMAND;

// Packet
typedef struct _tPACKET {
	uint16_t			src;
	uint16_t			dst;
	tPKT_COMMAND	tCmd;
} tPACKET;

/* Macro Functions -----------------------------------------------------------*/
#define PKT_MakeReq(src, dst, len, payload, pkt)	\
	PKT_MakePacket((src), (dst), ePKT_OPCODE_REQ, (len), (payload), (pkt))
#define PKT_MakeRsp(src, dst, len, payload, pkt)	\
	PKT_MakePacket((src), (dst), ePKT_OPCODE_RSP, (len), (payload), (pkt))
#define PKT_MakeNtf(src, len, payload, pkt)				\
	PKT_MakePacket((src), 0xffff, ePKT_OPCODE_NTF, (len), (payload), (pkt))

/* API Prototypes ------------------------------------------------------------*/
ePKT_STATUS	PKT_MakePacket(uint16_t src, uint16_t dst, uint8_t type, uint8_t len, uint8_t *payload, tPACKET* ptpkt);

#endif
