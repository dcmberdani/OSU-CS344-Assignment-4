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
	char* buf1 = malloc(sizeof(char) * MAX_IN_LEN);	

	printf("Nothing has broken horribly\n");
	testPrint();


	printf("First, we print the base input:\n");
	writeInputToBuffer(buf1);
	printf("%s\n", buf1);


	printf("Now we remove line separators:\n");
	replaceLineSep(buf1);
	printf("%s\n", buf1);

	printf("Now with plusses replaced:\n");
	replacePlus(buf1);
	printf("%s\n", buf1);


	free(buf1);

	/*
	char *test;
	while (test = getInput()) {
		printf("Printing Base Text: \n\t%s\n\n", test);


		//replaceLineSep(test);
		//printf("Without Separators: \n\t%s\n\n", test);

		//test = replacePlus(test);
		//printf("Plusses Replaces: \n\t%s\n\n", test);

		free(test);
	}
	*/

	//char *currLine = NULL;
    	//size_t len = 0;
    	//ssize_t nread;

	//FILE *iStream = fopen(stdin, "r");
	//FILE *oStream = fopen(stdin, "w");
	//fread(&currLine, &len, iStream);

	//while ((nread = getline(&currLine, &len, stdin)) != -1){
	//	expandVars(currLine);
	//	printf(currLine);
	//}
	



	return EXIT_SUCCESS;
}
