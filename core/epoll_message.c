/*
 * epoll_message.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "epoll_microdef.h"
#include "epoll_core.h"
#include "epoll_list.h"
#include "epoll_message.h"
#include "epoll_lib.h"
#include "epoll_log.h"
#include "epoll.h"

eMessage_t 	*_create_message(void *msg, s_int32 len);
void 		*_create_stream(void *msg, s_int32 len);

s_int8	epoll_send_msg(epoll_task_t task, void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	s_int8		ret = ES_FAILED;
	if((ptask = epoll_find_task(task))!=NULL)
	{
		pmsg = _create_message(msg,len);
		if(pmsg!=NULL)
		{
			epoll_enter_critical();
			ret = epoll_list_insert((void**)&ptask->pmsg,pmsg);
			epoll_exit_critical();
		}
	}
	return ret;
}
s_int8	epoll_send_msg_front(epoll_task_t task, void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	s_int8		ret = ES_FAILED;
	if((ptask = epoll_find_task(task))!=NULL)
	{
		pmsg = _create_message(msg,len);
		if(pmsg!=NULL)
		{
			epoll_enter_critical();
			ret = epoll_list_insert_front((void**)&ptask->pmsg,pmsg);
			epoll_exit_critical();
		}
	}
	return ret;
}
s_int8	epoll_send_stream(epoll_task_t task,void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	void		*pdata;
	s_int8		ret = ES_FAILED;
	if((ptask = epoll_find_task(task))!=NULL)
	{
		if((pdata = _create_stream(msg,len))!=NULL)
		{
			pmsg = _create_message(pdata,len);
			if(pmsg!=NULL)
			{
				pmsg->msg_type = MSG_TYPE_COPY_STREAM;
				epoll_enter_critical();
				ret = epoll_list_insert((void**)&ptask->pmsg,pmsg);
				epoll_exit_critical();
			}
			else
			{
				epoll_free(pdata);
			}
		}
	}
	return ret;
}
s_int8	epoll_send_stream_front(epoll_task_t task, void *msg, s_int32 len)
{
	eTStack_t	*ptask;
	eMessage_t 	*pmsg;
	void		*pdata;
	s_int8		ret = ES_FAILED;
	if((ptask = epoll_find_task(task))!=NULL)
	{
		if((pdata = _create_stream(msg,len))!=NULL)
		{
			pmsg = _create_message(pdata,len);
			if(pmsg!=NULL)
			{
				pmsg->msg_type = MSG_TYPE_COPY_STREAM;
				epoll_enter_critical();
				ret = epoll_list_insert_front((void**)&ptask->pmsg,pmsg);
				epoll_exit_critical();
			}
			else
			{
				epoll_free(pdata);
			}
		}
	}
	return ret;
}
eMessage_t *_create_message(void *msg, s_int32 len)
{
	eMessage_t *pmsg;
	pmsg = (eMessage_t*)epoll_malloc(sizeof(eMessage_t));
	if(pmsg!=NULL)
	{
		epoll_memset(pmsg,0,sizeof(eMessage_t));
		pmsg->msg_type = MSG_TYPE_REFRENCE;
		pmsg->pdata = msg;
		pmsg->len = len;
	}
	return pmsg;
}
void *_create_stream(void *msg, s_int32 len)
{
	void *pmsg;
	pmsg = (eMessage_t*)epoll_malloc(len);
	if(pmsg!=NULL)
	{
		epoll_memcpy(pmsg,msg,len);
	}
	return pmsg;
}


