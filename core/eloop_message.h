/*
 * eloop_message.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_MESSAGE_H_
#define _ELOOP_MESSAGE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "eloop_config.h"
#include "eloop_microdef.h"

	typedef enum
	{
		MSG_TYPE_REFRENCE = 0,
		MSG_TYPE_COPY_STREAM = 1,
	}ELOOP_TYPE;

	s_int8	eloop_send_msg(eloop_task_t task, void* msg, s_int32 len);
	s_int8	eloop_send_msg_front(eloop_task_t task, void* msg, s_int32 len);
	s_int8	eloop_send_stream(eloop_task_t task, void* msg, s_int32 len);
	s_int8	eloop_send_stream_front(eloop_task_t task, void* msg, s_int32 len);


#ifdef __cplusplus
}
#endif

#endif /* ELOOP_ELOOP_MESSAGE_H_ */
