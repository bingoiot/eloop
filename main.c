/*
 * main.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "stdio.h"
#include "unistd.h"
#include "epoll.h"
void epoll_exampe_init(void);
int main(void *arg)
{

	printf("hello cmake\r\n");
	epoll_exampe_init();
	while(1) sleep(1);
	return 0;
}

