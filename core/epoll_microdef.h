/*
 * epoll_microdef.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_MICRODEF_H_
#define _EPOLL_MICRODEF_H_

#include "epoll_config.h"

#if	(EPOLL_ENABLE_MICRO_DEFINE)

typedef unsigned char 			u_int8;
typedef unsigned short int 		u_int16;
typedef unsigned int			u_int32;
typedef unsigned long long		u_int64;

typedef char					s_int8;
typedef short int				s_int16;
typedef int						s_int32;
typedef long long				s_int64;

typedef unsigned int			u_size;

#ifndef NULL
#define NULL					((void*)0)
#endif

#define 	uint8	u_int8
#define 	uint16	u_int16
#define		uint32	u_int32
#define		uint64	u_int64

#define 	byte	u_int8
#define		sint8	s_int8
#define		sint16	s_int16
#define		sint32	s_int32
#define		sint64	s_int64
#define		usize	u_size

#endif//(ENABLE_MICRO_DEFINE)

#define EPOLL_ASSERT(x)	\
do{\
while(1);\
}while(0);

typedef enum
{
	TASK_MESSAGE_EVENT	= 	0x80000000,
}eDefaultEvt;

typedef enum
{
	ES_SUCCEED			=0,
	ES_FAILED			=1,
	ES_TRUE				=1,
	ES_FALSE			=0,
	ES_MEM_ERROR		=1,
	ES_TIMEOUT			=2,
}eSTATE;

typedef enum
{
	ES_TASK_READY		=1,
	ES_TASK_RUN			=2,
	ES_TASK_PAUSE		=3,
	ES_TASK_RESUME		=4,
	ES_TASK_STOP		=5,
	ES_TASK_BUSSY		=6,
	ES_TASK_IDLE		=7
}eTASK_STATUS;

typedef void 	(*epoll_timer_cb_t)(u_int8 sig);
typedef void 	(*epoll_task_t)(u_int32 evt, void *arg, u_int32 len);
typedef u_int8	(*epoll_power_conserve_t)(u_int32 tim);

typedef struct
{
	void 		*next;
	void 		*pdata;
	u_int32		len;
	u_int8		msg_type;//0:Reference, 1:memory copy
}eMessage_t;

typedef struct
{
	u_int32		start;
	u_int32		timeout;
}eTMeasure_t;

typedef struct
{
	void *next;
}eList_t;

typedef struct
{
	void*			next;
	epoll_task_t 	task;
	eMessage_t		*pmsg;
	u_int32			event;
	u_int8 			priority;
	u_int8			state;//0 invalid,1:stop,2:pause,3:running
}eTStack_t;

typedef struct
{
	int 	wday;
    uint16  year;
    uint8   mon;
    uint8   day;

    uint8   hour;
    uint8   min;
    uint8   sec;
    uint8	rstat;
} eTime_t;


#endif /* EPOLL_EPOLL_MICRODEF_H_ */
