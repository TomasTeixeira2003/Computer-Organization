// shell cmd: cc spark.c -o spark; ./spark

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include "SimpleCache.h"

#define NUM_REPETITIONS 100

int main() {
    int value;
    initCache();
    for(size_t r = 0; r < NUM_REPETITIONS; r++){

        for(size_t i = 0; i < L1_SIZE; i+=WORD_SIZE){
            read(i, (unsigned char *)(&value)); // same index
        }

    }
    fprintf(stdout, "Accessing memory of the size of L1, time elapsed: %u\n", getTime());
    resetTime();
    for(size_t r = 0; r < NUM_REPETITIONS; r++){

        for(size_t i = 0; i < L2_SIZE; i+=WORD_SIZE){
            read(i, (unsigned char *)(&value)); // same index
        }

    }
    fprintf(stdout, "Accessing memory of the size of L2, time elapsed: %u\n", getTime());

    resetTime();
    for(size_t r = 0; r < NUM_REPETITIONS; r++){

        for(size_t i = 0; i < L2_SIZE * 2; i+=WORD_SIZE){
            read(i, (unsigned char *)(&value)); // same index
        }

    }
    fprintf(stdout, "Accessing memory two times the size of L2, time elapsed: %u\n", getTime());

    resetTime();
    for(size_t r = 0; r < NUM_REPETITIONS*100; r++){

            read(0x0000000, (unsigned char *)(&value)); // same index
            read(0x0008000, (unsigned char *)(&value)); // same index

    }
    fprintf(stdout, "Alternating beetwen addresses 0x0000000 and 0x0008000, time elapsed: %u\n", getTime());

    return 0;
}