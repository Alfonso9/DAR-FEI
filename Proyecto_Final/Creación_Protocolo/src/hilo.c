#include "hilo.h"
#include <stdio.h>
#include <pthread.h>

	

int ejecutar(void * fn)
{
	pthread_t tid[2];
	int i = pthread_create(&(tid[0]), NULL, fn, NULL);

	return i;
}