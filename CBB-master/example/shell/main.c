

#include "shell.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

extern void test_p();

void* test(void *a)
{

}


int
main()
{
//	pthread_t id;
//	pthread_create(&id,NULL,test,NULL);
	shell("CBB> ");
	return 0;
}
