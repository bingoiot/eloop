/*
 * eloop_log.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_log.h"
#include "eloop_list.h"
#include "eloop_heap.h"

void   _PrintB(char *s,u_int8 *pdata, s_int32 len)
{
	s_int32 i;
    if(s!=NULL)
    {
    	ELOOP_PRINT("%s",s);
    }
    for(i=0;i<len;i++)
    {
    	ELOOP_PRINT("%x ",pdata[i]);
    }
    ELOOP_PRINT("\r\n");
}




