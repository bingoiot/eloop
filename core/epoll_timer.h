/*
 * epoll_timer.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_TIMER_H_
#define _EPOLL_TIMER_H_

#include "epoll_config.h"
#include "epoll_microdef.h"

s_int8 	epoll_start_timer(epoll_timer_cb_t timer_cb, u_int8 signal, u_int32 time, u_int8 reload);
s_int8 	epoll_start_timer_task(epoll_task_t task,u_int32 event, u_int32 time, u_int8 reload);
s_int8 	epoll_stop_timer(epoll_timer_cb_t timer_cb,u_int8 signal);
s_int8 	epoll_stop_timer_task(epoll_task_t timer_cb,u_int32 event);
s_int32 epoll_get_next_timeout(void);
void 	epoll_timer_clean(void);

void 	epoll_update_tick(u_int32 tick);

u_int32 epoll_get_timestamp(void);
u_int32 epoll_get_unixtime(void);
void    epoll_set_unixtime(u_int32 tim);
s_int8 	epoll_timer_start_measure(eTMeasure_t *pm,u_int32 timeout);
s_int8 	epoll_timer_expire(eTMeasure_t *pm);
u_int64 epoll_local_time_to_unix(int zone,eTime_t *date_time);
int     epoll_unix_to_local_time (int zone, eTime_t *tp,u_int64 t);

#endif /* EPOLL_EPOLL_TIMER_H_ */
