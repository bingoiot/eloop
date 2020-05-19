/*
 * port_esp8266.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */
#include "epoll_config.h"

#if (EPOLL_PLATFORM_ESP8266==1)

#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include "esp_err.h"
#include "esp_attr.h"
#include "sys/queue.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_pthread.h"
#include "esp_system.h"

#include "epoll.h"
#include "epoll_core.h"
#include "epoll_platform.h"

static pthread_mutex_t myMutex;
static void  *Eloop_Thread1(void *arg);

int  epoll_init(void)
{
	pthread_t id;
	epoll_core_init(NULL);
	int ret = pthread_mutex_init(&myMutex,NULL);
	if(ret<0)
		epoll_log(DBG_EPORT,"epoll_port_init:pthread_mutex_init error !\r\n");
	else
	{
		ret = pthread_create(&id,NULL,Eloop_Thread1,NULL);
		epoll_log(DBG_EPORT,"epoll_port_init:succeed !\r\n");
	}
	return ret;
}

void epoll_sleep(uint32 tim)
{
	vTaskDelay(tim/portTICK_PERIOD_MS);
}

#if	(EPOLL_EXTERNAL_MEM_HEAP==1)

s_int32 epoll_get_free_heap(void)
{
	return esp_get_free_heap_size();
}
void *epoll_malloc(s_int32 size)
{
	return malloc(size);
}
void epoll_free(void* p)
{
	free(p);
}

#endif

void  epoll_enter_critical(void)
{
	epoll_log(0,"pthread_mutex_lock\r\n");
	pthread_mutex_lock(&myMutex);
}
void  epoll_exit_critical(void)
{
	epoll_log(0,"pthread_mutex_unlock\r\n");
	pthread_mutex_unlock(&myMutex);
}
extern void esp_task_wdt_reset(void);
static void  *Eloop_Thread1(void *arg)
{
	static uint32 last = 0;
	uint32	temp,now;
	epoll_log(DBG_EPORT,"Eloop_Thread1: run 1... !\r\n");
	while(1)
	{
		now = xTaskGetTickCount();
		temp = epoll_sub32_loop(now,last)*portTICK_PERIOD_MS;
		last = now;
		epoll_update_tick(temp);//update per 2ms
		temp = 5;
		while(temp--)
		{
			if(epoll_task_poll()==ES_TASK_IDLE)
			{
				break;
			}
			esp_task_wdt_reset();
		}
		epoll_sleep(10);
	}
	return NULL;
}

#endif



