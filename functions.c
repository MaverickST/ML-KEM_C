#include "functions.h"

__uint8_t* bitsToBytes(__uint32_t* bitArray, __uint16_t numBits) {
    //Converts a bit array (of length a multiple of eight) into an array of bytes.

    // Calculates the number of bytes required
    __uint32_t numBytes = numBits/8;

    // Create an array of bytes
    __uint8_t* bytesArray = (__uint8_t*)calloc(numBytes, sizeof(__uint8_t));
    if (bytesArray == NULL) {
        fprintf(stderr, "Memory allocation error - bitsToBytes\n");
        return NULL;
    }

    // Iterate over the byteArray
    for (__uint32_t i = 0; i < numBytes; i++) {
        // Set the bit in the corresponding byte (little-endian format)
        bytesArray[i] = (bitArray[i / 4] >> ((i % 4) * 8)) & 0xFF;
    }

    // // Iterate over the bit array
    // for (__uint32_t i = 0; i < numBits; i++) {
    //     // Calculate the index of the corresponding element in bitArray
    //     __uint32_t bitIndex = i / 32;
    //     // Calculate the bit position within the element of bitArray
    //     __uint32_t bitPosition = i % 32;
    //     // Set the bit in the corresponding byte (little-endian format)
    //     bytesArray[i / 8] |= (bitArray[bitIndex] >> bitPosition) & 0x01;
    // }

    return bytesArray;
}

__uint32_t* bytesToBits(__uint8_t* bytesArray, __uint16_t numBytes) {
    //Converts an array of bytes into aarray of bits (of length a multiple of eight).
    
    // Create an array of bits
    int numBitsArray = numBytes / 4;
    __uint32_t* bitArray = (__uint32_t*)calloc(numBitsArray, sizeof(__uint32_t));
    if (bitArray == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Iterate over the bitArray
    for (__uint32_t i = 0; i < numBytes; i++) {

        bitArray[i/4] |= ((__uint32_t)bytesArray[i] & 0xFF) << ((i % 4)*8);
    }

    return bitArray;
}

__uint16_t rounding(struct rational num) {
    //Round a rational number to the nearest integer
    __uint16_t numReduce = 0;
    __uint16_t remainder = num.numerator % num.denominator;

    // Integer division to get the integer part of the rational number
    numReduce = num.numerator / num.denominator;

    // Check if the fractional part is greater than or equal to 0.5
    if (((remainder * 10) / num.denominator) >= 5) {
        numReduce += 1; // Round up if the fractional part is >= 0.5
    }

    return numReduce;
}

__uint16_t compress(__uint16_t numMod_d, __uint8_t d) {
    // Compress a number mod q to a number mod 2^d
    // x ------>  approx(2^d/q)*x

    struct rational numCompress;

    // Create a rational number with numerator as (2^d * numMod_d) and denominator as 1
    numCompress.numerator = (1 << d) * numMod_d;
    numCompress.denominator = Q; // Denominator is 1 because we're dealing with integers

    // Call rounding function to round the rational number to the nearest integer
    return rounding(numCompress);
}


__uint16_t decompress(__uint16_t numMod_2d, __uint8_t d) {
    //decompress a number modulo 2^d to a number modulo q
    // x ------> approx(q/2^d)*x

    struct rational numDecompress;

    // Create a rational number with numerator as numMod_2d and denominator as 2^d
    numDecompress.numerator = Q*numMod_2d;
    numDecompress.denominator = 1 << d;

    // Call rounding function to round the rational number to the nearest integer
    return rounding(numDecompress);
}

__uint8_t* byteEncode(__uint16_t F[], __uint16_t d) {
    // ByteEncoded serializes an array of d-bit integers into an array of 32d bytes.
    //Encodes an array of d-bit integers into a byte array of 32d, for 1 ≤ d ≤ 12

    // Create an array of bits
    __uint16_t numBits = 256*d;

    // Create an array of bits
    __uint32_t numBitsArray = numBits / 32;
    __uint32_t* bitArray = (__uint32_t*)calloc(numBitsArray, sizeof(__uint32_t));
    if (bitArray == NULL) {
        fprintf(stderr, "Memory allocation error - byteEncode\n");
        return NULL;
    }


    for (__uint16_t i = 0; i < 256; i++) {
        __uint32_t a = F[i];
        for (int j = 0; j < d; j++) {
            bitArray[(i*d + j)/32] |= (a%2) << (i*d + j);
            a = (a - ((bitArray[(i*d + j)/32] >> ((i*d + j)%32)) & 0x01) ) / 2;
        }
    }

    __uint8_t* byteArray = bitsToBytes(bitArray, numBits/8);

    // Free the allocated memory
    free(bitArray);

    return byteArray;
}