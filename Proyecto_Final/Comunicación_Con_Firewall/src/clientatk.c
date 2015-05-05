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
    struct atak msgAtak;
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
    	bzero(&msgAtak, sizeof(msgAtak));
    	printf("Please enter the message: ");
    	/*------------------------------*/
		/*----------- Envío ------------*/
		/*------------------------------*/
		int op;
		scanf ("%d",&op);

    	switch(op)
		{
			case 1:
				msgAtak.type = 1;
				printf("Tamaño: %i\n", (int) sizeof(msgAtak));
				printf("%s\n", "GET");				
				memcpy(buffer, &msgAtak, sizeof(msgAtak));
				n = write(sockfd,buffer,strlen(buffer));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				printf("%s\n", "Recibiendo...");
				n = read(sockfd,buffer,101);
		        if (n < 0)
		        { 
		             perror("ERROR reading from socket");
		             exit(1);
		        }
		        memcpy(&msgAtak, buffer, (int) sizeof(msgAtak));
		        printf("Mensaje recibido:\n\t%s\n", msgAtak.msg);
				break;
			case 2:
				msgAtak.type = 2;
				printf("Tamaño: %i\n", (int) sizeof(msgAtak));
				printf("%s\n", "IP'S");				
				memcpy(buffer, &msgAtak, sizeof(msgAtak));
				n = write(sockfd,buffer,strlen(buffer));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				printf("%s\n", "Recibiendo...");
				n = read(sockfd,buffer,101);
		        if (n < 0)
		        { 
		             perror("ERROR reading from socket");
		             exit(1);
		        }
		        memcpy(&msgAtak, buffer, (int) sizeof(msgAtak));
		        printf("Mensaje recibido:\n\t%s\n", msgAtak.msg);
				break;
			case 3:
				cond = false;
				msgAtak.type = 3;
				printf("Tamaño: %i\n", (int) sizeof(msgAtak));
				printf("%s\n", "Bye");				
				memcpy(buffer, &msgAtak, sizeof(msgAtak));
				n = write(sockfd,buffer,strlen(buffer));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				printf("%s\n", "Recibiendo...");
				n = read(sockfd,buffer,101);
		        if (n < 0)
		        { 
		             perror("ERROR reading from socket");
		             exit(1);
		        }
		        memcpy(&msgAtak, buffer, (int) sizeof(msgAtak));
		        printf("Mensaje recibido:\n\t%s\n", msgAtak.msg);
				break;
			
			break;
		}        		
	    /*------------------------------*/
		/*----------- Envío ------------*/
		/*------------------------------*/
    }
	return 0;	
}

void printAtakMsg(struct atak * msg)
{
	printf("=========================\n");
	printf("Message:\t%s\n", msg->msg);
	printf("=========================\n");
}