
#include<stdio.h>

#include "webrtc_vad.h"



#if 0
void TestVAD(char* pAudioFile,char* pResFile,int nSample,int nMode)  
{  
    VadInst* pVad = NULL;  
    if (WebRtcVad_Create(&pVad))  
    {  
        perror("WebRtcVad_Create failed!");  
        return;  
    }  
  
    if (WebRtcVad_Init(pVad))  
    {  
        perror("WebRtcVad_Init failed!");  
        return;  
    }  
  
    if (WebRtcVad_set_mode(pVad, nMode))  
    {  
        perror("WebRtcVad_set_mode failed!");  
        return;  
    }  

	#define LEN 480
	
    FILE* fp = NULL;  
    FILE* fpR = NULL;  
    fp = fopen(pAudioFile, "rb");  
    fpR = fopen(pResFile, "wb");  
    fseek(fp, 0, SEEK_END);  
    unsigned int nLen = ftell(fp);  
    fseek(fp, 0, SEEK_SET);  
    short shBufferIn[LEN] = { 0 };  
    while (1)  
    {  
        if (LEN != fread(shBufferIn, 2, LEN, fp))  
            break;  
//        int nRet = WebRtcVad_Process(pVad, 16000, shBufferIn, LEN);  

		 int nRet = voice_data_process(pVad,shBufferIn);
		
        printf("%d ", nRet);  
//        if (1 == nRet)  
//        {  
//            fwrite(shBufferIn, 2, 160, fpR);  
//        }  
    }  
  
//    fclose(fpR);  
    fclose(fp);  
    WebRtcVad_Free(pVad);  
}  

int
main(int argc,char **argv)
{
	char * pAudioFile = argv[1];
//	char * pAudioFile = "16kVoice.pcm";
	char * pResFile = "test.txt";
	int nSample = 16000;
	int nMode = 0;

	TestVAD(pAudioFile,pResFile,nSample,nMode);

	return 0;
}
#endif

#if 0
int main()
{
	int ret = 0;
	int i = 0;
	int kRates[] = { 8000, 12000, 16000, 24000, 32000, 48000 };
	//int kMaxFrameLength = 1440;
	int kFrameLengths[] = { 80, 120, 160, 240, 320, 480, 640, 960 };
	short zeros[640] = {0};
	short speech[640] = {0};
	
	for (i = 0; i < 640; i++) {
		speech[i] = (i * i);
	}
	
	VadInst* handle = NULL;
	int mode = 0;
	WebRtcVad_Create(&handle);
	WebRtcVad_Init(handle);
	WebRtcVad_set_mode(handle,mode);
	
	ret = WebRtcVad_Process(handle, kRates[0], zeros, kFrameLengths[0]);
	printf("ret1 = %d\n",ret);
	
	ret = WebRtcVad_Process(handle, kRates[0], speech, kFrameLengths[0]);
	printf("ret2 = %d\n",ret);	
	
	
	return 0;
}
#endif


