#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	FILE * fTemp = fopen("log.txt", "r");
	size_t pos = ftell(fTemp);    // Current position
	fseek(fTemp, 0, SEEK_END);    // Go to end
	size_t tam = ftell(fTemp); // read the position which is the size
	fseek(fTemp, pos, SEEK_SET);
	char * buffer;	
		/* allocate memory for entire content */
	buffer = calloc( 1, tam+1 );
	if( !buffer ) 
	{
		fclose(fTemp);
		fputs("memory alloc fails",stderr);
		exit(1);
	}
	/* copy the file into the buffer */
	if( 1!=fread( buffer , tam, 1 , fTemp) )
	{
	  	fclose(fTemp);
	  	free(buffer);
	  	fputs("entire read fails",stderr);
	  	exit(1);
	}
	printf("%s", buffer);
	return 0;
}