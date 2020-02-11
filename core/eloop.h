/*
 * eloop.h
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#ifndef ELOOP_ELOOP_H_
#define ELOOP_ELOOP_H_

#include "eloop_config.h"
#include "eloop_microdef.h"
#include "eloop_log.h"

/***************************eloop core funtion*************************************************/

extern int		    eloop_init(void);
extern void  	    eloop_enter_critical(void);
extern void  	    eloop_exit_critical(void);
extern void 		eloop_sleep(uint32 tim);

extern void 		eloop_set_power_conserve(u_int8 enable);
extern s_int8 		eloop_create_task(eloop_task_t task,u_int8 priority);
extern s_int8 		eloop_delete_task(eloop_task_t task);
extern void 		eloop_update_tick(u_int32 tick);
extern s_int8 		eloop_task_poll(void);
extern s_int8 		eloop_set_event(eloop_task_t task, u_int32 event);
extern s_int8 		eloop_clear_event(eloop_task_t task, u_int32 event);

extern void*		eloop_malloc(s_int32 size);
extern void 		eloop_free(void* p);
extern s_int32		eloop_get_free_heap(void);

extern s_int8 		eloop_list_insert(void **list, void *msg);
extern s_int8 		eloop_list_insert_front(void **list, void *msg);
extern void 		*eloop_list_detach(void **list);
extern void 		*eloop_list_detach_front(void **list);
extern s_int8 		eloop_list_delete(void **list, void *msg);
extern s_int8 		eloop_list_delete_list(void **list);

extern s_int8		eloop_send_msg(eloop_task_t task, void *msg, s_int32 len);
extern s_int8		eloop_send_msg_front(eloop_task_t task, void *msg, s_int32 len);
extern s_int8		eloop_send_stream(eloop_task_t task, void *msg, s_int32 len);
extern s_int8		eloop_send_stream_front(eloop_task_t task, void *msg, s_int32 len);

extern s_int8 		eloop_start_timer(eloop_timer_cb_t timer_cb, u_int8 signal, u_int32 time, u_int8 reload);
extern s_int8 		eloop_start_timer_task(eloop_task_t task,u_int32 event, u_int32 time, u_int8 reload);
extern s_int8 	    eloop_stop_timer(eloop_timer_cb_t timer_cb,u_int8 signal);
extern s_int8       eloop_stop_timer_task(eloop_task_t timer_cb,u_int32 event);
extern s_int32 		eloop_get_next_timeout(void);
extern u_int32 		eloop_get_timestamp(void);
extern u_int32      eloop_get_unixtime(void);
extern void         eloop_set_unixtime(u_int32 tim);
extern s_int8 		eloop_timer_start_measure(eTMeasure_t *pm,u_int32 timeout);
extern s_int8 		eloop_timer_expire(eTMeasure_t *pm);
extern u_int64          eloop_local_time_to_unix(int zone,eTime_t *date_time);
extern int              eloop_unix_to_local_time (int zone, eTime_t *tp,u_int64 t);

/********************************eloop lib function********************************************/
/*****random function*********/
extern void  		eloop_srand(u_int32 seek);
extern u_int32  	eloop_rand (void);
extern void  		eloop_ran_bytes(u_int8 *data, s_int32 len);

/********convert function********/
#define 	CONVERT_TPYE_BIG_ENDIAN			(0)
#define 	CONVERT_TPYE_LITTILE_ENDIAN		(!CONVERT_TPYE_BIG_ENDIAN)
extern void 		eloop_u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 conv_type);
extern void 		eloop_u32_to_bytes(u_int8 *pdata, u_int32 val, u_int8 conv_type);
extern void 		eloop_u16_to_bytes(u_int8 *pdata, u_int16 val, u_int8 conv_type);
extern u_int64 		eloop_bytes_to_u64(u_int8* pdata,u_int8 conv_type);
extern u_int32 		eloop_bytes_to_u32(u_int8* pdata,u_int8 conv_type);
extern u_int16 		eloop_bytes_to_u16(u_int8* pdata,u_int8 conv_type);
extern char  		eloop_byte2ascii(u_int8 x);
extern u_int8  		eloop_ascii2byte(char c);
extern char*		eloop_data2str(u_int8 *pdata, s_int32 len);
extern s_int32 		eloop_hex2bytes(u_int8 *d,s_int32 dlen,char *s,char split);
extern s_int32	 	eloop_bytes2hexstr(char *s,char split,u_int8 *b, s_int32 blen);
extern s_int32  	eloop_atoi(char *str);
extern char*		eloop_itoa(s_int32 n, char * chBuffer);
extern s_int32 		eloop_ftoa(char *str, float num, s_int32 n);
extern float 		eloop_atof(const char *str);

/********memory function************/
extern void*		eloop_memcpy(void *dst, const void *src, u_int32 ulcount);
extern s_int32  	eloop_memcmp(void *s, void *d, s_int32 len);
extern void *		eloop_memset(void *dst, s_int32 val, s_int32 ulcount);
extern s_int32  	eloop_mem_check_zero(void *s, s_int32 len);
extern void 		*eloop_memswap(void *dst, s_int32 len);


/********string function*************/
extern void  		eloop_strcpy(const void *d,const void *s);
extern void  		eloop_strcat(void *d,void *s);
extern s_int32  	eloop_strcmp(const char *source,const char *dest);
extern s_int32  	eloop_strncmp(const char *first,const char *last,s_int32 count);
extern s_int32  	eloop_strlen(const char * str);
extern s_int32  	eloop_strnlen(const char *s, s_int32 count);
extern s_int32  	eloop_toupper(s_int32 c);
extern char*		eloop_strchr(const char *str, s_int32 ch);
extern char*		eloop_strstr(char *str0,char *str1);
extern char*		eloop_case_strstr(char *str0,char *str1);

/*********math function ****************/
extern u_int32  	eloop_sub32_abs(u_int32 s,u_int32 n);
extern u_int32  	eloop_sub32_loop(u_int32 s, u_int32 n);
extern u_int16  	eloop_get_crc16(u_int16 crc, u_int8 *pdata, int len);
extern u_int8	 	eloop_get_xor(u_int8 *pdata, int len);
extern u_int16  	eloop_get_sum16(u_int8 *pdata, int len);

/*********socket functions**************/
u_int32     eloop_inet_addr(char *ip);
u_int32     eloop_htons(u_int32 val);
u_int32     eloop_ntohs(u_int32 val);

#endif /* ELOOP_ELOOP_H_ */
