/*******************************************************************************
 * @file		ll.c
 * @brief		Linked List module.
 * @author	llHoYall <hoya128@gmail.com>
 * @version	v1.0
 * @note
 *	- 2018.03.09	Created.
 ******************************************************************************/

/* Include Headers -----------------------------------------------------------*/
// Standard
#include <stdlib.h>
// System
#include "ll.h"

/* APIs ----------------------------------------------------------------------*/
eLL_STATUS LL_Init(ptLL head) {
	if (head == NULL)
		return eLL_STATUS_INVALID_ARGS;

	head->next = head;
	head->prev = head;
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_InsertHead(const ptLL head, const ptLL node) {
	if ((head == NULL) || (node == NULL))
		return eLL_STATUS_INVALID_ARGS;

	node->prev = head;
	node->next = head->next;
	head->next = node;
	(node->next)->prev = node;
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_AppendTail(const ptLL head, const ptLL node) {
	if ((head == NULL) || (node == NULL))
		return eLL_STATUS_INVALID_ARGS;

	node->prev = head->prev;
	node->next = head;
	head->prev = node;
	(node->prev)->next = node;
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_RemoveNode(const ptLL node) {
	if (node == NULL)
		return eLL_STATUS_INVALID_ARGS;

	(node->prev)->next = node->next;
	(node->next)->prev = node->prev;
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_RemoveHead(const ptLL head, const pptLL node) {
	if (head == NULL)
		return eLL_STATUS_INVALID_ARGS;

	LL_RemoveNode(head->next);
	*node = head->next;
	(*node)->prev = NULL;
	(*node)->next = NULL;
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_RemoveTail(const ptLL head, const pptLL node) {
	if (head == NULL)
		return eLL_STATUS_INVALID_ARGS;

	LL_RemoveNode(head->prev);
	*node = head->prev;
	(*node)->prev = NULL;
	(*node)->next = NULL;
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_RemoveAll(const ptLL head) {
	if (head == NULL)
		return eLL_STATUS_INVALID_ARGS;

	while (head->next != head) {
		ptLL node;
		LL_RemoveHead(head, (pptLL)&node);
		free(node);
	}
	return eLL_STATUS_SUCCESS;
}

eLL_STATUS LL_GetNext(const ptLL node, pptLL next_node) {
	if (node == NULL)
		return eLL_STATUS_INVALID_ARGS;

	*next_node = node->next;
	return eLL_STATUS_SUCCESS;
}
