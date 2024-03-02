#include "test.h"

void printBitArray(__uint32_t* bitArray, __uint16_t numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        printf("%d", (bitArray[i / 32] >> (i % 32)) & 0x01);
        if (i  % 32 == 0 && i > 0) {
            printf(" - ");
        }
    }
    printf("\n");
}

void printByteArray(__uint8_t* byteArray, __uint16_t numBytes) {
    for (int i = numBytes - 1; i >= 0; i--) {
        printf("%d", byteArray[i]);
        if (i > 0) {
            printf(" - ");
        }
    }
    printf("\n");
}

void runTestsConvertions(){

    // Declare variables
    __uint32_t bitArray[8]; // Array of 256 bits
    __uint16_t numBits = 256; // Number of bits in the array

     // Fill the bitArray with random bit values (0 or 1)
    for (int i = 0; i < 8; i++) {
        bitArray[i] = rand(); // Randomly generate
    }

    // Convert the array of bits into an array of bytes
    __uint8_t* byteArray = bitsToBytes(bitArray, numBits);

    // Convert the bytes back into bits
    __uint32_t* reconstructedBitArray = bytesToBits(byteArray, numBits / 8);

    // Display the resulting bits
    printf("Original bits: ");
    printBitArray(bitArray, numBits);
    printf("\n");
    printf("Bits to bytes: ");
    printByteArray(byteArray, numBits / 8);
    printf("\n");
    printf("Bytes to bits: ");
    printBitArray(reconstructedBitArray, numBits);
    printf("\n");

    // Free allocated memory
    free(byteArray);
    free(reconstructedBitArray);
}