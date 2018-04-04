

#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */

#include "sal/core/thread.h"


#define SERVER_PORT 8111

void *  
udp_server_init(void *arg)
{
	int ret,sockfd;
		
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("udp_server_init\n");

	struct sockaddr_in to_sockaddr,from_sockaddr;

	to_sockaddr.sin_family = AF_INET;
	to_sockaddr.sin_port = htons(SERVER_PORT);
	to_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

//	from_sockaddr.sin_family = AF_INET;
//	from_sockaddr.sin_port = htons(CLIENT_PORT);
//	from_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	ret =bind(sockfd,(struct sockaddr*)&to_sockaddr,sizeof(to_sockaddr));
	if(ret<0)
		perror("bind:");

	
	char buf[512];

	while(1)
	{
		int count;
		socklen_t addrlen = sizeof(struct sockaddr);

		count = recvfrom(sockfd,buf,512,0,(struct sockaddr*)&from_sockaddr,&addrlen);
		if(count<0)
			perror("recvfrom");

		printf("server:revbuf=%s\n",buf);
		sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&from_sockaddr,addrlen);
	}

}


void * 
udp_client_init(void *arg)
{
	int sockfd;
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("udp_client_init\n");

	while(1)
	{
		int size;
		char buf[512];
		printf("Input>> ");
		
		if(!fgets(buf,512,stdin))
			continue;
			
		struct sockaddr_in to_sockaddr,from_sockaddr;

		to_sockaddr.sin_family = AF_INET;
		to_sockaddr.sin_port = htons(SERVER_PORT);
		to_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
		sendto(sockfd,buf,strlen(buf)+1,0,(struct sockaddr*)&to_sockaddr,sizeof(to_sockaddr));

		socklen_t addrlen = sizeof(struct sockaddr);
		recvfrom(sockfd,buf,512,0,(struct sockaddr*)&from_sockaddr,&addrlen);

		printf("client:revbuf=%s\n",buf);

	}
	
}


int 
main(int argc,char **argv)
{
	
	sal_thread_create("UDP_SERVER", 0, 0,udp_server_init,NULL);
	sal_thread_create("UDP_CLIENT", 0, 0,udp_client_init,NULL);

	while(1);
}
