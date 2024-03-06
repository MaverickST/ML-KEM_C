#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define N 256
#define Q 3329
#define K 2
#define ETA_1 3
#define ETA_2 2
#define D_u 10
#define D_v 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


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

__uint8_t* byteEncode(__uint16_t* F, __uint8_t d);
__uint16_t* byteDecode(__uint8_t* byteArray, __uint8_t d);

// Algorithm 6: SampleNTT
__uint16_t* sampleNTT(__uint8_t* byteArray);
// Algorithm 7: SamplePolyCBDη
__uint16_t* samplePolyCBD(__uint8_t* byteArray, __uint8_t eta);
// Algorithm 8: NTT
__uint16_t* polyF2polyNTT(__uint16_t* polyF);
// Algorithm 9: Inverse NTT
__uint16_t* polyNTT2polyF(__uint16_t* polyNTT);
// Algorithm 10: Multiply NTT
__uint16_t* multiplyNTT(__uint16_t* polyNTT1, __uint16_t* polyNTT2);
// Algorithm 11: Base case multiply
__uint16_t baseCaseMultiplyC0(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1, __uint16_t gamma);
__uint16_t baseCaseMultiplyC1(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1);
// Algorithm 12: K-PKE.KeyGen()
void PKE_KeyGen(__uint8_t* ekPKE, __uint8_t* dkPKE);
__uint16_t** multiplyMatrixByVector(__uint16_t** matrix, __uint16_t** vector);
__uint16_t* sumPoly(__uint16_t* poly1, __uint16_t* poly2);
__uint16_t* mulPoly(__uint16_t* poly1, __uint16_t* poly2);

__uint8_t bitRev7(__uint8_t i);
// Modular arithmetic functions
__uint16_t conditionalReduce(__uint16_t a);
__uint16_t reduceBarrett(__uint32_t a);
__uint16_t addModq(__uint16_t a, __uint16_t b);
__uint16_t subModq(__uint16_t a, __uint16_t b);
__uint16_t mulModq(__uint16_t a, __uint16_t b);

__uint8_t* generateRandomBytes(__uint8_t d);

#endif