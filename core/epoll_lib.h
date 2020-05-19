/*
 * epoll_lib.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _EPOLL_LIB_H_
#define _EPOLL_LIB_H_

#include "epoll_config.h"
#include "epoll_microdef.h"

/*****memory function*********/
void*		epoll_malloc(s_int32 size);
void 		epoll_free(void* p);
u_int32		epoll_free_heap(void);

/*****random function*********/
void  		epoll_srand(u_int32 seek);
u_int32  	epoll_rand (void);
void  		epoll_ran_bytes(u_int8 *data, s_int32 len);

/********convert function********/
#define 	CONVERT_TPYE_BIG_ENDIAN			(0)
#define 	CONVERT_TPYE_LITTILE_ENDIAN		(!CONVERT_TPYE_BIG_ENDIAN)
void 		epoll_u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 conv_type);
void 		epoll_u32_to_bytes(u_int8 *pdata, u_int32 val, u_int8 conv_type);
void 		epoll_u16_to_bytes(u_int8 *pdata, u_int16 val, u_int8 conv_type);
u_int64 	epoll_bytes_to_u64(u_int8* pdata,u_int8 conv_type);
u_int32 	epoll_bytes_to_u32(u_int8* pdata,u_int8 conv_type);
u_int16 	epoll_bytes_to_u16(u_int8* pdata,u_int8 conv_type);
char  		epoll_byte2ascii(u_int8 x);
u_int8  	epoll_ascii2byte(char c);
char 		*epoll_data2str(u_int8 *pdata, s_int32 len);
s_int32 	epoll_hex2bytes(u_int8 *d,s_int32 dlen,char *s,char split);
s_int32	 	epoll_bytes2hexstr(char *s,char split,u_int8 *b, s_int32 blen);
s_int32  	epoll_atoi(char *str);
char  		*epoll_itoa(s_int32 n, char * chBuffer);
s_int32 	epoll_ftoa(char *str, float num, s_int32 n);
float 		epoll_atof(const char *str);

/********memory function************/
void 		*epoll_memcpy(void *dst, const void *src, u_int32 ulcount);
s_int32  	epoll_memcmp(void *s, void *d, s_int32 len);
void  		*epoll_memset(void *dst, s_int32 val, s_int32 ulcount);
s_int32  	epoll_mem_check_zero(void *s, s_int32 len);
void 		*epoll_memswap(void *dst, s_int32 len);


/********string function*************/
void  		epoll_strcpy(const void *d,const void *s);
void  		epoll_strcat(void *d,void *s);
s_int32  	epoll_strcmp(const char *source,const char *dest);
s_int32  	epoll_strncmp(const char *first,const char *last,s_int32 count);
s_int32  	epoll_strlen(const char * str);
s_int32  	epoll_strnlen(const char *s, s_int32 count);
s_int32  	epoll_toupper(s_int32 c);
char  		*epoll_strchr(const char *str, s_int32 ch);
char  		*epoll_strstr(char *str0,char *str1);
char  		*epoll_case_strstr(char *str0,char *str1);

/*********math function ****************/
u_int32  	epoll_sub32_abs(u_int32 s,u_int32 n);
u_int32  	epoll_sub32_loop(u_int32 s, u_int32 n);
u_int16  	epoll_get_crc16(u_int16 crc, u_int8 *pdata, int len);
u_int8	 	epoll_get_xor(u_int8 *pdata, int len);
u_int16  	epoll_get_sum16(u_int8 *pdata, int len);

#endif /* EPOLL_EPOLL_LIB_H_ */
