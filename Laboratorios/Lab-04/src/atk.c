#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <time.h>

struct atak
{
	uint8_t op;     //Message opcode/type
	struct in_addr ciaddr;//Client IP address (if already in use)
    struct in_addr yiaddr;//Client IP address (if already in use)
    struct in_addr siaddr;//Client IP address (if already in use)
    char msg[1000];
    char opt[2];
};

#define ATAK_CLIENT_PORT        30001
#define ATAK_SERVER_PORT        30000

void fillAtak(struct atak *);
void printAtakMsg(struct atak *);

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
		bzero(&msgAtakrecv, sizeof(msgAtakrecv));	
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
		printf("%i\n", sizeof(msgAtakrecv));		
		/*-------- Receiving ----------*/
		switch(msgAtakrecv.opt[1])
		{
			case 1:
				printf("%s\n", "Oh Shiit");
				break;
			case 2:
				printf("%s\n", "Visto a tu Visto");
				break;
			break;
		}
	}

	return 0;
}


void fillAtak(struct atak * atakmsg)
{
	atakmsg->op = 0;
	inet_aton("0.0.0.0", &atakmsg->ciaddr);
	inet_aton("0.0.0.0", &atakmsg->yiaddr);
	inet_aton("0.0.0.0", &atakmsg->siaddr);
	strcpy(atakmsg->msg, "Hola Oh Shiit -.-");
   	atakmsg->opt[1] = 1;
   	atakmsg->opt[1] = 255;
}

void printAtakMsg(struct atak * msg)
{
    printf("=========================\n");
    printf("OP:     \t%i\n", msg->op);
    printf("CIADDR: \t%s\n", inet_ntoa(msg->ciaddr));
    printf("YIADDR: \t%s\n", inet_ntoa(msg->yiaddr));
    printf("SIADDR: \t%s\n", inet_ntoa(msg->siaddr));
    printf("Message:\t%s\n", msg->msg);
    printf("Option: \t%02x\n", msg->opt[1]);
    printf("Option: \t%02x\n", msg->opt[2]);
    printf("=========================\n");
}