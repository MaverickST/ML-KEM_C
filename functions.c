#include "functions.h"
#include "test.h"
#include "arrays.h"

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

    int i = 0, j = 0;
    __uint16_t d1, d2;
    while(j < 256){
        d1 = byteArray[i] + 256*(byteArray[i + 1]%16);
        d2 = (byteArray[i + 1]>>4) + (16*byteArray[i + 2]);
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

    for (int i = 0; i < 256; i++) {
        __uint16_t x = 0, y = 0;
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
__uint16_t* NTT(__uint16_t* polyF){
    // Computes the NTT representation f_ˆ of the given polynomial f ∈ Rq.

    __uint16_t* polyNTT = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (polyNTT == NULL) {
        fprintf(stderr, "Memory allocation error - NTT\n");
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
__uint16_t* inverseNTT(__uint16_t* polyNTT) {
    // Computes the polynomial f ∈ Rq corresponding to the given NTT representation f_ˆ ∈ Tq.

    __uint16_t* polyF = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    if (polyF == NULL) {
        fprintf(stderr, "Memory allocation error - inverseNTT\n");
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

struct Keys PKE_KeyGen() {
    // Generates an encryption key and a corresponding decryption key
    __uint8_t d[32] = {0x92,0xAC,0x7D,0x1F,0x83,0xBA,0xFA,0xE6,0xEE,0x86,0xFE,0x00,0xF9,0x5D,0x81,
                        0x33,0x75,0x77,0x24,0x34,0x86,0x0F,0x5F,0xF7,0xD5,0x4F,0xFC,0x37,0x39,0x9B,0xC4,0xCC};
    __uint8_t n = 0;

    // Getting ρ and σ hashes by G(d)
    __uint8_t rho[32];
    __uint8_t sigma[32];
    __uint8_t rho_sigma[64];

    SHA3_512(d, 32, rho_sigma);
    for (int i = 0; i < 32; i++) {
        rho[i] = rho_sigma[i];
        sigma[i] = rho_sigma[i + 32];
    }
    

    // Generates a KxK matrix of polynomials (in NTT domain) mod q
    __uint16_t** matrixA = (__uint16_t **)calloc(K*K, sizeof(__uint16_t *));
    if (matrixA == NULL) {
        fprintf(stderr, "Memory allocation error (matrixA) - PKE_KeyGen\n");
        // return NULL;
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            __uint8_t* outXOF = XOF(rho, i, j);
            matrixA[i*K + j] = sampleNTT(outXOF); // XOF(ρ,i,j)
            free(outXOF);
        }
    }

    // Generates a Kx1 vector (s vector) of polynomials mod q
    __uint16_t** vectorS = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorS == NULL) {
        fprintf(stderr, "Memory allocation error (Vector s) - PKE_KeyGen\n");
        // return;
    }
    for (int i = 0; i < K; i++) {
        __uint8_t* outPRF = PRF(sigma, n, (__uint8_t)ETA_1); // PRF(σ,n,η1)
        vectorS[i] = samplePolyCBD(outPRF, ETA_1); // PRFη1 (σ,N)
        free(outPRF);
        n++;
    }

    // Generates a Kx1 vector (e vector) of polynomials mod q
    __uint16_t** vectorE = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorE == NULL) {
        fprintf(stderr, "Memory allocation error (Vector e) - PKE_KeyGen\n");
        // return;
    }
    for (int i = 0; i < K; i++) {
        __uint8_t* outPRF = PRF(sigma, n, (__uint8_t)ETA_1); // PRF(σ,n,η2)
        vectorE[i] = samplePolyCBD(outPRF, ETA_1); // PRFη1 (σ,N)
        free(outPRF);
        n++;
    }

    // Calculations and transformations

    // s vector in NTT domain
    __uint16_t** vectorS_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorS_NTT == NULL) {
        fprintf(stderr, "Memory allocation error (Vector s) - PKE_KeyGen\n");
        // return;
    }
    for (int i = 0; i < K; i++) {
        vectorS_NTT[i] = NTT(vectorS[i]);
    }
    // e vector in NTT domain
    __uint16_t** vectorE_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorE_NTT == NULL) {
        fprintf(stderr, "Memory allocation error (Vector e) - PKE_KeyGen\n");
        // return;
    }
    for (int i = 0; i < K; i++) {
        vectorE_NTT[i] = NTT(vectorE[i]);
    }
    // t vector in NTT domain
    __uint16_t** vectorResult = multiplyMatrixByVector(matrixA, vectorS_NTT);
    __uint16_t** vectorT_NTT = sumVector(vectorResult, vectorE_NTT);

    // Keys generation
    struct Keys keysPKE;

    keysPKE.ek = vector2Bytes(vectorT_NTT, 384*K + 32);
    for (int i = 0; i < 32; i++) {
        keysPKE.ek[384*K + i] = rho[i];
    }

    keysPKE.dk = vector2Bytes(vectorS_NTT, 384*K);

    // Free each variable from memory
    freeVector(matrixA, K*K);
    freeVector(vectorS, K);
    freeVector(vectorE, K);
    freeVector(vectorT_NTT, K);
    freeVector(vectorS_NTT, K);
    freeVector(vectorE_NTT, K);
    // free(d);

    return keysPKE;
}

__uint8_t* PKE_Encrypt(__uint8_t* ekPKE, __uint8_t* m, __uint8_t* r) {

    __uint16_t sizeEkPKE = 384*K + 32;
    __uint16_t sizeM = 32;
    __uint16_t sizeR = 32;
    
    __uint8_t n = 0;
    __uint8_t d = 12;

    // Decoding ekPKE   
    __uint16_t** vectorT_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorT_NTT == NULL) {
        fprintf(stderr, "Memory allocation error - PKE_Encrypt\n");
        return NULL;
    }
    for (int i = 0; i < K; i++) {

        __uint8_t ekPKE_i[384];
        for (int j = 0; j < 384; j++) {
            ekPKE_i[j] = ekPKE[384*i + j];
        }
        vectorT_NTT[i] = byteDecode(ekPKE_i, d);
    }

    // Getting rho
    __uint8_t sizeRho = 32;
    __uint8_t rho[32];
    for (int i = 0; i < 32; i++) {
        rho[i] = ekPKE[384*K + i];
    }

    // Generates a KxK matrix of polynomials (in NTT domain) mod q
    __uint16_t** matrixAT = (__uint16_t **)calloc(K*K, sizeof(__uint16_t *));
    if (matrixAT == NULL) {
        fprintf(stderr, "Memory allocation error (Matrix A) - PKE_Encrypt\n");
        return NULL;
    }
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < K; j++) {
            __uint8_t* outXOF = XOF(rho, i, j); // XOF(ρ,j,i)
            matrixAT[j*K + i] = sampleNTT(outXOF); 
            free(outXOF);
        }
    }

    // Generates a Kx1 vector (r vector) of polynomials mod q
    __uint16_t** vectorR = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorR == NULL) {
        fprintf(stderr, "Memory allocation error (Vector r) - PKE_Encrypt\n");
        return NULL;
    }
    for (int i = 0; i < K; i++) {
        __uint8_t* outPRF = PRF(r, n, (__uint8_t)ETA_1); // PRF(r,n,η1)
        vectorR[i] = samplePolyCBD(outPRF, (__uint8_t)ETA_1);
        free(outPRF);
        n++;
        
    }

    // Generates a Kx1 vector (e1 vector) of polynomials mod q
    __uint16_t** vectorE1 = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorE1 == NULL) {
        fprintf(stderr, "Memory allocation error (Vector e1) - PKE_Encrypt\n");
        return NULL;
    }
    for (int i = 0; i < K; i++) {
        __uint8_t* outPRF = PRF(r, n, (__uint8_t)ETA_2); // PRF(r,n,η2)
        vectorE1[i] = samplePolyCBD(outPRF, (__uint8_t)ETA_2);
        free(outPRF);
        n++;
    }

    // e2
    __uint8_t* outPRF = PRF(r, n, (__uint8_t)ETA_2); // PRF(r,n,η2)
    __uint16_t* e2 = samplePolyCBD(outPRF, (__uint8_t)ETA_2);

    // r vector in NTT domain
    __uint16_t **vectorR_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorR_NTT == NULL) {
        fprintf(stderr, "Memory allocation error - PKE_Decrypt\n");
        return NULL;
    }
    for (int i = 0; i < K; i++) {
        vectorR_NTT[i] = NTT(vectorR[i]);
    }

    // Generating u vector:  u ← NTT−1(Aˆ ⊺ ◦ rˆ)+ e1
    __uint16_t** matrixByVector_NTT = multiplyMatrixByVector(matrixAT, vectorR_NTT);
    __uint16_t** matrixByVector = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (matrixByVector == NULL) {
        fprintf(stderr, "Memory allocation error - PKE_Encrypt\n");
        return NULL;
    }
    for(int i = 0; i < K; i++) {
        matrixByVector[i] = inverseNTT(matrixByVector_NTT[i]);
    }

    __uint16_t** vectorU = sumVector(matrixByVector, vectorE1);


    // Decoding and decompressing m, which results µ
    __uint16_t* mDecoded = byteDecode(m, 1);
    __uint16_t mu[256];
    for (int i = 0; i < 256; i++) {
        mu[i] = decompress(mDecoded[i], 1);
    }


    // v ← NTT−1(ˆt⊺ ◦ rˆ)+ e2 + µ

    __uint16_t* dotProduct_NTT = vectorDotProduct(vectorT_NTT, vectorR_NTT);
    __uint16_t* dotProduct = inverseNTT(dotProduct_NTT);
    __uint16_t* sum1 = sumPoly(dotProduct, e2);
    __uint16_t* v = sumPoly(sum1, mu);

    //c1 ← ByteEncode_du (Compress_du (u))
    // Compressing and encoding u
    __uint16_t uCompressed[K][256];
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < 256; j++) {
            uCompressed[i][j] = compress(vectorU[i][j], D_u);
        }
    }

    __uint8_t c1[32*D_u*K];
    for (int i = 0; i < K; i++) {
        __uint8_t* uCompressedEncoded = byteEncode(uCompressed[i], D_u);
        for (int j = 0; j < 32*D_u; j++) {
            c1[32*D_u*i + j] = uCompressedEncoded[j];
        }
        free(uCompressedEncoded);
    }
    
    //c2 ← ByteEncode_dv (Compress_dv (v))
    // Compressing and encoding v
    __uint16_t vCompressed[256];
    for (int i = 0; i < 256; i++) {
        vCompressed[i] = compress(v[i], D_v);
    }
    __uint8_t* c2 = byteEncode(vCompressed, D_v);

    // Concatenation to get c
    __uint8_t* c = concatenateBytes(c1, c2, 32*D_u*K, 32*D_v);

    // Free each dinamic variable.
    freeVector(vectorT_NTT, K);
    freeVector(matrixAT, K*K);
    freeVector(vectorR, K);
    freeVector(vectorE1, K);
    free(outPRF);
    free(e2);
    freeVector(vectorR_NTT, K);
    freeVector(matrixByVector_NTT, K);
    freeVector(matrixByVector, K);
    freeVector(vectorU, K);
    free(mDecoded);
    free(dotProduct_NTT);
    free(dotProduct);
    free(sum1);
    free(v);
    free(c2);

    return c;
}

