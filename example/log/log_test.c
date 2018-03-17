

#include <stdlib.h>
#include <stdio.h>

#include "log.h"


int 
main(int argc,char **argv)
{
	log_init();
	
	int i=0;


//	log_console_severity_set(bslSeverityError,bslSeverityWarn);
//	log_file_severity_set(bslSeverityFatal,bslSeverityWarn);

#if 0
	LOG_FATAL(0,(BSL_META("[%d] LOG_ERROR TEST\n"),i++));
	LOG_ERROR(0,(BSL_META("[%d] LOG_ERROR TEST\n"),i++));
	LOG_WARN(0,(BSL_META("[%d] LOG_WARN TEST\n"),i++));
	LOG_INFO(0,(BSL_META("[%d] LOG_INFO TEST\n"),i++));
	LOG_DEBUG(0,(BSL_META("[%d] LOG_DEBUG TEST\n"),i++));
#endif

	LOG_FATAL((BSL_META("[%d] LOG_ERROR TEST\n"),i++));
	LOG_ERROR((BSL_META("[%d] LOG_ERROR TEST\n"),i++));
	LOG_WARN((BSL_META("[%d] LOG_WARN TEST\n"),i++));
	LOG_INFO((BSL_META("[%d] LOG_INFO TEST\n"),i++));
	LOG_DEBUG((BSL_META("[%d] LOG_DEBUG TEST\n"),i++));


	cli_out("[%d] CLI_OUT TEST\n",i++);
	
	return 0;
}




