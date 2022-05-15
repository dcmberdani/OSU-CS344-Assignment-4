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
	initProg();
	
	pthread_t input_t, sep_t, plus_t, output_t;
	pthread_create(&input_t, NULL, writeInputToBuffer, NULL);
	pthread_create(&sep_t, NULL, replaceLineSep, NULL);
	pthread_create(&plus_t, NULL, replacePlus, NULL);
	pthread_create(&output_t, NULL, writeOutput, NULL);

	pthread_join(input_t, NULL);
	pthread_join(sep_t, NULL);
	pthread_join(plus_t, NULL);
	pthread_join(output_t, NULL);
	

	freeProg();

	return EXIT_SUCCESS;
}