__uint8_t *PKE_Decrypt(__uint8_t *dkPKE, __uint8_t *cipherText) {


    // Segmentation
    __uint8_t c1[32*D_u*K];
    __uint8_t c2[32*D_v];
    for(int i = 0; i < 32*(D_u*K + D_v); i++) {
        if (i < 32*D_u*K) {
            c1[i] = cipherText[i];
        }else {
            c2[i - 32*D_u*K] = cipherText[i];
        }
    }

    // Initilization of u vector
    __uint16_t **vectorU = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorU == NULL) {
        fprintf(stderr, "Memory allocation error - PKE_Decrypt\n");
        return NULL;
    }
    for (int i = 0; i < K; i++) {
        vectorU[i] = (__uint16_t *)calloc(256, sizeof(__uint16_t));
        if (vectorU[i] == NULL) {
            fprintf(stderr, "Memory allocation error - PKE_Decrypt\n");
            return NULL;
        }
    }
    // Decompressing and decoding c1. Generate the vectorU, which is K polynomials.
    for (int i = 0; i < K; i++) {
        // Get K packets of 32*D_u bytes
        __uint8_t ci[32*D_u];
        for (int j = 0; j < 32*D_u; j++) {
            ci[j] = c1[32*D_u*i + j];
        }
        // Decode the byte array into 256 integers mod 2^D_u.
        __uint16_t* c1Decoded = byteDecode(ci, D_u);
        for (int j = 0; j < 256; j++) {
            vectorU[i][j] = decompress(c1Decoded[j], D_u);
        }
        free(c1Decoded);
    }

    // Decompressing and decoding c2, which generates v
    __uint16_t v[256];
    __uint16_t* c2Decoded = byteDecode(c2, D_v);
    for (int i = 0; i < 256; i++) {
        v[i] = decompress(c2Decoded[i], D_v);
    }
    free(c2Decoded);

    // Initialization of s vector
    __uint16_t **vectorS_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    if (vectorS_NTT == NULL) {
        fprintf(stderr, "Memory allocation error - PKE_Decrypt\n");
        return NULL;
    }

    // Decoding dkPKE
    __uint8_t d = 12;
    for (int i = 0; i < K; i++) {
        // Get K packets of 32*d bytes
        __uint8_t dkPKE_i[32*d];
        for (int j = 0; j < 32*d; j++) {
            dkPKE_i[j] = dkPKE[32*d*i + j];
        }
        // Decode the byte array into 256 integers mod 2^d (Q).
        vectorS_NTT[i] = byteDecode(dkPKE_i, d);
    }

    // Generation w
    __uint16_t** vectorU_NTT = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    for (int i = 0; i < K; i++) {
        vectorU_NTT[i] = NTT(vectorU[i]);
    }
    __uint16_t* resultDot_NTT = vectorDotProduct(vectorS_NTT, vectorU_NTT);
    __uint16_t* resultDot = inverseNTT(resultDot_NTT);
    __uint16_t w[256];
    for (int i = 0; i < 256; i++) {
        w[i] = subModq(v[i], resultDot[i]);
    }

    // Compressing and encoding w.
    for (int i = 0; i < 256; i++) {
        w[i] = compress(w[i], 1);
    }
    __uint8_t* m = byteEncode(w, 1);

    // Free each dinamic variable.
    freeVector(vectorU, K);
    freeVector(vectorS_NTT, K);
    freeVector(vectorU_NTT, K);
    free(resultDot_NTT);
    free(resultDot);

    return m;
}

