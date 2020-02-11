/*
 * efs_config.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EFS_CONFIG_H_
#define _EFS_CONFIG_H_

#include "eloop.h"

#if ELOOP_ENABLE_EFS

#define DBG_EFS			3
#define DBG_EFS_DATA	3
#define DBG_EFS_ERROR	3

#define EFS_NAME_MAX_LENGTH			128
#define EFS_DELAY_FLUSH_DB			1000

#endif




#endif /* SRC_EFS_EFS_CONFIG_H_ */
