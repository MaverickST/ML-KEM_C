#include "functions.h"


void printArrayInt(__uint16_t* array, __uint32_t size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}
void printArrayChar(unsigned char* array, __uint32_t size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}
__uint8_t bitRev7(__uint8_t i){
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

// unsigned char* byteEncode(__uint16_t F[], __uint16_t d) {
//     // ByteEncoded serializes an array of d-bit integers into an array of 32d bytes.
//     //Encodes an array of d-bit integers into a byte array of 32d, for 1 ≤ d ≤ 12

//     // Create an array of bits
//     __uint16_t numBits = 256*d;

//     // Create an array of bits
//     unsigned char* bitArray = (unsigned char*)calloc(numBits + 1, sizeof(unsigned char));
//     if (bitArray == NULL) {
//         fprintf(stderr, "Memory allocation error - byteEncode\n");
//         return NULL;
//     }


//     for (int i = 0; i < 256; i++) {
//         __uint16_t a = F[i];
//         for (int j = 0; j < d; j++) {
//             bitArray[i * d + j] = (a%2) + '0';
//             //a = a >> 1;
//             a = (a - (bitArray[i * d + j] - '0')) / 2;
//         }
//     }


//     unsigned char* byteArray = BitsToBytes(bitArray);

//     // Free the allocated memory
//     free(bitArray);

//     return byteArray;
// }

__uint16_t* byteDecode(unsigned char* byteArray, __uint8_t d){
    // Decodes an array of 32d bytes into an array of d-bit integers.
    //Decodes an array of 32d bytes into an array of d-bit integers.

    unsigned char* bitString = BytesToBits(byteArray);

    // Create an array of bits
    __uint16_t numBytes = 256;
    __uint16_t* intArrayF = (__uint16_t*)calloc(numBytes, sizeof(__uint16_t));

    if (intArrayF == NULL) {
        fprintf(stderr, "Memory allocation error - byteDncode\n");
        return NULL;
    }
    // Iterate over the bytes
    for(int i = 0; i < 256; i++) {
        for (int j = 0; j < d; j++) {
            intArrayF[i] += (bitString[i * d + j] - '0')<<j;// sum up the bits
        }
        if (d < 12){
            intArrayF[i] = intArrayF[i]%(1 << d);// truncate to d bits (mod 2^d)
        }else {
            intArrayF[i] = intArrayF[i]%(3329);// truncate to 3329 bits (mod q)
        }
        
    }

    // Free the allocated memory
    free(bitString);

    return intArrayF;
}

__uint16_t reduceBarrett(__uint32_t aMul) {
    // Implement the Barrett reduction algorithm to do a multiplication between 12-bit integers.
    __uint32_t t = ((__uint64_t)aMul*(__uint64_t)r_BARRETT) >> (2*k_BARRETT); // Generate a 13-bit integer
    t = aMul - t*3329;
    return conditionalReduce((__uint16_t)t);
}
__uint16_t conditionalReduce(__uint16_t a){
    // Implement condition reduction to avoid overflow.
    a -= 3329;
    if (a & (1<<15)) { // If the most significant bit is 1 it means it overflowed
        a += 3329;
    }
    return a;
}
__uint16_t addModq(__uint16_t a, __uint16_t b){
    a += b;
    a = conditionalReduce(a);
    return a;
}
__uint16_t subModq(__uint16_t a, __uint16_t b){
    a = a - b + 3329;
    a = conditionalReduce(a);
    return a;
}
__uint16_t mulModq(__uint16_t a, __uint16_t b){
    __uint32_t mulResult = (__uint32_t)a*(__uint32_t)b;
    return reduceBarrett(mulResult);
}

