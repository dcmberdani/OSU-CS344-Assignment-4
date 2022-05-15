// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning
//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "mthreads.h"


//Here's my MUTEX stuff
// Initialize the mutex for buffer 1
pthread_mutex_t buf1_mut = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t buf1_cond = PTHREAD_COND_INITIALIZER;
// Variable to indicate that buffer 1 is not empty
int buf1_isempty = 1;

pthread_mutex_t buf2_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf2_cond = PTHREAD_COND_INITIALIZER;
int buf2_isempty = 1;

pthread_mutex_t buf3_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf3_cond = PTHREAD_COND_INITIALIZER;
int buf3_isempty = 1;

//int stop_signal = 0;
//	Stop signal propogates down, allowing for one final cycle
int stop_t1 = 0;
int stop_t2 = 0;
int stop_t3 = 0;
int stop_t4 = 0;

//Here's the initial allocation of buffers
//	Global because idk how to pass stuff in with threads;
char* buf1;	
char* buf2;	
char* buf3;	


//Initializes buffers
void initProg()
{
	buf1 = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);	
	strcpy(buf1, "\0");

	buf2 = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);	
	strcpy(buf2, "\0");

	buf3 = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);	
	strcpy(buf3, "\0");
}

//Frees Buffers
void freeProg()
{
	free(buf1);
	free(buf2);
	free(buf3);
}



//"Putters" place a string into the buffers
void putBuf1(char* tempStr) {
	pthread_mutex_lock(&buf1_mut);

	strcat(buf1, tempStr);

	buf1_isempty = 0;

	pthread_cond_signal(&buf1_cond);
	pthread_mutex_unlock(&buf1_mut);
}

void putBuf2(char* tempStr) {
	pthread_mutex_lock(&buf2_mut);

	strcat(buf2, tempStr);
	//strcpy(buf1, tempStr);

	buf2_isempty = 0;

	pthread_cond_signal(&buf2_cond);
	pthread_mutex_unlock(&buf2_mut);
}

void putBuf3(char* tempStr) {
	pthread_mutex_lock(&buf3_mut);

	//if (stop_t3 == 0)
	strcat(buf3, tempStr);
	//strcpy(buf1, tempStr);

	buf3_isempty = 0;

	pthread_cond_signal(&buf3_cond);
	pthread_mutex_unlock(&buf3_mut);
}



//Grabs info from buffers
char* getBuf1() {
	char* tempStr = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);

	pthread_mutex_lock(&buf1_mut);

	while (buf1_isempty == 1)
		pthread_cond_wait(&buf1_cond, &buf1_mut);

	strcpy(tempStr, buf1);

	memset(buf1, '\0', MAX_IN_LEN * MAX_IN_COUNT);
	buf1_isempty = 1;

	pthread_mutex_unlock(&buf1_mut);

	return tempStr;
}

char* getBuf2() {
	char* tempStr = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);

	pthread_mutex_lock(&buf2_mut);

	while (buf2_isempty == 1)
		pthread_cond_wait(&buf2_cond, &buf2_mut);

	strcpy(tempStr, buf2);

	memset(buf2, '\0', MAX_IN_LEN * MAX_IN_COUNT);
	buf2_isempty = 1;

	pthread_mutex_unlock(&buf2_mut);
	
	return tempStr;
}

char* getBuf3() {
	char* tempStr = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);

	pthread_mutex_lock(&buf3_mut);

	while (buf3_isempty == 1)
		pthread_cond_wait(&buf3_cond, &buf3_mut);

	strcpy(tempStr, buf3);

	memset(buf3, '\0', MAX_IN_LEN * MAX_IN_COUNT);
	buf3_isempty = 1;

	pthread_mutex_unlock(&buf3_mut);
	
	return tempStr;
}


//Checks if the stop flag is in a string
//	If it is, then end the threads
//This is called in all threads
int foundStop(char* currStr){
	//getline reads until the \n terminator anyways
	if ( strstr(currStr, " STOP\n") || strstr(currStr, " STOP ") || strcmp(currStr, "STOP\n") == 0 ) 
	//if ( strstr(currStr, " STOP\n") || strstr(currStr, "\nSTOP\n")  || strstr(currStr, "\nSTOP ") || strstr(currStr, " STOP ")) 
		return 1;
	//if ( strstr(currStr, " STOP\n") || strstr(currStr, "\nSTOP\n") ) 
	//if ( strstr(currStr, "STOP\n") ) 
		//return 0;
	

	return 0;
}

/*
 *
 *
 * BLOCK 1: ALL FUNCTIONS ARE USED BY THREAD 1
 *
 *
 */
