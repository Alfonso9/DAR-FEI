#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <time.h>
#include <unistd.h>

//using namespace std;

struct atak
{
    int8_t type; //Message opcode/type
	/*struct in_addr ciaddr;//Client IP address (if already in use)
	struct in_addr yiaddr;//Client IP address (if already in use)*/
    char msg[100];
};


#define ATAK_CLIENT_PORT        30001
#define ATAK_SERVER_PORT        30000

void fillAtak(struct atak *);
void printAtakMsg(struct atak *);
//char * escanPuertos(struct in_addr *, char *);

int main(int argc, char * argv[])
{	
	int on = 1;
	struct sockaddr_in servaddr;
	int listensrv;

/*------------------------------*/
/*----------- Servidor ---------*/
/*------------------------------*/
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_port = htons(ATAK_SERVER_PORT);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((listensrv = socket(AF_INET, SOCK_DGRAM,0)) < 0)
		perror("Can't create socket");

	if(bind(listensrv, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		perror("Can't to bind");
/*------------------------------*/
/*----------- Servidor ---------*/
/*------------------------------*/

/*-------- Paquete envío -------*/
	struct atak msgAtak;
	bzero(&msgAtak, sizeof(msgAtak));	
/*-------- Paquete envío -------*/
	printf("%s\n", "Esperando datos...");
	while(1)		
	{
		/*-------- Paquete recepción ---*/
		struct atak msgAtakrecv;
		bzero(&msgAtakrecv, sizeof(struct atak));	
		/*-------- Paquete recepción ---*/

		int serv_len = sizeof(servaddr);
		/*-------- Receiving ----------*/
		if(recvfrom(listensrv,
					&msgAtakrecv,
					sizeof(msgAtakrecv),
					0,
					(struct sockaddr*)&servaddr,
					&serv_len) < 0)
			perror("Can't receive");
		
		printAtakMsg(&msgAtakrecv);
		printf("La estructura recibida: %i\n", sizeof(msgAtakrecv));		
		/*-------- Receiving ----------*/
		switch(msgAtakrecv.type)
		{
			case 1:
				printf("%s\n", "ESCANER DE PUERTOS");
				//struct in_addr ciaddr = msgAtakrecv.ciaddr;
				//printf("CIADDR: \t%s\n", inet_ntoa(ciaddr));
				//escanPuertos(&ciaddr, "UDP");				
				break;
			case 2:
				printf("%s\n", "DOS");
				break;
			break;
		}
	}

	return 0;
}

void fillAtak(struct atak * atakmsg)
{
	atakmsg->type = 0;
	/*inet_aton("0.0.0.0", &atakmsg->ciaddr);	
	inet_aton("0.0.0.0", &atakmsg->yiaddr);*/
	strcpy(atakmsg->msg, "Hola");
}

void printAtakMsg(struct atak * msg)
{
    printf("=========================\n");
    printf("OP:     \t%i\n", msg->type);
    /*printf("CIADDR: \t%s\n", inet_ntoa(msg->ciaddr));
    printf("YIADDR: \t%s\n", inet_ntoa(msg->yiaddr));    */
    printf("Message:\t%s\n", msg->msg);    
    printf("=========================\n");
}