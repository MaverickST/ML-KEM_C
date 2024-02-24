/*!
\brief this is the first program in where we will start the program
\author Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int main() {
    const char* bitString = "0100100001100101011011000110110001101111";  // "Hello" in ASCII binary
    size_t length = strlen(bitString);

    // Convert the bit string to bytes in little-endian format
    unsigned char* bytesArrayLittleEndian = convertBitsToBytes(bitString);

    // Print the bytes in little-endian format
    printf("Bytes (Little Endian): ");
    for (size_t i = 0; i < length / 8; i++) {
        printf("%d ", bytesArrayLittleEndian[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(bytesArrayLittleEndian);

    return 0;
}

