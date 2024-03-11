/*
 * utilities.h
 *
 *  Created on: Sep 2, 2023
 *      Author: ravel
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

//#define VERBOSE

/**
 * \brief Print an error message to terminal in case of wrong command line arguments
 */
void printHelp(void);

/**
 * \brief Compute module function as defined by standard NIST.FIPS.202
 * For integers x and y , mod(x,y) is the integer r for which 0 <= r < y and
 * x-r is a multiple of y
 * \param x any integer value
 * \param y a positive integer value
 * \return positive integer value r where 0 <= r < y and x-r is a multiple of y
 */
int mod(int x, int y);

/**
 * \brief rotLeft implements the rotation of 64bits data
 * \param X the data to rotate
 * \param shift the number of bits to rotate
 * \return Y the actual rotation of X shift bits to the left
 */

uint64_t rotLeft(uint64_t X, uint8_t shift);

/**
 * \brief rotRight implements the rotation of 64bits data
 * \param X the data to rotate
 * \param shift the number of bits to rotate
 * \return Y the actual rotation of X shift bits to the right
 */
uint64_t rotRight(uint64_t X, uint8_t shift);

/**
 * \brief Swap bits in a byte, bit0 goes to bit 7, bit 1 goes to 6, and so on
 * \param x byte whose bit will be swapped
 * \param y byte with bits swapped
 */
uint8_t swapBits(uint8_t x);

/**
 * \brief Transform a hexadecimal string to an one-dimensional byte array
 * \param h hexadecimal string
 * \param b byte array
 */
void h2b(uint8_t *b, char * h);

/**
 * \brief Transform an unidimensional byte array to an hexadecimal string
 * \param h hexadecimal string
 * \param b byte array
 */
void b2h(char *h, uint8_t *b, uint64_t lb);

/**
 * \brief transform an uint64_t array into a byte array
 * \param b8 output byte array
 * \param b64 input uint64_t array
 * \param l64 uint64_t array's length
 */
void cnv64to8bitsVector(uint8_t * b8, uint64_t * b64, uint64_t l64);

/**
 * \brief transform an byte array into a uint64_t array
 * \param b8 pointer to input byte array
 * \param b64 pointer to output uint64_t array
 * \param l8 uint64_t array's length
 */
void cnv8to64bitsVector(uint64_t *b64, uint8_t *b8, uint64_t l8);

/**
 * \brief Print in hexadecimal format a uint64_t array
 * \param b64 pointer to the array to print
 * \param l64 uint64_t array's length
 */
void print64bitsVector(uint64_t *b64, uint64_t l64);

/**
 * \brief Print in hexadecimal format a byte array
 * \param b8 Pointer to the byte array to print
 * \param l8 byte array length
 * \param nl number of byte to print
 */
void print8bitsVector(uint8_t *b8, uint64_t l8, uint8_t nl);

/**
 * \brief Print the bitstring S used by the Sponge
 * \param S Pointer to the bitstring, an array  of 25 uint64_t
 */
void printS(uint64_t *S);

/**
 * \brief Print status matrix A
 * Print the status matrix/vector in hexadecimal format
 * \param A is a pointer to the status matrix, an array of 25 positions of 64 bits
 */
void printA(uint64_t *A);

/**
 * \brief Print the hash
 * Print the hash in hexadecimal format
 * \param A pointer to Status Matrix A
 * \param hsize lnegth of the hash
 */
void printHash(uint64_t *A, uint64_t hsize);

/**
 * \brief Print the hash
 * Print the hash in hexadecimal format
 * \param A pointer to Byte array with hash
 * \param hsize length of the hash in bits
 */
void printHashBytes(uint8_t *A, uint16_t hsize);

#endif /* UTILITIES_H_ */
