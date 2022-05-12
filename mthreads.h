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


//Grabs input from STDin
char* getInput();


//Adapted from my function from smallsh
void replaceLineSep(char* currStr);
void replacePlus(char* currStr);




#endif
