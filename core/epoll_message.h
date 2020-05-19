/*
 * epoll_message.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_MESSAGE_H_
#define _EPOLL_MESSAGE_H_

#include "epoll_config.h"
#include "epoll_microdef.h"

typedef enum
{
	MSG_TYPE_REFRENCE		=0,
	MSG_TYPE_COPY_STREAM	=1,
}EPOLL_TYPE;

s_int8	epoll_send_msg(epoll_task_t task, void *msg, s_int32 len);
s_int8	epoll_send_msg_front(epoll_task_t task, void *msg, s_int32 len);
s_int8	epoll_send_stream(epoll_task_t task, void *msg, s_int32 len);
s_int8	epoll_send_stream_front(epoll_task_t task, void *msg, s_int32 len);


#endif /* EPOLL_EPOLL_MESSAGE_H_ */
