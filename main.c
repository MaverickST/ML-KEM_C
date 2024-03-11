/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include "functions.h"
#include "test.h"
#include "utilities.h"
#include "mappings.h"
#define D 128



int main() {

    srand(time(NULL)); // use current time as seed for random generator

    // Testing PKE_Decrypt
    __uint8_t* dkPKE = generateRandomBytes(12*K);
    __uint8_t* cipherText = generateRandomBytes(D_u*K + D_v);
    __uint8_t* m = PKE_Decrypt(dkPKE, cipherText);

    // Printing
    printf("dkPKE: %d \n", sizeof(*dkPKE));
    printBytesHex(dkPKE, 12*K);

    printf("cipherText: %d \n", sizeof(*cipherText));
    printBytesHex(cipherText, D_u*K + D_v);

    printf("m: %d \n", sizeof(*m));
    printBytesHex(m, 1);

    // free memory
    free(dkPKE);
    free(cipherText);
    free(m);

    return 0;

}


