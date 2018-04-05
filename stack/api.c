/*******************************************************************************
 * @file		stack/api.c
 * @brief		Network stack.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.02.23	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdio.h>
#include <string.h>
// System
#include "hDef.h"
#include "socket.h"
#include "api.h"

/* Private Variables ---------------------------------------------------------*/
static tDEV_INFO	tDevInfo = { 0xffff };
static tPACKET		tTxPkt;

/* APIs ----------------------------------------------------------------------*/
eAPI_STATUS API_Init(const uint16_t addr) {
	tDevInfo.addr = addr;
	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_MakePacket(const uint16_t dst, const uint8_t type) {
	tTxPkt.src = tDevInfo.addr;
	tTxPkt.dst = dst;
	tTxPkt.tCmd.type = type;
	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_SendPacket(void) {
	if (tDevInfo.addr == 0xffff)
		return eAPI_STATUS_NOT_INIT;

	int src;
	if (SOCKET_GetMyAddress(&src) < 0)
		return eAPI_STATUS_NO_ADDR;

	int dst = tTxPkt.dst;
	if (dst == 0xffff) {
		if (SOCKET_SendBroadcast(sizeof(tTxPkt), (char*)&tTxPkt) < 0)
			return eAPI_STATUS_FAIL_SEND;
	}
	else if (dst != tDevInfo.addr) {
		char ip[] = "127.0.0.1";
		if (SOCKET_SendUnicast(ip, dst, sizeof(tTxPkt), (char*)&tTxPkt) < 0)
			return eAPI_STATUS_FAIL_SEND;
	}

	return eAPI_STATUS_SUCCESS;
}

void API_ReceivePacket(const int* const len, const void* const data) {
	UNUSED(len);

	ptPACKET	ptPkt = (ptPACKET)data;

	// Ignore
	if (ptPkt->src == tDevInfo.addr)
		return ;

	// Response
	if (ptPkt->tCmd.type == eAPI_TYPE_REQ) {
		API_MakeRsp(ptPkt->src);
		API_SendPacket();
	}

	printf("src: 0x%04x, dst: 0x%04x, type: 0x%02x\n", ptPkt->src, ptPkt->dst, ptPkt->tCmd.type);
}
