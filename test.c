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


void runTestCompress(){
    // Test case values
    __uint16_t numMod_d = 16; // Example value for numMod_d, you can change it as needed
    __uint8_t d = 11; // Example value for d, you can change it as needed

    // Test rounding function
    struct rational numRounding = {numMod_d, 2001};
    __uint16_t roundedValue = rounding(numRounding);
    printf("Rounding result: %u\n", roundedValue);

    // Test compress function
    __uint16_t compressedValue = compress(numMod_d, d);

    // Test decompress function
    __uint16_t decompressedValue = decompress(compressedValue, d);

    printf("Result of Compress and Decompress for d=%u and numMod_d=%u\n", d, numMod_d);
    printf("Compress result: %u\n", compressedValue);
    printf("Decompress result: %u\n", decompressedValue);
    printf("\n");
}

void runTestsEncode(){
    // Create a test array of 256 elements
    __uint16_t F[256];
    for (int i = 0; i < 256; i++) {
        F[i] = i;  // Or any other test values you desire
    }

    // Call byteEncode with a specific value of d
    __uint16_t d = 8;  // Change this value as needed
    __uint8_t* byteArray = byteEncode(F, d);

    // Print the result (the array of bytes)
    printf("Result of byteEncode for d=%u:\n", d);
    for (int i = 0; i < 256 * d / 8; i++) {
        printf("%u ", byteArray[i]);
    }
    printf("\n");

    // Free allocated memory
    free(byteArray);
}

void runTestByteEncode(__uint8_t d){
    // Make a random test to byteDecode function
    // Generate 32*d random byte, then use byteDecode function to convert to integer mod q (or mod 2^d if d<12)

    srand(time(NULL)); // use current time as seed for random generator
    
    __uint16_t mod;
    if (d < 12) {
        mod = 1 << d;
    }else {
        mod = Q;
    }

    __uint16_t* intArray = generateRandomPoly(mod);
    printf("Random integers mod %d: \n", d);
    printPoly(intArray);
    
    // Test the function byteDecode
    __uint8_t* byteArray = byteEncode(intArray, d);
    printf("Result of byteEncode for d=%d:\n", d);
    printBytes(byteArray, d);

    free(intArray);
    free(byteArray);

}

void runTestByteDecode(__uint8_t d){
    // Make a random test to byteDecode function
    // Generate 32*d random byte, then use byteDecode function to convert to integer mod q (or mod 2^d if d<12)

    srand(time(NULL)); // use current time as seed for random generator

    __uint8_t* byteArray = generateRandomBytes(d);
    printf("Random bytes generated: \n");
    printBytes(byteArray, d);
    
    // Test the function byteDecode
    __uint16_t* intArray = byteDecode(byteArray, d);
    printf("Result of byteDecode for d=%d:\n", d);
    printPoly(intArray);

    free(intArray);
    free(byteArray);

}


void runTestEncodeDecode(__uint8_t d){
    // Testing functions byteEncode and byteDecode

    __uint16_t mod;
    if (d < 12) {
        mod = 1 << d;
    }else {
        mod = Q;
    }

    __uint16_t* intArray = generateRandomPoly(mod);
    __uint8_t* byteArray = byteEncode(intArray, d);
    __uint16_t* decodedArray = byteDecode(byteArray, d);

    printf("IntArray: \n");
    printPoly(intArray);

    printf("ByteArray: \n");
    printBytes(byteArray, d);

    printf("DecodedArray: \n");
    printPoly(decodedArray);

}

void runTestSamples(__uint8_t eta){ 
    // Testing functions sampleNTT and SamplePolyCBD

    __uint8_t* randomBytes = generateRandomBytes(2*eta);
    __uint16_t* polyNTT = sampleNTT(randomBytes);
    __uint16_t* poly = samplePolyCBD(randomBytes, eta);

    printf("Random bytes: \n");
    printBytes(randomBytes, 2*eta);

    printf("Poly NTT: \n");
    printPoly(polyNTT);

    printf("Poly CBD: \n");
    printPoly(poly);

    free(randomBytes);
    free(polyNTT);
    free(poly);
}

void runTest_NTT_inverseNTT(){

    __uint16_t* intArray = generateRandomPoly(Q);
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

    __uint16_t* f = generateRandomPoly(Q);
    sleep(1);
    __uint16_t* g = generateRandomPoly(Q);
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

__uint16_t* generateRandomPoly(__uint16_t mod){

    srand(time(NULL)); // use current time as seed for random generator
    
    // Generate 256 random integers mod q (o any number)
    __uint16_t* poly = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    for (int i = 0; i < 256; i++) {
        poly[i] = rand()/((RAND_MAX + 1u)/(mod));
    }
    return poly;
}
__uint8_t* generateRandomBytes(__uint8_t d){
    srand(time(NULL)); // use current time as seed for random generator

    // Generate 32*d random byte, then use byteDecode function to convert to integer mod q (or mod 2^d if d<12)
    __uint8_t* byteArray = (__uint8_t*)calloc(32*d, sizeof(__uint8_t));
    // Fit the byteArray with random values
    for (int i = 0; i < 32*d; i++) {
        byteArray[i] = rand()/((RAND_MAX + 1u)/256);
    }

    return byteArray;
}

void printPoly(__uint16_t* poly){
    for (int i = 0; i < 256; i++) {
        printf("%d ", poly[i]);
    }
    printf("\n");
}

void printBytes(__uint8_t* byteArray, __uint8_t d){
    for (int i = 0; i < 32*d; i++) {
        printf("%d ", byteArray[i]);
    }
    printf("\n");
}
