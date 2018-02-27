/*******************************************************************************
 * @file		pakcet/packet.c
 * @brief		Packet module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.23	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <string.h>
// System
#include "packet.h"

/* APIs ----------------------------------------------------------------------*/
ePKT_STATUS	PKT_MakePacket(uint16_t src, uint16_t dst, uint8_t opcode, uint8_t len, uint8_t* payload, tPACKET* ptpkt) {
	ptpkt->src = src;
	ptpkt->dst = dst;
	ptpkt->tCmd.opcode = opcode;
	memcpy(ptpkt->tCmd.payload, payload, len);
	return ePKT_STATUS_SUCCESS;
}
