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


//BLOCK 1: ALL FUNCTIONS USED BY THREAD 1
//Reads in input from stdin; Mallocs a string to hold the new input
char* getInput() {
	char *currLine = malloc(sizeof(char) * MAX_IN_LEN);
   	//size_t len = 0;
	//ssize_t nread;

	
	int test;

	
	//if ((nread = getline(&currLine, &len, stdin)) != -1) {
	if (fgets(currLine, MAX_IN_LEN, stdin)) {
		//strcat(currLine, "|"); //Concatenate a pipe to make sure we know the ending
		//test = foundStop(currLine);
		//printf("Stop found?: %d\n", test);
		return currLine;
	} else {
		free(currLine);
		return NULL;
	}
	


	//if(foundStop(currLine))
	//	printf("Found the terminating string; Need to process it now.\n");

}

int foundStop(char* currStr){
	//getline reads until the \n terminator anyways
	if ( strstr(currStr, "STOP\n") ) 
		return 1;
	

	return 0;
	

}

//Grabs input, writes it to buffer 1
//	Doesn't write to buffer if "STOP\n" is found
//This handles basically all of the tasks of the first thread
//Note: Buffer1 is assumed to be an empty string
//	This string is allocated either globally or in main, and is only added to here; 
void writeInputToBuffer(char *buf1) {
	char* input;

	//get input one line at a time, concatenate it to the buffer
	while (input = getInput()) {
		//printf("Printing Base Text: \n\t%s\n\n", test);

		strcat(buf1, input);
		free(input);
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
char* replacePlus(char* currStr) {
	char *newStr, *temp;

	char *rPtr, *idPtr;

	//If an instance of "++" is found, place it in 'idPtr'
	while ( idPtr = strstr(currStr, "++") ) {
		rPtr = NULL;

		//Right pointer is first value right of the '++'
		if ( *(idPtr+2) != '\0' )
			rPtr = (idPtr+2);

		*idPtr = '^'; //Set the first + to '^', set the second to be the terminator
		*(idPtr+1) = '\0'; 


		//If there is something to the right, then concatenate it to the "new" str
		//	This works since we've null-terminated the left side of the str
		if (rPtr != NULL) {
			newStr = malloc(sizeof(char) * MAX_IN_LEN);
			strcpy(newStr, currStr);

			strcat(newStr, rPtr);

			temp = currStr;
			currStr = newStr;
			free(temp);
		}

	}

	return currStr;


}
