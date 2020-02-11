/*
 * eloop_heap.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_HEAP_H_
#define _ELOOP_HEAP_H_

#include "eloop_config.h"
#include "eloop_microdef.h"

void*		eloop_malloc(s_int32 size);
void 		eloop_free(void* p);
s_int32		eloop_get_free_heap(void);


#endif /* MAIN_ELOOP_ELOOP_HEAP_H_ */
