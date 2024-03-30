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
    for (int i = 0; i < 32 * d / 8; i++) {
        printf("%u ", byteArray[i]);
    }
    printf("\n");

    // Free allocated memory
    free(byteArray);
}

void runTestByteEncode(__uint8_t d){
    // Make a random test to byteDecode function
    // Generate 32*d random byte, then use byteDecode function to convert to integer mod q (or mod 2^d if d<12)

    
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

    __uint16_t* polyNTT = NTT(intArray);
    printf("PolyNTT: \n");
    printPoly(polyNTT);

    __uint16_t* polyF = inverseNTT(polyNTT);
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

void runTestSumPoly() {

    __uint16_t* poly1 = generateRandomPoly(Q);
    __uint16_t* poly2 = generateRandomPoly(Q);
    __uint16_t* sum = sumPoly(poly1, poly2);

    printf("Poly1: \n");
    printPoly(poly1);

    printf("Poly2: \n");
    printPoly(poly2);

    printf("Sum: \n");
    printPoly(sum);

    free(poly1);
    free(poly2);
    free(sum);
}

void runTestMatrixVectorOperations() {
    // Make a random test to multiplyMatrixByVector and sumVector functions

    __uint16_t** matrix = (__uint16_t **)calloc(K*K, sizeof(__uint16_t *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    __uint16_t** vector = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vector == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    printf("Matrix: \n");
    for (int i = 0; i < K; i++) {
        for(int j = 0; j < K; j++) {
            matrix[i*K + j] = generateRandomPoly(Q);
            printPoly(matrix[i*K + j]);
        }
        printf("\n");
    }
    printf("Vector: \n");
    for (int i = 0; i < K; i++) {
        vector[i] = generateRandomPoly(Q);
        printPoly(vector[i]);
    }

    __uint16_t** result = multiplyMatrixByVector(matrix, vector);
    printf("Result multiply MatrixByVector: \n");
    for (int i = 0; i < K; i++) {
        printPoly(result[i]);
    }

    __uint16_t** vectorSum = sumVector(vector, vector);
    printf("Vector sum: \n");
    for (int i = 0; i < K; i++) {
        printPoly(vectorSum[i]);
    }

    // Free each element of the matrix
    for (int i = 0; i < K*K; i++) {
        free(matrix[i]);
    }
    free(matrix);

    // Free each element of the vector
    for (int i = 0; i < K; i++) {
        free(vector[i]);
    }
    free(vector);

    // Free each element of the result
    for (int i = 0; i < K; i++) {
        free(result[i]);
    }
    free(result);

    // Free each element of the vectorSum
    for (int i = 0; i < K; i++) {
        free(vectorSum[i]);
    }
    free(vectorSum);

}

void runtTestConcatenateBytes(__uint8_t a, __uint8_t b) {

    __uint8_t* bytesA = generateRandomBytes(a);
    __uint8_t* bytesB = generateRandomBytes(b);

    printf("Bytes A: \n");
    printBytes(bytesA, a);

    printf("Bytes B: \n");
    printBytes(bytesB, b);

    printf("Concatenated A||B: \n");
    printBytes(concatenateBytes(bytesA, bytesB, 32*a, 32*b), a + b);

    free(bytesA);
    free(bytesB);
}

void runTestVector2Bytes() {
    // Make a random test to vector2Bytes function

    __uint8_t* ekPKE;
    __uint8_t* dkPKE;
    __uint16_t **vector1 = (__uint16_t**)calloc(K, sizeof(__uint16_t *));
    __uint16_t **vector2 = (__uint16_t**)calloc(K, sizeof(__uint16_t *));
    __uint8_t *rho;

    printf("Vector 1: \n");
    for (int i = 0; i < K; i++) {
        vector1[i] = generateRandomPoly(Q);
        printPoly(vector1[i]);
    }
    printf("\n");

    printf("Vector 2: \n");
    for (int i = 0; i < K; i++) {
        vector2[i] = generateRandomPoly(Q);
        printPoly(vector2[i]);
    }
    printf("\n");

    printf("Bytes rho: \n");
    rho = generateRandomBytes(1);
    printBytesHex(rho, 32);
    printf("\n");

    printf("Bytes ekPKE: \n");
    ekPKE = vector2Bytes(vector1, 384*K + 32);
    for (int i = 0; i < 32; i++){
        ekPKE[384*K + i] = rho[i];
    }
    printBytesHex(ekPKE, (384*K + 32));

    printf("Bytes dkPKE: \n");
    dkPKE = vector2Bytes(vector2, 384*K);
    printBytesHex(dkPKE, (384*K));

    // Free to each element 
    for (int i = 0; i < K; i++){
        free(vector2[i]);
    }
    free(vector2);

    for (int i = 0; i < K; i++){
        free(vector1[i]);
    }
    free(vector1);

    free(rho);
    free(ekPKE);
    free(dkPKE);
}

void runTestVectorDotProduct() {
    // Test the function vectorDotProduct

    printf("Vector 1: \n");
    __uint16_t** vector1 = (__uint16_t**)calloc(K, sizeof(__uint16_t*));
    for (int i = 0; i < K; i++) {
        vector1[i] = generateRandomPoly(Q);
        printPoly(vector1[i]);
    }
    printf("Vector 2: \n");
    __uint16_t** vector2 = (__uint16_t**)calloc(K, sizeof(__uint16_t*));
    for (int i = 0; i < K; i++) {
        vector2[i] = generateRandomPoly(Q);
        printPoly(vector2[i]);
    }

    printf("Vector 1 dot Vector 2: \n");
    __uint16_t* result = vectorDotProduct(vector1, vector2);
    printPoly(result);

    freeVector(vector1, K);
    freeVector(vector2, K);
    free(result);
}

void runTestML_KEM() {

    // Key generation
    struct Keys keysML_KEM = ML_KEM_KeyGen();

    printf("Encaps key: \n");
    printBytesHex(keysML_KEM.ek, 384*K + 32);

    printf("Decaps key: \n");
    printBytesHex(keysML_KEM.dk, 768*K + 96);

    // Encapsulation
    __uint8_t sharedKey_B[32];
    __uint8_t c[32*(D_u*K + D_v)];
    __uint8_t* K_c = ML_KEM_Encaps(keysML_KEM.ek);

    for (int i = 0; i < 32 + 32*(D_u*K + D_v); i++) {
        if (i < 32) {
            sharedKey_B[i] = K_c[i];
        } else {
            c[i - 32] = K_c[i];
        
        }
    }

    printf("Shared key B: \n");
    printBytesHex(sharedKey_B, 32);

    printf("Ciphertext: \n");
    printBytesHex(c, 32*(D_u*K + D_v));

    // Decapsulation
    __uint8_t* sharedKey_A = ML_KEM_Decaps(c, keysML_KEM.dk);

    printf("Shared key A: \n");
    printBytesHex(sharedKey_A, 32);

    // Validation
    if (memcmp(sharedKey_A, sharedKey_B, 32) == 0) {
        printf("Shared keys are equal!!\n");
    } else {
        printf("Shared keys are different!!\n");
    }

    // Free memory
    free(K_c);
    free(sharedKey_A);
    free(keysML_KEM.ek);
    free(keysML_KEM.dk);
}

void runTest_XOF_PRF()
{
    // Test the functions XOF and PRF

    __uint8_t* rho = generateRandomBytes(1);
    __uint8_t* r = generateRandomBytes(1);
    __uint8_t j = 1, i = 2, n = 3;

    __uint8_t* outXOF = XOF(rho, i, j);
    __uint8_t* outPRF = PRF(r, n, ETA_2);

    printf("Rho: \n");
    printBytesHex(rho, 32);

    printf("R: \n");
    printBytesHex(r, 32);

    printf("XOF: \n");
    printBytesHex(outXOF, 96);

    printf("PRF: \n");
    printBytesHex(outPRF, 64*ETA_2);

    // Free memory
    free(rho);
    free(r);
    free(outXOF);
    free(outPRF);
}

__uint16_t* generateRandomPoly(__uint16_t mod){
    
    // Generate 256 random integers mod q (o any number)
    __uint16_t* poly = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    for (int i = 0; i < 256; i++) {
        poly[i] = rand()/((RAND_MAX + 1u)/(mod));
    }
    return poly;
}

void runTestPKE_KeyGen() {

    // Testing PKE_KeyGen
    struct Keys keysPKE = PKE_KeyGen();

    printf("ekPKE: \n");
    printBytesHex(keysPKE.ek, 384*K + 32);

    printf("dkPKE: \n");
    printBytesHex(keysPKE.dk, 384*K);

    // Free memory
    free(keysPKE.ek);
    free(keysPKE.dk);
}

void runTestPKE_Encrypt() {

    // Testing PKE_Encrypt
    __uint8_t* ekPKE = generateRandomBytes(12*K + 1);
    __uint8_t* m = generateRandomBytes(1);
    __uint8_t* r = generateRandomBytes(1);

    __uint8_t* c = PKE_Encrypt(ekPKE, m, r);

    printf("ekPKE: \n");
    printBytesHex(ekPKE, 384*K + 32);

    printf("c: \n");
    printBytesHex(c, 32*(D_u*K + D_v));

    // Free memory
    free(ekPKE);
    free(r);
    free(c);
    free(m);
}

void runTestPKE_Decrypt() {

    // Testing PKE_Decrypt
    __uint8_t* dkPKE = generateRandomBytes(12*K);
    __uint8_t* cipherText = generateRandomBytes(D_u*K + D_v);
    __uint8_t* m = PKE_Decrypt(dkPKE, cipherText);

    // Printing
    printf("dkPKE: \n");
    printBytesHex(dkPKE, 384*K);

    printf("cipherText: \n");
    printBytesHex(cipherText, 32*(D_u*K + D_v));

    printf("m: \n");
    printBytesHex(m, 32);

    // free memory
    free(dkPKE);
    free(cipherText);
    free(m);
}

void runTestPKE() {
    // Testing the PKE scheme

    // Key generation
    struct Keys keysPKE;
    keysPKE = PKE_KeyGen();

    // Encryption
    __uint8_t* m = generateRandomBytes(1);
    __uint8_t Hek[32];
    __uint8_t r[32];
    __uint8_t k[32];
    __uint8_t K_r[64];

    SHA3_256(keysPKE.ek, 384*K + 32, Hek); // H(ek)
    __uint8_t* concat_m_Hek = concatenateBytes(m, Hek, 32, 32);
    SHA3_512(concat_m_Hek, 64, K_r); // (K,r) ← G(m∥H(ek))

    // Split K_r into k and r
    for (int i = 0; i < 32; i++) {
        k[i] = K_r[i];
        r[i] = K_r[i + 32];
    }

    __uint8_t* c = PKE_Encrypt(keysPKE.ek, m, r);

    // Decryption
    __uint8_t* mDec = PKE_Decrypt(keysPKE.dk, c);

    // Printing
    printf("ekPKE: \n");
    printBytesHex(keysPKE.ek, 384*K + 32);

    printf("dkPKE: \n");
    printBytesHex(keysPKE.dk, 384*K);

    printf("m: \n");
    printBytesHex(m, 32);

    printf("mDec: \n");
    printBytesHex(mDec, 32);

    // Free memory
    free(keysPKE.ek);
    free(keysPKE.dk);
    free(m);
    free(c);
    free(mDec);

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

void printBytesHex(__uint8_t *byteArray, __uint16_t numBytes) {
    for (int i = 0; i < numBytes; i++) {
        printf("%02x", byteArray[i]);
    }
    printf("\n");
}

void printMatrixVector(__uint16_t **matrixVector, __uint8_t k)
{
    for (int i = 0; i < k; i++) {
        if(i%K == 0){
            printf("\n");
        }
        printPoly(matrixVector[i]);
    }
}
