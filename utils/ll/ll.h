/*******************************************************************************
 * @file		ll.h
 * @brief		Linked List module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.03.09	Created.
 ******************************************************************************/

#ifndef __HoYa_DOUBLY_CIRCULAR_LINKED_LIST_H__
#define __HoYa_DOUBLY_CIRCULAR_LINKED_LIST_H__

/* Enumerations --------------------------------------------------------------*/
typedef enum _eLL_STATUS {
	eLL_STATUS_SUCCESS				= 0,
	eLL_STATUS_INVALID_ARGS	= -1,
} eLL_STATUS;

/* Structures ----------------------------------------------------------------*/
typedef struct _tLL {
	struct _tLL*	prev;
	struct _tLL* next;
} tLL, *ptLL, **pptLL;

/* API Prototypes ------------------------------------------------------------*/
eLL_STATUS LL_Init(const ptLL head);
eLL_STATUS LL_InsertHead(const ptLL head, const ptLL node);
eLL_STATUS LL_AppendTail(const ptLL head, const ptLL node);
eLL_STATUS LL_RemoveNode(const ptLL node);
eLL_STATUS LL_RemoveHead(const ptLL head, const pptLL node);
eLL_STATUS LL_RemoveTail(const ptLL head, const pptLL node);
eLL_STATUS LL_RemoveAll(const ptLL head);
eLL_STATUS LL_GetNext(const ptLL node, pptLL next_node);

#endif
