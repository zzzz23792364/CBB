

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
#define CLIENT_PORT 6666

int 
udp_client_init()
{
	int sockfd;
	
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("udp_client_init\n");

	while(1)
	{
		int size;
		char sd_buf[512],rc_buf[512];
		printf("Input>> ");
		
		if(!fgets(sd_buf,512,stdin))
			continue;
			
		struct sockaddr_in to_sockaddr,from_sockaddr;

		to_sockaddr.sin_family = AF_INET;
		to_sockaddr.sin_port = htons(SERVER_PORT);
		to_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
		sendto(sockfd,sd_buf,strlen(sd_buf)+1,0,(struct sockaddr*)&to_sockaddr,sizeof(to_sockaddr));

		socklen_t addrlen;
		recvfrom(sockfd,rc_buf,512,0,(struct sockaddr*)&from_sockaddr,&addrlen);

		printf("client:revbuf=%s\n",rc_buf);

	}
	
}


int 
main(int argc,char **argv)
{
	udp_client_init();
}
