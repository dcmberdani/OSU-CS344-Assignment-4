#ifndef __MTHREADS_H
#define __MTHREADS_H
/*
 * Dev: Daniel Mendes
 * Date: 5/8/22
 * Description: 
 */

//Useful Constants; Determine the size of input
#define MAX_IN_LEN 1000
#define MAX_IN_COUNT 50

/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void initProg();

/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void freeProg();

//Grabs input from STDin
/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
char* getInput();


//Checks if the str 'STOP\n' is present or not
/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
int foundStop(char* currStr);


/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
int foundStop(char* currStr);


//Does all of the thread1 stuff
/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void *writeInputToBuffer();


//Adapted from my function from smallsh
/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void *replaceLineSep();


/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void *replacePlus();


//Writes 80 chars to output at a time;
/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void *writeOutput();




/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
char* getBuf1();
char* getBuf2();
char* getBuf3();


/*
 * Name: 
 * Description: 
 * Preconditions: 
 * Postconditions: 
 */
void putBuf1(char *tempStr);
void putBuf3(char *tempStr);
void putBuf2(char *tempStr);

#endif
