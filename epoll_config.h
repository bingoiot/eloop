/*
 * epoll_config.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_CONFIG_H_
#define _EPOLL_CONFIG_H_

#include "stdlib.h"

#define EPOLL_PLATFORM_ESP8266              0
#define EPOLL_PLATFORM_ESP32                1
#define EPOLL_PLATFORM_LINUX                0

#define EPOLL_EXTERNAL_MEM_HEAP             1
#define EPOLL_ENABLE_MICRO_DEFINE			1
#define EPOLL_ENABLE_CJSON					1
#define EPOLL_ENABLE_ECDH					0

#define EPOLL_PWRCONSERVE_LIMIT_TIME	(10)//the limit time of enter power conserve(sleep)

#define EPOLL_PRINT(...)				printf(__VA_ARGS__)
#define EPOLL_SPRINT(...)				sprintf(__VA_ARGS__)

#define DBG_ELINE			2

#define DBG_EPORT			(0)
#define DBG_ECORE			(0)
#define DBG_ETIMER			(0)
#define DBG_EERROR			(3)


#endif /* EPOLL_EPOLL_CONFIG_H_ */
