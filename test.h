#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

void printBitArray(__uint32_t* bitArray, __uint16_t numBits);
void printByteArray(__uint8_t* byteArray, __uint16_t numBytes);
void runTestsConvertions();

#endif // TEST_H