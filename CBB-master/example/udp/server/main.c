

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
udp_server_init()
{
	int ret,sockfd;
		
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
		perror("udp_server_init");

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

	while(1)
	{
		char sd_buf[512],rc_buf[512];
		int count;
		socklen_t addrlen;

		count = recvfrom(sockfd,rc_buf,512,0,(struct sockaddr*)&from_sockaddr,&addrlen);
		if(count<0)
			perror("recvfrom");

		printf("server:revbuf=%s",rc_buf);

		sprintf(sd_buf,"server:I am server\n");
		
		sendto(sockfd,sd_buf,strlen(sd_buf),0,(struct sockaddr*)&from_sockaddr,addrlen);
	}

}


int 
main(int argc,char **argv)
{

	udp_server_init();

}
