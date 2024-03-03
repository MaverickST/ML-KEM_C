#include "test.h"

void printBitArray(__uint32_t* bitArray, __uint16_t numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        printf("%d", (bitArray[i / 32] >> (i % 32)) & 0x01);
        if (i  % 32 == 0 && i > 0) {
            printf(" - ");
        }
    }
    printf("\n");
}

void printByteArray(__uint8_t* byteArray, __uint16_t numBytes) {
    for (int i = numBytes - 1; i >= 0; i--) {
        printf("%d", byteArray[i]);
        if (i > 0) {
            printf(" - ");
        }
    }
    printf("\n");
}

void runTestsConvertions(){

    // Declare variables
    __uint32_t bitArray[8]; // Array of 256 bits
    __uint16_t numBits = 256; // Number of bits in the array

     // Fill the bitArray with random bit values (0 or 1)
    for (int i = 0; i < 8; i++) {
        bitArray[i] = rand(); // Randomly generate
    }

    // Convert the array of bits into an array of bytes
    __uint8_t* byteArray = bitsToBytes(bitArray, numBits);

    // Convert the bytes back into bits
    __uint32_t* reconstructedBitArray = bytesToBits(byteArray, numBits / 8);

    // Display the resulting bits
    printf("Original bits: ");
    printBitArray(bitArray, numBits);
    printf("\n");
    printf("Bits to bytes: ");
    printByteArray(byteArray, numBits / 8);
    printf("\n");
    printf("Bytes to bits: ");
    printBitArray(reconstructedBitArray, numBits);
    printf("\n");

    // Free allocated memory
    free(byteArray);
    free(reconstructedBitArray);
}

void runTestByteDecode(__uint8_t d){
    // Make a random test to byteDecode function
    // Generate 32*d random byte, then use byteDecode function to convert to integer mod q (or mod 2^d if d<12)

    srand(time(NULL)); // use current time as seed for random generator

    __uint8_t* byteArray = (__uint8_t*)calloc(32*d, sizeof(__uint8_t));
    // Fit the byteArray with random values
    printf("Random bytes: \n");
    for (int i = 0; i < 32*d; i++) {
        byteArray[i] = rand()/((RAND_MAX + 1u)/256);
        printf("%d ", byteArray[i]);
    }
    printf("\n");
    
    // Test the function byteDecode
    __uint16_t* intArray = byteDecode(byteArray, d);
    printPoly(intArray);

    free(intArray);
    free(byteArray);

}

void runTest_NTT_inverseNTT(){

    __uint16_t* intArray = generateRandomPoly();
    printf("Int array: \n");
    printPoly(intArray);

    __uint16_t* polyNTT = polyF2polyNTT(intArray);
    printf("PolyNTT: \n");
    printPoly(polyNTT);

    __uint16_t* polyF = polyNTT2polyF(polyNTT);
    printf("PolyF: \n");
    printPoly(polyF);


    free(intArray);
    free(polyNTT);
    free(polyF);
}
void runTestMultiplyNTT(){
    // Make a random test to multiplyNTT function
    // Generate 2 random polynomials (f, g), and use multiplyNTT function to multiply f and g

    __uint16_t* f = generateRandomPoly();
    sleep(1);
    __uint16_t* g = generateRandomPoly();
    __uint16_t* product = multiplyNTT(f, g);

    printf("Array f:\n");
    printPoly(f);

    printf("Array g:\n");
    printPoly(g);

    printf("Product: \n");
    printPoly(product);

    free(f);
    free(g);
    free(product);
}

__uint16_t* generateRandomPoly(){

    srand(time(NULL)); // use current time as seed for random generator
    
    // Generate 256 random integers mod q
    __uint16_t* poly = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    for (int i = 0; i < 256; i++) {
        poly[i] = rand()/((RAND_MAX + 1u)/(Q));
    }
    return poly;
}

void printPoly(__uint16_t* poly){
    for (int i = 0; i < 256; i++) {
        printf("%d ", poly[i]);
    }
    printf("\n");
}