/*
 * epoll_heap.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_HEAP_H_
#define _EPOLL_HEAP_H_

#include "epoll_config.h"
#include "epoll_microdef.h"

void*		epoll_malloc(s_int32 size);
void 		epoll_free(void* p);
s_int32		epoll_get_free_heap(void);


#endif /* MAIN_EPOLL_EPOLL_HEAP_H_ */
