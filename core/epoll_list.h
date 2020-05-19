/*
 * epoll_list.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_LIST_H_
#define _EPOLL_LIST_H_

#include "epoll_config.h"
#include "epoll_microdef.h"

s_int8 	epoll_list_insert( void **list, void *msg);
s_int8	epoll_list_insert_front(void **list, void *msg);
void 	*epoll_list_detach(void **list);
void 	*epoll_list_detach_front(void **list);
s_int8 	epoll_list_delete(void **list, void *msg);
s_int8 	epoll_list_delete_list(void **list);


#endif /* EPOLL_EPOLL_LIST_H_ */
