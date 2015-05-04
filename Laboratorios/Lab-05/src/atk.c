#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>


struct atak
{
    int8_t type;
    char msg[100];
};

void fillAtak(struct atak *);
void printAtakMsg(struct atak *);

int main(int argc, char * argv[])
{
	if (argc < 2) 
    {    	
        fprintf(stderr,"ERROR, no port provided\n");        
        exit(1);
    }

	int sockfd, newsockfd, portno, n;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    char buffer[101];

    bzero((char *) &serv_addr, sizeof(serv_addr));
	/*------------------------------*/
	/*----------- Servidor ---------*/
	/*------------------------------*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    portno = atoi(argv[1]);
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;    

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd,1);
	/*------------------------------*/
	/*----------- Servidor ---------*/
	/*------------------------------*/

    /*------------------------------*/
	/*----------- Cliente ----------*/
	/*------------------------------*/    
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) 
    {
          perror("ERROR on accept");
          exit(1);
    }
	/*------------------------------*/
	/*----------- Cliente ----------*/
	/*------------------------------*/

    bool cond = true;
		
	while(cond)		
	{
		printf("%s\n", "Esperando datos...");		
		/*-------- Paquete Recibido -------*/	
		struct atak msgAtakrecv;
		bzero(&msgAtakrecv, sizeof(struct atak));		
		/*-------- Paquete Recibido -------*/
		
		/*-------- Recibiendo ----------*/
		bzero(buffer, 101);
		n = read(newsockfd,buffer,101);
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
				strcpy(msgAtakrecv.msg, "GET");
				memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
				n = write(newsockfd, buffer, sizeof(msgAtakrecv));
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
				n = write(newsockfd, buffer, sizeof(msgAtakrecv));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				break;
			case 3:
				cond = false;
				printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
				printAtakMsg(&msgAtakrecv);
				printf("%s\n", "Bye");
				strcpy(msgAtakrecv.msg, "Bye");
				memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
				n = write(newsockfd, buffer, sizeof(msgAtakrecv));
		        if (n < 0)
		        { 
		            perror("ERROR writing to socket");
		            exit(1);
				}
				break;
			printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
			printAtakMsg(&msgAtakrecv);
			printf("%s\n", "DESCONOCIDO");
			strcpy(msgAtakrecv.msg, "DESCONOCIDO");
			memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));
			n = write(newsockfd, buffer, sizeof(msgAtakrecv));
	        if (n < 0)
	        { 
	            perror("ERROR writing to socket");
	            exit(1);
			}
			break;
		}
	}	
	close(newsockfd);
    close(sockfd);
	return 0;
}

void printAtakMsg(struct atak * msg)
{
    printf("=========================\n");
    printf("OP:     \t%i\n", msg->type);
    printf("Message:\t%s\n", msg->msg);    
    printf("=========================\n");
}