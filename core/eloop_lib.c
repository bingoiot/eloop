/*
 * eloop_lib.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "eloop_microdef.h"
#include "eloop_heap.h"
#include "eloop_lib.h"
#include "eloop_timer.h"

#define is_digit(c) 	((c) >= '0' && (c) <= '9')
#define is_hexstr(c)	(((c) >= '0' && (c) <= '9')||(c>='a'&&c<='f')||(c>='A'&&c<='F'))
const u_int8 			ascii_table[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const u_int16 CRC16Table[256] =
{
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

static u_int32 	srandx = 1;
#define 		SRANDK  1103515245	//0x41C64E6D;
#define  		SRANDB 	12345		//0x3039;

static void 		_u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 len, u_int8 type);
static u_int64 		_bytes_to_u64(u_int8* pdata,u_int8 len, u_int8 conv_type);

void  eloop_srand(u_int32 seek)
{
	srandx = seek;
}
u_int32  eloop_rand (void)
{
	srandx = srandx *SRANDK +SRANDB ;
    return (srandx>>1);
}
void  eloop_ran_bytes(u_int8 *data, s_int32 len)
{
	s_int32 i = 0;
	u_int32 temp;
	temp = eloop_get_timestamp();
	if(temp)
		eloop_srand(temp);
	for(i=0;i<len;i++)
	{
		if(i%(sizeof(u_int32))==0)
			temp = eloop_rand();
		data[i] = temp;temp>>=8;
	}
}
static void _u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 len, u_int8 conv_type)
{
	u_int64 temp;
	s_int32 i;
	temp = val;
	if(conv_type==CONVERT_TPYE_BIG_ENDIAN)
	{
		for(i=(len-1);i>=0;i--)
		{
			pdata[i] = (u_int8)temp;
			temp >>= 8;
		}
	}
	else
	{
		for(i=0;i<len;i++)
		{
			pdata[i] = (u_int8)temp;
			temp >>= 8;
		}
	}
}
void eloop_u64_to_bytes(u_int8 *pdata, u_int64 val, u_int8 conv_type)
{
	_u64_to_bytes(pdata,val,8,conv_type);
}
void eloop_u32_to_bytes(u_int8 *pdata, u_int32 val, u_int8 conv_type)
{
	_u64_to_bytes(pdata,(u_int64)val,4,conv_type);
}
void eloop_u16_to_bytes(u_int8 *pdata, u_int16 val, u_int8 conv_type)
{
	_u64_to_bytes(pdata,(u_int64)val,2,conv_type);
}
static u_int64 _bytes_to_u64(u_int8* pdata,u_int8 len, u_int8 conv_type)
{
	u_int64 temp = 0;
	s_int32 i;
	if(conv_type==CONVERT_TPYE_BIG_ENDIAN)
	{
		for(i=0;i<len;i++)
		{
			temp <<= 8;
			temp |= pdata[i];
		}
	}
	else
	{
		for(i=(len-1);i>=0;i--)
		{
			temp <<= 8;
			temp |= pdata[i];
		}
	}
	return temp;
}
u_int64 eloop_bytes_to_u64(u_int8* pdata,u_int8 conv_type)
{
	u_int64 temp;
	temp = (u_int64)_bytes_to_u64(pdata,8,conv_type);
	return temp;
}
u_int32 eloop_bytes_to_u32(u_int8* pdata,u_int8 conv_type)
{
	u_int32 temp;
	temp = (u_int32)_bytes_to_u64(pdata,4,conv_type);
	return temp;
}

u_int16 eloop_bytes_to_u16(u_int8* pdata,u_int8 conv_type)
{
	u_int16 temp;
	temp = (u_int16)_bytes_to_u64(pdata,2,conv_type);
	return temp;
}
char  eloop_byte2ascii(u_int8 x)
{
	return ascii_table[x&0x0f];
}
u_int8  eloop_ascii2byte(char c)
{
	switch(c)
	{
	case 'a':
	case 'A':
		return 0x0a;
	case 'b':
	case 'B':
		return 0x0b;
	case 'c':
	case 'C':
		return 0x0c;
	case 'd':
	case 'D':
		return 0x0d;
	case 'e':
	case 'E':
		return 0x0e;
	case 'f':
	case 'F':
		return 0x0f;
	default:
		return (c&0x0f)%10;
	}
}
char *eloop_data2str(u_int8 *pdata, s_int32 len)
{
	char *str;
	str = (char*)eloop_malloc(len+1);
	if(str!=NULL)
	{
		eloop_memcpy(str,pdata,len);
		str[len] = '\0';
	}
	return str;
}
s_int32 eloop_hex2bytes(u_int8 *d,s_int32 dlen,char *s,char split)
{
	s_int32 i,k;
	u_int8 temp;
	k = 0;
	for(i=0;i<dlen;i++)
	{
		if(!is_hexstr(s[k]))return i;
		temp = eloop_ascii2byte(s[k++]);
		d[i] = temp;
		if(!is_hexstr(s[k]))return i;
		temp<<=4;
		temp |= eloop_ascii2byte(s[k++]);
		if(split)
			k++;
		d[i] = temp;
	}
	return i;
}
s_int32	 eloop_bytes2hexstr(char *s,char split,u_int8 *b, s_int32 blen)
{
	s_int32 i,k;
	u_int8 temp;
	if((s==NULL)||(b==NULL))
		return 0;
	k = 0;
	for(i=0;i<blen;i++)
	{
		temp = b[i];
		s[k++] = ascii_table[temp>>4];
		s[k++] = ascii_table[temp&0x0f];
		if(split)
			s[k++] = split;
	}
	s[k] = '\0';
	return k;
}
s_int32  eloop_atoi(char *str)
{
   s_int32 sign;
   s_int32 n = 0;
   char *p = str;
   if(p==NULL)
	   return 0;
   while (*p&&(!is_digit(*p))&&(*p!='-')) p++;
   sign = ('-' == *p) ? -1 : 1;
   if ('+' == *p || '-' == *p) p++;
   for (n = 0; is_digit(*p); p++)
	  n = 10 * n + (*p - '0');
   return sign*n;
}
char  *eloop_itoa(s_int32 n, char * chBuffer)
{
    s_int32 i = 1;
    char * pch = chBuffer;
    if(!pch) return 0;
    while(n / i) i *= 10;

    if(n < 0)
    {
        n = -n;
        *pch++ = '-';
    }
    if (0 == n) i = 10;

    while(i /= 10)
    {
        *pch++ = n / i + '0';
        n %= i;
    }
    *pch = '\0';
    return chBuffer;
}
s_int32 eloop_ftoa(char *str, float num, s_int32 n)        //n��ת���ľ��ȣ������ַ���'.'���м�λС��
{
    s_int32     sumI;
    float   sumF;
    s_int32     sign = 0;
    s_int32     temp;
    s_int32     count = 0;
    char *p;
    char *pp;

    if(str == NULL) return -1;
    p = str;

    /*Is less than 0*/
    if(num < 0)
    {
        sign = 1;
        num = 0 - num;
    }
    sumI = (s_int32)num;    //sumI is the part of s_int32
    sumF = num - sumI;  //sumF is the part of float
    /*Int ===> String*/
    do
    {
        temp = sumI % 10;
        *(str++) = temp + '0';
    }while((sumI = sumI /10) != 0);
    /*******End*******/
    if(sign == 1)
    {
        *(str++) = '-';
    }
    pp = str;
    pp--;
    while(p < pp)
    {
        *p = *p + *pp;
        *pp = *p - *pp;
        *p = *p -*pp;
        p++;
        pp--;
    }
    *(str++) = '.';     //point
    /*Float ===> String*/
    do
    {
        temp = (s_int32)(sumF*10);
        *(str++) = temp + '0';

        if((++count) == n)
            break;

        sumF = sumF*10 - temp;

    }while(!(sumF > -0.000001 && sumF < 0.000001));
    *str = 0x00;
    return 0;
}


