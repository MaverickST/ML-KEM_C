#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define Q 3329

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#endif