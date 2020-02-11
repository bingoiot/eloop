/*
 * eloop_port.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include <netdb.h>
#include <sys/socket.h>
//#include <sys/msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "time.h"
#include "sys/time.h"
#include "pthread.h"
#include "stdarg.h"
#include <string.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include<sys/wait.h>
#include <string.h>
#include <sys/sysinfo.h>

#include "eloop.h"
#include "eloop_port.h"

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
	}
	eloop_log(DBG_EPORT,"eloop_port_init:succeed !\r\n");
	return ret;
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
		usleep(5000);
	}
}
static void  *Eloop_Thread2(void *arg)
{
	eloop_log(DBG_EPORT,"Eloop_Thread2: run 1... !\r\n");
	while(1)
	{
		eloop_task_poll();
		usleep(5000);
	}
}

void  eloop_enter_critical(void)
{
	pthread_mutex_lock(&myMutex);
}
void  eloop_exit_critical(void)
{
	pthread_mutex_unlock(&myMutex);
}
void eloop_sleep(uint32 tim)
{
	usleep(tim/1000);
}


#if	(ELOOP_EXTERNAL_MEM_HEAP==1)

s_int32 eloop_get_free_heap(void)
{
	struct sysinfo si;
	sysinfo(&si);
	eloop_log(DBG_EPORT,"Totalram:%d\n", (int)si.totalram);
	eloop_log(DBG_EPORT,"Available:%d\n", (int)si.freeram);
	return si.freeram;
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


