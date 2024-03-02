/*!
\brief this is the first program in where we will start the program
\author Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

int main() {
    unsigned char* bitArray = "0100100001100101011011000110110001101111";  // "Hello" in ASCII binary
    int length = strlen(bitArray);

    printf("Original bits: %s\n", bitArray);

//     // Convert the bit string to bytes in little-endian format
//     unsigned char* bytesArrayLittleEndian = BitsToBytes(bitArray);

//     // Print the bytes in little-endian format
//     printf("Bytes (Little Endian): ");
//     for (int i = 0; i < length / 8; i++) {
//         printf("%d ", bytesArrayLittleEndian[i]);
//     }
//     printf("\n");

//     // Convert the bytes back to bits
//     printf("Bits (Little Endian): ");
//     unsigned char* bitStringLittleEndian = BytesToBits(bytesArrayLittleEndian);
//     for (int i = 0; i < length; i++) {
//         printf("%c", bitStringLittleEndian[i] + '0'); // Convert the numeric value to its character representation ('0' or '1')
//     }
//     printf("\n");

//     // Free the allocated memory
//     free(bytesArrayLittleEndian);
//     free(bitStringLittleEndian);


// // ------------------------------------------------------------------------------------
//     // Testing ByteEncode and ByteDecode functions

//     // Define and initialize an array of 256 __uint16_t elements
//     __uint16_t F[256];
//     for (int i = 0; i < 256; i++) {
//         F[i] = (__uint16_t)i;
//     }

//     // Define and initialize an array of 256 __uint16_t elements
//     // Define the value of d
//     __uint8_t d = 8; // Note: d should be in the range 1 ≤ d ≤ 12

//     // Call the byteEncode function
//     unsigned char* byteArray = byteEncode(F, d);
//     if (byteArray == NULL) {
//         fprintf(stderr, "Error: byteEncode returned NULL.\n");
//         return 1; // Exit with an error status
//     }

//     // Print the result
//     printf("Result of byteEncode:\n");
//     printArrayChar(byteArray, 32*d);

    
//     __uint16_t* intArray = byteDecode(byteArray, d);
//     if (intArray == NULL) {
//         fprintf(stderr, "Error: byteDecode returned NULL.\n");
//         return 1; // Exit with an error status
//     }

//     // Print intArray
//     printf("Result of byteDecode:\n");
//     printArrayInt(intArray, 256);

//     // Free the allocated memory
//     free(intArray);
//     free(byteArray);


    return 0;
}


