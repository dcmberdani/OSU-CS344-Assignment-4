#ifndef __MTHREADS_H
#define __MTHREADS_H
/*
 * Dev: Daniel Mendes
 * Date: 5/8/22
 * Description: 
 */

void testPrint();

/*
struct inputInfo {
	char* currInput;

}
*/

//Useful Constants; Determine the size of things
#define MAX_IN_LEN 1000
#define MAX_IN_COUNT 50


void initProg();
void freeProg();

//Grabs input from STDin
char* getInput();

//Checks if the str 'STOP\n' is present or not
//int foundStop(char* currStr);
int foundStop(char* currStr);

//Does all of the thread1 stuff
//void writeInputToBuffer();
void *writeInputToBuffer();
//int writeInputToBuffer(char *buf1);

//Adapted from my function from smallsh
void *replaceLineSep();
//void replaceLineSep(char *buf1, char *buf2);
void *replacePlus();
//void replacePlus(char *buf2, char *buf3);
//char* replacePlus(char *buf2, char *buf3);


//Writes 80 chars to output at a time;
//void writeOutput(char* buf3);
void *writeOutput();




char* getBuf1();
char* getBuf2();
char* getBuf3();

void putBuf1(char *tempStr);
void putBuf3(char *tempStr);
void putBuf2(char *tempStr);

#endif
