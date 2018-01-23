



#include "log.h"


#define LOG_FILE "cbb.log"



bslsink_sink_t logfile_sink;



int 
logfile_vfprintf(void *file,bsl_meta_t *meta,const char*format,va_list args)
{
	return 
}

int
log_file_init()
{
	bslsink_sink_t *sink;

    /* Create logfile sink */
    sink = &logfile_sink;
    bslsink_sink_t_init(sink);
    sal_strncpy(sink->name, "logfile", sizeof(sink->name));
    sink->vfprintf = bslfile_vfprintf;

    bslsink_sink_add(sink);
}


