


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include <sal/core/thread.h>

#include "cbb_types.h"


#define DEFAULT_RECORD_DEVICE "hw:0,0"

typedef struct voice_device_params_s
{
	U8 *name; 
	U32 rate; 
	U16 bits;
	U32 channel;
	U32 rec_time; //unit:ms
	U32 rec_len;
	snd_pcm_sframes_t frames; 
	snd_pcm_format_t format;
}voice_device_params_t;


voice_device_params_t vd_params;




/*****************************************************************************
 * Function      : pcm_hw_params_configs
 * Description   : 配置声卡参数：声卡名、采样率、通道数、格式
 * Input         : voice_device_params_t *vd_params  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180314
 *   Author      : ZengChao
 *   Modification: Created function

*****************************************************************************/
void 
pcm_hw_params_configs(voice_device_params_t *params)
{
	params->name = DEFAULT_RECORD_DEVICE;
	params->rate = 16000;
	params->bits = 16;
	params->channel = 1,
	params->format = SND_PCM_FORMAT_S16_LE;
	params->rec_time = 80;	
	params->frames = (params->rate*params->rec_time)/1000;
	params->rec_len = (params->frames * params->channel * params->bits)/8;
}


/*****************************************************************************
 * Function      : pcm_device_init
 * Description   : 初始化声卡设备
 * Input         : snd_pcm_t **handler  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180314
 *   Author      : ZengChao
 *   Modification: Created function

*****************************************************************************/
int 
pcm_device_init(snd_pcm_t **handler)
{
	int ret;
	
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	
	pcm_hw_params_configs(&vd_params);

	ret = snd_pcm_open(&capture_handle,vd_params.name,SND_PCM_STREAM_CAPTURE, 0);
	if(ret<0)
	{
		fprintf (stderr, "cannot open audio device %s (%s)\n",
						vd_params.name,snd_strerror(ret));
		return -1;
	}

	ret = snd_pcm_hw_params_malloc(&hw_params);
	if(ret<0)
	{
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
						snd_strerror(ret));
		snd_pcm_close(capture_handle);
		return -1;
	}

	ret = snd_pcm_hw_params_any(capture_handle, hw_params);
	if(ret<0)
	{
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
						snd_strerror(ret));
		goto err_exit;
	}
	
	ret = snd_pcm_hw_params_set_access(capture_handle, hw_params, 
								SND_PCM_ACCESS_RW_INTERLEAVED);
	if(ret<0)
	{
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror(ret));
		goto err_exit;
	}
	
	ret = snd_pcm_hw_params_set_format(capture_handle, hw_params,vd_params.format);
	if(ret<0)
	{
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (ret));
		return -1;
	}

	ret = snd_pcm_hw_params_set_rate_near(capture_handle,hw_params,&vd_params.rate,0);
	if(ret<0)
	{
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (ret));
		goto err_exit;
	}

	ret = snd_pcm_hw_params_set_channels(capture_handle, hw_params,vd_params.channel);
	if(ret<0)
	{
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (ret));
		goto err_exit;
	}

	ret = snd_pcm_hw_params (capture_handle,hw_params);
	if(ret<0)
	{
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (ret));
		goto err_exit;
	}

	*handler = capture_handle;

	snd_pcm_hw_params_free (hw_params);
	
	return 0;

err_exit:
	snd_pcm_hw_params_free (hw_params);
	snd_pcm_close (capture_handle);
	return -1;
}


extern int 
pcm2wav(const U8* pcm_path,const U8* wave_path,U16 channels,U32 rate,U16 bits);

/*****************************************************************************
 * Function      : pcm_capture_thread
 * Description   : 声音录制线程
 * Input         : void *arg  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180314
 *   Author      : ZengChao
 *   Modification: Created function

*****************************************************************************/
void
pcm_capture_thread(void *arg)
{
	int i,ret;
	char *capture_data;
	snd_pcm_t *capture_handle;

	ret = pcm_device_init(&capture_handle);
	if(ret<0)
	{
		fprintf(stderr,"pcm_device_init failed\n");
		return;
	}

	capture_data = malloc(vd_params.rec_len);
	if(NULL == capture_data)
	{
		fprintf(stderr,"malloc failed --> [%s]:[%d]\n",__FILE__,__LINE__);
		return;
	}

	FILE* fp = NULL;

	fp = fopen("record.pcm","w+");
	if(NULL == fp)
	{
		perror("fopen failed\n");
		return;
	}

	printf("fwrite start ...\n");
	
	for(i=0;i<100;i++)
	{
		snd_pcm_sframes_t snd_res;
		snd_res = snd_pcm_readi(capture_handle,capture_data,vd_params.frames);
		if(snd_res!=vd_params.frames)
		{
			fprintf (stderr, "read from audio interface failed (%s)\n",
				 snd_strerror(snd_res));
		}
		fwrite(capture_data,sizeof(char),vd_params.rec_len,fp);
	}

	snd_pcm_close(capture_handle);
	free(capture_data);
	fclose(fp);

	pcm2wav("record.pcm","record.wav",1,16000,16);

	printf("record end ... \n");
	
	return;
}


int	  
main (int argc, char **argv)
{
	sal_thread_t capture_threadid = SAL_THREAD_ERROR;
	capture_threadid = sal_thread_create("capture thread",SAL_THREAD_STKSZ*512, 
									0,pcm_capture_thread,NULL);
	if(capture_threadid == SAL_THREAD_ERROR)
	{
		printf("pcm_capture_thread create failed\n");
		return -1;
	}

	while(1);
	
	return 0;
}


#if 0
int	  
test()
{
	int i;
	int err;
	short buf[128];
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;

	if ((err = snd_pcm_open (&capture_handle,"hw:0,0", SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n", 
			 "hw:0,0", snd_strerror (err));
		exit (1);
	}
	   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
			 
	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	unsigned int rate = 44100;
	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, &rate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (capture_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	for (i = 0; i < 10; ++i) {
		if ((err = snd_pcm_readi (capture_handle, buf, 128)) != 128) {
			fprintf (stderr, "read from audio interface failed (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

		int j;
		for(j=0;j<128;j++)
			printf("%d ",buf[j]);
		printf("\n");	
	}

	snd_pcm_close (capture_handle);
	exit (0);
}
#endif

