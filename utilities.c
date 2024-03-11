/*
 * utilities.c
 *
 *  Created on: Sep 2, 2023
 *      Author: ravel
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"


/**
 * \brief Compute module function as defined by standard NIST.FIPS.202
 * For integers x and y , mod(x,y) is the integer r for which 0 <= r < y and
 * x-r is a multiple of y
 * \param x any integer value
 * \param y a positive integer value
 * \return positive integer value r where 0 <= r < y and x-r is a multiple of y
 */
int mod(int x, int y){
	if (x>=0){
		return x % y;
	}
	else{
		return ((x % y) + y) % y;
	}
}

/**
 * \brief rotLeft implements the rotation of 64bits data
 * \param X the data to rotate
 * \param shift the number of bits to rotate
 * \return Y the actual rotation of X shift bits to the left
 */

uint64_t rotLeft(uint64_t X, uint8_t shift){
	uint64_t Y=0ULL;
	Y = (X << shift)|(X>>(64-shift));
	return Y;
}

/**
 * \brief rotRight implements the rotation of 64bits data
 * \param X the data to rotate
 * \param shift the number of bits to rotate
 * \return Y the actual rotation of X shift bits to the right
 */
uint64_t rotRight(uint64_t X, uint8_t shift){
	uint64_t Y=0ULL;
	Y = (X >> shift)|(X<<(64-shift));
	return Y;
}

/**
 * \brief Swap bits in a byte, bit0 goes to bit 7, bit 1 goes to 6, and so on
 * \param x input byte whose bits will be swapped
 * \param y output byte with bits swapped
 */
uint8_t swapBits(uint8_t x){
	uint8_t y=0;
	y =  (x&0x01) << 7;
	y |= (x&0x02) << 5;
	y |= (x&0x04) << 3;
	y |= (x&0x08) << 1;
	y |= (x&0x10) >> 1;
	y |= (x&0x20) >> 3;
	y |= (x&0x40) >> 5;
	y |= (x&0x80) >> 7;
	return y;
}


/**
 * \brief Transform a hexadecimal string to an unidemensional byte array
 * \param h hexadecimal string
 * \param b byte array
 */
void h2b(uint8_t *b, char * h){
	uint8_t hex=0;
	uint8_t cnt=0;
	while((*h)!=0){
		switch(*h){
		case 'A':
			hex=(hex<<4)|0x0A;
			break;
		case 'B':
			hex=(hex<<4)|0x0B;
			break;
		case 'C':
			hex=(hex<<4)|0x0C;
			break;
		case 'D':
			hex=(hex<<4)|0x0D;
			break;
		case 'E':
			hex=(hex<<4)|0x0E;
			break;
		case 'F':
			hex=(hex<<4)|0x0F;
			break;
		default:
			hex=(hex<<4)|((*h)-48);
			break;
		}
		cnt++;
		if (cnt==2){
			cnt = 0;
			*b = swapBits(hex);
			hex=0;
			b++;
		}
		h++;
	}
}

/**
 * \brief Transform an unidimensional byte array to an hexadecimal string
 * \param h hexadecimal string
 * \param b byte array
 */
void b2h(char *h, uint8_t *b, uint64_t lb)
{
	uint64_t i,j;
	uint8_t bin, tmp;
	for(i=0;i<lb;i++)
	{
		bin = swapBits(*b);
		for (j=0;j<1;j++)
		{
			 tmp = (bin >> 4);
			 switch(tmp){
			 case 10:
				 *h = 'A';
				 break;
			 case 11:
				 *h = 'B';
				 break;
			 case 12:
				 *h = 'C';
				 break;
			 case 13:
				 *h = 'D';
				 break;
			 case 14:
				 *h = 'E';
				 break;
			 case 15:
				 *h = 'F';
				 break;
			 default:
				 *h = tmp+48;
				 break;
			 }
			 h++;
			 bin = bin<<4;
		}
	}
}

/**
 * \brief transform an uint64_t array into a byte array
 * \param b8 output byte array
 * \param b64 input uint64_t array
 * \param l64 uint64_t array's length
 */
void cnv64to8bitsVector(uint8_t * b8, uint64_t * b64, uint64_t l64)
{
	uint64_t i;
	uint8_t j;
	for (i=0;i<l64;i++)
	{
		for(j=0;j<8;j++)
		{
			*b8 = (*b64)>>(8*(8-1-j));
			b8++;
		}
		b64++;
	}
}

