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
    int8_t count;
    int8_t ack;    
    char msg[100];
};

void printAtakMsg(struct atak *);
char * GET();
struct atak ** partirMensaje(char * msj, int tipo, int * tamArreglo);

int main(int argc, char * argv[])
{	
	struct hostent *lh = gethostbyname("www.scopus.com");

    if (lh)
        printf("IP: %s\n", inet_ntoa(lh->h_addr));
    else
        herror("gethostbyname");

	if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    char buffer[103];
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

    while(true)
    {
    	usleep(5000000);
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
					printf("%s\n", "Procesando solicitud...");
					printf("Tamaño: %i\n", (int) sizeof(msgAtakrecv));		
					printAtakMsg(&msgAtakrecv);
					printf("%s\n", "GET");
					/*------------------------------------------*/
					char * message = GET();	
					int i;
					struct atak ** arregloatk = partirMensaje(message, 1, &i);		
					free(message);
					printf("%i\n", i);
					/*------------------------------------------*/	
					bool enterado = false;
					while(!enterado)
					{			
						/*Respondiendo con ACK a Solicitud*/
						printf("%s\n", "Respondiendo con ACK solicitud...");
						msgAtakrecv.type = 1;
						msgAtakrecv.count = i;
						msgAtakrecv.ack = 1;
						strncpy(msgAtakrecv.msg, "Respondiendo", 12);				
						memcpy(buffer, &msgAtakrecv, sizeof(msgAtakrecv));				
						n = write(sockfd, buffer, sizeof(msgAtakrecv));
						if (n < 0)
				        { 
				            perror("ERROR writing to socket");
				            exit(1);
						}
						/*Respondiendo con ACK a Solicitud*/
						/*Recibiendo ACK de enterado*/
						printf("%s\n", "Recibiendo ACK de enterado...");
						bzero(buffer, 103);
						n = read(sockfd,buffer,103);
						if (n < 0)
						{ 
				            perror("ERROR reading from socket");
				            exit(1);
				        }
				        bzero(&msgAtakrecv, sizeof(struct atak));	
						memcpy(&msgAtakrecv, buffer, (int) sizeof(msgAtakrecv));
						/*Recibiendo ACK de enterado*/
						printAtakMsg(&msgAtakrecv);
						printf("%s\n", "Enviando paquetes...");
						if (msgAtakrecv.ack == 1)
						{
							enterado = true;
							int j = 0;
							while(j < i)
							{			
								usleep(1);
								//printf("Mensaje %i: %s\n", j,arregloatk[j]->msg);		
								struct atak msgAtak;
								memcpy(&msgAtak, arregloatk[j], (int) 103);
								printAtakMsg(&msgAtak);
								memcpy(buffer, &msgAtak, (int) 103);
								struct atak msgAtaka;
								memcpy(&msgAtaka, buffer, (int) 103);				
								//printAtakMsg(&msgAtaka);
								n = write(sockfd, buffer, 103);
								//printf("WRITE RETURN: %i BUFFER: %s\n", n, buffer);
						        if (n < 0)
						        { 
						            perror("ERROR writing to socket");
						            exit(1);
								}
								bzero(buffer, 103);
								n = read(sockfd, buffer, 103);
								if (n < 0)
								{ 
						            perror("ERROR reading from socket");
						            exit(1);
						        }
						        bzero(&msgAtakrecv, sizeof(struct atak));	
								memcpy(&msgAtakrecv, buffer, (int) sizeof(msgAtakrecv));
								printAtakMsg(&msgAtakrecv);
								if (msgAtakrecv.ack == 1)
								{
									j++;
								}
							}	
						}
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
				case 3:
					//message = GET();				
					//partirMensaje(message, 3);
				/*	cond = false;
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
					}*/
					break;
				default:
					break;
			}
	    }
	}
	return 0;	
}

void printAtakMsg(struct atak * msg)
{
	printf("=========================\n");
	printf("Type:\t%i\n", msg->type);
	printf("Count:\t%i\n", msg->count);
	printf("ACK:\t%i\n", msg->ack);
	printf("Message:\t%s\n", msg->msg);
	printf("=========================\n");
}

char * GET()
{
	int socket_desc;
	struct sockaddr_in servget;
	char * message;
	char * servreq = malloc(2000);

	//create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("%s\n", "Error create socket");
	}

	servget.sin_addr.s_addr = inet_addr("216.58.216.238");//216.58.216.238//148.226.81.254
	servget.sin_family = AF_INET;
	servget.sin_port = htons(80);

	if (connect(socket_desc,(struct sockaddr *) &servget,sizeof(servget)) < 0) 
    {
        perror("ERROR connecting");
        exit;
    }

    message = "GET / HTTP/1.1\r\n\r\n";

    if(send(socket_desc, message, strlen(message), 0) < 0)
    {
    	perror("ERROR sending");        
    }

    if(recv(socket_desc, servreq, 2000, 0) < 0)
    {
    	perror("Recv Failed");
    }

    //puts(servreq);
    close(socket_desc);
    return servreq;
}

struct atak ** partirMensaje(char * msj, int tipo, int * tamArreglo)
{	
	int partes = (strlen(msj)/100);
	if (strlen(msj)/100 > 0)
	{
			partes++;
	}	
	struct atak e;
	struct atak ** arreglo = malloc(sizeof(e)*partes);
	int i;	
	for (i = 0; i < partes; ++i)
	{
		arreglo[i] = malloc(sizeof(e));
		arreglo[i]->type = tipo;
		arreglo[i]->count = i+1;
		arreglo[i]->ack = 0;
		strncpy(arreglo[i]->msg, msj ,100);
		//printf("%i -- %s\n", msj, msj);
		msj += 100;
	}	
	/*for (i = 0; i < partes; ++i)
	{
		printAtakMsg(arreglo[i]);
	}*/
	*tamArreglo = partes;
	return arreglo;
}