/*
 * File : atof.c
 */

/*String ===> Float*/

float eloop_atof(const char *str)
{
    float   sumF = 0;
    s_int32     sumI = 0;
    s_int32     sign = 0;
    if(str == NULL) return -1;
    /*Is less than 0 ?*/
    if(*str == '-')
    {
        sign = 1;
        str++;
    }
    /*The part of s_int32*/
    while(*str != '.')
    {
        sumI = 10*sumI + (*str - '0');
        str++;
    }
    /*Let p point to the end*/
    while(*str != '\0')
    {
        str++;
    }
    str--;          //Your know!
    /*The part of float*/
    while(*str != '.')
    {
        sumF = 0.1*sumF + (*str - '0');
        str--;
    }
    sumF = 0.1*sumF;
    sumF += sumI;
    if(sign == 1)
    {
        sumF = 0 - sumF;
    }
    return sumF;
}

void  *eloop_memcpy(void *dst, const void *src, u_int32 ulcount)
{
	int i;
	char *s,*d;
	if(!(dst && src)) return 0;
	s = (char*)src; d = (char*)dst;
	for(i=0;i<ulcount;i++)
	{
	   d[i] = s[i];
	}
	return dst;
}
s_int32  eloop_memcmp(void *s, void *d, s_int32 len)
{
	s_int32 i;
	u_int8 *ss,*dd;
	ss = s;
	dd = d;
	for(i=0;i<len;i++)
	{
		if(ss[i]>dd[i])
			return 1;
		else if(ss[i]<dd[i])
			return -1;
	}
	return 0;
}
void  *eloop_memset(void *dst, s_int32 val, s_int32 ulcount)
{
    if(!dst) return 0;
    char * pchdst = (char*)dst;
    while(ulcount--) *pchdst++ = (char)val;

    return dst;
}

