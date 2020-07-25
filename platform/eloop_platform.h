/*
 * eloop_platform.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef __ELOOP_PLATFORM_H__
#define __ELOOP_PLATFORM_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "eloop_config.h"

#include "eloop_config.h"
#include "eloop_microdef.h"

	int		    eloop_init(void);
	void  	    eloop_enter_critical(void);
	void  	    eloop_exit_critical(void);
	void* eloop_malloc(s_int32 size);
	void 		eloop_free(void* p);
	s_int32		eloop_get_free_heap(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_ELOOP_ELOOP_PORT_H_ */
