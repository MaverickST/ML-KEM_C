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

#include <time.h>

int main() {

    clock_t start, end;
    double cpu_time_used;

    start = clock(); // Take the time before calling the function
    runTestML_KEM(); // Run the function
    end = clock(); // Take the time after the function has finished

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", cpu_time_used);

    

	return EXIT_SUCCESS;

}


