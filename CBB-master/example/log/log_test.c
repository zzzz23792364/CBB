

#include <stdlib.h>
#include <stdio.h>
#include "log.h"

int 
main(int argc,char **argv)
{
	log_console_init();
	
	int i=0;
	
	LOG_ERROR(0,(BSL_META("[%d] LOG_ERROR TEST\n"),i++));
	LOG_WARN(0,(BSL_META("[%d] LOG_WARN TEST\n"),i++));
	LOG_INFO(0,(BSL_META("[%d] LOG_INFO TEST\n"),i++));
	LOG_DEBUG(0,(BSL_META("[%d] LOG_DEBUG TEST\n"),i++));

	cli_out("[%d] CLI_OUT TEST\n",i++);
	
	return 0;
}