struct Keys ML_KEM_KeyGen() {
    struct Keys keysML_KEM;

    // Random bytes 
    __uint8_t z [32] = {0x92,0xAC,0x7D,0x1F,0x83,0xBA,0xFA,0xE6,0xEE,0x86,0xFE,0x00,0xF9,0x5D,0x81,
                        0x33,0x75,0x77,0x24,0x34,0x86,0x0F,0x5F,0xF7,0xD5,0x4F,0xFC,0x37,0x39,0x9B,0xC4,0xCC};

    // PKE keys
    struct Keys keysPKE = PKE_KeyGen();
    keysML_KEM.ek = keysPKE.ek;

    // Hash of ek -> H(ek)
    __uint8_t Hek[32];
    SHA3_256(keysML_KEM.ek, 384*K + 32, Hek);

    // Concatenation
    __uint8_t* concat_dkPKE_ekMLKEM = concatenateBytes(keysPKE.dk, keysML_KEM.ek, 384*K, 384*K + 32);
    __uint8_t* concat_dK_ek_Hek = concatenateBytes(concat_dkPKE_ekMLKEM, Hek, 2*384*K + 32, 32);
    keysML_KEM.dk = concatenateBytes(concat_dK_ek_Hek, z, 2*384*K + 32 + 32, 32);
    
