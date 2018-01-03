






#ifndef BPU_SHELL_H
#define BPU_SHELL_H

#include <stdio.h>

#define HIDE_CMD        0
#define SHOW_CMD        1

#define DEBUG           0

#define CMD_MAX_ARGS		 20
#define CMD_MAX_CHAR		 128


#define MIN_HIDE_CMD_LEN     4
//#define AUTOTEST_VERSION	"BPU testing V1R0\n"

#define SHELL_ASSERT(condition, errorCode)\
{\
    if ((condition))\
    {\
        printf("[%s(%d)] : error .\n", __FUNCTION__,__LINE__);\
        return errorCode;\
    }\
}


#define shell_printf(condition,arg) do\
{	\
	if(condition) \
	{	\
		printf arg;	\
		fflush(stdout);	\
	}	\
}while(0)


#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define __NONE__         "\033[0m"
#define __BLACK__        "\033[0;30m"
#define __DARK_GRAY__    "\033[1;30m"
#define __BLUE__         "\033[0;34m"
#define __LIGHT_BLUE__   "\033[1;34m"
#define __GREEN__        "\033[0;32m"
#define __LIGHT_GREEN__  "\033[1;32m"
#define __CYAN__         "\033[0;36m"
#define __LIGHT_CYAN__   "\033[1;36m"
#define __RED__          "\033[0;31m"
#define __LIGHT_RED__    "\033[1;31m"
#define __PURPLE__       "\033[0;35m"
#define __LIGHT_PURPLE__ "\033[1;35m"
#define __BROWN__        "\033[0;33m"
#define __YELLOW__       "\033[1;33m"
#define __LIGHT_GRAY__   "\033[0;37m"
#define __WHITE          "\033[1;37m"


typedef enum cmd_result_e
{
	CMD_OK = 0,
	CMD_FAILED = -1,
	CMD_USAGE = -2,
	CMD_RESOURCE = -4
}cmd_result_t;


extern int debug;
extern int shell_debug;
extern int bpu_shell(void);
extern int cmd_parse_line (char *pline,char **argv);


#endif


