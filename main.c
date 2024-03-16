/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
gcc main.c functions.c utilities.c mappings.c test.c fips202.c fips202x2.c  -o ./program.out
*/

#include "functions.h"
#include "test.h"
#include "utilities.h"
#include "mappings.h"
#define D 128



int main() {

    srand(time(NULL)); // use current time as seed for random generator

    runTestML_KEM();

    // runTestSamples(ETA_1);


	return EXIT_SUCCESS;

}


