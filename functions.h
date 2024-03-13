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

// -----------------------------------------------------------------------------------------------------
// ------------------------------------------ MAIN ALGORITHMS ------------------------------------------
// -----------------------------------------------------------------------------------------------------

/**
 * @brief Aglorithm 2: Converts a bit string (of length a multiple of eight) into an array of bytes.
 * 
 * @param bitArray 
 * @param numBits 
 * @return __uint8_t* 
 */
__uint8_t* bitsToBytes(__uint32_t* bitArray, __uint16_t numBits);

/**
 * @brief Algorithm 3: Performs the inverse of BitsToBytes, converting a byte array into a bit array.
 * 
 * @param bytesArray 
 * @param numBytes 
 * @return __uint32_t* 
 */
__uint32_t* bytesToBits(__uint8_t* bytesArray, __uint16_t numBytes);

/**
 * @brief Algorithm 4: Encodes an array of 256 d-bit integers (polynomial) into a byte array, for 1 ≤ d ≤ 12.
 * 
 * @param F Polynomial
 * @param d 
 * @return __uint8_t* byte array of size 32*d
 */
__uint8_t* byteEncode(__uint16_t* F, __uint8_t d);

/**
 * @brief Algorithm 5: Decodes a byte array into an array of d-bit integers, for 1 ≤ d ≤ 12.
 * 
 * @param byteArray array of 32*d bytes
 * @param d 
 * @return __uint16_t* 256 d-bit integers
 */
__uint16_t* byteDecode(__uint8_t* byteArray, __uint8_t d);

/**
 * @brief Algorithm 6: If the input is a stream of uniformly random bytes, 
 * the output is a uniformly random element of Tq
 * 
 * @param byteArray stream of bytes.
 * @return __uint16_t* 256 integers mod Q.
 */
__uint16_t* sampleNTT(__uint8_t* byteArray);

/**
 * @brief Algorithm 7: If the input is a stream of uniformly random bytes, 
 * outputs a sample from the distribution Dη (Rq)
 * 
 * @param byteArray array of 64*eta bytes
 * @param eta 
 * @return __uint16_t* 256 integers mod Q.
 */
__uint16_t* samplePolyCBD(__uint8_t* byteArray, __uint8_t eta);

/**
 * @brief Algorithm 8: Numeric-Theoretic Transform
 * Computes the NTT representation fˆ of the given polynomial f ∈ Rq.
 * 
 * @param polyF the coeffcients of the input polynomial
 * @return __uint16_t* the coeffcients of the NTT of the input polynomial
 * @note Input and output parameters are 256 integers mod Q.
 */
__uint16_t* NTT(__uint16_t* polyF);

/**
 * @brief Algorithm 9: Inverse NTT
 * Computes the polynomial f ∈ Rq corresponding to the given NTT representation fˆ ∈ Tq.
 * 
 * @param polyNTT the coeffcients of input NTT representation
 * @return __uint16_t* the coeffcients of the inverse-NTT of the input
 * @note Input and output parameters are 256 integers mod Q.
 */
__uint16_t* inverseNTT(__uint16_t* polyNTT);

/**
 * @brief Algorithm 10: Computes the product (in the ring Tq) of two NTT representations.
 * 
 * @param polyNTT1 
 * @param polyNTT2 
 * @return __uint16_t* 
 * @note Input and output parameters are 256 integers mod Q.
 */
__uint16_t* multiplyNTT(__uint16_t* polyNTT1, __uint16_t* polyNTT2);

/**
 * @brief Computes the product of two degree-one polynomials with respect to a quadratic modulus.
 * 
 * @param a0 
 * @param a1 
 * @param b0 
 * @param b1 
 * @param gamma value of an array pre-calculated
 * @return __uint16_t 
 */
__uint16_t baseCaseMultiplyC0(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1, __uint16_t gamma);
__uint16_t baseCaseMultiplyC1(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1);

/**
 * @brief Algorithm 12: Generates an encryption key and a corresponding decryption key.
 * 
 * @return struct Keys which containt ek and dk keys.
 * @note encrypt key (ek) is an array of 384*K + 32 bytes.
 * @note decrypt key (dk) is an array of 384*K bytes.
 */
struct Keys PKE_KeyGen();

/**
 * @brief Algorithm 14: Uses the decryption key to decrypt a ciphertext.
 * 
 * @param dkPKE array of 384*K bytes
 * @param cipherText array of 32*(d_u*K + d_v) bytes
 * @return __uint8_t* m -> array of 32 bytes
 */
__uint8_t* PKE_Decrypt(__uint8_t* dkPKE, __uint8_t* cipherText);

/**
 * @brief Algorithm 15: Generates an encapsulation key and a corresponding decapsulation key.
 * 
 * @return struct Keys 
 * @note encapsulation key (ek) is an array of 384*K + 32 bytes
 * @note decapsulation key (dk) is an array of 768*K + 96 bytes
 */
struct Keys ML_KEM_KeyGen();

