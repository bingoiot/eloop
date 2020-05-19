/*
 * epoll_core.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef EPOLL_CORE_H_
#define EPOLL_CORE_H_

#include "epoll_config.h"
#include "epoll_microdef.h"

s_int8 		epoll_core_init(epoll_power_conserve_t cb);
void 		epoll_set_power_conserve(u_int8 enable);
s_int8 		epoll_create_task(epoll_task_t task,u_int8 priority);
s_int8 		epoll_delete_task(epoll_task_t task);
eTStack_t* 	epoll_find_task(epoll_task_t task);
eTStack_t* 	epoll_unsafe_find_task(epoll_task_t task);
eTStack_t* 	epoll_get_task(u_int32 index);
s_int8 		epoll_task_poll(void);
s_int8 		epoll_set_event(epoll_task_t task, u_int32 event);
s_int8 		epoll_clear_event(epoll_task_t task, u_int32 event);
s_int8 		epoll_unsafe_set_event(epoll_task_t task, u_int32 event);
s_int8 		epoll_unsafe_clear_event(epoll_task_t task, u_int32 event);
/*
#define epoll_enter_critical(void)	\
do{\
	epoll_log(DBG_ECORE,"epoll_enter_critical! \r\n");\
	_epoll_enter_critical();\
}while(0)

#define epoll_exit_critical(void)\
do{\
	epoll_log(DBG_ECORE,"epoll_exit_critical! \r\n");\
	_epoll_exit_critical();\
}while(0)
*/
#endif





