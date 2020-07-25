/*
 * eloop_log.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_LOG_H_
#define _ELOOP_LOG_H_


#ifdef __cplusplus
extern "C"
{
#endif


#include "eloop_config.h"
#include "eloop_microdef.h"
#include "stdio.h"
#include "string.h"

	void   _PrintB(char* s, u_int8* pdata, s_int32 len);


#define eloop_log(fmt,...)\
		do{ \
			if(fmt&DBG_ELINE)\
				{ELOOP_PRINT("%s,%s,%d:",(char*)(strrchr(__FILE__,'/')+1) ,(char*)__FUNCTION__ ,__LINE__ );}\
			if(fmt)\
				{ELOOP_PRINT(__VA_ARGS__);}\
		}while(0)

#define eloop_logb(fmt,...)\
		do{ \
			if(fmt&DBG_ELINE)\
				{ELOOP_PRINT("%s,%s,%d:",(char*)(strrchr(__FILE__,'/')+1) ,(char*)__FUNCTION__ ,__LINE__ );}\
			if(fmt)		\
				{_PrintB(__VA_ARGS__);}\
		}while(0)


#ifdef __cplusplus
}
#endif


#endif /* MAIN_ELOOP_ELOOP_LOG_H_ */
