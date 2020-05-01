#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/err.h>
#include <openssl/ssl.h>


#define GET "GET /wiki/HTTP HTTP/1.1\r\nHost: ru.wikipedia.org\r\nUser-Agent: OpenSSL\r\nAccept: text/html\r\nConnection: close\r\n\r\n"

void socket_send(SSL* conn,int* sockfd);
void socket_recv(SSL* conn);
void socket_close(SSL* conn, int* sockfd);

int main(){
	
	struct sockaddr_in addr;
	
	{
		addr.sin_family = AF_INET;
		addr.sin_port = htons(443);
		addr.sin_addr.s_addr=inet_addr("91.198.174.192");
	}
	
	int sockfd = socket(AF_INET, SOCK_STREAM , 0);
	
	if(sockfd < 0){
		
		perror("don't open socket on line 29\n\n");
		exit(0);
	}else{
		printf("open socket! \033[1;32m[OK]\033[0m\n\n");
	}

	if(connect(sockfd,(struct sockaddr *) &addr,sizeof(addr))<0){                 	             
		  perror("not connect on line 36!\n\n");
                  exit(1);
        }else{
                  printf("connect! \033[1;32m[OK]\033[0m\n\n");
        }

	SSL_load_error_strings();
	SSL_library_init();
	SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method ());
	
	SSL* conn= SSL_new(ssl_ctx);
	SSL_set_fd(conn,sockfd);
	
	if(SSL_connect(conn)!=1){

		perror("not SSL connect on line 53!\n\n");
		socket_close(conn,&sockfd);
		abort();
	}
	

	socket_send(conn, &sockfd);
	socket_recv(conn);
	
	socket_close(conn,&sockfd);
	
	return 0;	
}

#define RW_C 2
void socket_close(SSL* conn,int* sockfd){
	
	SSL_shutdown(conn);
	SSL_free(conn);
	shutdown(*sockfd,RW_C);
}
#undef RW_C

void socket_send(SSL* conn, int* sockfd){
	
	char* request = GET; 
	printf("%s\r\n", request);
	
	if(SSL_write(conn,request,strlen(request))>0){
		printf("GET send from %d \033[1;32m[OK]\033[0m\n\n",*sockfd);
	}else{
		printf("Error!\n\n");
	}
}


void socket_recv(SSL* conn){

	char buff[1024];
	int len=0;	
	int i=0;	
	do{

		len=SSL_read(conn, buff, sizeof(buff));
		for(i=0; i<len; i++){
			printf("%c",buff[i]);
		}

	}while(i<len);
}
#undef GET
