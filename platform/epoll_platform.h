/*
 * epoll_platform.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef __EPOLL_PLATFORM_H__
#define __EPOLL_PLATFORM_H__

#include "epoll_config.h"

#include "epoll_config.h"
#include "epoll_microdef.h"

int		    epoll_init(void);
void  	    epoll_enter_critical(void);
void  	    epoll_exit_critical(void);
void*		epoll_malloc(s_int32 size);
void 		epoll_free(void* p);
s_int32		epoll_get_free_heap(void);

#endif /* MAIN_EPOLL_EPOLL_PORT_H_ */
