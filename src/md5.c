#include "md5.h"
#include "debug.h"
#include <stdint.h> 
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>
#include <math.h>

/*
    RFC1321 : Initialize MD Buffer (32-bit register)
    word A: 01 23 45 67
    word B: 89 ab cd ef
    word C: fe dc ba 98
    word D: 76 54 32 10
*/

void    md5_init(uint32_t state[4]) {
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
}  

/*
    Padding Bits : bloc 512bits (64octets)
    Add 1 bit
    Add bit 0 => 448 mod 512
*/

void    md5_padding(const uint8_t *input, uint8_t **output, uint64_t *padded_length) {
        size_t input_length = strlen((const char *)input);
        size_t total_length = ((input_length + 8) / 64 + 1) * 64;

        *padded_length = total_length;
        *output = (uint8_t *)calloc(total_length, sizeof(uint8_t));
        memcpy(*output, input, input_length);

        (*output)[input_length] = 0x80;
        uint64_t bit_length = input_length * 8;
        memcpy(*output + total_length - 8, &bit_length, sizeof(uint64_t));
}

/*
    F(X,Y,Z) = XY v not(X) Z
    G(X,Y,Z) = XZ v Y not(Z)
    H(X,Y,Z) = X xor Y xor Z
    I(X,Y,Z) = Y xor (X v not(Z))
*/

uint32_t    F(uint32_t X, uint32_t Y, uint32_t Z) {
            return (X & Y) | (~X & Z);
}

uint32_t    G(uint32_t X, uint32_t Y, uint32_t Z) {
            return (X & Z) | (Y & ~Z);
}

uint32_t    H(uint32_t X, uint32_t Y, uint32_t Z) {
            return X ^ Y ^ Z;
}

uint32_t    I(uint32_t X, uint32_t Y, uint32_t Z) {
            return Y ^ (X | ~Z);
}

/*
    Generate K[64] & S[64]
*/

void    generate_constants(uint32_t K[64]) {
        for (int i = 0; i < 64; i++) {
            K[i] = (uint32_t)(fabs(sin(i + 1)) * pow(2, 32));
        }
}

static const uint32_t S[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};


/*
    ROUND : 16step F, 16step G, 16step H, 16step I
*/

void    md5_round1(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t *M, const uint32_t *K, const uint32_t *S) {
        for (int i = 0; i < 16; i++) {
            uint32_t F_result = F(*B, *C, *D);
            uint32_t temp = *D;
            *D = *C;
            *C = *B;
            *B = *B + LEFTROTATE((*A + F_result + K[i] + M[i]), S[i]); // #define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
            *A = temp;
        }
}

void    md5_round2(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t *M, const uint32_t *K, const uint32_t *S) {
        for (int i = 0; i < 16; i++) {
            uint32_t G_result = G(*B, *C, *D);
            uint32_t temp = *D;
            *D = *C;
            *C = *B;
            *B = *B + LEFTROTATE((*A + G_result + K[16 + i] + M[(5 * i + 1) % 16]), S[16 + i]);
            *A = temp;
        }
}

void    md5_round3(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t *M, const uint32_t *K, const uint32_t *S) {
        for (int i = 0; i < 16; i++) {
            uint32_t H_result = H(*B, *C, *D);
            uint32_t temp = *D;
            *D = *C;
            *C = *B;
            *B = *B + LEFTROTATE((*A + H_result + K[32 + i] + M[(3 * i + 5) % 16]), S[32 + i]);
            *A = temp;
        }
}

void    md5_round4(uint32_t *A, uint32_t *B, uint32_t *C, uint32_t *D, uint32_t *M, const uint32_t *K, const uint32_t *S) {
        for (int i = 0; i < 16; i++) {
            uint32_t I_result = I(*B, *C, *D);
            uint32_t temp = *D;
            *D = *C;
            *C = *B;
            *B = *B + LEFTROTATE((*A + I_result + K[48 + i] + M[(7 * i) % 16]), S[48 + i]);
            *A = temp;
        }
}

void    decode_block(const uint8_t block[64], uint32_t M[16]) {
        for (int i = 0; i < 16; i++) {
            M[i] = (uint32_t)block[i * 4] |
                ((uint32_t)block[i * 4 + 1] << 8) |
                ((uint32_t)block[i * 4 + 2] << 16) |
                ((uint32_t)block[i * 4 + 3] << 24);
        }
}


/* 
    MD5_TRANSFORM
*/
void    md5_transform(uint32_t state[4], const uint8_t block[64]) {
        uint32_t A = state[0];
        uint32_t B = state[1];
        uint32_t C = state[2];
        uint32_t D = state[3];
        uint32_t M[16];
        static uint32_t K[64];
        static int initialized = 0;

        if (!initialized) {
            generate_constants(K);
            initialized = 1;
        }

        decode_block(block, M);
        //print_decoded_block(M,16);

        md5_round1(&A, &B, &C, &D, M, K, S);
        //print_state(A, B, C, D, "After Round 1");

        md5_round2(&A, &B, &C, &D, M, K, S);
        //print_state(A, B, C, D, "After Round 2");

        md5_round3(&A, &B, &C, &D, M, K, S);
        //print_state(A, B, C, D, "After Round 3");

        md5_round4(&A, &B, &C, &D, M, K, S);
        //print_state(A, B, C, D, "After Round 4");

        state[0] = state[0] + A;
        state[1] = state[1] + B;
        state[2] = state[2] + C;
        state[3] = state[3] + D;
}

void    md5_process(const char *input){
        uint8_t *padded = NULL;
        uint64_t padded_length = 0;
        uint32_t state[4];

        md5_init(state);
        md5_padding((const uint8_t *)input, &padded, &padded_length);

        for (uint64_t i = 0; i < padded_length; i += 64)
            md5_transform(state, padded + i);

        free(padded);
        print_md5(state);
}