/*
 * epoll_log.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_LOG_H_
#define _EPOLL_LOG_H_

#include "epoll_config.h"
#include "epoll_microdef.h"
#include "stdio.h"
#include "string.h"

void   _PrintB(char *s,u_int8 *pdata, s_int32 len);


#define epoll_log(fmt,...)\
		do{ \
			if(fmt&DBG_ELINE)\
				{EPOLL_PRINT("%s,%s,%d:",(char*)(strrchr(__FILE__,'/')+1) ,(char*)__FUNCTION__ ,__LINE__ );}\
			if(fmt)\
				{EPOLL_PRINT(__VA_ARGS__);}\
		}while(0)

#define epoll_logb(fmt,...)\
		do{ \
			if(fmt&DBG_ELINE)\
				{EPOLL_PRINT("%s,%s,%d:",(char*)(strrchr(__FILE__,'/')+1) ,(char*)__FUNCTION__ ,__LINE__ );}\
			if(fmt)		\
				{_PrintB(__VA_ARGS__);}\
		}while(0)

#endif /* MAIN_EPOLL_EPOLL_LOG_H_ */
