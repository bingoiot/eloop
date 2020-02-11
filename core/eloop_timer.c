/*
 * eloop_timer.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_core.h"
#include "eloop_list.h"
#include "eloop_lib.h"
#include "eloop_timer.h"
#include "eloop_heap.h"
#include "eloop_log.h"
#include "eloop.h"

typedef enum
{
	TIMER_TPYE_FORTASK = 0,
	TIMER_TPYE_FORTIMER = 1,
}_TIME_TYPE;

typedef enum
{
	TIMER_STATE_INVALID = 0,
	TIMER_STATE_NORMAL_TIMER = 1,
	TIMER_STATE_RELOAD_TIMER = 2
}_TIME_STATE;

typedef struct
{
	void* 				next;
	union
	{
		eloop_timer_cb_t	timer_cb;
		eloop_task_t		task;
	}cb;
	u_int32 			timeout;
	u_int32				runtime;
	u_int8				sig;
	u_int8				expire;
	u_int8				ttype;//0:task timer, 1:normal timer
	u_int8				state;//0:invalid,1:once,2:reload
}eTimer_t;

static eTimer_t		*pTimerList = NULL;
static void 		_timer_init(void);
static void 		_timer_task(u_int32 evt, void *arg0, u_int32 len);
static s_int8 		_timer_process(void);
static eTimer_t		*_find_timer(eTimer_t* list, void *timer_cb, u_int8 sig);
static eTimer_t 	*_update_timer(eTimer_t *ptimer, u_int32 time, u_int8 reload);
static eTimer_t 	*_create_timer(void *timer_cb,s_int8 ttype, u_int8 sig, u_int32 time, u_int8 reload);
static eTimer_t		*_get_free_timer(eTimer_t *plist);
static s_int32 		_get_next_timeout(eTimer_t *plist);
static s_int8 		_updata_tick(u_int32 tick);
static u_int8 		_convert_to_sig(u_int32 event);
static u_int32 		_convert_to_event(u_int8 sig);
s_int8 eloop_start_timer(eloop_timer_cb_t timer_cb,u_int8 sig, u_int32 time, u_int8 reload)
{
	eTimer_t 		*ptimer;
	s_int8			ret = ES_FAILED;
	eloop_log(DBG_ETIMER,"eloop_start_timer:cb:%08x,sig:%d,time:%d,isreload:%d\r\n",(int)timer_cb,sig,time,reload);
	_timer_init();
	eloop_enter_critical();
	ptimer = _find_timer(pTimerList,(void*)timer_cb,sig);
	if(ptimer!=NULL)
	{
		eloop_log(DBG_ETIMER,"eloop_start_timer:find old and update\r\n");
		_update_timer(ptimer,time,reload);
	}
	else
	{
		eloop_log(DBG_ETIMER,"eloop_start_timer: create timer\r\n");
		_create_timer((void*)timer_cb,TIMER_TPYE_FORTIMER,sig,time,reload);
	}
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_start_timer_task(eloop_task_t task,u_int32 event, u_int32 time, u_int8 reload)
{
	eTimer_t 		*ptimer;
	u_int8			sig;
	s_int8			ret = ES_FAILED;
	eloop_log(DBG_ETIMER,"eloop_start_timer_task:cb:%08x,sig:%d,time:%d,isreload:%d\r\n",(int)task,event,time,reload);
	_timer_init();
	sig = _convert_to_sig(event);
	eloop_enter_critical();
	ptimer = _find_timer(pTimerList,(void*)task,sig);
	if(ptimer!=NULL)
	{
		_update_timer(ptimer,time,reload);
	}
	else
	{
		_create_timer((void*)task,TIMER_TPYE_FORTASK,sig,time,reload);
	}
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_stop_timer(eloop_timer_cb_t timer_cb,u_int8 signal)
{
	eTimer_t 	*ptimer;
	u_int8		ret = ES_FAILED;
	eloop_enter_critical();
	ptimer = _find_timer(pTimerList,(void*)timer_cb,signal);
	if(ptimer!=NULL)
	{
		ptimer->state = TIMER_STATE_INVALID;//delete on next time to poll at eloop_core.c
	}
	eloop_exit_critical();
	return ret;
}
s_int8 eloop_stop_timer_task(eloop_task_t timer_cb,u_int32 event)
{
	eTimer_t 	*ptimer;
	u_int8		ret = ES_FAILED;
	u_int8		sig = _convert_to_sig(event);
	eloop_enter_critical();
	ptimer = _find_timer(pTimerList,(void*)timer_cb,sig);
	if(ptimer!=NULL)
	{
		ptimer->state = TIMER_STATE_INVALID;//delete on next time to poll at eloop_core.c
	}
	eloop_exit_critical();
	return ret;
}
s_int32 eloop_get_next_timeout(void)
{
	u_int32		timeout;
	eloop_enter_critical();
	timeout = _get_next_timeout(pTimerList);
	eloop_exit_critical();
	return timeout;
}
void eloop_timer_clean(void)
{
	eTimer_t	*ptimer;
	eloop_enter_critical();
	while(1)
	{
		ptimer = _get_free_timer(pTimerList);
		if(ptimer!=NULL)
		{
			eloop_log(DBG_ETIMER,"eloop_timer_clean: cb:%x, sig:%d \r\n",(int)ptimer->cb.timer_cb,ptimer->sig);
			eloop_list_delete((void**)&pTimerList,ptimer);
		}
		else
		{
			break;
		}
	}
	eloop_exit_critical();
}
static u_int32	mssecond = 0;
static u_int32	second = 0;
void eloop_update_tick(u_int32 tick)
{
	static u_int16 ms;
	ms += tick;
	mssecond += tick;
	if(ms>=1000)
	{
		second++;
		ms -= 1000;
		eloop_log(DBG_ETIMER,"eloop_update_timer: mssec:%d, sec:%d\r\n",mssecond,second);
	}
	_updata_tick(tick);
}
u_int32 eloop_get_timestamp(void)
{
	return mssecond;
}
u_int32 eloop_get_unixtime(void)
{
	return second;
}
void eloop_set_unixtime(u_int32 tim)
{
	second = tim;
}
s_int8 eloop_timer_start_measure(eTMeasure_t *pm,u_int32 timeout)
{
	pm->start = mssecond;
	pm->timeout = timeout;
	return ES_SUCCEED;
}
s_int8 eloop_timer_expire(eTMeasure_t *pm)
{
	u_int32 temp;
	temp = eloop_sub32_loop(mssecond,pm->start);
	if(temp>=pm->timeout)
	{
		return ES_TRUE;
	}
	else
	{
		return ES_FALSE;
	}
}
const uint8 DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
/* start of date:2000/1/1/0/0/0*/
#define defDayofSec		(24*60*60)
#define defStartYear	2000
u_int64 eloop_local_time_to_unix(int zone,eTime_t *date_time)
{
    uint16 iYear, iMon, iDay, iHour, iMin, iSec;
    iYear = date_time->year;
    iMon = date_time->mon;
    iDay = date_time->day;
    iHour = date_time->hour;
    iMin = date_time->min;
    iSec = date_time->sec;

    iHour = ((iHour+24)-zone)%24;
    uint16 i, Cyear=0;
    unsigned long CountDay=0;

    for(i=defStartYear; i<iYear; i++)     /* start of 2000 year*/
    {
        if(((i%4==0) && (i%100!=0)) || (i%400==0))  Cyear++;
    }
    CountDay = Cyear * 366 + (iYear-defStartYear-Cyear) * 365;
    for(i=1; i<iMon; i++)
    {
        if((i==2) && (((iYear%4==0)&&(iYear%100!=0)) || (iYear%400==0)))
            CountDay += 29;
        else
            CountDay += DayOfMon[i-1];
    }
    CountDay += (iDay-1);

    CountDay = CountDay*defDayofSec + (unsigned long)iHour*3600 + (unsigned long)iMin*60 + iSec;
    return CountDay;
}

