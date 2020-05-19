/*
 * epoll.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef EPOLL_EPOLL_H_
#define EPOLL_EPOLL_H_

#include "epoll_config.h"
#include "epoll_microdef.h"
#include "epoll_log.h"

/***************************epoll core funtion*************************************************/

extern int		    epoll_init(void);
extern void  	    epoll_enter_critical(void);
extern void  	    epoll_exit_critical(void);
extern void 		epoll_sleep(uint32 tim);

extern void 		epoll_set_power_conserve(u_int8 enable);
extern s_int8 		epoll_create_task(epoll_task_t task,u_int8 priority);
extern s_int8 		epoll_delete_task(epoll_task_t task);
extern void 		epoll_update_tick(u_int32 tick);
extern s_int8 		epoll_task_poll(void);
extern s_int8 		epoll_set_event(epoll_task_t task, u_int32 event);
extern s_int8 		epoll_clear_event(epoll_task_t task, u_int32 event);

extern void*		epoll_malloc(s_int32 size);
extern void 		epoll_free(void* p);
extern s_int32		epoll_get_free_heap(void);

extern s_int8 		epoll_list_insert(void **list, void *msg);
extern s_int8 		epoll_list_insert_front(void **list, void *msg);
extern void 		*epoll_list_detach(void **list);
extern void 		*epoll_list_detach_front(void **list);
extern s_int8 		epoll_list_delete(void **list, void *msg);
extern s_int8 		epoll_list_delete_list(void **list);

extern s_int8		epoll_send_msg(epoll_task_t task, void *msg, s_int32 len);
extern s_int8		epoll_send_msg_front(epoll_task_t task, void *msg, s_int32 len);
extern s_int8		epoll_send_stream(epoll_task_t task, void *msg, s_int32 len);
extern s_int8		epoll_send_stream_front(epoll_task_t task, void *msg, s_int32 len);

extern s_int8 		epoll_start_timer(epoll_timer_cb_t timer_cb, u_int8 signal, u_int32 time, u_int8 reload);
extern s_int8 		epoll_start_timer_task(epoll_task_t task,u_int32 event, u_int32 time, u_int8 reload);
extern s_int8 	    epoll_stop_timer(epoll_timer_cb_t timer_cb,u_int8 signal);
extern s_int8       epoll_stop_timer_task(epoll_task_t timer_cb,u_int32 event);
extern s_int32 		epoll_get_next_timeout(void);
extern u_int32 		epoll_get_timestamp(void);
extern u_int32      epoll_get_unixtime(void);
extern void         epoll_set_unixtime(u_int32 tim);
extern s_int8 		epoll_timer_start_measure(eTMeasure_t *pm,u_int32 timeout);
extern s_int8 		epoll_timer_expire(eTMeasure_t *pm);
extern u_int64          epoll_local_time_to_unix(int zone,eTime_t *date_time);
extern int              epoll_unix_to_local_time (int zone, eTime_t *tp,u_int64 t);

/********************************epoll lib function********************************************/
/*****random function*********/
extern void  		epoll_srand(u_int32 seek);
extern u_int32  	epoll_rand (void);
extern void  		epoll_ran_bytes(u_int8 *data, s_int32 len);

/********convert function********/
#define 	CONVERT_TPYE_BIG_ENDIAN			(0)
#define 	CONVERT_TPYE_LITTILE_ENDIAN		(!CONVERT_TPYE_BIG_ENDIAN)
extern void 		epoll_u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 conv_type);
extern void 		epoll_u32_to_bytes(u_int8 *pdata, u_int32 val, u_int8 conv_type);
extern void 		epoll_u16_to_bytes(u_int8 *pdata, u_int16 val, u_int8 conv_type);
extern u_int64 		epoll_bytes_to_u64(u_int8* pdata,u_int8 conv_type);
extern u_int32 		epoll_bytes_to_u32(u_int8* pdata,u_int8 conv_type);
extern u_int16 		epoll_bytes_to_u16(u_int8* pdata,u_int8 conv_type);
extern char  		epoll_byte2ascii(u_int8 x);
extern u_int8  		epoll_ascii2byte(char c);
extern char*		epoll_data2str(u_int8 *pdata, s_int32 len);
extern s_int32 		epoll_hex2bytes(u_int8 *d,s_int32 dlen,char *s,char split);
extern s_int32	 	epoll_bytes2hexstr(char *s,char split,u_int8 *b, s_int32 blen);
extern s_int32  	epoll_atoi(char *str);
extern char*		epoll_itoa(s_int32 n, char * chBuffer);
extern s_int32 		epoll_ftoa(char *str, float num, s_int32 n);
extern float 		epoll_atof(const char *str);

/********memory function************/
extern void*		epoll_memcpy(void *dst, const void *src, u_int32 ulcount);
extern s_int32  	epoll_memcmp(void *s, void *d, s_int32 len);
extern void *		epoll_memset(void *dst, s_int32 val, s_int32 ulcount);
extern s_int32  	epoll_mem_check_zero(void *s, s_int32 len);
extern void 		*epoll_memswap(void *dst, s_int32 len);


/********string function*************/
extern void  		epoll_strcpy(const void *d,const void *s);
extern void  		epoll_strcat(void *d,void *s);
extern s_int32  	epoll_strcmp(const char *source,const char *dest);
extern s_int32  	epoll_strncmp(const char *first,const char *last,s_int32 count);
extern s_int32  	epoll_strlen(const char * str);
extern s_int32  	epoll_strnlen(const char *s, s_int32 count);
extern s_int32  	epoll_toupper(s_int32 c);
extern char*		epoll_strchr(const char *str, s_int32 ch);
extern char*		epoll_strstr(char *str0,char *str1);
extern char*		epoll_case_strstr(char *str0,char *str1);

/*********math function ****************/
extern u_int32  	epoll_sub32_abs(u_int32 s,u_int32 n);
extern u_int32  	epoll_sub32_loop(u_int32 s, u_int32 n);
extern u_int16  	epoll_get_crc16(u_int16 crc, u_int8 *pdata, int len);
extern u_int8	 	epoll_get_xor(u_int8 *pdata, int len);
extern u_int16  	epoll_get_sum16(u_int8 *pdata, int len);

/*********socket functions**************/
u_int32     epoll_inet_addr(char *ip);
u_int32     epoll_htons(u_int32 val);
u_int32     epoll_ntohs(u_int32 val);

#endif /* EPOLL_EPOLL_H_ */
