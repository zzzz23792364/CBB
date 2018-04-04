

#include<stdio.h>

#include "webrtc_vad.h"


#define JUDGE_WINDOW_SIZE (300*16)
#define VAD_PROCESS_SIZE  (30*16)


typedef enum vad_status_e
{
	RUNNING = 0,
	LISTENING = 1
}vad_status_t;
	
vad_status_t current_status = LISTENING;


VadInst*
webrtc_vad_init()
{
	VadInst* pVad = NULL;  
    if (WebRtcVad_Create(&pVad))  
    {  
        perror("WebRtcVad_Create failed!");  
        return -1;  
    }  
  
    if (WebRtcVad_Init(pVad))  
    {  
        perror("WebRtcVad_Init failed!");  
        return -1;  
    }  
  
    if (WebRtcVad_set_mode(pVad, 0))  
    {  
        perror("WebRtcVad_set_mode failed!");  
        return -1;  
    } 

	return pVad;
}



int 
webrtc_vad_process(VadInst* pVad,short *buf)
{
	int vad_res;
	
	vad_res = WebRtcVad_Process(pVad, 16000, buf, VAD_PROCESS_SIZE); 
	
//	WebRtcVad_Free(pVad);  
	
	return vad_res;
}



int
voice_data_process(VadInst* pVad,short *audio_data)
{
	int i=0,vad_res;
	int have_voice;
	int num_per_judge = JUDGE_WINDOW_SIZE/VAD_PROCESS_SIZE;
	int sum_of_judge = 0;
	
	for(i=0;i<num_per_judge;i++)
	{
		vad_res = webrtc_vad_process(pVad,audio_data+i*VAD_PROCESS_SIZE);
		if(vad_res<0)
		{
			printf("webrtc_vad_process failed\n");
			return -1;
		}

		sum_of_judge += vad_res;
	}
	
	if((sum_of_judge * 2 ) > num_per_judge)
		have_voice = 1;
	else
		have_voice = 0;

	
	if(current_status == LISTENING)
	{
		if(have_voice)
			current_status = RUNNING;
	}
	else if(current_status == RUNNING)
	{
		if(have_voice == 0)
			current_status = LISTENING;
	}

	if(current_status == RUNNING)
		printf("data ... upload \n");
	else 
		printf("data ... no \n");
	
	return 0;	
}


#if 1
int 
main(int argc,char **argv)
{
	FILE *fp;
	char *audio_file = argv[1];

	fp = fopen(audio_file,"rb");
	if(NULL == fp)
	{
		printf("open audio file [%s]\n",audio_file);
		return -1;
	}

	fseek(fp,0,SEEK_END);
	long total_len = ftell(fp);
	fseek(fp,0,SEEK_SET);

	printf("total_len = %ld\n",total_len);

	int cur_len = 0;

	short buf[JUDGE_WINDOW_SIZE];

	VadInst *pVad = NULL;
	
	pVad = webrtc_vad_init();

	for(cur_len = 0;cur_len<total_len;cur_len += JUDGE_WINDOW_SIZE)
	{
		if(JUDGE_WINDOW_SIZE != fread(buf,2,JUDGE_WINDOW_SIZE,fp))
		{
			perror(fread);
			return -1;
		}

		voice_data_process(pVad,buf);
	}

	return 0;
}
#endif


