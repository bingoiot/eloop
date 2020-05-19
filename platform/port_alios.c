/*
 * port_esp8266.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */
#include "eloop_config.h"

#if (ELOOP_PLATFORM_ESP32==1)

#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "esp_err.h"
#include "esp_attr.h"
#include "sys/queue.h"

#include "aos/cli.h"
#include "aos/kernel.h"
#include "ulog/ulog.h"
#include "aos/yloop.h"

#ifdef CONFIG_PRINT_HEAP
#include "k_mm.h"
#endif

#include "eloop.h"
#include "eloop_core.h"
#include "eloop_platform.h"

static aos_mutex_t  myMutex;
static void  Eloop_Thread1(void *arg);

int  eloop_init(void)
{
	pthread_t id;
	eloop_core_init(NULL);
	int ret = aos_mutex_new(&myMutex);
	if(ret<0)
		eloop_log(DBG_EPORT,"eloop_port_init:aos_mutex_new error !\r\n");
	else
	{
		ret = aos_task_new("eloop_thread",Eloop_Thread1,NULL,4096);
		eloop_log(DBG_EPORT,"eloop_port_init: aos_mutex_new succeed !\r\n");
	}
	return ret;
}

void eloop_sleep(uint32 tim)
{
	aos_msleep(tim);
}

#if	(ELOOP_EXTERNAL_MEM_HEAP==1)

s_int32 eloop_get_free_heap(void)
{
	int free = 0;
#ifdef CONFIG_PRINT_HEAP
    extern k_mm_head *g_kmm_head;
    free = g_kmm_head->free_size;
#endif
	return free;
}
void *eloop_malloc(s_int32 size)
{
	return aos_malloc(size);
}
void eloop_free(void* p)
{
	aos_free(p);
}

#endif

void  eloop_enter_critical(void)
{
	eloop_log(0,"pthread_mutex_lock\r\n");
	aos_mutex_lock(&myMutex,AOS_WAIT_FOREVER);
}
void  eloop_exit_critical(void)
{
	eloop_log(0,"pthread_mutex_unlock\r\n");
	aos_mutex_unlock(&myMutex);
}
extern void esp_task_wdt_reset(void);
static void  Eloop_Thread1(void *arg)
{
	static uint32 last = 0;
	uint32	temp,now;
	eloop_log(DBG_EPORT,"Eloop_Thread1: run 1... !\r\n");
	while(1)
	{
		now = aos_now_ms();
		temp = eloop_sub32_loop(now,last);
		last = now;
		eloop_update_tick(temp);//update per 2ms
		temp = 5;
		while(temp--)
		{
			if(eloop_task_poll()==ES_TASK_IDLE)
			{
				break;
			}
			esp_task_wdt_reset();
		}
		eloop_sleep(10);
	}
}

#endif



