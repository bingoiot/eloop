/*
 * eloop_lib.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef _ELOOP_LIB_H_
#define _ELOOP_LIB_H_

#include "eloop_config.h"
#include "eloop_microdef.h"

/*****memory function*********/
void*		eloop_malloc(s_int32 size);
void 		eloop_free(void* p);
u_int32		eloop_free_heap(void);

/*****random function*********/
void  		eloop_srand(u_int32 seek);
u_int32  	eloop_rand (void);
void  		eloop_ran_bytes(u_int8 *data, s_int32 len);

/********convert function********/
#define 	CONVERT_TPYE_BIG_ENDIAN			(0)
#define 	CONVERT_TPYE_LITTILE_ENDIAN		(!CONVERT_TPYE_BIG_ENDIAN)
void 		eloop_u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 conv_type);
void 		eloop_u32_to_bytes(u_int8 *pdata, u_int32 val, u_int8 conv_type);
void 		eloop_u16_to_bytes(u_int8 *pdata, u_int16 val, u_int8 conv_type);
u_int64 	eloop_bytes_to_u64(u_int8* pdata,u_int8 conv_type);
u_int32 	eloop_bytes_to_u32(u_int8* pdata,u_int8 conv_type);
u_int16 	eloop_bytes_to_u16(u_int8* pdata,u_int8 conv_type);
char  		eloop_byte2ascii(u_int8 x);
u_int8  	eloop_ascii2byte(char c);
char 		*eloop_data2str(u_int8 *pdata, s_int32 len);
s_int32 	eloop_hex2bytes(u_int8 *d,s_int32 dlen,char *s,char split);
s_int32	 	eloop_bytes2hexstr(char *s,char split,u_int8 *b, s_int32 blen);
s_int32  	eloop_atoi(char *str);
char  		*eloop_itoa(s_int32 n, char * chBuffer);
s_int32 	eloop_ftoa(char *str, float num, s_int32 n);
float 		eloop_atof(const char *str);

/********memory function************/
void 		*eloop_memcpy(void *dst, const void *src, u_int32 ulcount);
s_int32  	eloop_memcmp(void *s, void *d, s_int32 len);
void  		*eloop_memset(void *dst, s_int32 val, s_int32 ulcount);
s_int32  	eloop_mem_check_zero(void *s, s_int32 len);
void 		*eloop_memswap(void *dst, s_int32 len);


/********string function*************/
void  		eloop_strcpy(const void *d,const void *s);
void  		eloop_strcat(void *d,void *s);
s_int32  	eloop_strcmp(const char *source,const char *dest);
s_int32  	eloop_strncmp(const char *first,const char *last,s_int32 count);
s_int32  	eloop_strlen(const char * str);
s_int32  	eloop_strnlen(const char *s, s_int32 count);
s_int32  	eloop_toupper(s_int32 c);
char  		*eloop_strchr(const char *str, s_int32 ch);
char  		*eloop_strstr(char *str0,char *str1);
char  		*eloop_case_strstr(char *str0,char *str1);

/*********math function ****************/
u_int32  	eloop_sub32_abs(u_int32 s,u_int32 n);
u_int32  	eloop_sub32_loop(u_int32 s, u_int32 n);
u_int16  	eloop_get_crc16(u_int16 crc, u_int8 *pdata, int len);
u_int8	 	eloop_get_xor(u_int8 *pdata, int len);
u_int16  	eloop_get_sum16(u_int8 *pdata, int len);

#endif /* ELOOP_ELOOP_LIB_H_ */
