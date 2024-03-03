#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

void printBitArray(__uint32_t* bitArray, __uint16_t numBits);
void printByteArray(__uint8_t* byteArray, __uint16_t numBytes);
void printPoly(__uint16_t* poly);
void runTestsConvertions();
void runTestCompress();
void runTestsEncode();
void runTestByteDecode(__uint8_t d);
void runTest_NTT_inverseNTT();
void runTestMultiplyNTT();
__uint16_t* generateRandomPoly();

#endif // TEST_H