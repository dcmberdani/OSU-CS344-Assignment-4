// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "mthreads.h"


/*
*/

int main(int argc, char *argv[])
{	

	printf("Nothing has broken horribly\n");
	testPrint();

	char *currLine = NULL;
    	size_t len = 0;
    	ssize_t nread;

	//FILE *iStream = fopen(stdin, "r");
	//FILE *oStream = fopen(stdin, "w");
	//fread(&currLine, &len, iStream);

	while ((nread = getline(&currLine, &len, stdin)) != -1){
		expandVars(currLine);
		printf(currLine);
	}
	



	return EXIT_SUCCESS;
}
