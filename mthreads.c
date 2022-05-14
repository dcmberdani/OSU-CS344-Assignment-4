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
int buf1_isfull = 0;
int buf1_isempty = 1;

pthread_mutex_t buf2_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf2_cond = PTHREAD_COND_INITIALIZER;
int buf2_isfull = 0;
int buf2_isempty = 1;

pthread_mutex_t buf3_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf3_cond = PTHREAD_COND_INITIALIZER;
int buf3_isfull = 0;
int buf3_isempty = 1;

//int stop_signal = 0;
//	Stop signal propogates down, allowing for one final cycle
int stop_t1 = 0;
int stop_t2 = 0;
int stop_t3 = 0;
int stop_t4 = 0;

//Here's the initial allocation of buffers
//	Global because idk how to pass stuff in with threads;
//	
/*
extern char* buf1;	
extern char* buf2;	
extern char* buf3;	
*/
char* buf1;	
char* buf2;	
char* buf3;	


//Initializes buffers
void initProg()
{
	buf1 = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);	
	//buf1 = malloc(sizeof(char) * MAX_IN_LEN);	
	strcpy(buf1, "\0");

	buf2 = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);	
	//buf2 = malloc(sizeof(char) * MAX_IN_LEN);	
	strcpy(buf2, "\0");

	buf3 = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);	
	//buf3 = malloc(sizeof(char) * MAX_IN_LEN);	
	strcpy(buf3, "\0");
}

void freeProg()
{
	free(buf1);
	free(buf2);
	free(buf3);
}

void testPrint()
{
	printf("Nothing broken in the file separation either\n");
	fflush(stdout);
}


//BLOCK 1: ALL FUNCTIONS USED BY THREAD 1
//Reads in input from stdin; Mallocs a string to hold the new input
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

int foundStop(char* currStr){
	//getline reads until the \n terminator anyways
	if ( strstr(currStr, " STOP\n") || strstr(currStr, "\nSTOP\n") ) 
	//if ( strstr(currStr, "STOP\n") ) 
		//return 0;
		return 1;
	

	return 0;
	

}

void putBuf1(char* tempStr) {
	pthread_mutex_lock(&buf1_mut);

	strcat(buf1, tempStr);
	//strcpy(buf1, tempStr);

	buf1_isempty = 0;

	pthread_cond_signal(&buf1_cond);
	pthread_mutex_unlock(&buf1_mut);

	//free(str);
}



//Grabs input, writes it to buffer 1
//	Doesn't write to buffer if "STOP\n" is found
//This handles basically all of the tasks of the first thread
//Note: Buffer1 is assumed to be an empty string
//	This string is allocated either globally or in main, and is only added to here; 
void *writeInputToBuffer() {
	char* input;

	int stopthis = 0;

	//Now, we lock buffer 1 while we operate on iet
	//	Then, when full, signal that buffer 1 has now gotten input
	//pthread_mutex_lock(&buf1_mut);
	//do {
	//
	for (int i = 0; i < 50; i++){
		input = getInput();
		if (input) {

			//printf("\tIN THREAD 1: \tPRINTING BUF1: %s\n", buf1);
			//fflush(stdout);
			//printf("Grabbed Input: %s\n", input);
			
			stop_t1 = foundStop(input);

			putBuf1(input);
			free(input);
		}

		if (stop_t1) break;
	}

	//} while (stop_t1 == 0);

	stop_t2 = 1;
	printf("We've reached the end of thread 1;\n");
}

//Grabs info from buffer1 and then resets it
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
//Replaces all '\n' instances with ' '
//	To be used by thread 2
void *replaceLineSep(){
	char* tempStr;
	do {
		//Store buf1 in a temporary buffer
		//	Prevents overlapping of locks	
		tempStr = getBuf1();


		//pthread_mutex_lock(&buf2_mut);

		//strcat(buf2, tempStr);
		//buf2_isempty = 0;
		//
		//printf("\tIN THREAD 2: \tFLUSHED BUF1;\n");
		//fflush(stdout);

		char *idPtr;
		//If an instance of '\n' is found, replace it;
		//	No need for fancy movement since this is a one-char substitution
		//while ( idPtr = strstr(buf2, "\n") )
		while ( idPtr = strstr(tempStr, "\n") )
			*idPtr = ' '; 

		putBuf2(tempStr);
		
		//printf("\tIN THREAD 2: \tPRINTING BUF2: %s\n", buf2);
		//fflush(stdout);

	//	pthread_cond_signal(&buf2_cond);
		//pthread_mutex_unlock(&buf2_mut);

		free(tempStr);
		
	} while (stop_t2 == 0);
	//} while (stop_t1 == 0);

	printf("We've reached the end of thread 2;\n");
	stop_t3 = 1;
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

	strcat(buf3, tempStr);
	//strcpy(buf1, tempStr);

	buf3_isempty = 0;

	pthread_cond_signal(&buf3_cond);
	pthread_mutex_unlock(&buf3_mut);
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

//Replaces all instances of '++' to '^'
//	To be used by thread 3
void *replacePlus() {
	char *newStr, *temp2, *tempStr;
	char *rPtr, *idPtr;

	do {

		tempStr = getBuf2();

		//pthread_mutex_lock(&buf3_mut);
		//Wait while buf2 is empty

		//Copy temp into buffer3
		//strcat(buf3, tempStr);
		//buf3_isempty = 0;

		//printf("\tIN THREAD 3: \tFLUSHED BUF2;\n");
		//fflush(stdout);

		//If an instance of "++" is found, place it in 'idPtr'
		//while ( idPtr = strstr(buf3, "++") ) {
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
				//strcpy(newStr, buf3);
				strcpy(newStr, tempStr);

				strcat(newStr, rPtr);

				//temp2 = buf3;
				temp2 = tempStr;
				//buf3 = newStr;
				tempStr = newStr;
				free(temp2);
			}

		}

		//printf("\tIN THREAD 3: \tPRINTING BUF3: %s\n", buf3);
		//fflush(stdout);
		//pthread_cond_signal(&buf3_cond);
		//pthread_mutex_unlock(&buf3_mut);

		putBuf3(tempStr);
		free(tempStr);

	} while (stop_t3 == 0);
	printf("We've reached the end of thread 3;\n");
	stop_t4 = 1;
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



//Prints out 80 characters at a time from a given buffer
//	To be used by thread 4
void *writeOutput() {
	int counter;
	int buflen;

	do {
		pthread_mutex_lock(&buf3_mut);

		while (buf3_isempty == 1)
			pthread_cond_wait(&buf3_cond, &buf3_mut);

		//If the buffer is long enough, then print out the first 80 chars
		//	Afterwards, memmove the string back
		//	Repeat for every instance of the string being above 80 chars
		//
		//printf("\tIN THREAD 4: \tPRINTING OUT: %s\n", buf3);
		//fflush(stdout);
		while (strlen(buf3) >= 80) {
			printf("%.80s\n", buf3);
			fflush(stdout);
			memmove(buf3, buf3+80, MAX_IN_LEN-80);
		}

		buf3_isempty = 1;

		pthread_mutex_unlock(&buf3_mut);

	} while (stop_t4 == 0);
	printf("We've reached the end of thread 4;\n");
}
