/*
 * eloop_message.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_core.h"
#include "eloop_list.h"
#include "eloop_message.h"
#include "eloop_lib.h"
#include "eloop_log.h"
#include "eloop_port.h"

eMessage_t 	*_create_message(void *msg, s_int32 len);
void 		*_create_stream(void *msg, s_int32 len);

s_int8	eloop_send_msg(eloop_task_t task, void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	s_int8		ret = ES_FAILED;
	if((ptask = eloop_find_task(task))!=NULL)
	{
		pmsg = _create_message(msg,len);
		if(pmsg!=NULL)
		{
			eloop_enter_critical();
			ret = eloop_list_insert((void**)&ptask->pmsg,pmsg);
			eloop_exit_critical();
		}
	}
	return ret;
}
s_int8	eloop_send_msg_front(eloop_task_t task, void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	s_int8		ret = ES_FAILED;
	if((ptask = eloop_find_task(task))!=NULL)
	{
		pmsg = _create_message(msg,len);
		if(pmsg!=NULL)
		{
			eloop_enter_critical();
			ret = eloop_list_insert_front((void**)&ptask->pmsg,pmsg);
			eloop_exit_critical();
		}
	}
	return ret;
}
s_int8	eloop_send_stream(eloop_task_t task,void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	void		*pdata;
	s_int8		ret = ES_FAILED;
	if((ptask = eloop_find_task(task))!=NULL)
	{
		if((pdata = _create_stream(msg,len))!=NULL)
		{
			pmsg = _create_message(pdata,len);
			if(pmsg!=NULL)
			{
				pmsg->msg_type = MSG_TYPE_COPY_STREAM;
				eloop_enter_critical();
				ret = eloop_list_insert((void**)&ptask->pmsg,pmsg);
				eloop_exit_critical();
			}
			else
			{
				eloop_free(pdata);
			}
		}
	}
	return ret;
}
s_int8	eloop_send_stream_front(eloop_task_t task, void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	void		*pdata;
	s_int8		ret = ES_FAILED;
	if((ptask = eloop_find_task(task))!=NULL)
	{
		if((pdata = _create_stream(msg,len))!=NULL)
		{
			pmsg = _create_message(pdata,len);
			if(pmsg!=NULL)
			{
				pmsg->msg_type = MSG_TYPE_COPY_STREAM;
				eloop_enter_critical();
				ret = eloop_list_insert_front((void**)&ptask->pmsg,pmsg);
				eloop_exit_critical();
			}
			else
			{
				eloop_free(pdata);
			}
		}
	}
	return ret;
}
eMessage_t *_create_message(void *msg, s_int32 len)
{
	eMessage_t *pmsg;
	pmsg = (eMessage_t*)eloop_malloc(sizeof(eMessage_t));
	if(pmsg!=NULL)
	{
		eloop_memset(pmsg,0,sizeof(eMessage_t));
		pmsg->msg_type = MSG_TYPE_REFRENCE;
		pmsg->pdata = msg;
		pmsg->len = len;
	}
	return pmsg;
}
void *_create_stream(void *msg, s_int32 len)
{
	void *pmsg;
	pmsg = (eMessage_t*)eloop_malloc(len);
	if(pmsg!=NULL)
	{
		eloop_memcpy(pmsg,msg,len);
	}
	return pmsg;
}


