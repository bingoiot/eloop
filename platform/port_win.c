/*
 * port_linux.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_config.h"

#if (ELOOP_PLATFORM_WINDOWS==1)

#include "stdio.h"
#include "stdlib.h"
//#include <sys/msg.h>
#include "time.h"
#include "stdarg.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>

#include<sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "eloop.h"
#include "eloop_core.h"
#include "eloop_platform.h"

static DWORD WINAPI Eloop_Thread1(LPVOID lpParam);
static DWORD WINAPI Eloop_Thread2(LPVOID lpParam);
static HANDLE myThread1 = -1;
static HANDLE myThread2 = -1;
static HANDLE myMutex = -1;

int  eloop_init(void)
{
	eloop_core_init(NULL);
	if(myMutex<0)
	{
		myMutex = CreateMutex(NULL, FALSE, NULL);
		myThread1 = CreateThread(NULL, 0, Eloop_Thread1, NULL, 0, NULL);
		myThread1 = CreateThread(NULL, 0, Eloop_Thread2, NULL, 0, NULL);
	}
	eloop_log(DBG_EPORT,"eloop_port_init:succeed !\r\n");
	return 0;
}
void  eloop_enter_critical(void)
{
	WaitForSingleObject(myMutex, INFINITE);
}
void  eloop_exit_critical(void)
{
	ReleaseMutex(myMutex);
}
void eloop_sleep(uint32 tim)
{
	Sleep(tim);
}
#if	(ELOOP_EXTERNAL_MEM_HEAP==1)

s_int32 eloop_get_free_heap(void)
{
	return 0;
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

static DWORD WINAPI Eloop_Thread1(LPVOID lpParam)
{
	LARGE_INTEGER tim;
	LARGE_INTEGER last;
	LARGE_INTEGER freq;
	uint64 ftemp1,ftemp2;
	uint32 temp;
	QueryPerformanceCounter(&last);
	eloop_log(DBG_EPORT,"Eloop_Thread1: run 1... !\r\n");
	while(1)
	{
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&tim);
		ftemp1 = (double)tim.QuadPart / (double)freq.QuadPart;
		ftemp2 = (double)last.QuadPart / (double)freq.QuadPart;
		
		ftemp1 /= 1000;
		ftemp2 /= 1000;
		temp = eloop_sub32_loop((uint32)ftemp1,(uint32)ftemp2);
		if(temp>=5)
		{
			memcpy(&last, &tim, sizeof(tim));
			eloop_update_tick(temp);//update per 2ms
		}
		Sleep(5);
	}
}
static DWORD WINAPI Eloop_Thread2(LPVOID lpParam)
{
	eloop_log(DBG_EPORT,"Eloop_Thread2: run 1... !\r\n");
	while(1)
	{
		eloop_task_poll();
		Sleep(5);
	}
}

#endif



