/*
 * eloop_timer.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_TIMER_H_
#define _ELOOP_TIMER_H_


#ifdef __cplusplus
extern "C"
{
#endif


#include "eloop_config.h"
#include "eloop_microdef.h"

	s_int8 	eloop_start_timer(eloop_timer_cb_t timer_cb, u_int8 signal, u_int32 time, u_int8 reload);
	s_int8 	eloop_start_timer_task(eloop_task_t task, u_int32 event, u_int32 time, u_int8 reload);
	s_int8 	eloop_stop_timer(eloop_timer_cb_t timer_cb, u_int8 signal);
	s_int8 	eloop_stop_timer_task(eloop_task_t timer_cb, u_int32 event);
	s_int32 eloop_get_next_timeout(void);
	void 	eloop_timer_clean(void);

	void 	eloop_update_tick(u_int32 tick);

	u_int32 eloop_get_timestamp(void);
	u_int32 eloop_get_unixtime(void);
	void    eloop_set_unixtime(u_int32 tim);
	s_int8 	eloop_timer_start_measure(eTMeasure_t* pm, u_int32 timeout);
	s_int8 	eloop_timer_expire(eTMeasure_t* pm);
	u_int64 eloop_local_time_to_unix(int zone, eTime_t* date_time);
	int     eloop_unix_to_local_time(int zone, eTime_t* tp, u_int64 t);


#ifdef __cplusplus
}
#endif

#endif /* ELOOP_ELOOP_TIMER_H_ */
