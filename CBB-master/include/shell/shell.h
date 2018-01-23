




#ifndef _SHELL_H_H
#define _SHELL_H_H

#include <stdio.h>
#include "parse.h"


#define HIDE_CMD        0
#define SHOW_CMD        1

#define DEBUG           0



#if 0
typedef struct {
  char *name;			/* User printable name of the function. */
  rl_icpfunc_t *func;		/* Function to call to do the job. */
  char *doc;			/* Documentation for this function.  */
  char *usage;
} COMMAND;
#endif




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



//extern int debug;
//extern int shell_debug;
//extern int bpu_shell(void);
//extern int cmd_parse_line (char *pline,char **argv);



/*
 * Typedef:     cmd_result_t
 * Purpose:    Type retured from all commands indicating success, fail, 
 *        or print usage.
 */
typedef enum cmd_result_e {
    CMD_OK   = 0,            /* Command completed successfully */
    CMD_FAIL = -1,            /* Command failed */
    CMD_USAGE= -2,            /* Command failed, print usage  */
    CMD_NFND = -3,            /* Command not found */
    CMD_EXIT = -4,            /* Exit current shell level */
    CMD_INTR = -5,            /* Command interrupted */
    CMD_NOTIMPL = -6,            /* Command not implemented */
} cmd_result_t;



/*
 * Typedef:    cmd_func_t
 * Purpose:    Defines command function type
 */
typedef cmd_result_t (*cmd_func_t)(args_t *);

/*
 * Typedef:    cmd_t
 * Purpose:    Table command match structure.
 */
typedef struct cmd_s {
    parse_key_t    c_cmd;            /* Command string */
    cmd_func_t    c_f;            /* Function to call */
    const char     *c_usage;        /* Usage string */
    const char    *c_help;        /* Help string */
} cmd_t;



#define DCL_CMD(_func,_usage) \
	extern cmd_result_t _func(args_t*);	\
	extern char _usage[];


int shell(char *prompt);

#endif


