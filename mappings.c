/*
 * mappings.c
 *
 *  Created on: Sep 4, 2023
 *      Author: ravel
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mappings.h"
#include "utilities.h"

uint64_t RC[24] = { 1ULL,32898ULL,9223372036854808714ULL,9223372039002292224ULL,32907ULL,2147483649ULL,9223372039002292353ULL,9223372036854808585ULL,
		            138ULL,136ULL,2147516425ULL,2147483658ULL,2147516555ULL,9223372036854775947ULL,9223372036854808713ULL,9223372036854808579ULL,
		            9223372036854808578ULL,9223372036854775936ULL,32778ULL,9223372039002259466ULL,9223372039002292353ULL,9223372036854808704ULL,2147483649ULL,
		            9223372039002292232ULL};


void theta(uint64_t * A, uint64_t * Ap){
	uint64_t C[5];
	uint64_t D[5];

	/// Step 1 - C is a plane product of computing the column parities of the
	/// five planes of A
	C[0] = A[0]^A[5]^A[10]^A[15]^A[20];
	C[1] = A[1]^A[6]^A[11]^A[16]^A[21];
	C[2] = A[2]^A[7]^A[12]^A[17]^A[22];
	C[3] = A[3]^A[8]^A[13]^A[18]^A[23];
	C[4] = A[4]^A[9]^A[14]^A[19]^A[24];

	/// Step 2 - D is also a plane. Each line of D is the product of xoring the the left and right neighbors of the corresponding C lane but
	/// rotating right lane neighbor to the left 1 position
	D[0] = C[4]^rotLeft(C[1],1);
	D[1] = C[0]^rotLeft(C[2],1);
	D[2] = C[1]^rotLeft(C[3],1);
	D[3] = C[2]^rotLeft(C[4],1);
	D[4] = C[3]^rotLeft(C[0],1);

	/// Step 3 - Xor each plane of State Array A with the D plane
	for (int x=0; x<5; x++)
	{
		for (int y=0; y<5; y++)
		{
			//Ap[5*((y+2)%5)+((x+2)%5)] = A[5*((y+2)%5)+((x+2)%5)] ^ D[(x+2)%5];
			Ap[5*y+x] = A[5*y+x] ^ D[x];
		}
	}
}

void rho(uint64_t * A, uint64_t * Ap){
	uint8_t x,y,v;
	// Step 1 consist on making the first Ap[0][0] position equal to the corresponding A[0][0] position
	Ap[0]=A[0];

	//Step 2 initialize the iterators x and y to 1 and 0 respectively
	x = 1;
	y = 0;

	// Step 3
	for(uint8_t t=0;t<=23;t++)
	{
		Ap[5*y + x] = rotLeft(A[5*y + x],((t+1)*(t+2)/2)%64);
		v = (2*x + 3*y)%5;
		x = y;
		y = v;
	}
}

void pi(uint64_t * A, uint64_t * Ap){
	uint8_t x,y,v;

	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			v = (x + 3*y)%5;
			Ap[5*y+x] = A[5*x+v];
		}
	}
}

void chi(uint64_t * A, uint64_t * Ap){
	uint8_t x,y;
	for(x=0;x<5;x++)
	{
		for(y=0;y<5;y++)
		{
			Ap[5*y+x] = A[5*y+x] ^ ((~A[5*y+((x+1)%5)]) & (A[5*y+((x+2)%5)]));
		}
	}
}

void iota(uint64_t * A, uint64_t * Ap, uint8_t nr){
	uint8_t x,y;
	for(x=0;x<5;x++){
		for(y=0;y<5;y++)
		{
			Ap[5*y+x] = A[5*y+x];
		}
	}
	//printf("%08X%08X, %08X%08X\n",(uint32_t)((Ap[0])>>32),(uint32_t)(Ap[0]),(uint32_t)((RC[nr])>>32),(uint32_t)(RC[nr]));
	Ap[0] = Ap[0] ^ RC[nr];
}

void rnd(uint64_t * A, uint64_t * Ap, uint8_t nr){
	for (uint8_t ir=0;ir<nr;ir++){
#ifdef VERBOSE
		printf("Round %d\n",ir);
#endif
		theta(A,Ap);
#ifdef VERBOSE
		printf("After Theta\n");
		printA(Ap);
#endif
		rho(Ap,A);
#ifdef VERBOSE
		printf("After Rho\n");
		printA(A);
#endif
		pi(A,Ap);
#ifdef VERBOSE
		printf("After Pi\n");
		printA(Ap);
#endif
		chi(Ap,A);
#ifdef VERBOSE
		printf("After Chi\n");
		printA(A);
#endif
		iota(A,Ap,ir);
#ifdef VERBOSE
		printf("After Iota\n");
		printA(Ap);
#endif
		if(ir<nr-1)
			for(uint8_t j=0;j<25;j++)
				A[j] = Ap[j];
	}
}

void rcConstants(void)
{
	uint8_t nr = 24;
	uint8_t l = (nr-12)/2;
	uint64_t vRC[24] = {0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL,0ULL};

	for (uint8_t ir=0;ir<nr;ir++)
	{
		uint64_t RC = 0ULL;
		for (uint8_t j=0;j<=l;j++)
		{
			uint16_t t = j+7*ir;
			uint8_t R = 128;
			if ((t%255)==0)
			{
				R=1;
			}
			else
			{
				for (uint8_t i=1; i<=(t%255);i++)
				{
					uint8_t R8 = R & 0x01;
					R = R >> 1;
					R = R ^ (R8 << 7);
					R = R ^ (R8 << 3);
					R = R ^ (R8 << 2);
					R = R ^ (R8 << 1);
				}
				R = R >> 7;
			}
			uint64_t k = (1<<j) - 1;
			uint64_t temp = ((uint64_t)R) << k;
			RC = RC | temp;
		}
		vRC[ir] = RC;
	}

	printf("{ ");
	for (uint8_t i=0;i<nr;i++)
	{
		printf("%lluULL,",(long long unsigned int)vRC[i]);
		if (((i+1)%8)==0)
			printf("\n");
	}
	printf(" }");
}

void SHA3_d(uint8_t * M, uint8_t * H, uint16_t d, uint16_t sM){
	uint8_t i,j=0,k,l;
	uint16_t n=0;
	uint8_t d1;
	uint64_t d2=0;
	uint64_t A[25]={0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull};
	uint64_t S[25]={0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull};

	uint8_t r = (1600-2*d)/64;

	static uint64_t cnt=0;

	// Arranca etapa de Absorción
	while(n<sM){
		for(i=0;i<25;i++){ // Llenar los 1600 bits de una ronda
			if(i < r){ // Con datos en los primeros r bloques de 64 bits
				d2 = 0;
				for(j=0;j<8;j++){//Iterar sobre el arreglo de datos para completar los bloques de 64 bits
					if(n >= sM)
						break;
					d1 = M[n++];
					d2 = d2 | ((uint64_t)d1<<(j*8));
				}
				if (j<8)
					break;
				A[i] = d2;
			}
			else // Con ceros los restantes bloques de64 bits
				A[i]=0ULL;
		}
		if(n < sM){ // Si los 1600 bits se completaron
			cnt++;
#ifdef VERBOSE
			printf("Data to be Absorbed\n");
			printA(A);
			printf("State (as lanes of integers\n)");
			printS(S);
			printf("\nBlock %llu\n",cnt);
#endif
			for(uint8_t m=0;m<25;m++)					{
				A[m] = S[m]^A[m];
			}
			rnd(A,S,24);
#ifdef VERBOSE
			printf("After Permutation\n");
			printA(A);
			printf("State (as lanes of integers\n)");
			printS(S);
#endif
		}
		else{
			if ((i==r-1)&&(j==7)){ // Si falta un byte para completar el bloque r concatenar el byte "10000110"
					A[i] = d2 |(0x86ULL << 56);
			}
			else // Si falta mas de un byte para completar los r bloques
			{
				d2 = d2 | (0x06ULL << (j*8)); // Concatenar subfijo "0110" e iniciar padding
				j++;
				for(k=i;k<r;k++)
				{
					for(l=j;l<8;l++)
					{
						if((k==r-1)&&(l==7))
						{
							d2 = d2 | (0x80ULL << 56); // Completar padding
						}
					}
					A[k]=d2;
					j=0;
					d2=0;
				}
			}
			for(k=r;k<25;k++) // completar con ceros
			{
				A[k] = 0ULL;
			}
			cnt++;
		}
	}
#ifdef VERBOSE
	printf("\nBlock %llu\n",cnt);
	printf("After Permutation\n");
	printA(A);
	printf("State (as lanes of integers\n)");
	printS(S);
#endif
	for(uint8_t m=0;m<25;m++)
	{
		A[m] = S[m]^A[m];
	}
#ifdef VERBOSE
	printf("Data to be Absorbed\n");
	printA(A);
	printf("State (as lanes of integers\n)");
	printS(S);
#endif
	rnd(A,S,24);
#ifdef VERBOSE
	printf("After Permutation\n");
	printA(S);
#endif

	//Arranca etapa de squeezing
	for(i=0;i<(d/64);i++){
		for(j=0;j<8;j++){
			H[i*8+j] = (S[i] >> (j*8)) & 0x00FFULL;
		}
	}
	return;
}



void XOF_d(uint8_t * M, uint8_t * H, uint16_t d, uint16_t h, uint16_t sM){
	uint8_t i,j=0,k,l;
	uint16_t n=0;
	uint8_t d1;
	uint64_t d2=0;
	uint64_t A[25]={0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull};
	uint64_t S[25]={0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull,0ull};

	uint8_t r = (1600-2*d)/64;

	static uint64_t cnt=0;

	// Arranca etapa de Absorción
	while(n<sM){
		for(i=0;i<25;i++){ // Llenar los 1600 bits de una ronda
			if(i < r){ // Con datos en los primeros r bloques de 64 bits
				d2 = 0;
				for(j=0;j<8;j++){//Iterar sobre el arreglo de datos para completar los bloques de 64 bits
					if(n >= sM)
						break;
					d1 = M[n++];
					d2 = d2 | ((uint64_t)d1<<(j*8));
				}
				if (j<8)
					break;
				A[i] = d2;
			}
			else // Con ceros los restantes bloques de64 bits
				A[i]=0ULL;
		}
		if(n < sM){ // Si los 1600 bits se completaron
			cnt++;
	#ifdef VERBOSE
			printf("Data to be Absorbed\n");
			printA(A);
			printf("State (as lanes of integers\n)");
			printS(S);
			printf("\nBlock %llu\n",cnt);
	#endif
			for(uint8_t m=0;m<25;m++)					{
				A[m] = S[m]^A[m];
			}
			rnd(A,S,24);
#ifdef VERBOSE
			printf("After Permutation\n");
			printA(A);
			printf("State (as lanes of integers\n)");
			printS(S);
#endif
		}
		else{
			if ((i==r-1)&&(j==7)){ // Si falta un byte para completar el bloque r concatenar el byte "10011111"
					A[i] = d2 |(0x9FULL << 56);
			}
			else // Si falta mas de un byte para completar los r bloques
			{
				d2 = d2 | (0x1FULL << (j*8)); // Concatenar subfijo "1111" e iniciar padding
				j++;
				for(k=i;k<r;k++)
				{
					for(l=j;l<8;l++)
					{
						if((k==r-1)&&(l==7))
						{
							d2 = d2 | (0x80ULL << 56); // Completar padding
						}
					}
					A[k]=d2;
					j=0;
					d2=0;
				}
			}
			for(k=r;k<25;k++) // completar con ceros
			{
				A[k] = 0ULL;
			}
			cnt++;
		}
	}
#ifdef VERBOSE
	printf("\nBlock %llu\n",cnt);
	printf("After Permutation\n");
	printA(A);
	printf("State (as lanes of integers\n)");
	printS(S);
#endif
	for(uint8_t m=0;m<25;m++)
	{
		A[m] = S[m]^A[m];
	}
#ifdef VERBOSE
	printf("Data to be Absorbed\n");
	printA(A);
	printf("State (as lanes of integers\n)");
	printS(S);
#endif
	rnd(A,S,24);
#ifdef VERBOSE
	printf("After Permutation\n");
	printA(S);
#endif

	//Arranca etapa de squeezing

#ifdef VERBOSE
	printf("¡¡¡ SQUEEZING !!!\n\n");
#endif

	if(h <= (r*64))
	{
#ifdef VERBOSE
		printHash(S,h);
#endif
		for(i=0;i<(h/64);i++){
			for(j=0;j<8;j++){
				H[i*8+j] = (S[i] >> (j*8)) & 0x00FFULL;
			}
		}
	}
	else
	{
		uint64_t s = (h/(r*64))+1;
		uint64_t *Z = malloc(s*r*64);
		for(i=0;i<s;i++)
		{
#ifdef VERBOSE
			printf("Hash Block %d\n",i);
#endif
			for(j=0;j<r;j++)
			{
				Z[i*r+j] = S[j];
			}
			rnd(S,A,24);
			for(j=0;j<25;j++)
				S[j] = A[j];
		}
#ifdef VERBOSE
		printHash(Z,h);
#endif
		for(i=0;i<(h/64);i++){
			for(j=0;j<8;j++){
				H[i*8+j] = (Z[i] >> (j*8)) & 0x00FFULL;
			}
		}
		free(Z);
	}
	return;
}

void SHA3_256(uint8_t * IN, uint16_t sin, uint8_t * OUT){
	SHA3_d(IN, OUT, 256, sin);
}

void SHA3_512(uint8_t * IN, uint16_t sin, uint8_t * OUT){
	SHA3_d(IN, OUT, 512, sin);
}

void SHAKE_128(uint8_t * IN, uint16_t sin, uint8_t * OUT, uint16_t sout){
	XOF_d(IN,OUT,128,sout*8,sin);
}

void SHAKE_256(uint8_t * IN, uint16_t sin, uint8_t * OUT, uint16_t sout){
	XOF_d(IN,OUT,256,sout*8,sin);
}
