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
    char msg[300];
    char opt[1];
};

#define ATAK_CLIENT_PORT        30001
#define ATAK_SERVER_PORT        30000

void fillAtak(struct atak *);

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
		perror("Can't doing bind");
/*------------------------------*/
/*----------- Servidor ---------*/
/*------------------------------*/

/*-------- Llenar paquete -------*/
	struct atak msgAtak;
	bzero(&msgAtak, sizeof(msgAtak));
	
/*-------- Llenar paquete -------*/

	while(1)
	{
	/*-------- Receiving ----------*/

	/*-------- Receiving ----------*/
	}

	return 0;
}


void fillAtak(struct atak * atakmsg)
{
	atakmsg->op = 0;
	inet_aton("0.0.0.0", &atakmsg->ciaddr);
	inet_aton("0.0.0.0", &atakmsg->yiaddr);
	inet_aton("0.0.0.0", &atakmsg->siaddr);
	strcpy("Hola Oh Shiit", atakmsg->msg);
   	atakmsg->opt[1] = 1;
}