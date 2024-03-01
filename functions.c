#include "functions.h"


unsigned char* BitsToBytes(unsigned char* bitArray) {
    //Converts a bit string (of length a multiple of eight) into an array of bytes.

    // Calculates the length of the bit string
    int numBits = strlen(bitArray);

    // Calculates the number of bytes required
    int numBytes = (numBits + 7) / 8; // Round up to the nearest multiple of 8

    // Create an array of bytes
    unsigned char* bytesArray = (unsigned char*)calloc(numBytes, sizeof(unsigned char));
    if (bytesArray == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Iterate over the bit string
    for (int i = 0; i < numBits; i++) {
        // Set the bit in the corresponding byte (little-endian format)
        bytesArray[i / 8] |= (bitArray[i] - '0') << (i % 8);
        // Set the bit in the corresponding byte (big-endian format)
        //bytesArray[i / 8] |= (bitArray[i] - '0') << (7 - (i % 8));
    }

    return bytesArray;
}

unsigned char* BytesToBits(unsigned char* bytesArray) {
    //Converts an array of bytes into a bit string (of length a multiple of eight).

    //Calculate the length of the byte array
    int numBytes = strlen(bytesArray);
    
    // Create an array of bits
    int numBits = numBytes * 8;
    unsigned char* bitString = (unsigned char*)calloc(numBits + 1, sizeof(unsigned char));
    if (bitString == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Iterate over the bytes
    for (int i = 0; i < numBytes; i++) {
        // Iterate over the bits in the byte (little-endian format)
        for (int j = 0; j < 8; j++) {
            // Set the bit in the corresponding position (little-endian format)
            bitString[i * 8 + j] = (bytesArray[i] >> j) & 1;

            //Ser the bit in the corresponding position (big-endian format)
            //bitString[i * 8 + (7 - j)] = (bytesArray[i] >> j) & 1;
        }
    }

    return bitString;
}

unsigned char* byteEncode(__uint16_t F[], __uint8_t d) {
    // ByteEncoded serializes an array of d-bit integers into an array of 32d bytes.
    //Encodes an array of d-bit integers into a byte array of 32d, for 1 ≤ d ≤ 12

    // Create an array of bits
    __uint16_t numBits = 256*d;

    // Create an array of bits
    unsigned char* bitArray = (unsigned char*)calloc(numBits, sizeof(unsigned char));
    if (bitArray == NULL) {
        fprintf(stderr, "Memory allocation error - byteEncode\n");
        return NULL;
    }


    for (int i = 0; i < 256; i++) {
        __uint8_t a = F[i];
        for (int j = 0; j < d; j++) {
            bitArray[i * d + j] = a%2;
            //a = a >> 1;
            a = (a - (bitArray[i * d + j] - '0')) / 2;
        }
    }

    unsigned char* byteArray = BitsToBytes(bitArray);

    // Free the allocated memory
    free(bitArray);

    return byteArray;
}


__uint16_t* byteDecode(unsigned char* byteArray, __uint8_t d){
    // Decodes an array of 32d bytes into an array of d-bit integers.
    //Decodes an array of 32d bytes into an array of d-bit integers.

    unsigned char* bitString = BytesToBits(byteArray);

    // Create an array of bits
    __uint16_t numBytes = 256;
    __uint16_t* intArrayF = (__uint16_t*)calloc(numBytes, sizeof(__uint16_t));

    if (intArrayF == NULL) {
        fprintf(stderr, "Memory allocation error - byteDncode\n");
        return NULL;
    }
    // Iterate over the bytes
    for(int i = 0; i < 256; i++) {
        for (int j = 0; j < d; j++) {
            intArrayF[i] += (bitString[i * d + j] - '0')<<j;// sum up the bits
        }
        if (d < 12){
            intArrayF[i] = intArrayF[i]%(1 << d);// truncate to d bits (mod 2^d)
        }else {
            intArrayF[i] = intArrayF[i]%(1 << 3329);// truncate to 3329 bits (mod q)
        }
        
    }

    // Free the allocated memory
    free(bitString);

    return intArrayF;
}