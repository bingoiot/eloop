/*
 * port_esp8266.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */
#include "eloop_config.h"

#if (ELOOP_PLATFORM_ESP8266==1)

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

#include "eloop.h"
#include "port_esp8266.h"

static pthread_mutex_t myMutex;
static void  *Eloop_Thread1(void *arg);
static void  *Eloop_Thread2(void *arg);

int  eloop_init(void)
{
	pthread_t id;
	int ret = pthread_mutex_init(&myMutex,NULL);
	if(ret<0)
		eloop_log(DBG_EPORT,"eloop_port_init:pthread_mutex_init error !\r\n");
	else
	{
		ret = pthread_create(&id,NULL,Eloop_Thread1,NULL);
		ret = pthread_create(&id,NULL,Eloop_Thread2,NULL);
		//xTaskCreate(Eloop_Thread1, "Eloop_Thread1", 1024, NULL, (tskIDLE_PRIORITY + 3), NULL);
		//xTaskCreate(Eloop_Thread2, "Eloop_Thread2", (1024*6), NULL, (tskIDLE_PRIORITY + 3), NULL);
	}
	eloop_log(DBG_EPORT,"eloop_port_init:succeed !\r\n");
	return ret;
}

void eloop_sleep(uint32 tim)
{
	vTaskDelay(tim/portTICK_PERIOD_MS);
}

#if	(ELOOP_EXTERNAL_MEM_HEAP==1)

s_int32 eloop_get_free_heap(void)
{
	return esp_get_free_heap_size();
}
void *eloop_malloc(s_int32 size)
{
	return malloc(size);
}
void eloop_free(void* p)
{
	free(p);
}

#endif

void  eloop_enter_critical(void)
{
	pthread_mutex_lock(&myMutex);
}
void  eloop_exit_critical(void)
{
	pthread_mutex_unlock(&myMutex);
}

static void  *Eloop_Thread1(void *arg)
{
	struct timeval tv;
	struct timezone tz;
	u_int32 tim;
	u_int32 temp;
	u_int32 last;
	gettimeofday(&tv,&tz);
	last = (tv.tv_usec/1000)+(tv.tv_sec*1000);
	eloop_log(DBG_EPORT,"Eloop_Thread1: run 1... !\r\n");
	while(1)
	{
		gettimeofday(&tv,&tz);
		tim = (tv.tv_usec/1000)+(tv.tv_sec*1000);
		temp = eloop_sub32_loop(tim,last);
		if(temp>=5)
		{
			last = tim ;
			eloop_update_tick(temp);//update per 2ms
		}
		eloop_sleep(10);
	}
	return NULL;
}
static void  *Eloop_Thread2(void *arg)
{
	eloop_log(DBG_EPORT,"Eloop_Thread2: run 1... !\r\n");
	while(1)
	{
		if(eloop_task_poll()==ES_TASK_IDLE)
		{
			eloop_sleep(10);
		}
	}
	return NULL;
}

#endif



