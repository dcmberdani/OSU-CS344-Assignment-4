// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>


#include "mthreads.h"

int main(int argc, char *argv[])
{	
	/*
	char* buf1 = malloc(sizeof(char) * MAX_IN_LEN);	
	char* buf2 = malloc(sizeof(char) * MAX_IN_LEN);	
	char* buf3 = malloc(sizeof(char) * MAX_IN_LEN);	
	*/

	printf("Nothing has broken horribly\n");
	testPrint();


	while (writeInputToBuffer(buf1)){
	//writeInputToBuffer(buf1);
	printf("First, we print the base input:\n");
	printf("%s\n", buf1);


	printf("Now we remove line separators:\n");
	replaceLineSep(buf1, buf2);
	printf("%s\n", buf2);

	printf("Now with plusses replaced:\n");
	replacePlus(buf2, buf3);
	printf("%s\n", buf3);


	printf("Now test if output is written:\n");
	writeOutput(buf3);
	printf("\n\n\n\n\n");


	}

	/*
	free(buf1);
	free(buf2);
	free(buf3);
	*/

	/*
	pthread_t input_1, sep_t, plus_t, output_t;
	pthread_create(&input_t, NULL, writeInputToBuffer, NULL);
	pthread_create(&sep_t, NULL, replaceLine_Sep, NULL);
	pthread_create(&plus_t, NULL, replacePlus, NULL);
	pthread_create(&output_t, NULL, writeOutput, NULL);
	*/


	return EXIT_SUCCESS;
}