//Reads in input from stdin; Mallocs a string to hold the new input
//	Helper function for thread 1
char* getInput() {
	char *currLine = malloc(sizeof(char) * MAX_IN_LEN);
	int test;
	//if ((nread = getline(&currLine, &len, stdin)) != -1) {
	if (fgets(currLine, MAX_IN_LEN, stdin)) {
		return currLine;
	} else {
		free(currLine);
		return NULL;
	}
	
}


//Grabs input, writes it to buffer 1
//	Doesn't write to buffer if "STOP\n" is found
//This handles basically all of the tasks of the first thread
//Note: Buffer1 is assumed to be an empty string
//	This string is allocated either globally or in main, and is only added to here; 
void *writeInputToBuffer() {
	char* input;
	//Now, we lock buffer 1 while we operate on iet
	//	Then, when full, signal that buffer 1 has now gotten input
	//for (int i = 0; i < 50; i++){
	do {
		input = getInput();
		//if (input = getInput()) {
			stop_t1 = foundStop(input);
			//printf("\tCurrent Input: %s\t Current foundStop: %d\n", input, stop_t1);
			

			if (stop_t1 == 1) 
				strcpy(input, "\0");
				

			putBuf1(input);
			free(input);
		//}
		//if (stop_t1) break;

	} while (stop_t1 == 0);
	printf("Exited Thread 1;\n");
	//}
}


/*
 *
 *
 * BLOCK 2: ALL FUNCTIONS ARE USED BY THREAD 2
 *
 *
 */
//Replaces all '\n' instances with ' '
//	To be used by thread 2
void *replaceLineSep(){
	char* tempStr;
	do {
		//Store buf1 in a temporary buffer
		//	Prevents overlapping of locks	
		//	This is used throughout the program
		tempStr = getBuf1();

		stop_t2 = foundStop(tempStr);

		char *idPtr;
		//If an instance of '\n' is found, replace it;
		//	No need for fancy movement since this is a one-char substitution
		while ( idPtr = strstr(tempStr, "\n") )
			*idPtr = ' '; 

		putBuf2(tempStr);
		
		free(tempStr);
		
	} while (stop_t2 == 0);

	printf("Exited Thread 2;\n");
}


/*
 *
 *
 * BLOCK 3: ALL FUNCTIONS ARE USED BY THREAD 3
 *
 *
 */
//Replaces all instances of '++' to '^'
//	To be used by thread 3
void *replacePlus() {
	char *newStr, *temp2, *tempStr;
	char *rPtr, *idPtr;

	do {

		tempStr = getBuf2();

		stop_t3 = foundStop(tempStr);

		//If an instance of "++" is found, place it in 'idPtr'
		while ( idPtr = strstr(tempStr, "++") ) {
			rPtr = NULL;

			//Right pointer is first value right of the '++'
			if ( *(idPtr+2) != '\0' )
				rPtr = (idPtr+2);

			*idPtr = '^'; //Set the first + to '^', set the second to be the terminator
			*(idPtr+1) = '\0'; 


			//If there is something to the right, then concatenate it to the "new" str
			//	This works since we've null-terminated the left side of the str
			if (rPtr != NULL) {
				newStr = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);
				strcpy(newStr, tempStr);

				strcat(newStr, rPtr);

				temp2 = tempStr;
				tempStr = newStr;
				free(temp2);
			}

		}
		putBuf3(tempStr);
		free(tempStr);

	} while (stop_t3 == 0);

	printf("Exited Thread 3;\n");
}


/*
 *
 *
 * BLOCK 4: ALL FUNCTIONS ARE USED BY THREAD 4
 *
 *
 */
//Prints out 80 characters at a time from a given buffer
//	To be used by thread 4
void *writeOutput() {
	int counter;
	int buflen;

	do {
		pthread_mutex_lock(&buf3_mut);

		while (buf3_isempty == 1)
			pthread_cond_wait(&buf3_cond, &buf3_mut);


		stop_t4 = foundStop(buf3);
		//If the buffer is long enough, then print out the first 80 chars
		//	Afterwards, memmove the string back
		//	Repeat for every instance of the string being above 80 chars
		while (strlen(buf3) >= 80) {
			printf("%.80s\n", buf3);
			fflush(stdout);
			memmove(buf3, buf3+80, MAX_IN_LEN-80);
		}

		//if (stop_t4 == 0)
		buf3_isempty = 1;

		pthread_mutex_unlock(&buf3_mut);

	} while (stop_t4 == 0);

	printf("Exited Thread 4;\n");
}
