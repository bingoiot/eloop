/*
 * eloop_example.c
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

#if(1)

#define DBG_APP_TIMER	1
#define DBG_APP_MSG		1
#define DBG_APP_EVT		1


typedef enum{
	TASK_TIMEOUT_EVENT = 1
}TAKK_ENUM;
static void StartTest(void);
static void DumpHeapMem(void);

static void  *Test_Thread1(void *arg);
static void  *Test_Thread2(void *arg);
static void  *Test_Thread3(void *arg);
static void  *Test_Thread4(void *arg);

static void Test_Task1(u_int32 evt, void *arg, u_int32 len);
static void Test_Task2(u_int32 evt, void *arg, u_int32 len);
static void Test_Task3(u_int32 evt, void *arg, u_int32 len);
static void Test_Task4(u_int32 evt, void *arg, u_int32 len);
static void Test_Timer1(u_int8 signal);
static void Test_Timer2(u_int8 signal);
static void Test_Timer3(u_int8 signal);
static void Test_Timer4(u_int8 signal);

const char *test_string1="aacdefghigklmnopqrstuvwxyz0123456789!@#$%^&*()_+";
const char *test_string2="bbcdefghigklmnopqrstuvwxyz0123456789!@#$%^&*()_+";
void eloop_exampe_init(void)
{
	pthread_t id0,id1,id2,id3;
	eloop_init();
	DumpHeapMem();
	eloop_create_task(Test_Task1,100);
	eloop_create_task(Test_Task2,100);
	eloop_create_task(Test_Task3,101);
	eloop_create_task(Test_Task4,102);
	pthread_create(&id0,NULL,Test_Thread1,NULL);
	pthread_create(&id1,NULL,Test_Thread2,NULL);
	pthread_create(&id2,NULL,Test_Thread3,NULL);
	pthread_create(&id3,NULL,Test_Thread4,NULL);
}
void examp_app_sleep(int tim)
{
	eloop_log(1,"examp_app_sleep:%d \r\n",tim);
	usleep(tim*1000);
}
static void StartTest(void)
{
	// *******************test 1**********************************
	eloop_send_msg(Test_Task1,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_msg(Test_Task1,(void*)test_string2,strlen(test_string2)+1);
	eloop_send_stream(Test_Task1,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_stream_front(Test_Task1,(void*)test_string2,strlen(test_string2)+1);

	eloop_set_event(Test_Task1,1);
	eloop_set_event(Test_Task1,2);
	eloop_set_event(Test_Task1,4);
	eloop_clear_event(Test_Task1,2);

	eloop_start_timer_task(Test_Task1,110,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task1,112,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task1,119,10000,ES_TRUE);

	eloop_start_timer(Test_Timer1,110,100,ES_TRUE);
	eloop_start_timer(Test_Timer1,112,1000,ES_TRUE);
	eloop_start_timer(Test_Timer1,119,10000,ES_TRUE);
	// *******************test 2**********************************
	eloop_send_msg(Test_Task2,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_msg(Test_Task2,(void*)test_string2,strlen(test_string2)+1);
	eloop_send_stream(Test_Task2,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_stream_front(Test_Task2,(void*)test_string2,strlen(test_string2)+1);

	eloop_set_event(Test_Task2,1);
	eloop_set_event(Test_Task2,2);
	eloop_set_event(Test_Task2,4);
	eloop_clear_event(Test_Task2,2);

	eloop_start_timer_task(Test_Task2,110,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task2,112,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task2,119,10000,ES_TRUE);

	eloop_start_timer(Test_Timer2,110,1000,ES_TRUE);
	eloop_start_timer(Test_Timer2,112,1000,ES_TRUE);
	eloop_start_timer(Test_Timer2,119,10000,ES_TRUE);
	// *******************test 3**********************************
	eloop_send_msg(Test_Task3,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_msg(Test_Task3,(void*)test_string2,strlen(test_string2)+1);
	eloop_send_stream(Test_Task3,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_stream_front(Test_Task3,(void*)test_string2,strlen(test_string2)+1);

	eloop_set_event(Test_Task3,1);
	eloop_set_event(Test_Task3,2);
	eloop_set_event(Test_Task3,4);
	eloop_clear_event(Test_Task3,2);

	eloop_start_timer_task(Test_Task3,110,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task3,112,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task3,119,10000,ES_TRUE);

	eloop_start_timer(Test_Timer3,110,1000,ES_TRUE);
	eloop_start_timer(Test_Timer3,112,1000,ES_TRUE);
	eloop_start_timer(Test_Timer3,119,1000,ES_TRUE);

	// *******************test 4**********************************
	eloop_send_msg(Test_Task4,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_msg(Test_Task4,(void*)test_string2,strlen(test_string2)+1);
	eloop_send_stream(Test_Task4,(void*)test_string1,strlen(test_string1)+1);
	eloop_send_stream_front(Test_Task4,(void*)test_string2,strlen(test_string2)+1);

	eloop_set_event(Test_Task4,1);
	eloop_set_event(Test_Task4,2);
	eloop_set_event(Test_Task4,4);
	eloop_clear_event(Test_Task4,2);

	eloop_start_timer_task(Test_Task4,110,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task4,112,1000,ES_TRUE);
	eloop_start_timer_task(Test_Task4,119,10000,ES_TRUE);

	eloop_start_timer(Test_Timer4,110,1000,ES_TRUE);
	eloop_start_timer(Test_Timer4,112,10000,ES_TRUE);
	eloop_start_timer(Test_Timer4,119,10000,ES_TRUE);
}
static void  *Test_Thread1(void *arg)
{
	eloop_log(1,"Test_Thread1 start \r\n");
	eloop_start_timer_task(Test_Task1,10,1000,ES_FALSE);
	eloop_start_timer(Test_Timer1,10,1000,ES_FALSE);
	while(1)
	{
		StartTest();
		usleep(1000000);
	}
}
static void  *Test_Thread2(void *arg)
{
	eloop_log(1,"Test_Thread2 start \r\n");
	eloop_start_timer_task(Test_Task2,10,1000,ES_FALSE);
	eloop_start_timer(Test_Timer2,10,1000,ES_FALSE);
	sleep(5);
	while(1)
	{
		StartTest();
		usleep(1000000);
	}
}
static void  *Test_Thread3(void *arg)
{
	eloop_log(1,"Test_Thread3 start \r\n");
	eloop_start_timer_task(Test_Task3,10,1000,ES_FALSE);
	eloop_start_timer(Test_Timer3,10,1000,ES_FALSE);
	sleep(5);
	while(1)
	{
		StartTest();
		usleep(1000000);
	}
}
static void  *Test_Thread4(void *arg)
{
	eloop_log(1,"Test_Thread4 start \r\n");
	eloop_start_timer_task(Test_Task4,10,1000,ES_FALSE);
	eloop_start_timer(Test_Timer4,10,1000,ES_FALSE);
	sleep(5);
	while(1)
	{
		StartTest();
		usleep(1000000);
	}
}
static void Test_Task1(u_int32 evt, void *arg, u_int32 len)
{
	if(evt&TASK_TIMEOUT_EVENT)//time out event
	{
		eloop_log(DBG_APP_TIMER,"Test_Task1,times expire:%d\r\n",eloop_get_timestamp());
	}
	if(evt&TASK_MESSAGE_EVENT)//messaget event
	{
		DumpHeapMem();
		eloop_log(DBG_APP_MSG,"Test_Task1:Recieve message:%08x, %s\r\n",(int)arg,(u_int8*)arg);
	}
	eloop_log(DBG_APP_EVT,"Test_Task1 event:%08x \r\n",evt);
}
static void Test_Task2(u_int32 evt, void *arg, u_int32 len)
{
	if(evt&TASK_TIMEOUT_EVENT)//time out event
	{
		eloop_log(DBG_APP_TIMER,"Test_Task2,times expire:%d\r\n",eloop_get_timestamp());
	}
	if(evt&TASK_MESSAGE_EVENT)//messaget event
	{
		DumpHeapMem();
		eloop_log(DBG_APP_MSG,"Test_Task2:Recieve message:%08x,%s\r\n",(int)arg,(u_int8*)arg);
	}
	eloop_log(DBG_APP_EVT,"Test_Task2 event:%08x \r\n",evt);
}
static void Test_Task3(u_int32 evt, void *arg, u_int32 len)
{
	if(evt&TASK_TIMEOUT_EVENT)//time out event
	{
		eloop_log(DBG_APP_TIMER,"Test_Task3,times expire:%d\r\n",eloop_get_timestamp());
	}
	if(evt&TASK_MESSAGE_EVENT)//messaget event
	{
		DumpHeapMem();
		eloop_log(DBG_APP_MSG,"Test_Task3:Recieve message:%08x, %s\r\n",(int)arg,(u_int8*)arg);
	}
	eloop_log(DBG_APP_EVT,"Test_Task3 event:%08x \r\n",evt);
}
static void Test_Task4(u_int32 evt, void *arg, u_int32 len)
{
	if(evt&TASK_TIMEOUT_EVENT)//time out event
	{
		eloop_log(DBG_APP_TIMER,"Test_Task4,times expire:%d\r\n",eloop_get_timestamp());
	}
	if(evt&TASK_MESSAGE_EVENT)//messaget event
	{
		DumpHeapMem();
		eloop_log(DBG_APP_MSG,"Test_Task4:Recieve message:%08x, %s\r\n",(int)arg,(u_int8*)arg);
	}
	eloop_log(DBG_APP_EVT,"Test_Task4 event:%08x \r\n",evt);
}
static void Test_Timer1(u_int8 signal)
{
	eloop_log(DBG_APP_TIMER,"Test_Timer1,times expire:%d sig:%d\r\n",eloop_get_timestamp(),signal);
}
static void Test_Timer2(u_int8 signal)
{
	eloop_log(DBG_APP_TIMER,"Test_Timer2,times expire:%d sig:%d\r\n",eloop_get_timestamp(),signal);
}
static void Test_Timer3(u_int8 signal)
{
	eloop_log(DBG_APP_TIMER,"Test_Timer3,times expire:%d sig:%d\r\n",eloop_get_timestamp(),signal);
}
static void Test_Timer4(u_int8 signal)
{
	eloop_log(DBG_APP_TIMER,"Test_Timer4,times expire:%d sig:%d\r\n",eloop_get_timestamp(),signal);
}
static void DumpHeapMem(void)
{
	eloop_log(DBG_APP_MSG,"eloop_exampe_init: free heap:%fMBytes \r\n",eloop_get_free_heap()/(1024*1024.0));
}
#endif

