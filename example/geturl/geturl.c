




/*****************************************************************************
 * Function      : url_to_sockaddr
 * Description   : 将URL转换成SOCKET地址
 * Input         : char *url                  
                struct sockaddr* sockaddr  
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180403
 *   Author      : ZengChao
 *   Modification: Created function

*****************************************************************************/
int
url_to_sockaddr(char *url,struct sockaddr* sockaddr)
{	
	int ret;
	char *startUrl,*stopUrl;
	struct addrinfo hints;
	struct addrinfo *res;

	/*去掉http[s]://前缀*/
	startUrl = strstr(url,"//");
	if(NULL == startUrl)
	{
		printf("Wrong url [%s]: no // ",url);
		return -1;
	}

	/*从第一个'/'截断*/
	stopUrl = strchr(startUrl,'/');
	if(NULL != *stopUrl)	
		*stopUrl = NULL;

	printf("real url:%s\n",startUrl);

	/*为调用getaddrinfo做准备*/
	memset(&hints,0,sizeof(hints));
	hints.ai_flags = AI_PASSIVE; /* For wildcard IP address */
	hints.ai_family = AF_INET;/* Allow IPv4 */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0 /* Any protocol */

	ret = getaddrinfo(startUrl,NULL,&hints,&res);
	if(ret<0)
	{
		printf("getaddrinfo failed:%s\n",strerror(errno));
		return -1;
	}

	if(NULL == res)
	{
		printf("Error: not get ip address\n");
		return -1;
	}
	
	memcpy(sockaddr,res->ai_addr,sizeof(struct sockaddr));
	freeaddrinfo(res);
	
	return 0;	
}

/*****************************************************************************
 * Function      : establish_tcp_connect
 * Description   : 与URL对应的服务器建立连接
 * Input         : char *url         
 * Output        : None
 * Return        : 
 * Others        : 
 * Record
 * 1.Date        : 20180403
 *   Author      : ZengChao
 *   Modification: Created function

*****************************************************************************/
int 
establish_tcp_connect(char *url)
{
	int ret,sockfd;
	struct sockaddr sockaddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("socket failed:%s\n",strerror(errno));
		return -1;
	}

	ret = get_url_resource(url, &sockaddr);
	if(ret<0)
	{
		close(sockfd);
		printf("get_url_resource failed\n");
		return -1;
	}	

	ret = connect(sockfd,&sockaddr,sizeof(struct sockaddr));
	if(ret<0)
	{
		close(sockfd);
		printf("connect failed:%s\n",strerror(errno));
		return -1;
	}

	return sockfd;
}

int 
get_url_resource(int sockfd,char* url,char *file_name)
{
	char buf[1000];
	U32 rd_len;
	U32 wt_len;
	memset(buf,0,sizeof(buf));

	snprintf(buf,sizeof(buf),
			"GET %s\r\n"
			url
			);

	/*通过TCP发送数据*/
	wt_len = strlen(buf);
	if(wt_len != write(sockfd,buf,wt_len))
	{
		printf("write failed:%s\n",strerror(errno));
		return -1;
	}

	/*通过TCP接收数据*/
	memset(buf,0,sizeof(buf));
	rd_len = read(sockfd,buf,sizeof(buf));
	if(rd_len<0)
	{
		printf("read failed:%s\n",strerror(errno));
		return -1;
	}	

	/*关闭socket连接*/
	close(sockfd);

	/*
	 * 服务器返回的数据格式：
	 *---------------------------------------------------------
	 *                      HTTP/1.1 200 OK
	 *                      Server: nginx/1.10.1
	 *                      Date: Thu, 16 Mar 2017 06:06:13 GMT
	 *                      Content-Type: text/json; charset=UTF-8
	 *                      Transfer-Encoding: chunked
	 *                      Connection: keep-alive
	 *                      Access-Control-Allow-Origin: *
	 *
	 *                      4c
	 *                      {"code":100000,"text":"我不会说英语的啦，你还是说中文吧。"}
	 *                      0
	 *---------------------------------------------------------
	 *需要提取的内容为code和text冒号后面的内容
	 *code和text的含义请去图灵官网查看，还有好多的类型。。。
	*/

	
}

int 
main(int argc,char **argv)
{
	int ret,sockfd;
	char url[] = "";
	char file_name[] = "url.mp3";
	
	sockfd = establish_tcp_connect(url);	
	if(sockfd<0)
	{
		printf("establish_tcp_connect failed\n");
		return -1;
	}
	
	get_url_resource(sockfd,file_name);
}