/**
 * \brief transform an byte array into a uint64_t array
 * \param b8 pointer to input byte array
 * \param b64 pointer to output uint64_t array
 * \param l8 uint64_t array's length
 */
void cnv8to64bitsVector(uint64_t *b64, uint8_t *b8, uint64_t l8)
{
	uint64_t i;
	uint8_t j;
	for(i=0;i<(l8/8);i++)
	{
		for(j=0;j<8;j++)
		{
			*b64 = (*b64)|(((uint64_t)(*b8)<<(j*8)));
			b8++;
		}
		b64++;
	}
}

/**
 * \brief Print in hexadecimal format a uint64_t array
 * \param b64 pointer to the array to print
 * \param l64 uint64_t array's length
 */
void print64bitsVector(uint64_t *b64, uint64_t l64)
{
	uint64_t i;
	for(i=0;i<l64;i++)
	{
		printf("%08x%08x\n",(uint32_t)((*b64)>>32),(uint32_t)(*b64));
		b64++;
	}
	printf("\n");
}

/**
 * \brief Print in hexadecimal format a byte array
 * \param b8 Pointer to the byte array to print
 * \param l8 byte array length
 * \param nl number of byte to print
 */
void print8bitsVector(uint8_t *b8, uint64_t l8, uint8_t nl)
{
	uint64_t i;
	uint8_t j=0;
	for(i=0;i<l8;i++)
	{
		printf("%02x ",(uint32_t)swapBits(*b8));
		b8++;
		j++;
		if(j==nl)
		{
			j=0;
			printf("\n");
		}
	}
	printf("\n");
}

/**
 * \brief Print the bitstring S used by the Sponge
 * \param S Pointer to the bitstring, an array  of 25 uint64_t
 */
void printS(uint64_t *S)
{
	uint8_t i,x,y;
	x=0;
	y=0;
	for(i=0;i<25;i++)
	{
		printf("[%d,%d] = %08X%08X\n",x,y,(uint32_t)((S[i])>>32),(uint32_t)(S[i]));
		if((x+1)==5)
			y=(y+1)%5;
		x=(x+1)%5;
	}
	printf("\n");
}

/**
 * \brief Print status matrix A
 * Print the status matrix/vector in hexadecimal format
 * \param A is a pointer to the status matrix, an array of 25 positions of 64 bits
 */
void printA(uint64_t *A)
{
	uint64_t i;
	uint8_t j;
	for (i=0;i<25;i++)
	{
		for(j=0;j<8;j++)
		{
			printf("%02X ",(uint32_t)((A[i])>>(j*8)&0xFF));

		}
		if((i%2)==1)
		{
			printf("\n");
		}
	}
	printf("\n\n");
}

/**
 * \brief Print the hash
 * Print the hash in hexadecimal format
 * \param A pointer to Status Matrix A
 * \param hsize lnegth of the hash
 */
void printHash(uint64_t *A, uint64_t hsize)
{
	uint64_t i,j,k;
	uint64_t numULL = hsize/64;
	for (i=0;i<numULL;i++)
	{
		for(j=0;j<8;j++)
		{
			printf("%02x",(uint32_t)((A[i])>>(j*8)&0xFF));

		}
	}
	uint64_t remBits = hsize % 64;
	uint64_t numBytes = remBits/8ULL;
	for(k=0;k<numBytes;k++)
	{
		printf("%02x",(uint32_t)((A[i])>>(k*8)&0xFF));
	}
	remBits = remBits%8ULL;
	if(remBits)
		printf("%x",(uint32_t)((A[i])>>(k*8)&((1<<remBits)-1)));
	printf("\n\n");
}

/**
 * \brief Print the hash
 * Print the hash in hexadecimal format
 * \param A pointer to Status Matrix A
 * \param hsize lnegth of the hash
 */
void printHashBytes(uint8_t *A, uint16_t hsize)
{
	uint16_t i;
	uint16_t hs = hsize/8;
	printf("\n");
	for (i=0;i<hs;i++)
	{
		printf("%02x ",A[i]);
		if((i%16)==15)
			printf("\n");
	}
	printf("\n");
}