    // Free each dinamic variable.
    free(concat_dkPKE_ekMLKEM);
    free(concat_dK_ek_Hek);
    // free(z);

    return keysML_KEM;
}

__uint8_t* ML_KEM_Encaps(__uint8_t *ekML) {

    // Random bytes
    __uint8_t* m = generateRandomBytes(1);

    // Hashes generation: G(m ∥ H(ek))
    __uint8_t Hek[32];
    __uint8_t r[32];
    __uint8_t k[32];
    __uint8_t K_r[64];

    SHA3_256(ekML, 384*K + 32, Hek); // H(ek)
    __uint8_t* concat_m_Hek = concatenateBytes(m, Hek, 32, 32);
    SHA3_512(concat_m_Hek, 64, K_r); // (K,r) ← G(m∥H(ek))

    // Split K_r into k and r
    for (int i = 0; i < 32; i++) {
        k[i] = K_r[i];
        r[i] = K_r[i + 32];
    }

    // Encrpting m and concatenation.
    __uint8_t* c = PKE_Encrypt(ekML, m, r);
    __uint8_t* K_c = concatenateBytes(k, c, 32, 32*(D_u*K + D_v));

    // Free each dinamic variable.
    free(concat_m_Hek);

    return K_c;

}

__uint8_t *ML_KEM_Decaps(__uint8_t *cipherText, __uint8_t *dkML) {

    // Getting dkPKE, ekPKE, h and z from dkML
    __uint8_t dkPKE[384*K];
    __uint8_t ekPKE[384*K + 32];
    __uint8_t h[32];
    __uint8_t z[32];

    for (int i = 0; i < 768*K + 96; i++) {
        if (i < 384*K) {
            dkPKE[i] = dkML[i];
        }else if (i < 768*K + 32){
            ekPKE[i - 384*K] = dkML[i];
        }else if (i < 768*K + 64){
            h[i - 768*K] = dkML[i];
        }else {
            z[i - 768*K - 32] = dkML[i];
        }
    }
    // Decryption
    __uint8_t* mp = PKE_Decrypt(dkPKE, cipherText);

    // Concatenation
    __uint8_t* mp_h = concatenateBytes(mp, h, 32, 32);
    __uint8_t* z_c = concatenateBytes(z, cipherText, 32, 32*(D_u*K + D_v));

    // Hashes
    __uint8_t Kp_rp[64];
    SHA3_512(mp_h, 64, Kp_rp); // (K',r') ← G(m'∥h)

    __uint8_t K_[32];
    SHAKE_256(z_c, 32*(D_u*K + D_v) + 32, K_, 32); // K¯ ← J(z∥c,32)

    // Split Kp_rp into K' and r'
    __uint8_t* Kp = (__uint8_t*)calloc(32, sizeof(__uint8_t));
    __uint8_t rp[32];
    for (int i = 0; i < 32; i++) {
        Kp[i] = Kp_rp[i];
        rp[i] = Kp_rp[i + 32];
    }

    // Encryption
    __uint8_t* cp = PKE_Encrypt(ekPKE, mp, rp); 

    // printf("Shared key Kp: \n");
    // printBytesHex(Kp, 32);

    // Implicit rejection
    for (int i = 0; i < 32*(D_u*K + D_v); i++) {
        if (cipherText[i] != cp[i]) {
            // cipherText and cp are different, copy K_ into Kp	
            for (int j = 0; j < 32; j++) {
                Kp[j] = K_[j];
            }
        }
    }

    // printf("Shared key K_: \n");
    // printBytesHex(K_, 32);

    // printf("Ciphertext cp: \n");
    // printBytesHex(cp, 32*(D_u*K + D_v));

    // Free each dinamic variable.
    free(mp);
    free(mp_h);
    free(z_c);
    free(cp);

    return Kp;

}

