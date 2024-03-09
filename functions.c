#include "functions.h"
#include "test.h"
#include "arrays.h"

__uint8_t bitRev7(__uint8_t i){

    printf("Zeta [%d]: %d\n", i, zetaArray[i]);
    printf("Gamma [%d]: %d\n", i, gammaArray[i]);

    // Reverse input 7-bits
    __uint8_t res = 0;
    for(int j = 6; j >= 0; j--){
        res |= (i & 1) << j;
        i >>= 1;
    }

    return res;
}

__uint8_t* bitsToBytes(__uint32_t* bitArray, __uint16_t numBits) {
    //Converts a bit array (of length a multiple of eight) into an array of bytes.
  
    // Calculates the number of bytes required
    __uint32_t numBytes = numBits/8;

    // Create an array of bytes
    __uint8_t* bytesArray = (__uint8_t*)calloc(numBytes, sizeof(__uint8_t));
    if (bytesArray == NULL) {
        fprintf(stderr, "Memory allocation error - bitsToBytes\n");
        return NULL;
    }

    // Iterate over the byteArray
    for (__uint32_t i = 0; i < numBytes; i++) {
        // Set the bit in the corresponding byte (little-endian format)
        bytesArray[i] = (bitArray[i / 4] >> ((i % 4) * 8)) & 0xFF;
    }

    // // Iterate over the bit array
    // for (__uint32_t i = 0; i < numBits; i++) {
    //     // Calculate the index of the corresponding element in bitArray
    //     __uint32_t bitIndex = i / 32;
    //     // Calculate the bit position within the element of bitArray
    //     __uint32_t bitPosition = i % 32;
    //     // Set the bit in the corresponding byte (little-endian format)
    //     bytesArray[i / 8] |= (bitArray[bitIndex] >> bitPosition) & 0x01;
    // }

    return bytesArray;
}

