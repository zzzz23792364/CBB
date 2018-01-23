
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


