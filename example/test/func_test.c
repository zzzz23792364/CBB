
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <execinfo.h>

void 
signal_handler(int signo)
{
	int i,size;
	void *buffer[16];
	char **str;

	size = backtrace(buffer,16);

	str = backtrace_symbols(buffer,16);

	for(i=0;i<size;i++)
		printf(" [%02d] %s\n",i,str[i]);

	signal(signo,SIG_DFL);
	raise(signo);
}


void test()
{
	char *p =NULL;

	*p = 1;
}


int 
main(int argc,char **argv)
{
	signal(SIGSEGV,signal_handler);

	test();

	return 0;
}
#endif



/* 
 * @FileName: once.c 
 * @Author: wzj 
 * @Brief:  
 * 1.验证pthread_once的小例子  
 * 2.验证pthread_key_t 的作用  
 *   
 *   
 *   
 * @History:  
 *  
 *  
 *  
 * @Date: 2012年06月03日星期日13:19:15 
 *  
 */   
#include <stdio.h>  
#include <pthread.h>  
#include <stdlib.h>  
//线程存储变量，用于一个线程内部的全局变量  
static pthread_key_t pid_key;  
  
pthread_once_t once = PTHREAD_ONCE_INIT;  
//pthread_once的回调函数, 一般用于一次性初始化  
void   
once_run(void)  
{  
    int ret = 0;  
    static int times = 1;  
      
    ret = pthread_key_create(&pid_key, NULL);     
    if(ret == 0)  
    {  
        printf("private key create success, by %u\n", pthread_self());  
    }  
  
    printf("The %d time run\n", times);  
    times++;  
}  
  
  
void*  
child(void* arg)  
{  
    pthread_t tid = pthread_self();  
  
    printf("Thread %u is run!\n", tid);  
    pthread_once(&once, once_run);  
    //设置私有值  
    pthread_setspecific(pid_key, &tid);  
  
    while(1)  
    {  
        printf("child:%d, private key:%d\n",   
                pthread_self(), (int)pthread_getspecific(pid_key));   
        sleep(1);  
    }  
    printf("Thread %u is quit!\n", tid);  
  
    return 0;  
}  
  
int   
main()  
{  
    pthread_t tid1, tid2;  
  
    pthread_create(&tid1, NULL, child, NULL);  
    pthread_create(&tid2, NULL, child, NULL);  
      
    pthread_join(tid1, NULL);  
    pthread_join(tid2, NULL);  
  
    printf("all exit!\n");  
    return 0;  
}  


