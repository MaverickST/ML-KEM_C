#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

void printBitArray(__uint32_t* bitArray, __uint16_t numBits);
void printByteArray(__uint8_t* byteArray, __uint16_t numBytes);
void printPoly(__uint16_t* poly);
void printBytes(__uint8_t* byteArray, __uint8_t d);
void printBytesHex(__uint8_t* byteArray, __uint16_t d);
void runTestsConvertions();
void runTestCompress();
void runTestsEncode();
void runTestByteDecode(__uint8_t d);
void runTestByteEncode(__uint8_t d);
void runTestEncodeDecode(__uint8_t d);
void runTestSamples(__uint8_t eta);
void runTest_NTT_inverseNTT();
void runTestMultiplyNTT();
void runTestSumPoly();
void runTestMatrixVectorOperations();
void runtTestConcatenateBytes(__uint8_t a, __uint8_t b);
void runTestVector2Bytes();
void runTestVectorDotProduct();
__uint16_t* generateRandomPoly(__uint16_t mod);

void runTestPKE_KeyGen();
void runTestPKE_Encrypt();
void runTestPKE_Decrypt();
void runTestPKE();

void runTestML_KEM();
void runTest_XOF_PRF();

#endif // TEST_H