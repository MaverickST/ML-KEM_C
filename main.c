/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include "functions.h"
#include "test.h"


int main() {

    srand(time(NULL)); // use current time as seed for random generator

    struct Keys kPKE = PKE_KeyGen();

    printf("Encryption Key: \n");
    printBytes(kPKE.ek, (383*K + 32)/32);

    printf("Decryption Key: \n");
    printBytes(kPKE.dk, (383*K)/32);


    return 0;

}


