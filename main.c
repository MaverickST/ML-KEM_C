/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include "functions.h"
#include "test.h"

int main() {

    __uint16_t* intArray = runTestByteDecode(8);
    printf("Poly f: \n");
    for (int i = 0; i < 256; i++) {
        printf("%d ", intArray[i]);
    }
    printf("\n");

    __uint16_t* polyNTT = polyF2polyNTT(intArray);
    printf("PolyNTT: \n");
    for (int i = 0; i < 256; i++) {
        printf("%d ", polyNTT[i]);
    }
    printf("\n");

    free(intArray);
    free(polyNTT);

    return 0;
}


