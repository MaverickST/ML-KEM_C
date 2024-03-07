/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include "functions.h"
#include "test.h"



int main() {

    srand(time(NULL)); // use current time as seed for random generator

    __uint8_t* ekPKE;
    __uint16_t **tNTT = (__uint16_t**)calloc(K, sizeof(__uint16_t *));
    __uint8_t *rho;

    printf("Vector NTT: \n");
    for (int i = 0; i < K; i++) {
        tNTT[i] = generateRandomPoly(Q);
        printPoly(tNTT[i]);
    }

    printf("Bytes rho: \n");
    rho = generateRandomBytes(1);
    printBytes(rho, 1);

    printf("Bytes ekPKE: \n");
    ekGeneration(ekPKE, tNTT, rho);
    printBytes(ekPKE, 25);

    // free(rho);
    free(ekPKE); // error here
    // free(tNTT);

    return 0;

}


