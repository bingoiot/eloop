/*
 * epoll_core.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "epoll_microdef.h"
#include "epoll_list.h"
#include "epoll_core.h"
#include "epoll_lib.h"
#include "epoll_timer.h"
#include "epoll_message.h"
#include "epoll_log.h"
#include "epoll.h"

static s_int8 					_task_run(void);
static eTStack_t*				_unsafe_get_free_task(void);
static void 					_task_clean(void);
static eTStack_t				*pTaskList = NULL;
static epoll_power_conserve_t	epoll_power_conserve = NULL;
static u_int8					trash_empty_flag = ES_FALSE;
static u_int8					epoll_enable_sleep = ES_FALSE;
s_int8 epoll_core_init(epoll_power_conserve_t cb)
{
	s_int8 ret = ES_SUCCEED;
	epoll_log(DBG_ECORE,"epoll_init!\r\n");
	epoll_power_conserve =  cb;
	epoll_enable_sleep = ES_FALSE;
	return ret;
}
void epoll_set_power_conserve(u_int8 enable)
{
	epoll_enable_sleep = enable;
}
s_int8 epoll_create_task(epoll_task_t task,u_int8 priority)
{
	s_int32 ret = ES_SUCCEED;
	eTStack_t	*pnew = NULL;
	epoll_log(DBG_ECORE,"epoll_create_task! task:%08x,  priority:%d \r\n",((int)task&0xFFFFFFFF),priority);
	pnew = epoll_find_task(task);
	if(pnew!=NULL)
	{
		EPOLL_ASSERT("epoll create task error ! task already exist !");
	}
	pnew = (eTStack_t*)epoll_malloc(sizeof(eTStack_t));
	if(pnew!=NULL)
	{
		epoll_memset(pnew,0,sizeof(eTStack_t));
		pnew->priority = priority;
		pnew->task = task;
		pnew->state = ES_TASK_READY;
		epoll_enter_critical();
		ret = epoll_list_insert((void**)&pTaskList,pnew);
		epoll_exit_critical();
	}
	else
	{
		ret = ES_MEM_ERROR;
	}
	return ret;
}
s_int8 epoll_task_poll(void)
{
	return _task_run();
}
eTStack_t* epoll_find_task(epoll_task_t task)
{
	eTStack_t 	*ret = NULL;
	epoll_log(DBG_ECORE,"epoll_find_task! task :%08x \r\n",(int)task);
	epoll_enter_critical();
	ret = epoll_unsafe_find_task(task);
	epoll_exit_critical();
	return ret;
}
eTStack_t* epoll_unsafe_find_task(epoll_task_t task)
{
	eTStack_t 	*pcur,*ret = NULL;
	epoll_log(DBG_ECORE,"epoll_find_task! task :%08x \r\n",(int)task);
	pcur = pTaskList;
	while(pcur!=NULL)
	{
		if(pcur->task==task)
		{
			ret = pcur;
			break;
		}
		pcur = pcur->next;
	}
	return ret;
}
eTStack_t* epoll_get_task(u_int32 index)
{
	eTStack_t 	*pcur,*ret = NULL;
	s_int32 	i = 0;
	epoll_log(DBG_ECORE,"epoll_get_task! task id:%d \r\n",index);
	epoll_enter_critical();
	pcur = pTaskList;
	while(pcur!=NULL)
	{
		if(i==index)
		{
			ret = pcur;
			break;
		}
		pcur = pcur->next;
	}
	epoll_exit_critical();
	return ret;
}
s_int8 epoll_delete_task(epoll_task_t task)
{
	eTStack_t 		*ptask;
	u_int8			ret = ES_FAILED;
	epoll_log(DBG_ECORE,"epoll_delete_task! :%08x\r\n",(int)task);
	epoll_enter_critical();
	ptask = epoll_unsafe_find_task(task);
	if(ptask!=NULL)
	{
		epoll_list_delete_list((void**)&ptask->pmsg);
		ret = epoll_list_delete((void**)&pTaskList,ptask);
	}
	epoll_exit_critical();
	return ret;
}
s_int8 epoll_set_event(epoll_task_t task, u_int32 event)
{
	u_int8			ret = ES_FAILED;
	epoll_log(DBG_ECORE,"epoll_set_event! task:%08x ,event:%08x \r\n",(int)task,event);
	epoll_enter_critical();
	ret = epoll_unsafe_set_event(task,event);
	epoll_exit_critical();
	return ret;
}
s_int8 epoll_clear_event(epoll_task_t task, u_int32 event)
{
	u_int8			ret = ES_FAILED;
	epoll_log(DBG_ECORE,"epoll_clear_event! task:%08x ,event:%08x \r\n",(int)task,event);
	epoll_enter_critical();
	ret = epoll_unsafe_clear_event(task,event);
	epoll_exit_critical();
	return ret;
}
s_int8 epoll_unsafe_set_event(epoll_task_t task, u_int32 event)
{
	eTStack_t 		*ptask;
	u_int8			ret = ES_FAILED;
	epoll_log(DBG_ECORE,"epoll_unsafe_set_event! task:%08x ,event:%08x \r\n",(int)task,event);
	ptask = epoll_unsafe_find_task(task);
	if(ptask!=NULL)
	{
		ptask->event |= event;
		ret = ES_SUCCEED;
	}
	else
	{
		epoll_log(DBG_EERROR,"epoll_unsafe_set_event! no task:%08x ,event:%08x \r\n",(int)task,event);
	}
	return ret;
}
s_int8 epoll_unsafe_clear_event(epoll_task_t task, u_int32 event)
{
	eTStack_t 		*ptask;
	u_int8			ret = ES_FAILED;
	epoll_log(DBG_ECORE,"epoll_clear_event! task:%08x ,event:%08x \r\n",(int)task,event);
	ptask = epoll_unsafe_find_task(task);
	if(ptask!=NULL)
	{
		ptask->event ^= event;
		ret = ES_SUCCEED;
	}
	else
	{
		epoll_log(DBG_EERROR,"epoll_unsafe_clear_event! no task:%08x ,event:%08x \r\n",(int)task,event);
	}
	return ret;
}

static s_int8 _task_run(void)
{
	eMessage_t		*pmsg;
	eTStack_t 		*pcur;
	eTStack_t		*high_prio_task = NULL;
	u_int32			event;
	u_int8			high_prio = 0;
	u_int8 			ret = ES_TASK_IDLE;
	epoll_log(DBG_ECORE,"epoll_task_poll!\r\n");
	epoll_enter_critical();
	pcur = pTaskList;
	while(pcur!=NULL)
	{
		if((pcur->event>0)||(pcur->pmsg!=NULL))
		{
			if(pcur->state==ES_TASK_STOP)
			{
				trash_empty_flag = ES_TRUE;
			}
			else if((pcur->state!=ES_TASK_PAUSE)&&(pcur->priority>=high_prio))
			{
				high_prio_task = pcur;
				high_prio = pcur->priority;
			}
		}
		pcur = pcur->next;
	}
	if(high_prio_task!=NULL)
	{
		event = high_prio_task->event;
		high_prio_task->event = 0;
	}
	epoll_exit_critical();
	if(high_prio_task!=NULL)
	{
		if(high_prio_task->task!=NULL)
		{
			while(high_prio_task->pmsg!=NULL)
			{
				epoll_log(DBG_ECORE,"epoll_task_poll got message!\r\n");
				epoll_enter_critical();
				pmsg = high_prio_task->pmsg;
				high_prio_task->pmsg = pmsg->next;
				epoll_exit_critical();
				if(high_prio_task!=NULL)
				{
					epoll_log(DBG_ECORE,"epoll_task_poll call task for message !\r\n");
					high_prio_task->task(TASK_MESSAGE_EVENT,pmsg->pdata,pmsg->len);
					if((pmsg->msg_type==MSG_TYPE_COPY_STREAM)&&(pmsg->pdata!=NULL))
					{
						epoll_free(pmsg->pdata);
					}
					epoll_free(pmsg);
				}
			}
			if(event)
			{
				high_prio_task->task(event,NULL,0);
			}
		}
		else
		{
			EPOLL_ASSERT("core error ! task = NULL \r\n");
		}
		ret = ES_TASK_BUSSY;
	}
	else
	{
		if(trash_empty_flag)
		{
			_task_clean();
			trash_empty_flag = ES_FALSE;
		}
		if((epoll_enable_sleep==ES_TRUE)&&(epoll_power_conserve!=NULL))
		{
			event = epoll_get_next_timeout();
			if(event>=EPOLL_PWRCONSERVE_LIMIT_TIME)
			{
				epoll_log(DBG_ECORE,"epoll core is idle, enter sleep:%d\r\n",event);
				if(epoll_power_conserve(event)==ES_SUCCEED)
				{
					epoll_update_tick(event);
				}
			}
		}
	}
	return ret;
}
static void _task_clean(void)
{
	eTStack_t 	*ptask;
	epoll_log(DBG_ECORE,"_task_clean! \r\n");
	epoll_enter_critical();
	while(1)
	{
		ptask = _unsafe_get_free_task();
		if(ptask!=NULL)
		{
			epoll_list_delete_list((void**)&ptask->pmsg);
			epoll_list_delete((void**)&pTaskList,ptask);
		}
		else
		{
			break;
		}
	}
	epoll_exit_critical();
}
static eTStack_t *_unsafe_get_free_task(void)
{
	eTStack_t 	*ptask;
	epoll_log(DBG_ECORE,"_get_free_task! \r\n");
	ptask = pTaskList;
	while(ptask!=NULL)
	{
		if(ptask->state == ES_TASK_STOP)
			return ptask;
		ptask = ptask->next;
	}
	return NULL;
}
