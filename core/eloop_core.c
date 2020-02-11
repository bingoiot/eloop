/*
 * eloop_core.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_list.h"
#include "eloop_core.h"
#include "eloop_lib.h"
#include "eloop_timer.h"
#include "eloop_message.h"
#include "eloop_log.h"
#include "eloop.h"

static s_int8 					_task_run(void);
static eTStack_t*				_unsafe_get_free_task(void);
static void 					_task_clean(void);
static eTStack_t				*pTaskList = NULL;
static eloop_power_conserve_t	eloop_power_conserve = NULL;
static u_int8					trash_empty_flag = ES_FALSE;
static u_int8					eloop_enable_sleep = ES_FALSE;
s_int8 eloop_core_init(eloop_power_conserve_t cb)
{
	s_int8 ret = ES_SUCCEED;
	eloop_log(DBG_ECORE,"eloop_init!\r\n");
	eloop_power_conserve =  cb;
	eloop_enable_sleep = ES_FALSE;
	return ret;
}
void eloop_set_power_conserve(u_int8 enable)
{
	eloop_enable_sleep = enable;
}
s_int8 eloop_create_task(eloop_task_t task,u_int8 priority)
{
	s_int32 ret = ES_SUCCEED;
	eTStack_t	*pnew = NULL;
	eloop_log(DBG_ECORE,"eloop_create_task! task:%08x,  priority:%d \r\n",((int)task&0xFFFFFFFF),priority);
	pnew = eloop_find_task(task);
	if(pnew!=NULL)
	{
		ELOOP_ASSERT("eloop create task error ! task already exist !");
	}
	pnew = (eTStack_t*)eloop_malloc(sizeof(eTStack_t));
	if(pnew!=NULL)
	{
		eloop_memset(pnew,0,sizeof(eTStack_t));
		pnew->priority = priority;
		pnew->task = task;
		pnew->state = ES_TASK_READY;
		eloop_enter_critical();
		ret = eloop_list_insert((void**)&pTaskList,pnew);
		eloop_exit_critical();
	}
	else
	{
		ret = ES_MEM_ERROR;
	}
	return ret;
}
s_int8 eloop_task_poll(void)
{
	_task_run();
	return 0;
}
eTStack_t* eloop_find_task(eloop_task_t task)
{
	eTStack_t 	*ret = NULL;
	eloop_log(DBG_ECORE,"eloop_find_task! task :%08x \r\n",(int)task);
	eloop_enter_critical();
	ret = eloop_unsafe_find_task(task);
	eloop_exit_critical();
	return ret;
}
eTStack_t* eloop_unsafe_find_task(eloop_task_t task)
{
	eTStack_t 	*pcur,*ret = NULL;
	eloop_log(DBG_ECORE,"eloop_find_task! task :%08x \r\n",(int)task);
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
eTStack_t* eloop_get_task(u_int32 index)
{
	eTStack_t 	*pcur,*ret = NULL;
	s_int32 	i = 0;
	eloop_log(DBG_ECORE,"eloop_get_task! task id:%d \r\n",index);
	eloop_enter_critical();
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
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_delete_task(eloop_task_t task)
{
	eTStack_t 		*ptask;
	u_int8			ret = ES_FAILED;
	eloop_log(DBG_ECORE,"eloop_delete_task! :%08x\r\n",(int)task);
	eloop_enter_critical();
	ptask = eloop_unsafe_find_task(task);
	if(ptask!=NULL)
	{
		eloop_list_delete_list((void**)&ptask->pmsg);
		ret = eloop_list_delete((void**)&pTaskList,ptask);
	}
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_set_event(eloop_task_t task, u_int32 event)
{
	u_int8			ret = ES_FAILED;
	eloop_log(DBG_ECORE,"eloop_set_event! task:%08x ,event:%08x \r\n",(int)task,event);
	eloop_enter_critical();
	eloop_unsafe_set_event(task,event);
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_clear_event(eloop_task_t task, u_int32 event)
{
	u_int8			ret = ES_FAILED;
	eloop_log(DBG_ECORE,"eloop_clear_event! task:%08x ,event:%08x \r\n",(int)task,event);
	eloop_enter_critical();
	eloop_unsafe_clear_event(task,event);
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_unsafe_set_event(eloop_task_t task, u_int32 event)
{
	eTStack_t 		*ptask;
	u_int8			ret = ES_FAILED;
	eloop_log(DBG_ECORE,"eloop_unsafe_set_event! task:%08x ,event:%08x \r\n",(int)task,event);
	ptask = eloop_unsafe_find_task(task);
	if(ptask!=NULL)
	{
		ptask->event |= event;
		ret = ES_SUCCEED;
	}
	else
	{
		eloop_log(DBG_EERROR,"eloop_unsafe_set_event! no task:%08x ,event:%08x \r\n",(int)task,event);
	}
	return ret;
}
s_int8 eloop_unsafe_clear_event(eloop_task_t task, u_int32 event)
{
	eTStack_t 		*ptask;
	u_int8			ret = ES_FAILED;
	eloop_log(DBG_ECORE,"eloop_clear_event! task:%08x ,event:%08x \r\n",(int)task,event);
	ptask = eloop_unsafe_find_task(task);
	if(ptask!=NULL)
	{
		ptask->event ^= event;
		ret = ES_SUCCEED;
	}
	else
	{
		eloop_log(DBG_EERROR,"eloop_unsafe_clear_event! no task:%08x ,event:%08x \r\n",(int)task,event);
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
	eloop_log(DBG_ECORE,"eloop_task_poll!\r\n");
	eloop_enter_critical();
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
	eloop_exit_critical();
	if(high_prio_task!=NULL)
	{
		if(high_prio_task->task!=NULL)
		{
			while(high_prio_task->pmsg!=NULL)
			{
				eloop_log(DBG_ECORE,"eloop_task_poll got message!\r\n");
				eloop_enter_critical();
				pmsg = high_prio_task->pmsg;
				high_prio_task->pmsg = pmsg->next;
				eloop_exit_critical();
				if(high_prio_task!=NULL)
				{
					eloop_log(DBG_ECORE,"eloop_task_poll call task for message !\r\n");
					high_prio_task->task(TASK_MESSAGE_EVENT,pmsg->pdata,pmsg->len);
					if((pmsg->msg_type==MSG_TYPE_COPY_STREAM)&&(pmsg->pdata!=NULL))
					{
						eloop_free(pmsg->pdata);
					}
					eloop_free(pmsg);
				}
			}
			if(event)
			{
				high_prio_task->task(event,NULL,0);
			}
		}
		else
		{
			ELOOP_ASSERT("core error ! task = NULL \r\n");
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
		if((eloop_enable_sleep==ES_TRUE)&&(eloop_power_conserve!=NULL))
		{
			event = eloop_get_next_timeout();
			if(event>=ELOOP_PWRCONSERVE_LIMIT_TIME)
			{
				eloop_log(DBG_ECORE,"eloop core is idle, enter sleep:%d\r\n",event);
				if(eloop_power_conserve(event)==ES_SUCCEED)
				{
					eloop_update_tick(event);
				}
			}
		}
	}
	return ret;
}
static void _task_clean(void)
{
	eTStack_t 	*ptask;
	eloop_log(DBG_ECORE,"_task_clean! \r\n");
	eloop_enter_critical();
	while(1)
	{
		ptask = _unsafe_get_free_task();
		if(ptask!=NULL)
		{
			eloop_list_delete_list((void**)&ptask->pmsg);
			eloop_list_delete((void**)&pTaskList,ptask);
		}
		else
		{
			break;
		}
	}
	eloop_exit_critical();
}
static eTStack_t *_unsafe_get_free_task(void)
{
	eTStack_t 	*ptask;
	eloop_log(DBG_ECORE,"_get_free_task! \r\n");
	ptask = pTaskList;
	while(ptask!=NULL)
	{
		if(ptask->state == ES_TASK_STOP)
			return ptask;
		ptask = ptask->next;
	}
	return NULL;
}
