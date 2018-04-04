


#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */


#define MCB_SWM_QUERY_CHANGE_VER                12792//²éÑ¯½»»»Ð¾Æ¬°æ±¾
#define MCB_SWM_DLD_CHANGE_VER                  12793//ÏÂÔØ½»»»Ð¾Æ¬°æ±¾
#define MCB_SWM_ACT_CHANGE_VER                  12794//¼¤»î½»»»Ð¾Æ¬°æ±¾
#define MCB_SWM_CHECK_CHANGE_VER                12795//½»»»Ð¾Æ¬°æ±¾ÉÏµçÐ£Ñé


typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;


//CSPLÍ·²¿
typedef struct cspl_header_s
{
    U32 magic;      /*¹Ì¶¨Ìî0*/
    U32 transId;    /*¹Ì¶¨Ìî0*/
    U32 srcId;      /*¹Ì¶¨Ìî195*/
    U32 dstId;      /*¹Ì¶¨Ìî1*/
    U16 apiId;      /*½»»¥ÏûÏ¢ºÅ*/
    U16 prio;       /*¹Ì¶¨Ìî5*/
    U32 length;     /*ÏûÏ¢³¤¶È*/
    U16 paramLen;   /*¹Ì¶¨Ìî0*/
    U16 payloadLen; /*¹Ì¶¨Ìî0*/
    U8  reserv[4];  /*±£Áô×Ö¶Î*/
} cspl_header_t;


typedef struct ENODEB_DEV
{
	U8 shNum;                                       
	U8 chNum;                   
	U8 slNum;                   
}ENODEB_DEV;

typedef struct VER_CHECK_REQUEST
{
	ENODEB_DEV tPos;
	U32  ulHardId;
	U32  ulRandNum;
	U8   ausVerNo[50];
} VER_CHECK_REQUEST;

typedef struct VER_CHECK_ACK
{
	ENODEB_DEV tPos; 
	U8   ausVerNo[50];
	U8   ausFilePath[255];
	bool	bIsReboot;		/*1---ç«‹å³ï¼Œ0---ä¸é‡å¯*/
} VER_CHECK_ACK;

typedef struct VER_QUERY
{
	U32 		ulRandNum;
	ENODEB_DEV	tPos;
}VER_QUERY;


typedef enum MSG_ID
{
	QUERY = 12786,
	DLD   = 12787,
	ACT   = 12788,
	CHECK = 12789
} MSG_ID;


typedef struct UDP_MSG
{
	cspl_header_t type;
	U8 msg[512];
} UDP_MSG;

typedef struct VER_DLD_REQUEST
{
ENODEB_DEV tPos; 
U8   ausVerNo[50];
U8   ausFilePath[255];
} VER_DLD_REQUEST;

typedef struct VER_ACT_REQUEST
{
    ENODEB_DEV tPos;
    U8 ausVerNo [50];
} VER_ACT_REQUEST;

typedef struct VER_ACT_ACK
{
    ENODEB_DEV tPos;
    U32 ulActResult;
} VER_ACT_ACK;

typedef struct VER_QUERY_ACK
{
ENODEB_DEV tPos;
U32  ulHardId;
U32  ulRandNum;
U8   ausVerNo[50];
} VER_QUERY_ACK;

typedef struct VER_DLD_ACK
{
    ENODEB_DEV tPos;
    U32 ulDownResult;
} VER_DLD_ACK;

typedef struct VER_CHECK_RESULT_REPORT
{
	ENODEB_DEV tPos; 
	U8   	usUpdateRslt;	/*0---å‡çº§æˆåŠŸï¼Œéž0---å‡çº§å¤±è´¥*/
} VER_CHECK_RESULT_REPORT;


#define BSP_VERSION "10GMCB_SWITCH_eNB_V17B.005.3"
#define FILE_PATH "switch.tar"

#define SERVER_PORT 10157
#define CLINET_PORT 6666

int sockfd;


void send_query(struct sockaddr* addr)
{
	UDP_MSG udp_msg;

	udp_msg.type.apiId = MCB_SWM_QUERY_CHANGE_VER;

	VER_QUERY request_data;

	request_data.ulRandNum = 0;
	request_data.tPos.chNum = 1;
	request_data.tPos.shNum = 2;
	request_data.tPos.slNum = 3;

	memcpy(udp_msg.msg,&request_data,sizeof(request_data));

	sendto(sockfd,&udp_msg,sizeof(udp_msg),0,addr,sizeof(struct sockaddr_in));
}

