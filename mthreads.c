// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mthreads.h"


void testPrint()
{
	printf("Nothing broken in the file separation either\n");
}

//Reads in input from stdin; Mallocs a string to hold the new input
char* getInput() {
	char *currLine = malloc(sizeof(char) * MAX_IN_LEN);
    	size_t len = 0;
    	ssize_t nread;

	if ((nread = getline(&currLine, &len, stdin)) != -1) {
		return currLine;
	} else {
		free(currLine);
		return NULL;
	}

}

//Replaces all '\n' instances with ' '
//	To be used by thread 2
void replaceLineSep(char* currStr){
	char *idPtr;

	//If an instance of '\n' is found, replace it;
	//	No need for fancy movement since this is a one-char substitution
	while ( idPtr = strstr(currStr, "\n") )
		*idPtr = ' '; 
}

//Replaces all instances of '++' to '^'
//	To be used by thread 3
void replacePlus(char* currStr) {
	char *pidStr;

	char *rPtr, *idPtr;

	//If an instance of "++" is found, place it in 'idPtr'
	while ( idPtr = strstr(currStr, "++") ) {
		rPtr = NULL;

		//Right pointer is first value right of the '++'
		if ( *(idPtr+2) != '\0' )
			rPtr = (idPtr+2);

		*idPtr = '^'; //Set the first + to '^', set the second to be NULL
		*(idPtr+1) = '\0'; //Set the first + to '^', set the second to be NULL


		//If there is something to the right, then concatenate it to the "new" str
		//	This works since we've null-terminated the left side of the str
		if (rPtr)
			strcat(currStr, rPtr);


	}


}