__uint16_t *vectorDotProduct(__uint16_t **vector1, __uint16_t **vector2) {
    // Compute the vector dot product, and return a polynomial

    __uint16_t *result = (__uint16_t*)calloc(256, sizeof(__uint16_t));
    for (int i = 0; i < K; i++) {
        // Multiply each polynomial of the vectors input
        __uint16_t *resultMulPoly = multiplyNTT(vector1[i], vector2[i]);
        for (int j = 0; j < 256; j++) {
            result[j] = addModq(result[j], resultMulPoly[j]);
        }
        free(resultMulPoly);
    }
    return result;
}

__uint8_t *vector2Bytes(__uint16_t **vector, __uint16_t numBytes) {

    __uint8_t *byteArray = (__uint8_t *)calloc(numBytes, sizeof(__uint8_t));

    for (int i = 0; i < K; i++) {
        // Encode polynomial
        __uint8_t* byteArrayEncoded = byteEncode(vector[i], 12);
        // Concatenation
        for (int j = 0; j < 384; j++) {
            byteArray[384*i + j] = byteArrayEncoded[j];
        }
        free(byteArrayEncoded);
    }

    return byteArray;
}

__uint8_t* XOF(__uint8_t* rho, __uint8_t i, __uint8_t j) {

    __uint8_t sizeRho = 32;
    __uint8_t sizeInput1 = 1 + sizeRho;
    __uint8_t* input1 = concatenateBytes(rho, &i, sizeRho, 1);

    __uint16_t sizeInput2 = 1 + sizeInput1;
    __uint8_t* input2 = concatenateBytes(input1, &j, sizeInput1, 1);

    __uint16_t sizeOut = 2*256;
    __uint8_t* output = (__uint8_t*)calloc(sizeOut, sizeof(__uint8_t));

    SHAKE_128(input2, sizeInput2, output, sizeOut);

    free(input1);
    free(input2);

    return output;
}

