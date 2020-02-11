/*
 * eloop_heap.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_heap.h"
#include "eloop_port.h"
#include "stdlib.h"

#if (ELOOP_EXTERNAL_MEM_HEAP==0)

void *eloop_malloc(s_int32 size)
{
	return malloc(size);
}
void eloop_free(void* p)
{
	free(p);
}
s_int32	eloop_get_free_heap(void)
{
	return 100;
}

#endif





