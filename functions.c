#include "functions.h"


unsigned char* BitsToBytes(unsigned char* bitArray) {
    //Converts a bit string (of length a multiple of eight) into an array of bytes.

    // Calculates the length of the bit string
    size_t numBits = strlen(bitArray);

    // Calculates the number of bytes required
    size_t numBytes = (numBits + 7) / 8; // Round up to the nearest multiple of 8

    // Create an array of bytes
    unsigned char* bytesArray = (unsigned char*)calloc(numBytes, sizeof(unsigned char));
    if (bytesArray == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Iterate over the bit string
    for (size_t i = 0; i < numBits; i++) {
        // Set the bit in the corresponding byte (little-endian format)
        bytesArray[i / 8] |= (bitArray[i] - '0') << (i % 8);
    }

    return bytesArray;
}

unsigned char* BytesToBits(unsigned char* bytesArray) {
    //Converts an array of bytes into a bit string (of length a multiple of eight).

    //Calculate the length of the byte array
    size_t numBytes = strlen(bytesArray);
    
    // Create an array of bits
    size_t numBits = numBytes * 8;
    unsigned char* bitString = (unsigned char*)calloc(numBits + 1, sizeof(unsigned char));
    if (bitString == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Iterate over the bytes
    for (size_t i = 0; i < numBytes; i++) {
        // Iterate over the bits in the byte (little-endian format)
        for (size_t j = 0; j < 8; j++) {
            // Set the bit in the corresponding position (little-endian format)
            bitString[i * 8 + j] = (bytesArray[i] >> j) & 1;

            //Ser the bit in the corresponding position (big-endian format)
            //bitString[i * 8 + (7 - j)] = (bytesArray[i] >> j) & 1;
        }
    }

    return bitString;
}

unsigned char* byteEncode(__uint8_t F[], __uint8_t d) {
    // ByteEncoded serializes an array of d-bit integers into an array of 32d bytes.
    //Encodes an array of d-bit integers into a byte array of 32d, for 1 ≤ d ≤ 12

    // Create an array of bits
    __uint16_t numBits = 256*d;
    unsigned char* bitArray = (unsigned char*)calloc(numBits, sizeof(unsigned char));

    for (int i = 0; i < 256; i++) {
        __uint8_t a = F[i];
        for (int j = 0; j < d; j++) {
            bitArray[i * d + j] = a%2;
            a = (a - (bitArray[i * d + j] - '0')) / 2;
        }
    }

    unsigned char* byteArray = BitsToBytes(bitArray);

    // Free the allocated memory
    free(bitArray);

    return byteArray;
}