#define	SECS_PER_HOUR	(60 * 60)
#define	SECS_PER_DAY	(SECS_PER_HOUR * 24)
#define STARTYEAR		2000//1970//2000
#define STARTWEEKDAY	6//4//6
# define __isleap(year)	((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))
const unsigned short int __mon_yday[2][13] =
{
/* Normal years.  */
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
/* Leap years.  */
{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};
/* Compute the `struct tm' representation of *T,
   offset OFFSET seconds east of UTC,
   and store year, yday, mon, mday, wday, hour, min, sec into *TP.
   Return nonzero if successful.  */
int eloop_unix_to_local_time (int zone, eTime_t *tp,u_int64 t)
{
	long int days, rem, y;
	long int yg;
	const unsigned short int *ip;

	days = t / SECS_PER_DAY;
	rem = t % SECS_PER_DAY;
	// rem += offset;
	while (rem < 0)
	{
		rem += SECS_PER_DAY;
		--days;
	}
	while (rem >= SECS_PER_DAY)
	{
		rem -= SECS_PER_DAY;
		++days;
	}
	tp->hour = rem / SECS_PER_HOUR;
	rem %= SECS_PER_HOUR;
	tp->min = rem / 60;
	tp->sec = rem % 60;
	tp->hour += zone;
	if(tp->hour>=24)
	{
		tp->hour%=24;
		days+=1;
	}
	/* January 1, 1970 was a Thursday.  */
	tp->wday = (STARTWEEKDAY + days) % 7;
	if (tp->wday < 0)
		tp->wday += 7;
	//if(tp->wday)tp->wday-=1;
	y = STARTYEAR;
	#define DIV(a, b) ((a) / (b) - ((a) % (b) < 0))
	#define LEAPS_THRU_END_OF(y) (DIV (y, 4) - DIV (y, 100) + DIV (y, 400))
	while (days < 0 || days >= (__isleap (y) ? 366 : 365))
	{
		/* Guess a corrected year, assuming 365 days per year.  */
		yg = y + days / 365 - (days % 365 < 0);
		/* Adjust DAYS and Y to match the guessed year.  */
		days -= ((yg - y) * 365
		   + LEAPS_THRU_END_OF (yg - 1)
		   - LEAPS_THRU_END_OF (y - 1));
		y = yg;
	}
	tp->year = y ;//- 1900;
	if (tp->year != y)
	{
		/* The year cannot be represented due to overflow.  */
		return ES_FAILED;
	}
	tp->day = days;
	ip = __mon_yday[__isleap(y)];
	for (y = 11; days < (long int) ip[y]; --y)
		continue;
	days -= ip[y];
	tp->mon = y+1;
	tp->day = days + 1;
	return ES_SUCCEED;
}
static void _timer_init(void)
{
	static u_int8 _init_flag = ES_FALSE;
	if(_init_flag==ES_FALSE)
	{
		eloop_create_task(_timer_task,255);
		_init_flag = ES_TRUE;
	}
}
#include "stdint.h"
static s_int8 _updata_tick(u_int32 tick)
{
	eTimer_t 	*pcur_timer;
	u_int32		event;
	s_int8		ret = ES_FALSE;
	eloop_log(DBG_ETIMER,"eloop_updata_tick!\r\n");
	eloop_enter_critical();
	pcur_timer = pTimerList;
	while(pcur_timer!=NULL)
	{
		eloop_log(DBG_ETIMER,"eloop_updata_tick: cb:%x, runtime:%d, timeout:%d, sig:%d \r\n",(int)pcur_timer->cb.timer_cb, pcur_timer->runtime,pcur_timer->timeout,pcur_timer->sig);
		if(pcur_timer->state!=TIMER_STATE_INVALID)
		{
			if(pcur_timer->runtime>tick)
			{
				pcur_timer->runtime-=tick;
			}
			else
			{
				pcur_timer->runtime = 0;
			}
			if(pcur_timer->runtime==0)
			{
				pcur_timer->expire = ES_TRUE;
				if(pcur_timer->state==TIMER_STATE_RELOAD_TIMER)
				{
					pcur_timer->runtime = pcur_timer->timeout;
					eloop_log(DBG_ETIMER,"is reload timer: cb:%x sig:%d \r\n",(int)pcur_timer->cb.timer_cb,pcur_timer->sig);
				}
				else
				{
					pcur_timer->state = TIMER_STATE_INVALID;
					eloop_log(DBG_ETIMER,"is normal timer and free: cb:%x sig:%d \r\n",(int)pcur_timer->cb.timer_cb,pcur_timer->sig);
				}
				if(pcur_timer->ttype==TIMER_TPYE_FORTASK)
				{				
					eloop_log(DBG_ETIMER,"time for task: cb:%x, runtime:%d, timeout:%d, sig:%d \r\n",(int)pcur_timer->cb.timer_cb, pcur_timer->runtime,pcur_timer->timeout,pcur_timer->sig);
					event = _convert_to_event(pcur_timer->sig);
					eloop_unsafe_set_event(pcur_timer->cb.task,event);
				}
				else
				{
					eloop_log(DBG_ETIMER,"time for timer: cb:%x, runtime:%d, timeout:%d, sig:%d \r\n",(int)pcur_timer->cb.timer_cb, pcur_timer->runtime,pcur_timer->timeout,pcur_timer->sig);
					eloop_unsafe_set_event(_timer_task,1);
				}
			}
		}
		else
		{
			ret = ES_TRUE;
		}
		pcur_timer = pcur_timer->next;
	}
	eloop_exit_critical();
	return ret;
}
static void _timer_task(u_int32 evt, void *arg0, u_int32 len)
{
	eloop_log(DBG_ETIMER,"_timer_task! evt:%x \r\n",evt);
	while(_timer_process()==ES_TRUE);
}
static s_int8 _timer_process(void)
{
	eTimer_t 			*pcur_timer;
	eloop_timer_cb_t	cb = NULL;
	s_int8				free_flag = ES_FALSE;
	s_int8				busy_flag = ES_FALSE;
	u_int8				sig;
	eloop_enter_critical();
	pcur_timer = pTimerList;
	while(pcur_timer!=NULL)
	{
		if(pcur_timer->expire==ES_TRUE)
		{
			if(pcur_timer->ttype!=TIMER_TPYE_FORTASK)
			{
				cb = pcur_timer->cb.timer_cb;
				sig = pcur_timer->sig;
				pcur_timer->expire = ES_FALSE;
				busy_flag = ES_TRUE;
				eloop_log(DBG_ETIMER,"_timer_process: cb:%x, runtime:%d, timeout:%d, sig:%d \r\n",(int)cb, pcur_timer->runtime,pcur_timer->timeout,pcur_timer->sig);
				break;
			}
			else
			{
				eloop_log(DBG_ETIMER,"_timer_process: cb:%x not for timer \r\n",(int)cb);
			}
		}
		if(pcur_timer->state==TIMER_STATE_INVALID)
		{
			free_flag = ES_TRUE;
		}
		pcur_timer = pcur_timer->next;
	}
	eloop_exit_critical();
	if(cb!=NULL)
	{
		cb(sig);
	}
	if(free_flag==ES_TRUE)
	{
		eloop_timer_clean();
	}
	return busy_flag;
}
static eTimer_t* _find_timer(eTimer_t* list, void *timer_cb, u_int8 sig)
{
	eTimer_t *pcur;
	pcur = list;
	while(pcur!=NULL)
	{
		if((pcur->sig==sig))
		{
			if(pcur->ttype==TIMER_TPYE_FORTIMER)
			{
				if((void*)pcur->cb.timer_cb==(void*)timer_cb)
				{
					return pcur;
				}
			}
			else if(pcur->ttype==TIMER_TPYE_FORTASK)
			{
				if((void*)pcur->cb.task==(void*)timer_cb)
				{
					return pcur;
				}
			}
		}
		pcur = pcur->next;
	}
	return NULL;
}
static eTimer_t *_update_timer(eTimer_t *ptimer, u_int32 time, u_int8 reload)
{
	eloop_log(DBG_ETIMER,"_update_timer !\r\n");
	ptimer->runtime = time;
	ptimer->state = (reload==ES_FALSE)?TIMER_STATE_NORMAL_TIMER:TIMER_STATE_RELOAD_TIMER;
	ptimer->expire = ES_FALSE;
	ptimer->timeout = (reload==ES_FALSE)?0:time;
	return ES_SUCCEED;
}
static eTimer_t *_create_timer(void *timer_cb,s_int8 ttype, u_int8 sig, u_int32 time, u_int8 reload)
{
	eTimer_t *ptimer;
	eloop_log(DBG_ETIMER,"_create_timer !\r\n");
	ptimer = eloop_malloc(sizeof(eTimer_t));
	if(ptimer!=NULL)
	{
		eloop_memset(ptimer,0,sizeof(eTimer_t));
		_update_timer(ptimer,time,reload);
		if(ttype == TIMER_TPYE_FORTIMER)
			ptimer->cb.timer_cb = (eloop_timer_cb_t)timer_cb;
		else if(ttype == TIMER_TPYE_FORTASK)
			ptimer->cb.task = (eloop_task_t)timer_cb;
		ptimer->sig = sig;
		ptimer->ttype = ttype;
		eloop_list_insert((void**)&pTimerList,ptimer);
	}
	return ptimer;
}
static s_int32 _get_next_timeout(eTimer_t *plist)
{
	eTimer_t 	*ptimer;
	u_int32		timeout = 0xFFFFFFFF;
	ptimer = plist;
	while(ptimer!=NULL)
	{
		if((ptimer->state!=TIMER_STATE_INVALID)&&(ptimer->runtime < timeout))
		{
			timeout = ptimer->runtime;
		}
		ptimer = ptimer->next;
	}
	return timeout;
}
static eTimer_t *_get_free_timer(eTimer_t *plist)
{
	eTimer_t 	*ptimer;
	ptimer = plist;
	while(ptimer!=NULL)
	{
		eloop_log(DBG_ETIMER,"_get_free_timer: cb:%x, state:%d, runtime:%d, timeout:%d, sig:%d \r\n",(int)ptimer->cb.timer_cb,ptimer->state,ptimer->runtime,ptimer->timeout,ptimer->sig);
		if(ptimer->state==TIMER_STATE_INVALID)
		{
			return ptimer;
		}
		ptimer = ptimer->next;
	}
	return NULL;
}
static u_int8 _convert_to_sig(u_int32 event)
{
	u_int8 i;
	for(i=0;i<32;i++)
	{
		if(event&1)
		{
			break;
		}
		event>>=1;
	}
	eloop_log(0,"_convert_to_sig:sig:%d\r\n",i);
	return i;
}
static u_int32 _convert_to_event(u_int8 sig)
{
	u_int32 evt = (1<<sig);
	eloop_log(0,"_convert_to_sig:evt:%08x\r\n",evt);
	return (1<<sig);
}
