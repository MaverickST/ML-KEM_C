/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
gcc main.c functions.c utilities.c mappings.c test.c -o ./program.out
*/

#include "functions.h"
#include "test.h"
#include "utilities.h"
#include "mappings.h"
#define D 128

#include <time.h>

int main() {

    // random seed
    srand(time(NULL));

    runTestML_KEM();

	return EXIT_SUCCESS;

}


