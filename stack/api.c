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
#include <stdlib.h>
#include <string.h>
// System
#include "hDef.h"
#include "timer.h"
#include "socket.h"
#include "api.h"

/* Structures ----------------------------------------------------------------*/
typedef struct _tHANDLER_TABLE {
	uint8_t			type;
	apiHandler	handler;
} tHANDLER_TABLE;

/* Private Variables ---------------------------------------------------------*/
static tDEV_INFO			tDevInfo = { 0xffff };
static tHANDLER_TABLE	tHandlerTable[eAPI_TYPE_SIZE];
static tLL						tTx;
static tLL						tRx;

/* Private Function Prototypes -----------------------------------------------*/
static eAPI_STATUS API_SendPacket(void);
static void API_ReceivePacket(const int* const len, const void* const data);
static void API_HandlerReq(void);
static void API_HandlerRsp(void);
static void API_HandlerNtf(void);

/* APIs ----------------------------------------------------------------------*/
eAPI_STATUS API_Init(const uint16_t addr) {
	tDevInfo.addr = addr;

	// Interface
	if (SOCKET_Create(addr, API_ReceivePacket) < 0) {
		return eAPI_STATUS_FAIL_INIT_SOCK;
	}

	// Packet Scheduler
	if ((LL_Init(&tTx) < 0) || (LL_Init(&tRx) < 0))
		return eAPI_STATUS_FAIL_INIT_LL;

	// Handler
	API_AddHandler(eAPI_TYPE_REQ, API_HandlerReq);
	API_AddHandler(eAPI_TYPE_RSP, API_HandlerRsp);
	API_AddHandler(eAPI_TYPE_NTF, API_HandlerNtf);

	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_AddHandler(const uint8_t type, apiHandler handler) {
	tHandlerTable[type].type = type;
	tHandlerTable[type].handler = handler;
	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_MakePacket(const uint16_t dst, const uint8_t type) {
	if (tDevInfo.addr == 0xffff)
		return eAPI_STATUS_NOT_INIT;

	ptPKT_LIST	ptPkt = (ptPKT_LIST)malloc(sizeof(tPKT_LIST));
	ptPkt->send = 0;
	TIMER_GetTime((int*)&ptPkt->time);
	ptPkt->pkt.src = tDevInfo.addr;
	ptPkt->pkt.dst = dst;
	ptPkt->pkt.tCmd.type = type;
	if (LL_AppendTail(&tTx, (ptLL)ptPkt) < 0)
		return eAPI_STATUS_FAIL_MAKE;
	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_Process(void) {
	if (tDevInfo.addr == 0xffff)
		return eAPI_STATUS_NOT_INIT;

	API_SendPacket();
	SOCKET_Run();

	ptLL	ptrRx = &tRx;
	while (ptrRx->next != &tRx) {
		ptPKT_LIST	pktRx;
		LL_GetNext(ptrRx, (pptLL)&pktRx);
		if (pktRx->pkt.tCmd.type == eAPI_TYPE_RSP) {
			ptLL	ptrTx = &tTx;
			while (ptrTx->next != &tTx) {
				ptPKT_LIST	pktTx;
				LL_GetNext(ptrTx, (pptLL)&pktTx);
				if (pktTx->pkt.tCmd.type == eAPI_TYPE_REQ) {
					ptLL	ptrNext = ptrTx->next;
					LL_RemoveNode(ptrNext);
					free(pktTx);
				}
				else if (pktTx->pkt.tCmd.type == eAPI_TYPE_RSP) {
					ptLL	ptrNext = ptrTx->next;
					LL_RemoveNode(ptrNext);
					free(pktTx);
				}
				else if (pktTx->pkt.tCmd.type == eAPI_TYPE_NTF) {
					ptLL	ptrNext = ptrTx->next;
					LL_RemoveNode(ptrNext);
					free(pktTx);
				}
				ptrTx = ptrTx->next;
			}
		}
		ptrRx = ptrRx->next;
	}

	return eAPI_STATUS_SUCCESS;
}

eAPI_STATUS API_Shutdown(void) {
	if (tDevInfo.addr == 0xffff)
		return eAPI_STATUS_NOT_INIT;

	if (SOCKET_Destroy() < 0)
		return eAPI_STATUS_FAIL_SHUTDOWN;

	tDevInfo.addr = 0xffff;

	return eAPI_STATUS_SUCCESS;
}

/* Private Functions ---------------------------------------------------------*/
static eAPI_STATUS API_SendPacket(void) {
	if (tDevInfo.addr == 0xffff)
		return eAPI_STATUS_NOT_INIT;

	ptLL	ptr = &tTx;
	while (ptr->next != &tTx) {
		ptPKT_LIST	ptPkt;
		LL_GetNext(ptr, (pptLL)&ptPkt);
		if (ptPkt->send == 0) {
			ptPkt->send = 1;
			TIMER_GetTime((int*)&ptPkt->time);
			printf("Tx - %d, 0x%04x, 0x%04x, 0x%02x\n", ptPkt->time, ptPkt->pkt.src, ptPkt->pkt.dst, ptPkt->pkt.tCmd.type);

			int dst = ptPkt->pkt.dst;
			if (dst == 0xffff) {
				if (SOCKET_SendBroadcast(sizeof(tPACKET), (char*)&ptPkt->pkt) < 0)
					return eAPI_STATUS_FAIL_SEND;
			}
			else if (dst != tDevInfo.addr) {
				char ip[] = "127.0.0.1";
				if (SOCKET_SendUnicast(ip, dst, sizeof(tPACKET), (char*)&ptPkt->pkt) < 0)
					return eAPI_STATUS_FAIL_SEND;
			}
			return eAPI_STATUS_SUCCESS;
		}
		ptr = ptr->next;
	}
	return eAPI_STATUS_SUCCESS;
}

static void API_ReceivePacket(const int* const len, const void* const data) {
	UNUSED(len);

	ptPKT_LIST	ptPkt = (ptPKT_LIST)malloc(sizeof(tPKT_LIST));

	// Ignore
	if (((ptPACKET)data)->src == tDevInfo.addr)
		return ;

	TIMER_GetTime((int*)&ptPkt->time);
	ptPkt->pkt.src = ((ptPACKET)data)->src;
	ptPkt->pkt.dst = ((ptPACKET)data)->dst;
	ptPkt->pkt.tCmd.type = ((ptPACKET)data)->tCmd.type;
	if (LL_AppendTail(&tRx, (ptLL)ptPkt) < 0)
		return ;
	printf("Rx - %d, 0x%04x, 0x%04x, 0x%02x\n", ptPkt->time, ptPkt->pkt.src, ptPkt->pkt.dst, ptPkt->pkt.tCmd.type);

	// Response
	if (ptPkt->pkt.tCmd.type == eAPI_TYPE_REQ) {
		API_MakeRsp(ptPkt->pkt.src);
	}

	// Handler
	int	n = 0;
	while (n < eAPI_TYPE_SIZE) {
		if (n == ptPkt->pkt.tCmd.type) {
			tHandlerTable[n].handler();
			return ;
		}
		n += 1;
	}
}

static void API_HandlerReq(void) {
	return ;
}

static void API_HandlerRsp(void) {
	return ;
}

static void API_HandlerNtf(void) {
	return ;
}