void send_dld(struct sockaddr* addr)
{
	UDP_MSG udp_msg;

	udp_msg.type.apiId = MCB_SWM_DLD_CHANGE_VER;

	VER_DLD_REQUEST request_data;

	request_data.tPos.chNum = 1;
	request_data.tPos.shNum = 2;
	request_data.tPos.slNum = 3;
	strcpy(request_data.ausFilePath,FILE_PATH);
	strcpy(request_data.ausVerNo,BSP_VERSION);

	memcpy(udp_msg.msg,&request_data,sizeof(request_data));

	sendto(sockfd,&udp_msg,sizeof(udp_msg),0,addr,sizeof(struct sockaddr_in));
}

void send_act(struct sockaddr* addr)
{
	UDP_MSG udp_msg;

	udp_msg.type.apiId = MCB_SWM_ACT_CHANGE_VER;

	VER_ACT_REQUEST request_data;

	request_data.tPos.chNum = 1;
	request_data.tPos.shNum = 2;
	request_data.tPos.slNum = 3;
	strcpy(request_data.ausVerNo,BSP_VERSION);

	memcpy(udp_msg.msg,&request_data,sizeof(request_data));

	sendto(sockfd,&udp_msg,sizeof(udp_msg),0,addr,sizeof(struct sockaddr_in));
}


void query_handler(UDP_MSG *msg)
{
	VER_QUERY_ACK ack_data;

	memcpy(&ack_data,msg->msg,sizeof(ack_data));
	
	printf("query-ulRandNum:%d\n",ack_data.ulRandNum);
	printf("query-ulHardId:%d\n",ack_data.ulHardId);
	printf("query-ausVerNo:%s\n",ack_data.ausVerNo);
	printf("query-chNum:%d\n",ack_data.tPos.chNum);
	printf("query-shNum:%d\n",ack_data.tPos.shNum);
	printf("query-slNum:%d\n",ack_data.tPos.slNum);
}

void dld_handler(UDP_MSG *msg)
{
	VER_DLD_ACK ack_data;

	memcpy(&ack_data,msg->msg,sizeof(ack_data));
	
	printf("dld-ulDownResult:%d\n",ack_data.ulDownResult);
	printf("dld-chNum:%d\n",ack_data.tPos.chNum);
	printf("dld-shNum:%d\n",ack_data.tPos.shNum);
	printf("dld-slNum:%d\n",ack_data.tPos.slNum);
}

void act_handler(UDP_MSG *msg)
{
	VER_ACT_ACK ack_data;

	memcpy(&ack_data,msg->msg,sizeof(ack_data));
	
	printf("act-ulActResult:%d\n",ack_data.ulActResult);
	printf("act-chNum:%d\n",ack_data.tPos.chNum);
	printf("act-shNum:%d\n",ack_data.tPos.shNum);
	printf("act-slNum:%d\n",ack_data.tPos.slNum);
}



void 
ack_handler(UDP_MSG *msg)
{
	switch(msg->type.apiId)
	{
		case QUERY:
			query_handler(msg);
			break;
		case DLD:
			dld_handler(msg);
			break;
		case ACT:
			act_handler(msg);
			break;
	}
}

void 
fill_cspl_header(U16 msg_id,UDP_MSG *msg)
{
	msg->type.apiId = msg_id;
	msg->type.dstId = 1;
	msg->type.length = sizeof(UDP_MSG);
	msg->type.magic = 0;
	msg->type.paramLen = 0;
	msg->type.prio = 5;
	msg->type.payloadLen = 0;
	msg->type.srcId = 195;
	msg->type.transId = 0;
	memset(msg->type.reserv,0,sizeof(msg->type.reserv));
}


