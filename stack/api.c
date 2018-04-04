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
#include <string.h>
// System
#include "socket.h"
#include "api.h"

/* Private Variables ---------------------------------------------------------*/
tDEV_INFO	tDevInfo = { 0xffff };
tPACKET		tPkt;

/* APIs ----------------------------------------------------------------------*/
eAPI_STATUS API_Init(const uint16_t addr) {
	tDevInfo.addr = addr;
	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_MakePacket(const uint16_t dst, const uint8_t type) {
	tPkt.src = tDevInfo.addr;
	tPkt.dst = dst;
	tPkt.tCmd.type = type;
	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_SendPacket(void) {
	if (tDevInfo.addr == 0xffff)
		return eAPI_STATUS_NOT_INIT;

	int src;
	if (SOCKET_GetMyAddress(&src) < 0)
		return eAPI_STATUS_NO_ADDR;

	char ip[] = "127.0.0.1";
	int dst = tPkt.dst;
	if (SOCKET_SendUnicast(ip, dst, sizeof(tPkt), (char*)&tPkt) < 0)
		return eAPI_STATUS_FAIL_SEND;

	return eAPI_STATUS_SUCCESS;
}
