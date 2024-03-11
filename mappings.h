/*
 * mappings.h
 *
 *  Created on: Sep 4, 2023
 *      Author: ravel
 */

#ifndef MAPPINGS_H_
#define MAPPINGS_H_

#include <stdint.h>

extern uint64_t RC[24];

/**
 * \brief Keccak-p's theta mapping
 * Compute the theta mapping used in Keccak-p
 * \param A Input state
 * \param Ap Output state after theta mapping
 */
void theta(uint64_t * A, uint64_t * Ap);

/**
 * \brief Keccak-p's rho mapping
 * Compute the rho mapping used in Keccak-p
 * \param A Input state
 * \param Ap Output state after rho mapping
 */
void rho(uint64_t * A, uint64_t * Ap);

/**
 * \brief Keccak-p's pi mapping
 * Compute the rho mapping used in Keccak-p
 * \param A Input state
 * \param Ap Output state after rho mapping
 */
void pi(uint64_t * A, uint64_t * Ap);

void chi(uint64_t * A, uint64_t * Ap);

void iota(uint64_t * A, uint64_t * Ap, uint8_t nr);

void rnd(uint64_t * A, uint64_t * Ap, uint8_t nr);

void rcConstants(void);

/**
 * \brief \brief Computes a variable length hash for a variable length input using FIPS 202 standard
 * ...
 * \param M pointer to byte array with data
 * \param H pointer to byte array where the hash is created
 * \param d digest length
 * \parar h hash size
 * \param sM number of bytes in M
 */
void XOF_d(uint8_t * M, uint8_t * H, uint16_t d, uint16_t h, uint16_t sM);

/**
 * \brief Computes a fixed length hash for a variable length input using FIPS 202 standard
 *
 * \param M pointer to byte array with data
 * \param H pointer to byte array where the hash is created
 * \param d hash size in bits
 * \param sM number of bytes in M
 */
void SHA3_d(uint8_t * M, uint8_t * H, uint16_t d, uint16_t sM);

/**
 * \brief hash functions with variable-length input and 256 bit(32 Byte) hash output
 * \param IN pointer to input byte vector
 * \param sin input vector size
 * \param OUT pointer to 32 byte vector with the hash
 */
void SHA3_256(uint8_t * IN, uint16_t sin, uint8_t * OUT);

/**
 * \brief hash functions with variable-length input and 512 bit(64 Byte) hash output
 * \param IN pointer to input byte vector
 * \param sin input vector size in bytes
 * \param OUT pointer to 64 byte vector with the hash
 */
void SHA3_512(uint8_t * IN, uint16_t sin, uint8_t * OUT);

/**
 * \brief hash functions with variable-length input and variable length hash output and digest length of 128 bits
 * \param IN pointer to input byte vector
 * \param sin input vector size in bytes
 * \param OUT pointer to output byte vector
 * \param sout output vector size in bytes
 */
void SHAKE_128(uint8_t * IN, uint16_t sin, uint8_t * OUT, uint16_t sout);

/**
 * \brief hash functions with variable-length input and variable length hash output and digest length of 128 bits
 * \param IN pointer to input byte vector
 * \param sin input vector size in bytes
 * \param OUT pointer to output byte vector
 * \param sout output vector size in bytes
 */
void SHAKE_256(uint8_t * IN, uint16_t sin, uint8_t * OUT, uint16_t sout);

#endif /* MAPPINGS_H_ */
