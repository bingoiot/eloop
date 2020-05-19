/*
 * epoll_heap.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "epoll_microdef.h"
#include "epoll_heap.h"
#include "stdlib.h"
#include "epoll.h"

#if (EPOLL_EXTERNAL_MEM_HEAP==0)

void *epoll_malloc(s_int32 size)
{
	return malloc(size);
}
void epoll_free(void* p)
{
	free(p);
}
s_int32	epoll_get_free_heap(void)
{
	return 100;
}

#endif





