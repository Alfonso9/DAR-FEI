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
	uint8_t op; //Message opcode/type
	struct in_addr ciaddr;//Client IP address (if already in use)
	struct in_addr yiaddr;//Client IP address (if already in use)
	struct in_addr siaddr;//Client IP address (if already in use)
	char msg[1000];
	char opt[2];	
};

#define ATAK_CLIENT_PORT 30001
#define ATAK_SERVER_PORT 30000

void fillAtak(struct atak *);
void printAtakMsg(struct atak *);

int main(int argc, char * argv[])
{
	int on = 1;
	struct sockaddr_in servaddr, cliaddr;
	int listensrv, sendcli;

	/*------------------------------*/
	/*----------- Cliente ----------*/
	/*------------------------------*/
		
	bzero(&cliaddr,sizeof(cliaddr));	
	cliaddr.sin_port = htons(ATAK_SERVER_PORT);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = inet_addr("255.255.255.255");

	//Create socket
	if((sendcli = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		perror("Can't create socket");
	
	/*------------------------------*/
	/*----------- Cliente ----------*/
	/*------------------------------*/

	/*------------------------------*/
	/*-------- Paquete envío -------*/
	/*------------------------------*/

	struct atak msgAtak;
	bzero(&msgAtak, sizeof(msgAtak));
	fillAtak(&msgAtak);
	
	/*------------------------------*/
	/*-------- Paquete envío -------*/
	/*------------------------------*/

	/*------------------------------*/
	/*----------- Envío ------------*/
	/*------------------------------*/

	
	if(setsockopt(	sendcli,
				  	SOL_SOCKET,
				  	SO_BROADCAST,
				  	&on,
					sizeof(on)) < 0)
		printf("Setsockopt");
	
	if(sendto( 	sendcli,
				&msgAtak,
				sizeof(msgAtak),
				0,
				(struct sockaddr*)&cliaddr,
				sizeof(cliaddr)) < 0)

		printf("Can't send");
	printf("Enviado\n");	
	close(sendcli);
	return 0;

	/*------------------------------*/
	/*----------- Envío ------------*/
	/*------------------------------*/
}

void fillAtak(struct atak * atakmsg)
{
	atakmsg->op = 0;
	inet_aton("0.0.0.0", &atakmsg->ciaddr);
	inet_aton("0.0.0.0", &atakmsg->yiaddr);
	inet_aton("0.0.0.0", &atakmsg->siaddr);
	strcpy(atakmsg->msg, "Hola Oh Shiit -.-lsdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuygdfñhganidygliubdcgilueybgiuyg");
   	atakmsg->opt[1] = 1;
   	atakmsg->opt[2] = 2;
}

void printAtakMsg(struct atak * msg)
{
	printf("=========================\n");
	printf("OP: \t%i\n", msg->op);
	printf("CIADDR: \t%s\n", inet_ntoa(msg->ciaddr));
	printf("YIADDR: \t%s\n", inet_ntoa(msg->yiaddr));
	printf("SIADDR: \t%s\n", inet_ntoa(msg->siaddr));
	printf("Message:\t%s\n", msg->msg);
	printf("Option: \t%i\n", msg->opt[1]);
	printf("	\t%i\n", msg->opt[2]);
	printf("=========================\n");
}