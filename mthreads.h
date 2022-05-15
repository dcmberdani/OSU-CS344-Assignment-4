#ifndef __MTHREADS_H
#define __MTHREADS_H
/*
 * Assignment: Multi-threaded Producer Consumer Pipeline (4)
 * Class: CS 344
 * Name: Daniel Mendes (mendesd@oregonstate.edu)
 * ID: 933827213
 * Date: 5/18/22
 * Description: This assignment performs input/output with a set of threads acting as producers/consumers
 * 	Each thread serves a specific purpose
 * 		Thread 1 does input
 * 		Threads 2/3 operate on the input 
 * 		Thread 4 does output
 *	Mutex locks and conditional variables are used to sync the threads during execution
 *	When the input "STOP\n" is reached, the threads begin to stop execution
 */

//Useful Constants; Determine the size of input
#define MAX_IN_LEN 1000
#define MAX_IN_COUNT 50

/*
 * Name: initProg
 * Description: Allocates memories for each of the inter-thread buffers
 * Preconditions: Buffers to be declared
 * Postconditions: Each of the buffers having allocated memory
 */
void initProg();


/*
 * Name: freeProg
 * Description: Frees memory for each of the inter-thread buffers
 * Preconditions: Buffers with memory allocated
 * Postconditions: Memories for the buffers freed
 */
void freeProg();


/*
 * Name: putBuf1/2/3
 * Description: 'Putters' for the buffers 
 * 	(technically not setters as they use concatenation instead of direct copying)
 * 	Uses mutex to allow for producers to write into buffers
 * Preconditions: Input to be written to buffers
 * Postconditions: Input is thread-safely written to buffers
 */
void putBuf1(char *tempStr);
void putBuf3(char *tempStr);
void putBuf2(char *tempStr);


/*
 * Name: getBuf1/2/3
 * Description: Getters for the buffer
 * 	Uses mutex to allow for consumers to grab from buffers
 * Preconditions: Buffers with some information inside
 * Postconditions: Input is thread-safely read from buffers
 */
char* getBuf1();
char* getBuf2();
char* getBuf3();


/*
 * Name: getInput
 * Description: Helper function for thread 1; 
 * 	Grabs input from stdin
 * Preconditions: Thread 1 being active
 * Postconditions: A line of input from stdin
 */
char* getInput();


/*
 * Name: foundStop
 * Description: a helper function for ALL threads
 * 	Identifies if the input "STOP\n" is present; Used to signal when to stop
 * Preconditions: String input being provided
 * Postconditions: Whether or not the stop flag is found
 */
int foundStop(char* currStr);

/*
 * Name: writeInputToBuffer
 * Description: This function is delegated to and performs the tasks of thread 1
 * 	It takes in input from stdin and writes it to the first buffer
 * 	Continuously repeats until the "STOP\n" flag is reached
 * Preconditions: Thread 1 to be active
 * Postconditions: Behaviors for thread 1 continuing until the stop flag is provided
 */
void *writeInputToBuffer();


/*
 * Name: replaceLineSep
 * Description: This function is delegated to and performs the tasks of thread 2
 * 	It grabs text from buffer 1, transforms all '\n's into ' 's, then writes the new string to buffer 2
 * Preconditions: Thread 2 to be active
 * Postconditions: Behaviors for thread 2 continuing until the stop flag is provided
 */
void *replaceLineSep();


/*
 * Name: 
 * Description: This function is delegated to and performs the tasks of thread 3
 * 	It grabs text from buffer 2, transforms all '++'s into '^'s, then writes the new string to buffer 3
 * Preconditions: Thread 3 to be active
 * Postconditions: Behaviors for thread 3 continuing until the stop flag is provided
 */
void *replacePlus();


//Writes 80 chars to output at a time;
/*
 * Name: 
 * Description: This function is delegated to and performs the tasks of thread 4
 * 	It reads 80 chars from buffer 3 at a time, writes them to STDOUT, then removes them from the buffer
 * 	Continues until there are less than 80 chars in buf3
 * Preconditions: Thread 4 to be active
 * Postconditions: Behaviors for thread 4 continuing until the stop flag is provided
 */
void *writeOutput();

#endif
