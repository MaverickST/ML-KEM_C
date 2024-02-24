#include "functions.h"


unsigned char* convertBitsToBytes(const char* bitString) {
    // Calculates the length of the bit string
    size_t length = strlen(bitString);

    // Calculates the number of bytes required
    size_t numBytes = (length + 7) / 8; // Round up to the nearest multiple of 8

    // Create an array of bytes
    unsigned char* bytesArray = (unsigned char*)calloc(numBytes, sizeof(unsigned char));
    if (bytesArray == NULL) {
        fprintf(stderr, "Error en la asignación de memoria\n");
        return NULL;
    }

    // Iterate over the bit string
    for (size_t i = 0; i < length; i++) {
        // Set the bit in the corresponding byte (little-endian format)
        bytesArray[i / 8] |= (bitString[i] - '0') << (i % 8);
    }

    return bytesArray;
}
