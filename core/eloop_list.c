/*
 * eloop_list.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_list.h"
#include "eloop_heap.h"

s_int8 eloop_list_insert(void **list, void *msg)
{
	eList_t *pcur;
	if(*list==NULL)
	{
		*list = msg;
	}
	else
	{
		pcur = *list;
		while(pcur->next!=NULL)
		{
			pcur = pcur->next;
		}
		pcur->next = msg;
	}
	return ES_SUCCEED;
}
s_int8 eloop_list_insert_front(void **list, void *msg)
{
	eList_t *pcur;
	if(*list==NULL)
	{
		*list = msg;
	}
	else
	{
		pcur = msg;
		pcur->next = *list;
		*list = pcur;
	}
	return ES_SUCCEED;
}
void *eloop_list_detach(void **list)
{
	eList_t *pcur;
	pcur = *list;
	if(pcur!=NULL)
	{
		*list = pcur->next;
		pcur->next = NULL;
		return pcur;
	}
	return NULL;
}
void *eloop_list_detach_front(void **list)
{
	eList_t *pcur,*pre;
	pcur = *list;
	if(pcur!=NULL)
	{
		pre = pcur;
		while(pcur->next!=NULL)
		{
			pre = pcur;
			pcur = pcur->next;
		}
		pre->next = pcur->next;
		pcur->next = NULL;
		return pcur;
	}
	return NULL;
}
s_int8 eloop_list_delete(void **list, void *msg)
{
	eList_t 	*pcur,*pre;
	void		 *pfree;
	u_int8		ret = ES_FAILED;
	pcur = *list;
	pre = pcur;
	while(pcur!=NULL)
	{
		if(pcur==msg)
		{
			ret = ES_SUCCEED;
			pfree = pcur;
			if(pre==pcur)//first task stack
			{
				pcur = pcur->next;
				*list = pcur;
				pre = pcur;
			}
			else
			{
				pre->next = pcur->next;
				pcur = pre->next;
			}
			eloop_free(pfree);
			continue;
		}
		pre = pcur;
		pcur = pcur->next;
	}
	return ret;
}
s_int8 eloop_list_delete_list(void **list)
{
	eList_t *pcur;
	void		 *pfree;
	pcur = *list;
	while(pcur!=NULL)
	{
		pfree = pcur;
		pcur = pcur->next;
		eloop_free(pfree);
	}
	*list = NULL;
	return ES_SUCCEED;
}


