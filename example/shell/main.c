

#include "shell.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>
#include <readline/readline.h>

extern void test_p();

int
main()
{
	test_p();
	time_t ti;
	time(&ti);
	readline(NULL);
	printf("i am main\n");
	shell();

	return 0;
}
