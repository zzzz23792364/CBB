#include <stdio.h>
#include <string.h>

#include "cbb_types.h"

typedef U8 ID[4];


typedef struct wave_header_s
{
	ID  id;
	U32 size;
	ID format;
}wave_header_t;

typedef struct fmt_chunk_s
{
	ID subid;
	U32 subsize;
	U16 audio_fmt;//如果为PCM，改值为 1
	U16 num_chn;
	U32 sample_rate; 
	U32 byte_rate;	// = SampleRate*NumChannels*uiBitsPerSample/8 
	U16 block_align; // = NumChannels*BitsPerSample/8
	U16 bits_per_sample;//每个采样点的bit数，一般为8,16,32
}fmt_chunk_t;


typedef struct data_chunk_s
{
	ID subid;
	U32 subsize;
}data_chunk_t;


/*****************************************************************************
 * Function      : pcm2wav
 * Description   : 将PCM格式的音频转换成WAV格式
 * Input         : const S8* pcm_path   
                const S8* wave_path  
                U16 channels         
                U32 rate             
                U16 bits             
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180314
 *   Author      : ZengChao
 *   Modification: Created function

*****************************************************************************/
int 
pcm2wav(const S8* pcm_path,const S8* wave_path,U16 channels,U32 rate,U16 bits)
{
	FILE *pcm_fp,*wave_fp;
	wave_header_t wav_header;
	fmt_chunk_t fmt_chunk;
	data_chunk_t data_chunk;
	long pcm_file_len,read_len;

	pcm_fp = fopen(pcm_path,"rb");
	if(NULL == pcm_fp)
	{
	  	printf("Open pcm file error.\n");
        return -1;	
	}

	wave_fp = fopen(wave_path,"wb+");
	if(NULL == wave_fp)
	{
	  	printf("Create wav file error.\n");
        return -1;	
	}

	fseek(pcm_fp, 0, SEEK_END);
	pcm_file_len = ftell(pcm_fp);
	fseek(pcm_fp, 0, SEEK_SET);

	/* WAVE_HEADER */
    memcpy(wav_header.id , "RIFF", strlen("RIFF"));
    memcpy(wav_header.format, "WAVE", strlen("WAVE"));
    fseek(wave_fp, sizeof(wave_header_t), SEEK_CUR);

	/* WAVE_FMT */
    memcpy(fmt_chunk.subid, "fmt ", strlen("fmt "));
	fmt_chunk.subsize = 16;
	fmt_chunk.audio_fmt = 1;
	fmt_chunk.sample_rate = rate;
	fmt_chunk.num_chn = channels;
	fmt_chunk.bits_per_sample = bits;
	fmt_chunk.byte_rate = rate*channels*bits/8;
	fmt_chunk.block_align = channels*bits/8;
    fwrite(&fmt_chunk, sizeof(fmt_chunk_t), 1,wave_fp);
	
    /* WAVE_DATA */
    memcpy(data_chunk.subid, "data", strlen("data"));
    data_chunk.subsize = pcm_file_len;
	fwrite(&data_chunk,sizeof(data_chunk_t),1,wave_fp);

	char buf[1024];
	
	while((read_len = fread(buf, 1, sizeof(buf), pcm_fp)) != 0) 
		fwrite(buf, 1, read_len, wave_fp);

	fseek(wave_fp, 0, SEEK_SET);
	wav_header.size = 4 + (8 + fmt_chunk.subsize) + ( 4 + data_chunk.subsize);
	fwrite(&wav_header,sizeof(wav_header),1,wave_fp);

	fclose(pcm_fp);
	fclose(wave_fp);

	return 0;
}











#if 0



/**
 * Convert PCM16LE raw data to WAVE format
 * @param pcmpath       Input PCM file.
 * @param channels      Channel number of PCM file.
 * @param sample_rate   Sample rate of PCM file.
 * @param wavepath      Output WAVE file.
 */
