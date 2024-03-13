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

#include "utilities.h"
#include "mappings.h"

#define k_BARRETT 12     // k = log2(q)
#define r_BARRETT 5039   // r = 4^k/q


struct rational {
    __uint32_t numerator;
    __uint32_t denominator;
};

struct Keys
{
    __uint8_t* ek;
    __uint8_t* dk;
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
__uint16_t* NTT(__uint16_t* polyF);
// Algorithm 9: Inverse NTT
__uint16_t* inverseNTT(__uint16_t* polyNTT);
// Algorithm 10: Multiply NTT
__uint16_t* multiplyNTT(__uint16_t* polyNTT1, __uint16_t* polyNTT2);
// Algorithm 11: Base case multiply
__uint16_t baseCaseMultiplyC0(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1, __uint16_t gamma);
__uint16_t baseCaseMultiplyC1(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1);
// Algorithm 12: K-PKE.KeyGen()
struct Keys PKE_KeyGen();
//algorithm 13: K-PKE.Encrypt()
__uint8_t PKE_Encrypt(__uint8_t* ekPKE, __uint8_t* m, __uint8_t* r, __uint8_t d);

__uint8_t* XOF(__uint8_t* rho, __uint8_t i, __uint8_t j, __uint16_t sizeRho, __uint16_t* sizeOut);
__uint8_t* PRF(__uint8_t* r, __uint16_t sizeR, __uint8_t n, __uint8_t eta, __uint16_t* sizeOut);

__uint8_t* vector2Bytes(__uint16_t** vector, __uint16_t numBytes);
__uint16_t** multiplyMatrixByVector(__uint16_t** matrix, __uint16_t** vector);
__uint16_t** sumVector(__uint16_t** vector1, __uint16_t** vector2);
__uint16_t* sumPoly(__uint16_t* poly1, __uint16_t* poly2);
__uint16_t* mulPoly(__uint16_t* poly1, __uint16_t* poly2);
__uint8_t* concatenateBytes(__uint8_t *byteArray1, __uint8_t *byteArray2, __uint16_t numBytes1, __uint16_t numBytes2, __uint16_t* numBytes);

__uint8_t bitRev7(__uint8_t i);
// Modular arithmetic functions
__uint16_t conditionalReduce(__uint16_t a);
__uint16_t reduceBarrett(__uint32_t a);
__uint16_t addModq(__uint16_t a, __uint16_t b);
__uint16_t subModq(__uint16_t a, __uint16_t b);
__uint16_t mulModq(__uint16_t a, __uint16_t b);

__uint8_t* generateRandomBytes(__uint8_t d);

__uint8_t* copyBytesArray(__uint8_t* byteArray, __uint16_t numBytes);
__uint8_t* segmentBytesArray(__uint8_t *byteArray, __uint16_t start, __uint16_t end, __uint16_t* numBytes);
#endif