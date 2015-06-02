#include <stdio.h>
#include "hilo.h"

void * imp(void * arg)
{
	printf("Hola\n");
}

int main (int argc, char *argv[])
{

	printf("%i\n", ejecutar(imp));
	
	return 0;
}