s_int32  eloop_mem_check_zero(void *s, s_int32 len)
{
	u_int8 *p = s;
	s_int32 i;
	s_int32 ss;
	for(i=0;i<len;i++)
	{
		ss = p[i];
		if(ss>0)
			return 1;
		else if(ss<0)
			return -1;
	}
	return 0;
}
void *eloop_memswap(void *dst, s_int32 len)
{
	s_int32 i;
	s_int32 c,d;
	u_int8	temp;
	u_int8 *p;
	c = len/2;
	len -= 1;
	p = (u_int8*)dst;
	for(i=0;i<c;i++)
	{
		temp = p[i];
		d = len - i;
		p[i] = p[d];
		p[d] = temp;
	}
	return dst;
}





void  eloop_strcpy(const void *d,const void *s)
{
	char *ss,*dd;
	ss = (char *)s;
	dd = (char *)d;
	while(*ss!='\0')
	{
		*dd = *ss;
		dd++;ss++;
	}
	*dd='\0';
}
void  eloop_strcat(void *d,void *s)
{
	u_int8 *ss,*dd;
	ss = s;
	dd = d;
	while(*dd!='\0')dd++;
	while(*ss!='\0')
	{
		*dd = *ss;
		dd++;ss++;
	}
	*dd='\0';
}
s_int32  eloop_strcmp(const char *source,const char *dest)
{
    s_int32 ret = 0;
    if(!source || !dest) return -2;
    while( *source && *dest && !(ret = *( unsigned char *)source - *(unsigned char *)dest))
    {
        source++;
        dest++;
    }
    if ( ret < 0 )
        ret = -1 ;
    else if ( ret > 0 )
        ret = 1 ;
    return(ret);
}
s_int32  eloop_strncmp(const char *first,const char *last,s_int32 count)
{
   if (!count)  return 0;

   while (--count && *first && *first == *last) first++,last++;

   return ( *(unsigned char *)first - *(unsigned char *)last );
}
s_int32  eloop_strlen(const char * str)
{
	s_int32 i = 0;
	u_int8 *ss;
	if(str==NULL)
		return 0;
	ss = (u_int8*)str;
	while(*ss!='\0')
	{
		i++;
		ss++;
	}
	return i;
}
s_int32  eloop_strnlen(const char *s, s_int32 count)
{
	s_int32 i = 0;
	u_int8 *ss;
	if(s==NULL)
		return 0;
	ss = (u_int8*)s;
	while((*ss!='\0')&&(count--))
	{
		i++;
		ss++;
	}
	return i;
}
s_int32  eloop_toupper(s_int32 c)
{
    if(c >= 'a' && c<= 'z')
        return c + 'A' - 'a';
    return c;
}
char  *eloop_strchr(const char *str, s_int32 ch)
{
    while ((*str!='\0') && (*str != (char)ch)) str++;
    if (*str == (char)ch)
        return((char *)str);
    return 0;
}
char  *eloop_strstr(char *str0,char *str1)
{
	s_int32 i,j;
	s_int32 start;
	if(str1[0]=='\0')
		return NULL;
	for(i=0;str0[i]!='\0';)
	{
		start = i;
		j = 0;
		while(str0[i]==str1[j])
		{
			if(str1[j+1]=='\0')
				return &str0[start];
			if((str0[i]=='\0')||(str0[i]==')')||(str0[i]=='('))
				break;
			i++;j++;
		}
		i++;
	}
	return NULL;
}
char  *eloop_case_strstr(char *str0,char *str1)
{
	s_int32 i,j;
	s_int32 start;
	if(str1[0]=='\0')
		return NULL;
	for(i=0;str0[i]!='\0';)
	{
		start = i;
		j = 0;
		while(eloop_toupper(str0[i])==eloop_toupper(str1[j]))
		{
			if(str1[j+1]=='\0')
				return &str0[start];
			if((str0[i]=='\0')||(str0[i]==')')||(str0[i]=='('))
				break;
			i++;j++;
		}
		i++;
	}
	return NULL;
}



u_int32  eloop_sub32_abs(u_int32 s,u_int32 n)
{
	u_int32 temp;
	if(s>=n)
		temp = s-n;
	else
		temp = n-s;
	return temp;
}
u_int32  eloop_sub32_loop(u_int32 s, u_int32 n)
{
	u_int32 temp;
	if(s>=n)
		temp = s-n;
	else
		temp = (0xFFFFFFFF-n)+s;
	return temp;
}
u_int16  eloop_get_crc16(u_int16 crc, u_int8 *pdata, int len)
{
	int i;
    u_int16 crc16=crc;
    for(i=0; i<len; i++)
    {
        crc16 = (crc16 >> 8) ^ CRC16Table[(crc16 & 0xFF) ^ pdata[i]];
    }
    return crc16;
}
u_int8	 eloop_get_xor(u_int8 *pdata, int len)
{
	int i;
	u_int8 sum = 0;
	for(i=0;i<len;i++)
	{
		sum ^= pdata[i];
	}
	return sum;
}
u_int16  eloop_get_sum16(u_int8 *pdata, int len)
{
	int i;
	u_int16 temp = 0;
	for(i=0;i<len;i++)
	{
		temp = (temp<<1)+pdata[i];
	}
	return temp;
}

