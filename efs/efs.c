/*
 * efs.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "efs_config.h"
#include "efs.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <dirent.h>
#include "unistd.h"
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "unistd.h"

#if ELOOP_ENABLE_EFS

const 	char	*basic_dir="spiffs";
static 	char	path_buf[EFS_NAME_MAX_LENGTH];
static 	void  	init_file_system(void);
static char 	*gen_file_name(char *file);

int efs_read_file(char *fname, char *buf, int offset, int len)
{
	char	*path;
	FILE	*pfile;
	int		slen = 0;
	init_file_system();
	if((path = gen_file_name(fname))!=NULL)
	{
		eloop_log(DBG_EFS,"efs_read_file:%s,len:%d\r\n",path,len);
		pfile = fopen(path, "rb");
		if(pfile!=NULL)
		{
			fseek(pfile,offset,SEEK_SET);
			slen = fread(buf,sizeof(char),len,pfile);
			fclose(pfile);
		}
		else
		{
			eloop_log(DBG_EFS_ERROR,"efs_read_file: open file :%s error \r\n",path);
		}
	}
	else
	{
		eloop_log(DBG_EFS_ERROR,"efs_read_file: file name :%s error \r\n",path);
	}
	return slen;
}
int  efs_write_file(char *fname, int offset, char *pdata, int len)
{
	char	*path;
	FILE 	*pfile;
	int 	slen = 0;
	init_file_system();
	if((path = gen_file_name(fname))!=NULL)
	{
		pfile = fopen(path, "wb");
		if(pfile!=NULL)
		{
			fseek(pfile,offset,SEEK_SET);
			slen = fwrite(pdata,sizeof(char),len,pfile);
			if(slen>=0)
			{
				fclose(pfile);
				eloop_logb(DBG_EFS_DATA,"efs_write_file:",pdata,slen);
			}
			else
				eloop_log(DBG_EFS_ERROR,"efs_write_file: fs_write file :%s error \r\n",path);
			fclose(pfile);
		}
		else
			eloop_log(DBG_EFS_ERROR,"efs_write_file: open file :%s error \r\n",path);
	}
	else
	{
		eloop_log(DBG_EFS_ERROR,"efs_write_file: file name :%s error \r\n",path);
	}
	return slen;
}
int  efs_get_file_length(char *fname)
{
	char	*path;
	struct 	stat  Stat;
	int	slen = 0;
	init_file_system();
	if((path = gen_file_name(fname))!=NULL)
	{
		if(stat(path,&Stat)>=0)
		{
			slen = Stat.st_size;
			eloop_log(DBG_EFS,"efs_get_file_length:%s,len%d \r\n",path,slen);
		}
		else
			eloop_log(DBG_EFS_ERROR,"efs_get_file_length: get file:%s state error \r\n",path);
	}
	else
	{
		eloop_log(DBG_EFS_ERROR,"efs_get_file_length: file name :%s error \r\n",path);
	}
	return slen;
}
char *efs_get_file_name(char *filter, int id)
{
	char	*path;
	char	*fname = NULL;
	char	*p = NULL;
	DIR 	*dir;
	struct 	dirent *ptr;
	int		cnt = 0;
	u_int8 	match = 0x00;
	u_int8	match_flag;
	init_file_system();
	eloop_log(DBG_EFS,"efs_get_file_name: Open dir:%s\r\n",path);
	if((path = gen_file_name(fname))!=NULL)
	{
		if ((dir=opendir(path)) != NULL)
		{
			if((p = eloop_strchr(filter,'*'))!=NULL)
			{
				if(p[1]=='\0')//match all file
				{
					match = 0xFF;
				}
				else
				{
					match =0x01;
					p++;
				}
			}
			eloop_log(DBG_EFS,"try read dir id:%d \r\n",cnt);
			while ((ptr=readdir(dir)) != NULL)
			{
				match_flag = ES_FALSE;
				eloop_log(DBG_EFS,"read dir succeed type:%d,name:%s  filter:%s \r\n",ptr->d_type,ptr->d_name,filter);
				switch(match)
				{
				case 0x00://match name
					if(eloop_strcmp(ptr->d_name,(char*)filter)==0)
					{
						match_flag = ES_TRUE;
					}
					break;
				case 0x01://match type
					if(eloop_case_strstr(ptr->d_name,(char*)p)!=NULL)
					{
						match_flag = ES_TRUE;
					}
					break;
				case 0xFF://match all file
					match_flag = ES_TRUE;
					break;
				}
				if(match_flag == ES_TRUE)
				{
					eloop_log(DBG_EFS,"try read dir filter pass, id = %d name:%s \r\n",id,ptr->d_name);
					if(cnt==id)
					{
						fname = (char*)eloop_malloc(eloop_strlen(ptr->d_name)+1);
						if(fname!=NULL)
						{
							eloop_strcpy(fname,ptr->d_name);
						}
						break;
					}
					cnt++;
				}
			}
			closedir(dir);
		}
		else
		{
			eloop_log(DBG_EFS,"Open dir error...\r\n");
		}
	}
	else
	{
		eloop_log(DBG_EFS_ERROR,"efs_write_file: file name :%s error \r\n",path);
	}
	return fname;
}
int efs_delete_file(char *fname)
{
	char	*path;
	init_file_system();
	eloop_log(DBG_EFS,"efs_delete_file:%s\r\n",fname);
	if((path = gen_file_name(fname))!=NULL)
	{
		if(remove(path)>=0)
		{
			eloop_log(DBG_EFS,"efs_delete_file succeed!-->%s\r\n",path);
			return ES_SUCCEED;
		}
	}
	else
	{
		eloop_log(DBG_EFS_ERROR,"efs_delete_file: file name :%s error \r\n",path);
	}
	return ES_FAILED;
}
int efs_delete_all_file(void)
{
	char	*path;
	DIR 	*dir;
	struct 	dirent *ptr;
	init_file_system();
	eloop_log(DBG_EFS,"efs_delete_all_file: Open dir:%s\r\n",path);
	if((path = gen_file_name(NULL))!=NULL)
	{
		if ((dir=opendir(path)) != NULL)
		{
			eloop_log(DBG_EFS,"efs_delete_all_file:try read dir \r\n");
			while ((ptr=readdir(dir)) != NULL)
			{
				eloop_log(DBG_EFS,"efs_delete_all_file:read dir succeed type:%d,name:%s \r\n",ptr->d_type,ptr->d_name);
				efs_delete_file(ptr->d_name);
			}
			closedir(dir);
		}
		else
		{
			eloop_log(DBG_EFS,"efs_delete_all_file:Open dir error...\r\n");
		}
	}
	else
	{
		eloop_log(DBG_EFS_ERROR,"efs_delete_file: file name :%s error \r\n",path);
	}
	return ES_SUCCEED;
}
int efs_disk_format(void)
{
	eloop_log(DBG_EFS,"pluto_disk_format\r\n");
	init_file_system();
	efs_delete_all_file();
	return ES_SUCCEED;
}
int	efs_rename(char *old_name,char *new_name)
{
	char 	*path;
	char	path_old[EFS_NAME_MAX_LENGTH];
	init_file_system();
	if((path = gen_file_name(old_name))!=NULL)
	{
		eloop_strcpy(path_old,path);
		if((path = gen_file_name(new_name))!=NULL)
		{
			rename(path_old,path);
			return ES_SUCCEED;
		}
	}
	return ES_FAILED;
}
static void init_file_system(void)
{
}
static char *gen_file_name(char *file)
{
	if((eloop_strlen(basic_dir)+eloop_strlen(file)+1)<EFS_NAME_MAX_LENGTH)
	{
		eloop_strcpy(path_buf,basic_dir);
		eloop_strcat(path_buf,"/");
		if(file!=NULL)
			{eloop_strcat(path_buf,file);}
		return path_buf;
	}
	else
	{
		return NULL;
	}
}

#endif







