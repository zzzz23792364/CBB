

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "noise_suppression.h"



void TestNs(char *szFileIn, int nSample, int nMode)  
{  
    int nRet = 0;  
    NsHandle *pNS_inst = NULL;  
  
    FILE *fpIn = NULL;  
    FILE *fpOut = NULL;  
  
    char *pInBuffer = NULL;  
    char *pOutBuffer = NULL;  
  
    do  
    {  
        int i = 0;  
        int nFileSize = 0;  
        int nTime = 0;  
        if (0 != WebRtcNs_Create(&pNS_inst))  
        {  
            printf("Noise_Suppression WebRtcNs_Create err! \n");  
            break;  
        }  
  
        if (0 != WebRtcNs_Init(pNS_inst, nSample))  
        {  
            printf("Noise_Suppression WebRtcNs_Init err! \n");  
            break;  
        }  
  
        if (0 != WebRtcNs_set_policy(pNS_inst, nMode))  
        {  
            printf("Noise_Suppression WebRtcNs_set_policy err! \n");  
            break;  
        }  
  
        fpIn = fopen(szFileIn, "rb");  
        if (NULL == fpIn)  
        {  
            printf("open src file err \n");  
            break;  
        }  
        fseek(fpIn, 0, SEEK_END);  
        nFileSize = ftell(fpIn);  
        fseek(fpIn, 0, SEEK_SET);  
  
        pInBuffer = (char*)malloc(nFileSize);  
        memset(pInBuffer, 0, nFileSize);  
        fread(pInBuffer, sizeof(char), nFileSize, fpIn);  
  
        pOutBuffer = (char*)malloc(nFileSize);  
        memset(pOutBuffer, 0, nFileSize);  
  
        nTime = time(0);  
        for (i = 0; i < nFileSize; i += 320)  
        {  
            if (nFileSize - i >= 320)  
            {  
                short shBufferIn[160] = { 0 };  
                short shBufferOut[160] = { 0 };  
//                float fBufferIn[160] = { 0 };  
//                float fBufferOut[160] = { 0 };  
  
                memcpy(shBufferIn, (char*)(pInBuffer + i), 160 * sizeof(short));  
//                for (int k = 0; k < 160; ++k)  
//                {  
//                    fBufferIn[k] = (float)shBufferIn[k];  
//                }  
  
                if (0 == WebRtcNs_Process(pNS_inst, shBufferIn, NULL, shBufferOut, NULL))  
                {  
//                    for (int k = 0; k < 160; ++k)  
//                    {  
//                        shBufferOut[k] = (short)fBufferOut[k];  
//                    }  
                    memcpy(pOutBuffer + i, shBufferOut, 160 * sizeof(short));  
                }  
            }  
        }  
  
        nTime = (int)time(0) - nTime;  
        printf("n_s user time=%dms\n", nTime);  
        char szFileOut[20] = "result.pcm";  
//        char* pLine = strchr(szFileIn, '\\');  
//        strcat(szFileOut, pLine);  
        fpOut = fopen(szFileOut, "wb");  
        if (NULL == fpOut)  
        {  
            printf("open out file err! \n");  
            break;  
        }  
        fwrite(pOutBuffer, sizeof(char), nFileSize, fpOut);  
    } while (0);  
  
    WebRtcNs_Free(pNS_inst);  
    fclose(fpIn);  
    fclose(fpOut);  
    free(pInBuffer);  
    free(pOutBuffer);  
}  



int main(int argc,char **argv)
{
	char szFileIn[] = "record-1.pcm";

	int nSample = 16000;
	int nMode = 3;
	
	TestNs(szFileIn,nSample,nMode);

	return 0;
}



#if 0


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "noise_suppression.h"



void TestNs(char *szFileIn, int nSample, int nMode)  
{  
    int nRet = 0;  
    NsHandle *pNS_inst = NULL;  
  
    FILE *fpIn = NULL;  
    FILE *fpOut = NULL;  
  
    char *pInBuffer = NULL;  
    char *pOutBuffer = NULL;  
  
    do  
    {  
        int i = 0;  
        int nFileSize = 0;  
        int nTime = 0;  
        if (0 != WebRtcNs_Create(&pNS_inst))  
        {  
            printf("Noise_Suppression WebRtcNs_Create err! \n");  
            break;  
        }  
  
        if (0 != WebRtcNs_Init(pNS_inst, nSample))  
        {  
            printf("Noise_Suppression WebRtcNs_Init err! \n");  
            break;  
        }  
  
        if (0 != WebRtcNs_set_policy(pNS_inst, nMode))  
        {  
            printf("Noise_Suppression WebRtcNs_set_policy err! \n");  
            break;  
        }  
  
        fpIn = fopen(szFileIn, "rb");  
        if (NULL == fpIn)  
        {  
            printf("open src file err \n");  
            break;  
        }  
        fseek(fpIn, 0, SEEK_END);  
        nFileSize = ftell(fpIn);  
        fseek(fpIn, 0, SEEK_SET);  
  
        pInBuffer = (char*)malloc(nFileSize);  
        memset(pInBuffer, 0, nFileSize);  
        fread(pInBuffer, sizeof(char), nFileSize, fpIn);  
  
        pOutBuffer = (char*)malloc(nFileSize);  
        memset(pOutBuffer, 0, nFileSize);  
  
        nTime = time(0);  
        for (i = 0; i < nFileSize; i += 320)  
        {  
            if (nFileSize - i >= 320)  
            {  
                short shBufferIn[160] = { 0 };  
                short shBufferOut[160] = { 0 };  
                float fBufferIn[160] = { 0 };  
                float fBufferOut[160] = { 0 };  
  
                memcpy(shBufferIn, (char*)(pInBuffer + i), 160 * sizeof(short));  
                for (int k = 0; k < 160; ++k)  
                {  
                    fBufferIn[k] = (float)shBufferIn[k];  
                }  
  
                if (0 == WebRtcNs_Process(pNS_inst, fBufferIn, NULL, fBufferOut, NULL))  
                {  
                    for (int k = 0; k < 160; ++k)  
                    {  
                        shBufferOut[k] = (short)fBufferOut[k];  
                    }  
                    memcpy(pOutBuffer + i, shBufferOut, 160 * sizeof(short));  
                }  
            }  
        }  
  
        nTime = (int)time(0) - nTime;  
        printf("n_s user time=%dms\n", nTime);  
        char szFileOut[20] = "result.pcm";  
//        char* pLine = strchr(szFileIn, '\\');  
//        strcat(szFileOut, pLine);  
        fpOut = fopen(szFileOut, "wb");  
        if (NULL == fpOut)  
        {  
            printf("open out file err! \n");  
            break;  
        }  
        fwrite(pOutBuffer, sizeof(char), nFileSize, fpOut);  
    } while (0);  
  
    WebRtcNs_Free(pNS_inst);  
    fclose(fpIn);  
    fclose(fpOut);  
    free(pInBuffer);  
    free(pOutBuffer);  
}  



int main(int argc,char **argv)
{
	char szFileIn[] = "record-4.pcm";

	int nSample = 16000;
	int nMode = 0;
	
	TestNs(szFileIn,nSample,nMode);

	return 0;
}
#endif