int simplest_pcm16le_to_wave(const char *pcmpath, int channels, int sample_rate, 
										const char *wavepath)
{
    typedef struct WAVE_HEADER{
        char    fccID[4];       //内容为""RIFF
        unsigned int dwSize;   //最后填写，WAVE格式音频的大小
        char    fccType[4];     //内容为"WAVE"
    }WAVE_HEADER;

    typedef struct WAVE_FMT{
        char    fccID[4];          //内容为"fmt "
        unsigned int  dwSize;     //内容为WAVE_FMT占的字节数，为16
        unsigned short wFormatTag; //如果为PCM，改值为 1
        unsigned short wChannels;  //通道数，单通道=1，双通道=2
        unsigned int  dwSamplesPerSec;//采用频率
        unsigned int  dwAvgBytesPerSec;/* ==dwSamplesPerSec*wChannels*uiBitsPerSample/8 */
        unsigned short wBlockAlign;//==wChannels*uiBitsPerSample/8
        unsigned short uiBitsPerSample;//每个采样点的bit数，8bits=8, 16bits=16
    }WAVE_FMT;

    typedef struct WAVE_DATA{
        char    fccID[4];       //内容为"data"
        unsigned int dwSize;   //==NumSamples*wChannels*uiBitsPerSample/8
    }WAVE_DATA;

    if(channels==2 || sample_rate==0)
    {
        channels = 2;
        sample_rate = 44100;
    }
    int bits = 16;

    WAVE_HEADER pcmHEADER;
    WAVE_FMT    pcmFMT;
    WAVE_DATA   pcmDATA;

    unsigned short m_pcmData;
    FILE *fp, *fpout;

    fp = fopen(pcmpath, "rb+");
    if(fp==NULL)
    {
        printf("Open pcm file error.\n");
        return -1;
    }
    fpout = fopen(wavepath, "wb+");
    if(fpout==NULL)
    {
        printf("Create wav file error.\n");
        return -1;
    }

    /* WAVE_HEADER */
    memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
    memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
    fseek(fpout, sizeof(WAVE_HEADER), 1);   //1=SEEK_CUR
    /* WAVE_FMT */
    memcpy(pcmFMT.fccID, "fmt ", strlen("fmt "));
    pcmFMT.dwSize = 16;
    pcmFMT.wFormatTag = 1;
    pcmFMT.wChannels = channels;
    pcmFMT.dwSamplesPerSec = sample_rate;
    pcmFMT.uiBitsPerSample = bits;
    /* ==dwSamplesPerSec*wChannels*uiBitsPerSample/8 */
    pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec*pcmFMT.
    						 wChannels*pcmFMT.uiBitsPerSample/8;
    /* ==wChannels*uiBitsPerSample/8 */
    pcmFMT.wBlockAlign = pcmFMT.wChannels*pcmFMT.uiBitsPerSample/8;


    fwrite(&pcmFMT, sizeof(WAVE_FMT), 1, fpout);

    /* WAVE_DATA */
    memcpy(pcmDATA.fccID, "data", strlen("data"));
    pcmDATA.dwSize = 0;
    fseek(fpout, sizeof(WAVE_DATA), SEEK_CUR);

    fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    while(!feof(fp))
    {
        pcmDATA.dwSize += 2;
        fwrite(&m_pcmData, sizeof(unsigned short), 1, fpout);
        fread(&m_pcmData, sizeof(unsigned short), 1, fp);
    }

    /*pcmHEADER.dwSize = 44 + pcmDATA.dwSize;*/
    //修改时间：2018年1月5日
    pcmHEADER.dwSize = 36 + pcmDATA.dwSize;

    rewind(fpout);
    fwrite(&pcmHEADER, sizeof(WAVE_HEADER), 1, fpout);
    fseek(fpout, sizeof(WAVE_FMT), SEEK_CUR);
    fwrite(&pcmDATA, sizeof(WAVE_DATA), 1, fpout);

    fclose(fp);
    fclose(fpout);

    return 0;
}
#endif

