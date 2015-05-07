#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

struct atak
{
    int8_t type;
    char msg[100];
};

void printAtakMsg(struct atak *);

int main(int argc, char * argv[])
{
	if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    char buffer[101];
    struct atak msgAtakrecv;
    
	/*------------------------------*/
	/*----------- Cliente ----------*/
	/*------------------------------*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    { 
        perror("ERROR opening socket");
        exit(0);
	}

	portno = atoi(argv[2]);    
	server = gethostbyname(argv[1]);
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    {
        perror("ERROR connecting");
        exit(0);
    }
	/*------------------------------*/
	/*----------- Cliente ----------*/
	/*------------------------------*/
	bool cond = true;
	while(cond)
    {
    	printf("%s\n", "Esperando datos...");		
		/*-------- Paquete Recibido -------*/			
		bzero(&msgAtakrecv, sizeof(struct atak));		
		/*-------- Paquete Recibido -------*/
		
		/*-------- Recibiendo ----------*/
		bzero(buffer, 101);
		n = read(sockfd,buffer,101);
		if (n < 0)
		{ 
            perror("ERROR reading from socket");
            exit(1);
        }
		memcpy(&msgAtakrecv, buffer, (int) sizeof(msgAtakrecv));					
		/*-------- Recibiendo ----------*/

		switch(msgAtakrecv.type)
		{
			case 1:
				printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
				printAtakMsg(&msgAtakrecv);
				printf("%s\n", "GET");
				/*------------------------------------------*/
				/*int socket_desc;
				struct sockaddr_in servget;
				char * message, servreq[1000];

				//create socket
				socket_desc = socket(AF_INET, SOCK_STREAM, 0);
				if(socket_desc == -1)
				{
					printf("%s\n", "Error create socket");
				}

				servget.sin_addr.s_addr = inet_addr("216.58.216.238");//216.58.216.238
				servget.sin_family = AF_INET;
				servget.sin_port = htons(80);

				if (connect(socket_desc,(struct sockaddr *) &servget,sizeof(servget)) < 0) 
			    {
			        perror("ERROR connecting");
			        return 1;
			    }

			    message = "GET / HTTP/1.1\r\n\r\n";

			    if(send(socket_desc, message, strlen(message), 0) < 0)
			    {
			    	perror("ERROR connecting");
			        return 1;
			    }

			    if(recv(socket_desc, servreq, 2000, 0) < 0)
			    {
			    	perror("Recv Failed");
			    }

			    //puts(servreq);
			    close(socket_desc);*/
				/*------------------------------------------*/
				strcpy(msgAtakrecv.msg, "<HTML><HEAD><TITLE></TITLE></HEAD><BODYBGCOLOR='#FFFFFF'><TABLEBORDER=0ALIGN=CENTERWIDTH=100%><TR><");
				//strcpy(msgAtakrecv.msg, message);				
				memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
				usleep(20);
				n = write(sockfd, buffer, sizeof(msgAtakrecv));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				break;
			case 2:
				printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
				printAtakMsg(&msgAtakrecv);
				printf("%s\n", "IP'S");
				strcpy(msgAtakrecv.msg, "IP'S");
				memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
				n = write(sockfd, buffer, sizeof(msgAtakrecv));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				break;
			/*case 3:
				cond = false;
				printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
				printAtakMsg(&msgAtakrecv);
				printf("%s\n", "Bye");
				strcpy(msgAtakrecv.msg, "Bye");
				memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
				n = write(sockfd, buffer, sizeof(msgAtakrecv));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				break;*/
			printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
			printAtakMsg(&msgAtakrecv);
			printf("%s\n", "DESCONOCIDO");
			strcpy(msgAtakrecv.msg, "DESCONOCIDO");
			memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
			n = write(sockfd, buffer, sizeof(msgAtakrecv));
	        if (n < 0)
	        { 
	            perror("ERROR writing to socket");
	            exit(1);
			}
			break;
		}
    }
	return 0;	
}

void printAtakMsg(struct atak * msg)
{
	printf("=========================\n");
	printf("Message:\t%s\n", msg->msg);
	printf("=========================\n");
}
