/*
 * epoll_log.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "epoll_microdef.h"
#include "epoll_log.h"
#include "epoll_list.h"
#include "epoll_heap.h"

void   _PrintB(char *s,u_int8 *pdata, s_int32 len)
{
	s_int32 i;
    if(s!=NULL)
    {
    	EPOLL_PRINT("%s",s);
    }
    for(i=0;i<len;i++)
    {
    	EPOLL_PRINT("%x ",pdata[i]);
    }
    EPOLL_PRINT("\r\n");
}




