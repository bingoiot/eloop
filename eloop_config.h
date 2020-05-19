/*
 * eloop_config.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_CONFIG_H_
#define _ELOOP_CONFIG_H_

#include "stdlib.h"

#define ELOOP_PLATFORM_ESP8266              0
#define ELOOP_PLATFORM_ESP32                1
#define ELOOP_PLATFORM_LINUX                0

#define ELOOP_EXTERNAL_MEM_HEAP             1
#define ELOOP_ENABLE_MICRO_DEFINE			1
#define ELOOP_ENABLE_CJSON					1
#define ELOOP_ENABLE_ECDH					0

#define ELOOP_PWRCONSERVE_LIMIT_TIME	(10)//the limit time of enter power conserve(sleep)

#define ELOOP_PRINT(...)				printf(__VA_ARGS__)
#define ELOOP_SPRINT(...)				sprintf(__VA_ARGS__)

#define DBG_ELINE			2

#define DBG_EPORT			(0)
#define DBG_ECORE			(0)
#define DBG_ETIMER			(0)
#define DBG_EERROR			(3)


#endif /* ELOOP_ELOOP_CONFIG_H_ */