__uint32_t* bytesToBits(__uint8_t* bytesArray, __uint16_t numBytes) {
    //Converts an array of bytes into aarray of bits (of length a multiple of eight).
    
    // Create an array of bits
    int numBitsArray = numBytes / 4;
    __uint32_t* bitArray = (__uint32_t*)calloc(numBitsArray, sizeof(__uint32_t));
    if (bitArray == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    // Iterate over the bitArray
    for (__uint32_t i = 0; i < numBytes; i++) {

        bitArray[i/4] |= ((__uint32_t)bytesArray[i] & 0xFF) << ((i % 4)*8);
    }

    return bitArray;
}

__uint16_t rounding(struct rational num) {
    //Round a rational number to the nearest integer
    __uint16_t numReduce = 0;
    __uint16_t remainder = num.numerator % num.denominator;

    // Integer division to get the integer part of the rational number
    numReduce = num.numerator / num.denominator;

    // Check if the fractional part is greater than or equal to 0.5
    if (((remainder * 10) / num.denominator) >= 5) {
        numReduce += 1; // Round up if the fractional part is >= 0.5
    }

    return numReduce;
}

__uint16_t compress(__uint16_t numMod_d, __uint8_t d) {
    // Compress a number mod q to a number mod 2^d
    // x ------>  approx(2^d/q)*x

    struct rational numCompress;

    // Create a rational number with numerator as (2^d * numMod_d) and denominator as 1
    numCompress.numerator = (1 << d) * numMod_d;
    numCompress.denominator = Q; // Denominator is 1 because we're dealing with integers

    // Call rounding function to round the rational number to the nearest integer
    return rounding(numCompress);
}


__uint16_t decompress(__uint16_t numMod_2d, __uint8_t d) {
    //decompress a number modulo 2^d to a number modulo q
    // x ------> approx(q/2^d)*x

    struct rational numDecompress;

    // Create a rational number with numerator as numMod_2d and denominator as 2^d
    numDecompress.numerator = Q*numMod_2d;
    numDecompress.denominator = 1 << d;

    // Call rounding function to round the rational number to the nearest integer
    return rounding(numDecompress);
}

__uint8_t* byteEncode(__uint16_t* F, __uint8_t d) {
    // ByteEncoded serializes an array of d-bit integers into an array of 32d bytes.
    //Encodes an array of d-bit integers into a byte array of 32d, for 1 ≤ d ≤ 12

    // Create an array of bits
    __uint16_t numBits = 256*d;

    // Create an array of bits
    __uint32_t numBitsArray = numBits / 32;
    __uint32_t* bitArray = (__uint32_t*)calloc(numBitsArray, sizeof(__uint32_t));
    if (bitArray == NULL) {
        fprintf(stderr, "Memory allocation error - byteEncode\n");
        return NULL;
    }

    for (__uint16_t i = 0; i <= 256; i++) {
        __uint16_t a = F[i];
        for (__uint16_t j = 0; j < d; j++) {
            bitArray[(i*d + j)/32] |= (a%2)  << ((i*d + j)%32);
            a = subModq(a , ((bitArray[(i*d + j)/32] >> ((i*d + j)%32)) & 0x01) ) / 2;
        }
    }

    __uint8_t* byteArray = bitsToBytes(bitArray, numBits);

    // Free the allocated memory
    free(bitArray);

    return byteArray;
}

__uint16_t* byteDecode(__uint8_t* byteArray, __uint8_t d){
    // Decodes an array of 32d bytes into an array of 256 d-bit integers mod q (if d<12, else mod 2^d).

    __uint32_t* bitArray = bytesToBits(byteArray, 32*d);

    // Create an array of integers
    __uint16_t* intArrayF = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (intArrayF == NULL) {
        fprintf(stderr, "Memory allocation error - byteDncode\n");
        return NULL;
    }
    // Iterate over the bytes
    for(int i = 0; i < 256; i++) {
        for (int j = 0; j < d; j++) {
            intArrayF[i] += ((bitArray[(i * d + j)>>5] >> ((i * d + j)%32))&1)<<j;// sum up the bits
            //               | index a 32-bit segment |    |    get the bit   | 
        }
        if (d < 12){
            intArrayF[i] = intArrayF[i]%(1 << d);// truncate to d-bits (mod 2^d)
        }else {
            intArrayF[i] = intArrayF[i]%(Q);// truncate to 3329 bits (mod q)
        }
        
    }

    // Free the allocated memory
    free(bitArray);

    return intArrayF;
}

__uint16_t* sampleNTT(__uint8_t* byteArray){
    // If the input is a stream of uniformly random bytes, the output is a uniformly random element of Tq.

    // Reserves memory for an array of 256 integers mod q (
    __uint16_t* aNTT = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (aNTT == NULL) {
        fprintf(stderr, "Memory allocation error - sampleNTT\n");
        return NULL;
    }

    int i = 0, j = 0, d1, d2;
    while(j < 256){
        d1 = byteArray[i] + 256*(byteArray[i + 1]%16);
        d2 = byteArray[i + 1]>>4 + 16*byteArray[i + 2];
        if (d1 < Q) {
            aNTT[j] = d1;
            j++;
        }
        if (d2 < Q && j < 256) {
            aNTT[j] = d2;
            j++;
        }
        i += 3;
    }
    return aNTT;
}

__uint16_t* samplePolyCBD(__uint8_t* byteArray, __uint8_t eta){
    // If the input is a stream of uniformly random bytes, outputs a sample from the distribution Dη(Rq).

    __uint32_t* bitArray = bytesToBits(byteArray, 64*eta);
    __uint16_t* polyCBD = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (polyCBD == NULL) {
        fprintf(stderr, "Memory allocation error - samplePolyCBD\n");
        return NULL;
    }

    __uint16_t x, y;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < eta; j++) {
            x += (bitArray[(2*i*eta + j)>>5] >> ((2*i*eta + j)%32))&1;
        }
        for (int j = 0; j < eta; j++) {
            y += (bitArray[(2*i*eta + eta + j )>>5] >> ((2*i*eta + eta + j)%32))&1;
        }
        polyCBD[i] = subModq(x%Q, y%Q);
    }
    free(bitArray);

    return polyCBD;
}
__uint16_t* polyF2polyNTT(__uint16_t* polyF){
    // Computes the NTT representation f_ˆ of the given polynomial f ∈ Rq.

    __uint16_t* polyNTT = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (polyNTT == NULL) {
        fprintf(stderr, "Memory allocation error - polyF2polyNTT\n");
        return NULL;
    }
    // Copy each element of polyF into polyNTT
    for (int i = 0; i < 256; i++) {
        polyNTT[i] = polyF[i];
    }
    
    __uint16_t zeta, t;
    int k = 1;
    for (int len = 128; len >= 2; len = len/2){
        for (int start = 0; start < 256; start = start + 2*len){
            zeta = zetaArray[k];
            k += 1;
            for (int j = start; j < start + len; j++){
                t = mulModq(zeta, polyNTT[j + len]);
                polyNTT[j + len] = subModq(polyNTT[j], t);
                polyNTT[j] = addModq(polyNTT[j], t);
            }
        }
    }
    
    return polyNTT;
}
__uint16_t* polyNTT2polyF(__uint16_t* polyNTT) {
    // Computes the polynomial f ∈ Rq corresponding to the given NTT representation f_ˆ ∈ Tq.

    __uint16_t* polyF = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (polyF == NULL) {
        fprintf(stderr, "Memory allocation error - polyNTT2polyF\n");
        return NULL;
    }
    // Copy each element of polyNTT into polyF
    for (int i = 0; i < 256; i++) {
        polyF[i] = polyNTT[i];
    }

    __uint16_t zeta, t;
    int k = 127;
    for (int len = 2; len <= 128; len = 2*len){
        for (int start = 0; start < 256; start = start + 2*len){
            zeta = zetaArray[k];
            k -= 1;
            for (int j = start; j < start + len; j++){
                t = polyF[j];
                polyF[j] = addModq(t, polyF[j + len]);
                polyF[j + len] = mulModq(zeta, subModq(polyF[j + len], t));
            }
        }
    }

    for (int i = 0; i < 256; i++){
        polyF[i] = mulModq(polyF[i], 3303);
    }

    return polyF;
}