__uint8_t* PRF(__uint8_t* r, __uint8_t n, __uint8_t eta)
{
    
    __uint8_t sizeR = 32;
    __uint16_t sizeInput = 1 + sizeR; 
    __uint8_t* input = concatenateBytes(r, &n, sizeR, 1);
    
    __uint8_t sizeOut = 64*eta;
    __uint8_t* output = (__uint8_t*)calloc(sizeOut, sizeof(__uint8_t));

    SHAKE_256(input, sizeInput, output, sizeOut);

    free(input);
    
    return output;
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
            // Multiply matrix(i,j) by vector(j)
            __uint16_t* polyMultiply = multiplyNTT(matrix[i*K + j], vector[j]);
            __uint16_t* polySum = sumPoly(product[i], polyMultiply);
            for (int n = 0; n < 256; n++) {
                product[i][n] = polySum[n];
            }
            free(polyMultiply);
            free(polySum);
        }
    }
    return product;
}

__uint16_t **sumVector(__uint16_t **vector1, __uint16_t **vector2) {
    // Sum 2 vectors of polynomials mod q
    
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

__uint8_t* concatenateBytes(__uint8_t *byteArray1, __uint8_t *byteArray2, __uint16_t numBytes1, __uint16_t numBytes2)
{
    // Concatenates two byte arrays

    __uint8_t* nuwByteArray = (__uint8_t *)calloc(numBytes1 + numBytes2, sizeof(__uint8_t));

    for (int i = 0; i < numBytes1 + numBytes2; i++) {
        if (i < numBytes1) {
            nuwByteArray[i] = byteArray1[i];
        }else {
            nuwByteArray[i] = byteArray2[i - numBytes1];
        }
    }

    return nuwByteArray;
}

__uint16_t reduceBarrett(__uint32_t aMul) {
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

void freeVector(__uint16_t **vector, __uint8_t sizeK) {
    for (int i = 0; i < sizeK; i++) {
        free(vector[i]);
    }
    free(vector);
}

__uint8_t *copyBytesArray(__uint8_t *byteArray, __uint16_t numBytes) {
    
    __uint8_t *newByteArray = (__uint8_t *)calloc(numBytes, sizeof(__uint8_t));

    for (int i = 0; i < numBytes; i++) {
        newByteArray[i] = byteArray[i];
    }

    return newByteArray;
}