/**
 * @brief Algorithm 16: Uses the encapsulation key to generate a shared key and an associated ciphertext.
 * 
 * @param ekML 384*K+32 bytes array
 * @return __uint8_t* concatenation of the Shared key k (32 bytes array) 
 * and c the cipher text(32*(d_u*K + d_v) bytes array)
 */
__uint8_t* ML_KEM_Encaps(__uint8_t* ekML);

/**
 * @brief Algorithm 17: Uses the decapsulation key to produce a shared key from a ciphertext.
 * 
 * @param cipherText 32*(d_u*K + d_v) bytes array. Ciphertext.
 * @param dkML decapsulation key (768*K + 96 bytes array)
 * @return __uint8_t* shared key k (32 bytes array)
 */
__uint8_t* ML_KEM_Decaps(__uint8_t* cipherText, __uint8_t* dkML);

// ---------------------------------------------------------------------------------
// -------------------------------- SOPPORT FUNCTIONS ------------------------------
// ---------------------------------------------------------------------------------

/**
 * @brief Round a rational number to the nearest integer
 * 
 * @param num is a rational number, a structure
 * @return __uint16_t 
 */
__uint16_t rounding(struct rational num);
/**
 * @brief Compress a integer mod Q into a integer mod 2^d
 * 
 * @param numMod_d integer mod Q
 * @param d 
 * @return __uint16_t 
 */
__uint16_t compress(__uint16_t numMod_d, __uint8_t d);
/**
 * @brief Decompress a integer mod 2^d into a integer mod Q
 * 
 * @param numMod_2d integer mod 2^d
 * @param d 
 * @return __uint16_t integer mod Q
 */
__uint16_t decompress(__uint16_t numMod_2d, __uint8_t d);

/**
 * @brief Compute the dot product of two vectors. 
 * 
 * @param vector1 K-polynomials
 * @param vector2 K-polynomials
 * @return __uint16_t* 256 integers mod Q
 */
__uint16_t* vectorDotProduct(__uint16_t** vector1, __uint16_t** vector2);

/**
 * @brief Convert a vector of K polynomials into an array of the speficied number of bytes
 * 
 * @param vector K-polynomials
 * @param numBytes number of bytes to convert the vector
 * @return __uint8_t* array of numBytes bytes
 */
__uint8_t* vector2Bytes(__uint16_t** vector, __uint16_t numBytes);

/**
 * @brief Computes de multiplication between a matrix of K*K pylimomials and a vector of K pylimomials
 * 
 * @param matrix K*K-polynomials matrix
 * @param vector K-polynomials vector
 * @return __uint16_t** the result is a K-polynomials vector
 */
__uint16_t** multiplyMatrixByVector(__uint16_t** matrix, __uint16_t** vector);

/**
 * @brief Computes the sum of two vectors
 * 
 * @param vector1 K-polynomials vector
 * @param vector2 K-polynomials vector
 * @return __uint16_t** another K polynomials vector
 */
__uint16_t** sumVector(__uint16_t** vector1, __uint16_t** vector2);

/**
 * @brief Computes de sum of two polynomials.
 * 
 * @param poly1 
 * @param poly2 
 * @return __uint16_t* 
 * @note Input and output are 256 integers mod Q (polynomials).
 */
__uint16_t* sumPoly(__uint16_t* poly1, __uint16_t* poly2);

/**
 * @brief Compute the cancatenation of two byte arrays: byteArray1 || byteArray2
 * 
 * @param byteArray1 
 * @param byteArray2 
 * @param numBytes1 
 * @param numBytes2 
 * @return __uint8_t* 
 */
__uint8_t* concatenateBytes(__uint8_t* byteArray1, __uint8_t* byteArray2, __uint16_t numBytes1, __uint16_t numBytes2);

/**
 * @brief Aply the conditional reduction to integers mod Q
 * 
 * @param a 
 * @return __uint16_t 
 */
__uint16_t conditionalReduce(__uint16_t a);

/**
 * @brief Implement the Barrett reduction algorithm to do a multiplication between 12-bit integers.
 * 
 * @param a 
 * @return __uint16_t 
 */
__uint16_t reduceBarrett(__uint32_t a);

/**
 * @brief Modular sum between two integers mod Q.
 * 
 * @param a integer mod Q.
 * @param b integer mod Q.
 * @return __uint16_t 
 */
__uint16_t addModq(__uint16_t a, __uint16_t b);

/**
 * @brief Modular substraction between two integers mod Q.
 * 
 * @param a integer mod Q.
 * @param b integer mod Q.
 * @return __uint16_t 
 */
__uint16_t subModq(__uint16_t a, __uint16_t b);

/**
 * @brief Computes the modular multiplication between two integers mod Q using Barret Algorithm.
 * 
 * @param a integer mod Q.
 * @param b integer mod Q.
 * @return __uint16_t 
 */
__uint16_t mulModq(__uint16_t a, __uint16_t b);

__uint8_t* generateRandomBytes(__uint8_t d);
void freeVector(__uint16_t **vector, __uint8_t sizeK);

__uint8_t* copyBytesArray(__uint8_t* byteArray, __uint16_t numBytes);

#endif