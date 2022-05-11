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


//Expands all instances of '$$' to the PID of the shell
void expandVars(char* currStr) {
	int counter = 0;

	char *pidStr;

	char *rPtr, *idPtr, *out;

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