__uint16_t* multiplyNTT(__uint16_t* polyNTT1, __uint16_t* polyNTT2){

    __uint16_t* product = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation error - multiplyNTT\n");
        return NULL;
    }
    // Multiply each element of polyNTT1 with each element of polyNTT2
    for (int i = 0; i < 128; i++) {
        product[2*i] = baseCaseMultiplyC0(polyNTT1[2*i], polyNTT1[2*i + 1], polyNTT2[2*i], polyNTT2[2*i + 1], gammaArray[i]);
        product[2*i + 1] = baseCaseMultiplyC1(polyNTT1[2*i], polyNTT1[2*i + 1], polyNTT2[2*i], polyNTT2[2*i + 1]);
    }

    return product;
}

__uint16_t baseCaseMultiplyC0(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1, __uint16_t gamma){

    __uint16_t c0;
    c0 = addModq( mulModq(a0, b0), mulModq(mulModq(a1, b1), gamma));

    return c0;
}
__uint16_t baseCaseMultiplyC1(__uint16_t a0, __uint16_t a1, __uint16_t b0, __uint16_t b1){

    __uint16_t c1;
    c1 = addModq( mulModq(a0, b0), mulModq(a1, b1));

    return c1;
}

void PKE_KeyGen(__uint8_t* ekPKE, __uint8_t* dkPKE) {
    // Generates an encryption key and a corresponding decryption key
    __uint8_t* d = generateRandomBytes(1);

    __uint8_t* rho, sigma;
    __uint8_t n = 0;

    // Generates a KxK matrix of polynomials (in NTT domain) mod q
    __uint16_t** matrixA = (__uint16_t **)calloc(K*K, sizeof(__uint16_t *));
    if (matrixA == NULL) {
        fprintf(stderr, "Memory allocation error (Matrix) - PKE_KeyGen\n");
        return;
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            matrixA[i*K + j] = sampleNTT(d); // d -> XOF(ρ,i,j)
        }
    }

    // Generates a Kx1 vector (s vector) of polynomials mod q
    __uint16_t** vectorS = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorS == NULL) {
        fprintf(stderr, "Memory allocation error (Vector s) - PKE_KeyGen\n");
        return;
    }
    for (int i = 0; i < K; i++) {
        vectorS[i] = samplePolyCBD(d, ETA_1); // d -> PRFη1 (σ,N)
        n++;
    }

    // Generates a Kx1 vector (e vector) of polynomials mod q
    __uint16_t** vectorE = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorE == NULL) {
        fprintf(stderr, "Memory allocation error (Vector e) - PKE_KeyGen\n");
        return;
    }
    for (int i = 0; i < K; i++) {
        vectorE[i] = samplePolyCBD(d, ETA_2); // d -> PRFη1 (σ,N)
        n++;
    }

    // Calculations and transformations

    // s vector in NTT domain
    __uint16_t** vectorS_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorS_NTT == NULL) {
        fprintf(stderr, "Memory allocation error (Vector s) - PKE_KeyGen\n");
        return;
    }
    for (int i = 0; i < K; i++) {
        vectorS_NTT[i] = polyF2polyNTT(vectorS[i]);
    }
    // e vector in NTT domain
    __uint16_t** vectorE_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorE_NTT == NULL) {
        fprintf(stderr, "Memory allocation error (Vector e) - PKE_KeyGen\n");
        return;
    }
    for (int i = 0; i < K; i++) {
        vectorE_NTT[i] = polyF2polyNTT(vectorE[i]);
    }
    // t vector in NTT domain
    __uint16_t** vectorT_NTT = sumVector(multiplyMatrixByVector(matrixA, vectorS_NTT), vectorE_NTT);

    // Keys generation
    struct keysPKE keys;

    keys.ek = ekGeneration(vectorT_NTT, rho);

    // Free matrix elements from memory
    for(int i = 0; i < K*K; i++) {
        free(matrixA[i]);
    }
    free(matrixA);
    
}

