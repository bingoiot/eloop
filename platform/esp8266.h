/*
 * eloop_port.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef MAIN_ELOOP_ELOOP_PORT_H_
#define MAIN_ELOOP_ELOOP_PORT_H_

#include "eloop_config.h"

#if (ELOOP_PLATFORM_ESP8266==1)

#include "eloop_config.h"
#include "eloop_microdef.h"

int		    eloop_init(void);
void  	    eloop_enter_critical(void);
void  	    eloop_exit_critical(void);
void*		eloop_malloc(s_int32 size);
void 		eloop_free(void* p);
s_int32		eloop_get_free_heap(void);

#endif

#endif /* MAIN_ELOOP_ELOOP_PORT_H_ */