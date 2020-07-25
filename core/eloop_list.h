/*
 * eloop_list.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_LIST_H_
#define _ELOOP_LIST_H_


#ifdef __cplusplus
extern "C"
{
#endif


#include "eloop_config.h"
#include "eloop_microdef.h"

	s_int8 	eloop_list_insert(void** list, void* msg);
	s_int8	eloop_list_insert_front(void** list, void* msg);
	void* eloop_list_detach(void** list);
	void* eloop_list_detach_front(void** list);
	s_int8 	eloop_list_delete(void** list, void* msg);
	s_int8 	eloop_list_delete_list(void** list);


#ifdef __cplusplus
}
#endif


#endif /* ELOOP_ELOOP_LIST_H_ */