__uint8_t* ekGeneration(__uint16_t **tNTT, __uint8_t *rho) {
    // Concatenating tNTT vector with rho, generate encryption key ek.

    __uint8_t* byteArrayEncoded;
    __uint8_t* byteArrayAux;
    __uint8_t* tNTT_encoded;

    for (__uint16_t i = 0; i < K; i++) {
        tNTT_encoded = byteEncode(tNTT[i], 12); // byte array

        if(i == 0){
            byteArrayEncoded = concatenateBytes(tNTT_encoded, byteEncode(tNTT[i + 1], 12), 12*32, 12*32);

        }else if (i <= K - 2) {
            byteArrayAux = copyBytesArray(byteArrayEncoded, 12*32*(i + 1));
            free(byteArrayEncoded);
            byteArrayEncoded = NULL;

            byteArrayEncoded = concatenateBytes(byteArrayAux, byteEncode(tNTT[i + 1], 12), 12*32*(i + 1), 12*32);
            free(byteArrayAux);
            byteArrayAux = NULL;
            
        }else {
            byteArrayAux = copyBytesArray(byteArrayEncoded, 12*32*(i + 1));
            free(byteArrayEncoded);
            byteArrayEncoded = NULL;

            byteArrayEncoded = concatenateBytes(byteArrayAux, rho, 12*32*(i + 1), 32);
            free(byteArrayAux);
            byteArrayAux = NULL;

        }

        free(tNTT_encoded);
        tNTT_encoded = NULL;
    }

    return byteArrayEncoded;
}



__uint16_t **multiplyMatrixByVector(__uint16_t** matrix, __uint16_t** vector){

    __uint16_t **product = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation error - multiplyMatrixByVector\n");
        return NULL;
    }
    // Reservation of each element of the vector product
    for (int i = 0; i < K; i++) {
        product[i] = (__uint16_t *)calloc(256, sizeof(__uint16_t));
        if (product[i] == NULL) {
            fprintf(stderr, "Memory allocation error - multiplyMatrixByVector\n");
            return NULL;
        }
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            product[i] = sumPoly(product[i], multiplyNTT(matrix[i*K + j], vector[j]));
        }
    }
    return product;
}

