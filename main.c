/*
 * main.c
 *
 *  Created on: 20200201
 *      Author: luyunyi
 */

#include "stdio.h"
#include "unistd.h"
#include "eloop.h"
void eloop_exampe_init(void);
int main(void *arg)
{

	printf("hello cmake\r\n");
	eloop_exampe_init();
	while(1) sleep(1);
	return 0;
}