int 
main(int argc,char **argv)
{
	int ret;
	
	VER_CHECK_REQUEST request_data;
	VER_CHECK_ACK ack_data;

	memset(&ack_data,0,sizeof(ack_data));
	ack_data.tPos.chNum = 1;
	ack_data.tPos.shNum = 2;
	ack_data.tPos.slNum = 3;
	ack_data.bIsReboot = 0;
	strcpy(ack_data.ausFilePath,FILE_PATH);
	strcpy(ack_data.ausVerNo,BSP_VERSION);
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("udp_client_init\n");

	struct sockaddr_in to_sockaddr,from_sockaddr;

	to_sockaddr.sin_family = AF_INET;
	to_sockaddr.sin_port = htons(SERVER_PORT);
	to_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//	to_sockaddr.sin_addr.s_addr = inet_addr("10.161.50.101");

	

	ret =bind(sockfd,(struct sockaddr*)&to_sockaddr,sizeof(to_sockaddr));
	if(ret<0)
		perror("bind:");

	int size;
			
			socklen_t addrlen = sizeof(to_sockaddr);

#if 1
//	while(1)
	{
		fd_set rd;
		struct timeval tv;
		
		FD_ZERO(&rd);
		FD_SET(sockfd,&rd);
		
		tv.tv_sec = 2;
		tv.tv_usec = 0;


	#if 0
		from_sockaddr.sin_family = AF_INET;
		from_sockaddr.sin_port = htons(CLINET_PORT);
		//		to_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		from_sockaddr.sin_addr.s_addr = inet_addr("10.161.50.110");
		
		size = sendto(sockfd,&ack_data,sizeof(ack_data),0,(struct sockaddr*)&from_sockaddr,sizeof(from_sockaddr));
		if(size<0)
			perror("sendto:");
	#endif


	#if 0
		ret = select(sockfd+1,&rd,NULL,NULL,&tv);
		if(ret == 0 )
			printf("timeout\n");
		else if(ret == -1)
			printf("failed\n");
		else
		{
			size = recvfrom(sockfd,&request_data,sizeof(request_data),0,(struct sockaddr*)&from_sockaddr,&addrlen);
			printf("recvfrom\n");
			if(size<0)
				perror("recvfrom:");
		}
	#endif

		printf("boot-check-start\n");

		UDP_MSG udp_msg;

		size = recvfrom(sockfd,&request_data,sizeof(request_data),0,(struct sockaddr*)&from_sockaddr,&addrlen);
		if(size<0)
			perror("recvfrom:");

//		printf("ip=%s ....\n",inet_ntoa(from_sockaddr.sin_addr)); 


//		from_sockaddr.sin_family = AF_INET;
//		from_sockaddr.sin_port = htons(CLINET_PORT);
//		to_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//		from_sockaddr.sin_addr.s_addr = inet_addr("10.161.50.110");

		fill_cspl_header(MCB_SWM_CHECK_CHANGE_VER,&udp_msg);
		memcpy(udp_msg.msg,&ack_data,sizeof(ack_data));
		
		size = sendto(sockfd,&udp_msg,sizeof(udp_msg),0,(struct sockaddr*)&from_sockaddr,sizeof(from_sockaddr));
		if(size<0)
			perror("sendto:");

		size = recvfrom(sockfd,&udp_msg,sizeof(udp_msg),0,(struct sockaddr*)&from_sockaddr,&addrlen);
		if(size<0)
			perror("recvfrom:");

		VER_CHECK_RESULT_REPORT report_data;
	
		memcpy(&report_data,udp_msg.msg,sizeof(report_data));
		printf("report->usUpdateRslt=%d\n",report_data.usUpdateRslt);

	}
#endif


	


	while(1)
	{
//		size = sendto(sockfd,&ack_data,sizeof(ack_data),0,(struct sockaddr*)&from_sockaddr,sizeof(from_sockaddr));

//		if(size<0)
//			perror("sendto:");
//		printf("sendtosendtosendto\n");


		from_sockaddr.sin_port = htons(8888);
		from_sockaddr.sin_family = AF_INET;
		from_sockaddr.sin_addr.s_addr = inet_addr("10.161.50.110");


		char sd_buf[512],rc_buf[512];
		printf("Input>> ");

		if(!fgets(sd_buf,512,stdin))
			continue;
		if(!strncasecmp(sd_buf,"q",1))
			send_query(&from_sockaddr);
		
		if(!strncasecmp(sd_buf,"d",1))
			send_dld(&from_sockaddr);

		if(!strncasecmp(sd_buf,"a",1))
			send_act(&from_sockaddr);

		UDP_MSG ack_data;

		size = recvfrom(sockfd,&ack_data,sizeof(ack_data),0,(struct sockaddr*)&from_sockaddr,&addrlen);
		if(size<0)
			perror("recvfrom:");
		ack_handler(&ack_data);

	}
	
}
