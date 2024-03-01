#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char* BitsToBytes(unsigned char* bitArray);
unsigned char* BytesToBits(unsigned char* bytesArray);
unsigned char* byteEncode(__uint16_t F[], __uint8_t d);
__uint16_t* byteDecode(unsigned char* byteArray, __uint8_t d);
#endif