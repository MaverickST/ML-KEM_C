/*!
\brief this is the first program in where we will start the program
\autho Cristian David Araujo Araujo, Maverick Sossa Tobon
*/

#include "functions.h"
#include "test.h"



int main() {

    srand(time(NULL)); // use current time as seed for random generator


    // __uint16_t** matrix = (__uint16_t **)calloc(K*K, sizeof(__uint16_t *));
    // if (matrix == NULL) {
    //     fprintf(stderr, "Memory allocation error\n");
    //     exit(1);
    // }
    // __uint16_t** vector = (__uint16_t **)calloc(K, sizeof(__uint16_t *));
    // if (vector == NULL) {
    //     fprintf(stderr, "Memory allocation error\n");
    //     exit(1);
    // }

    // printf("Matrix: \n");
    // for (int i = 0; i < K; i++) {
    //     for(int j = 0; j < K; j++) {
    //         matrix[i*K + j] = generateRandomPoly(Q);
    //         printPoly(matrix[i*K + j]);
    //     }
    //     printf("\n");
    // }
    // printf("Vector: \n");
    // for (int i = 0; i < K; i++) {
    //     vector[i] = generateRandomPoly(Q);
    //     printPoly(vector[i]);
    // }

    // __uint16_t** result = multiplyMatrixByVector(matrix, vector);
    // printf("Result: \n");
    // for (int i = 0; i < K; i++) {
    //     printPoly(result[i]);
    // }

    // free(vector);
    // for (int i = 0; i < K; i++) {
    //     free(vector[i]);
    // }
    // free(result);
    // for (int i = 0; i < K; i++) {
    //     free(result[i]);
    // }
    // free(matrix);
    // for(int i = 0; i < K*K; i++) {
    //     free(matrix[i]);
    // }

    return 0;
}