__uint16_t **sumVector(__uint16_t **vector1, __uint16_t **vector2) {
    
    __uint16_t **sumResult = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (sumResult == NULL) {
        fprintf(stderr, "Memory allocation error - sumVector\n");
        return NULL;
    }
    // Sum of each element of the vectors 
    for (int i = 0; i < K; i++) {
        sumResult[i] = sumPoly(vector1[i], vector2[i]);
    }
    return sumResult;
}

__uint16_t* sumPoly(__uint16_t* poly1, __uint16_t* poly2) {

    __uint16_t* sum = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (sum == NULL) {
        fprintf(stderr, "Memory allocation error - sumPoly\n");
        return NULL;
    }
    for (int i = 0; i < 256; i++) {
        sum[i] = addModq(poly1[i], poly2[i]);
    }
    return sum;
}

__uint16_t* mulPoly(__uint16_t *poly1, __uint16_t *poly2) {

    __uint16_t *product = (__uint16_t *)calloc(256, sizeof(__uint16_t));
    if (product == NULL) {
        fprintf(stderr, "Memory allocation error - mulPoly\n");
        return NULL;
    }
    for (int i = 0; i < 256; i++) {
        product[i] = mulModq(poly1[i], poly2[i]);
    }
    return product;
}

__uint8_t *concatenateBytes(__uint8_t *byteArray1, __uint8_t *byteArray2, __uint16_t numBytes1, __uint16_t numBytes2) {
    // Concatenates two byte arrays

    __uint8_t *concBytes = (__uint8_t *)calloc(numBytes1 + numBytes2, sizeof(__uint8_t));
    if (concBytes == NULL) {
        fprintf(stderr, "Memory allocation error - contateBytes\n");
        return NULL;
    }
    // concBytes = (byteArray1 << numBytes1) | byteArray2;

    for (int i = 0; i < numBytes1 + numBytes2; i++) {
        if (i < numBytes1) {
            concBytes[i] = byteArray1[i];
        }else {
            concBytes[i] = byteArray2[i - numBytes1];
        }
    }

    return concBytes;
}

__uint16_t reduceBarrett(__uint32_t aMul) {
    // Implement the Barrett reduction algorithm to do a multiplication between 12-bit integers.
    __uint32_t t = ((__uint64_t)aMul*(__uint64_t)r_BARRETT) >> (2*k_BARRETT); // Generate a 13-bit integer
    t = aMul - t*Q;
    return conditionalReduce((__uint16_t)t);
}
__uint16_t conditionalReduce(__uint16_t a){
    // Implement condition reduction to avoid overflow.
    a -= Q;
    if (a & (1<<15)) { // If the most significant bit is 1 it means it overflowed
        a += Q;
    }
    return a;
}
__uint16_t addModq(__uint16_t a, __uint16_t b){
    a += b;
    a = conditionalReduce(a);
    return a;
}
__uint16_t subModq(__uint16_t a, __uint16_t b){
    a = a - b + Q;
    a = conditionalReduce(a);
    return a;
}
__uint16_t mulModq(__uint16_t a, __uint16_t b){
    __uint32_t mulResult = (__uint32_t)a*(__uint32_t)b;
    return reduceBarrett(mulResult);
}

__uint8_t* generateRandomBytes(__uint8_t d){

    // Generate 32*d random byte, then use byteDecode function to convert to integer mod q (or mod 2^d if d<12)
    __uint8_t* byteArray = (__uint8_t*)calloc(32*d, sizeof(__uint8_t));
    // Fit the byteArray with random values
    for (int i = 0; i < 32*d; i++) {
        byteArray[i] = rand()/((RAND_MAX + 1u)/256);
    }

    return byteArray;
}

__uint8_t *copyBytesArray(__uint8_t *byteArray, __uint16_t numBytes) {
    
    __uint8_t *newByteArray = (__uint8_t *)calloc(numBytes, sizeof(__uint8_t));

    for (int i = 0; i < numBytes; i++) {
        newByteArray[i] = byteArray[i];
    }

    return newByteArray;
}
