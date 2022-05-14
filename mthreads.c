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

pthread_mutex_t buf2_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf2_cond = PTHREAD_COND_INITIALIZER;
int buf2_isfull = 0;

pthread_mutex_t buf3_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buf3_cond = PTHREAD_COND_INITIALIZER;
int buf3_isfull = 0;

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
}


//BLOCK 1: ALL FUNCTIONS USED BY THREAD 1
//Reads in input from stdin; Mallocs a string to hold the new input
char* getInput() {
	char *currLine = malloc(sizeof(char) * MAX_IN_LEN);
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
	if ( strstr(currStr, " STOP\n") || strstr(currStr, "\nSTOP\n") ) 
	//if ( strstr(currStr, "STOP\n") ) 
		return 1;
	

	return 0;
	

}

//Grabs input, writes it to buffer 1
//	Doesn't write to buffer if "STOP\n" is found
//This handles basically all of the tasks of the first thread
//Note: Buffer1 is assumed to be an empty string
//	This string is allocated either globally or in main, and is only added to here; 
//void writeInputToBuffer(char *buf1) {
void *writeInputToBuffer() {
//int writeInputToBuffer() {
//int *writeInputToBuffer() {
//int writeInputToBuffer(char *buf1) {
	char* input;


	//Now, we lock buffer 1 while we operate on iet
	//	Then, when full, signal that buffer 1 has now gotten input
	//pthread_mutex_lock(&buf1_mut);
	do {
		//pthread_mutex_lock(&buf1_mut);
		input = getInput();
		//while (input = getInput()){
		//while (input) {
		if (input) {
			pthread_mutex_lock(&buf1_mut);

			//if (buf1_isfull == 1)
			//	pthread_cond_wait(&buf1_cond, &buf1_mut);

			strcat(buf1, input);
			buf1_isfull = 1;

			//printf("\tIN THREAD 1: \tPRINTING BUF1: %s\n", buf1);
			
			stop_t1 = foundStop(buf1);


			pthread_cond_signal(&buf1_cond);
			pthread_mutex_unlock(&buf1_mut);


			free(input);
		}

		//pthread_cond_signal(&buf1_cond);
		//pthread_mutex_unlock(&buf1_mut);
	} while (stop_t1 == 0);

	stop_t2 = 1;
}

//Replaces all '\n' instances with ' '
//	To be used by thread 2
//void replaceLineSep(char *buf1, char *buf2){
void *replaceLineSep(){
//void replaceLineSep(){
	//Lock mutex 1/2 and check if buf1 it has any data
	//char* temp = malloc(sizeof(char) * MAX_IN_LEN * MAX_IN_COUNT);
	//char* temp = malloc(sizeof(char) * MAX_IN_LEN);
	do {

		pthread_mutex_lock(&buf1_mut);

		while (buf1_isfull == 0)
			pthread_cond_wait(&buf1_cond, &buf1_mut);



		pthread_mutex_lock(&buf2_mut);

		//
		strcat(buf2, buf1);
		buf2_isfull = 1;

		//Now empty out the buf1 array
		//memset(buf1, '\0', MAX_IN_LEN);
		memset(buf1, '\0', MAX_IN_LEN * MAX_IN_COUNT);
		buf1_isfull = 0;

		pthread_mutex_unlock(&buf1_mut);


		//Signal to input thread that we are good to unlock

		//pthread_cond_signal(&buf1_cond);
		//pthread_mutex_unlock(&buf1_mut);


		//pthread_mutex_lock(&buf2_mut);



		char *idPtr;

		//If an instance of '\n' is found, replace it;
		//	No need for fancy movement since this is a one-char substitution
		while ( idPtr = strstr(buf2, "\n") )
			*idPtr = ' '; 

		
		//printf("\tIN THREAD 2: \tPRINTING BUF2: %s\n", buf2);

		//printf("PRINTING SPACE REPLACED: %s\n", buf2);

		pthread_cond_signal(&buf2_cond);
		pthread_mutex_unlock(&buf2_mut);
		//pthread_mutex_unlock(&buf1_mut);
		

	} while (stop_t2 == 0);

	//free(temp);

	stop_t3 = 1;
}



//Replaces all instances of '++' to '^'
//	To be used by thread 3
//void replacePlus() {
void *replacePlus() {
//char* replacePlus(char *buf2, char *buf3) {
	char *newStr, *temp2;
	char *rPtr, *idPtr;

	do {
		pthread_mutex_lock(&buf2_mut);

		while (buf2_isfull == 0)
			pthread_cond_wait(&buf2_cond, &buf2_mut);


		pthread_mutex_lock(&buf3_mut);
		//Wait while buf2 is empty

		//Copy buffer 2 into temp
		strcat(buf3, buf2);
		//strcpy(buf3, buf2);
		buf3_isfull = 1;

		//Now empty out the buf2 array
		memset(buf2, '\0', MAX_IN_LEN * MAX_IN_COUNT);
		//memset(buf2, '\0', MAX_IN_LEN);
		buf2_isfull = 0;


		pthread_mutex_unlock(&buf2_mut);


		//If an instance of "++" is found, place it in 'idPtr'
		while ( idPtr = strstr(buf3, "++") ) {
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
				strcpy(newStr, buf3);

				strcat(newStr, rPtr);

				temp2 = buf3;
				buf3 = newStr;
				free(temp2);
			}

		}

		//printf("PRINTING LIST INTO FINAL OUTPUT: %s\n", buf3);

		//printf("\tIN THREAD 3: \tPRINTING BUF3: %s\n", buf3);
		pthread_cond_signal(&buf3_cond);
		pthread_mutex_unlock(&buf3_mut);


	} while (stop_t3 == 0);

	stop_t4 = 1;
}

//Prints out 80 characters at a time from a given buffer
//	To be used by thread 4
//void writeOutput(char* buf3) {
void *writeOutput() {
	int counter;
	int buflen;

	do {
		pthread_mutex_lock(&buf3_mut);

		while (buf3_isfull == 0)
			pthread_cond_wait(&buf3_cond, &buf3_mut);

		buflen = strlen(buf3);
		//printf("Length of Output: %d\n", buflen);

		//If the buffer is long enough, then print out the first 80 chars
		//	Afterwards, memmove the string back
		//	Repeat for every instance of the string being above 80 chars
		//
		//printf("\tIN THREAD 4: \tPRINTING OUT: %s\n", buf3);
		while (strlen(buf3) >= 80) {
			printf("%.80s\n", buf3);
			memmove(buf3, buf3+80, MAX_IN_LEN-80);
		}

		buf3_isfull = 0;

		pthread_mutex_unlock(&buf3_mut);

	} while (stop_t4 == 0);
}
