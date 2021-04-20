#include<unistd.h>
#include<sys/socket.h> /*socket*/
#include<sys/types.h>
#include<netdb.h>
#include<stdio.h>
#include<errno.h> /*perror()*/
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdlib.h>// for atoi()
#include<netinet/in.h>
#include<arpa/inet.h>//for inet_addr()
// Using:
////       ./client_web ip:port 
//       ./client_web url
//char char_at_str(const char *d, const char f)
char char_at_str(const char f,const char *d)
{
	const char *c=d;
	while(*d)
		if(*d++==f)
			return d-c-1;
	return 0;
}
void char_sit_at_char_in_str(char *d, const char s, const char r)
{
	d--;
	while(*++d)
	if(*d==s)
		*d=r;
}
// 0 domain 
// 1 ip
//"http://" should be the seven element of http_des array
char is_ip(const char *http_des)
{
	//http_des+=7;
	//http_des=strstr(http_des,"://")+3;// In case that it is "https://"
	//http_des=strstr(http_des,"://");// In case that it is "https://"
	//if(http_des==NULL)
	//if(http_des==0x3)
	//	return 0;
	//http_des+=3;
	char c=0;//count the times of symbol period
//	while(*http_des)
//	while(*http_des)
	while(1)
	{
	/*
		if('.'==*http_des)
		c++;
		else 
			if(*http_des<48||*http_des>57)
				return 0;
*/
		switch(*http_des)
		{
			case '\0':
			case ':':
				goto bye;
			case '.':
				c++;
				break;
			case ' ':
				printf("invalid url for space in \n");
				return -1;
				break;
			default:
				//if(*http_des<48||*http_des>57)
				if(*http_des<'0'||*http_des>'9')
					return 0;
		}
		http_des+=1;	
	}
	//bye:return 1;
	bye: if(c==3)
			return 1;
		else
			return 0;
}
const char * terminology[]={"http://"};
char space[1024*4];
int main(int C,char *COMMAND[])
{
	if(2!=C)
	{
		printf("Usage:\n%s url\n", COMMAND[0]);
		return -1;
	}

//	char * url=strstr(COMMAND[1],"http://");
	char * url=strstr(COMMAND[1],"://");
	int s=0;
	/*
	if(url==NULL)
	{
//		s=strlen(COMMAND[1]);
		s=strlen(terminology[0]);
//		url=(char *)malloc(s*sizeof(char)+8); // strlen("http://")+'\0'=7+1=8
//		url=(char *)malloc(s*sizeof(char)+strlen(terminology[0])+1); //+1 for '\0'
		url=(char *)malloc(strlen(COMMAND[1])+s*sizeof(char)+1); //+1 for '\0'
		strcpy(url,terminology[0]);
		strcpy(url+s,COMMAND[1]);
	}
	*/
	/*
	if(url!=NULL)
//	{
		url+=strlen("://");	
//	}
	else
	//url+=strlen("://");
	url=COMMAND[1];
*/
	if(url==NULL)
	//url=COMMAND[1]:
	url=COMMAND[1];
	else
	url+=strlen("://");

	int i=socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in wheres;
	wheres.sin_family=AF_INET;
	char c=char_at_str(':',url);
//	char c=char_at_str(':',url);
//	if(char_at_str(':',url))
	if(c)
	{
		wheres.sin_port=htons(atoi(url+c+1));
		char_sit_at_char_in_str(url,':','\0');
	}
	else
	wheres.sin_port=htons(80);//default web server port
		
	if(is_ip(url))
	{
		//int i=socket(AF_INET,SOCK_STREAM,0);
		//connect(i, 
	//	wheres.sin_addr.s_addr=inet_addr(strstr(url,"://")+3);
		wheres.sin_addr.s_addr=inet_addr(url);
		if (-1==connect(i,(struct sockaddr *)&wheres,sizeof(wheres)))
		{
			perror("connect ");
			return -2;
		}
	}
	else

	{
	struct addrinfo hint;
	memset(&hint,0,sizeof(hint));
	//hint.ai_flags=AF_INET;
	hint.ai_family=AF_INET;
	hint.ai_socktype=SOCK_STREAM;
	struct addrinfo *res;
	if(-1==getaddrinfo(url,"http",&hint,&res))
		{
			perror("getaddrinfo:");
			return -1;
		}
	else
		{
			struct addrinfo*rp;
			for(rp=res;rp!=NULL;rp=rp->ai_next)
//			if(-1!=connect(i,rp->ai_addr,rp->addrlen))
			{
				wheres.sin_addr.s_addr=((struct sockaddr_in*)(rp->ai_addr))->sin_addr.s_addr;
				if(-1!=connect(i,(struct sockaddr *)&wheres,sizeof(wheres)))
				break;
			}	
			freeaddrinfo(res);
			/*
			if(rp==NULL)
			{
				perror("Connect:");
				return -3;
			}
			*/
	
		}

	}
/*
	struct addrinfo hint;
	memset(&hint, 0,sizeof(hint));
	hint.ai_family=AF_INET;
	hine.ai_socktype=SOCK_STREAM;

	struct addrinfo *res;

	getaddrinfo(strstr(url,"://")+3,"http",&hint,&res);
	}
	connect(i,,);
*/
/*
 if(char_at_str(url,'/'))
	snprintf(space, sizeof(space),
	"GET %s HTTP/1.1\r\nHost: %s 
	//",strstr(url,"/");
	else

*/
	
	char *ss=strrchr(url,'/');
//	if(char_at_str(url,'/'))
	//if(ss)
//	snprintf(space, "GET %s HTTP/1.1\r\n",strrchr(url,'/'));
	s=snprintf(space, sizeof space,"GET %s HTTP/1.1\r\n",ss?ss:"/");
	char_sit_at_char_in_str(url,'/','\0');
	if(wheres.sin_port!=htons(80))
	s+=snprintf(space+s,sizeof space,"Host: %s:%d\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: en-US,en;q=0.9\
\r\n\r\n",url,ntohs(wheres.sin_port));
	else
	s+=snprintf(space+s,sizeof space,"Host: %s\r\n\r\n",url);
	send(i,space,s,0);
	//s=receive(i,space,sizeof(space),0);
	//s=accept(i,space,sizeof(space),0);
	s=recv(i,space,sizeof(space),0);
	printf("receive %d size\n",s);
	if(!ss)
	{
	ss=url;
	}
	//int	f=open(ss,O_CREAT||O_WRONLY||O_APPEND,0600);
	int	f=open(ss,O_CREAT|O_WRONLY|O_APPEND,0600);
	if(-1==f)
	{
		perror("open:");
		return -3;
	}
	write(f, space,s);
	close(f);
	return 0;
	//else
	//snprintf(space,"GET / HTTP/1.1
	
	
	

	
	
}

