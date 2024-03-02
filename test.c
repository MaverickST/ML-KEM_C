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

void runTestCompress(){
    // Test case values
    __uint16_t numMod_d = 16; // Example value for numMod_d, you can change it as needed
    __uint8_t d = 11; // Example value for d, you can change it as needed

    // Test rounding function
    struct rational numRounding = {numMod_d, 2001};
    __uint16_t roundedValue = rounding(numRounding);
    printf("Rounding result: %u\n", roundedValue);

    // Test compress function
    __uint16_t compressedValue = compress(numMod_d, d);

    // Test decompress function
    __uint16_t decompressedValue = decompress(compressedValue, d);

    printf("Result of Compress and Decompress for d=%u and numMod_d=%u\n", d, numMod_d);
    printf("Compress result: %u\n", compressedValue);
    printf("Decompress result: %u\n", decompressedValue);
    printf("\n");
}

void runTestsEncode(){
    // Create a test array of 256 elements
    __uint16_t F[256];
    for (int i = 0; i < 256; i++) {
        F[i] = i;  // Or any other test values you desire
    }

    // Call byteEncode with a specific value of d
    __uint16_t d = 8;  // Change this value as needed
    __uint8_t* byteArray = byteEncode(F, d);

    // Print the result (the array of bytes)
    printf("Result of byteEncode for d=%u:\n", d);
    for (int i = 0; i < 256 * d / 8; i++) {
        printf("%u ", byteArray[i]);
    }
    printf("\n");

    // Free allocated memory
    free(byteArray);

}