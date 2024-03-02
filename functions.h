#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define Q 3329

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define k_BARRETT 12     // k = log2(q)
#define r_BARRETT 5039   // r = 4^k/q

struct rational {
    __uint32_t numerator;
    __uint32_t denominator;
};

__uint8_t* bitsToBytes(__uint32_t* bitArray, __uint16_t numBits);
__uint32_t* bytesToBits(__uint8_t* bytesArray, __uint16_t numBytes);
__uint16_t rounding(struct rational num);
__uint16_t compress(__uint16_t numMod_d, __uint8_t d);
__uint16_t decompress(__uint16_t numMod_2d, __uint8_t d);

unsigned char* byteEncode(__uint16_t F[], __uint8_t d);
__uint16_t* byteDecode(unsigned char* byteArray, __uint8_t d);

void printArrayInt(__uint16_t* array, __uint32_t size);
void printArrayChar(unsigned char* array, __uint32_t size);
__uint8_t bitRev7(__uint8_t i);


// Modular arithmetic functions
__uint16_t conditionalReduce(__uint16_t a);
__uint16_t reduceBarrett(__uint32_t a);
__uint16_t addModq(__uint16_t a, __uint16_t b);
__uint16_t subModq(__uint16_t a, __uint16_t b);
__uint16_t mulModq(__uint16_t a, __uint16_t b);

#endif