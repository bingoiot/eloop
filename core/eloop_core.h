/*
 * eloop_core.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef ELOOP_CORE_H_
#define ELOOP_CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "eloop_config.h"
#include "eloop_microdef.h"

	s_int8 		eloop_core_init(eloop_power_conserve_t cb);
	void 		eloop_set_power_conserve(u_int8 enable);
	s_int8 		eloop_create_task(eloop_task_t task, u_int8 priority);
	s_int8 		eloop_delete_task(eloop_task_t task);
	eTStack_t* eloop_find_task(eloop_task_t task);
	eTStack_t* eloop_unsafe_find_task(eloop_task_t task);
	eTStack_t* eloop_get_task(u_int32 index);
	s_int8 		eloop_task_poll(void);
	s_int8 		eloop_set_event(eloop_task_t task, u_int32 event);
	s_int8 		eloop_clear_event(eloop_task_t task, u_int32 event);
	s_int8 		eloop_unsafe_set_event(eloop_task_t task, u_int32 event);
	s_int8 		eloop_unsafe_clear_event(eloop_task_t task, u_int32 event);
	/*
	#define eloop_enter_critical(void)	\
	do{\
		eloop_log(DBG_ECORE,"eloop_enter_critical! \r\n");\
		_eloop_enter_critical();\
	}while(0)

	#define eloop_exit_critical(void)\
	do{\
		eloop_log(DBG_ECORE,"eloop_exit_critical! \r\n");\
		_eloop_exit_critical();\
	}while(0)
	*/


#ifdef __cplusplus
}
#endif


#endif





