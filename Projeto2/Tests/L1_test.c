// shell cmd: cc spark.c -o spark; ./spark

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // exit()
#include <string.h> // memset()
#include "SimpleCache.c"

int main() {
    int value = 5, value2, value3;
    initCache();
    write(0x0000000F,(unsigned char *)(&value));
    read(0x0000400F, (unsigned char *)(&value2)); // same index
    read(0x0000000F, (unsigned char *)(&value3)); // misses
    printf("\nFirst miss on L1(first access),\nthen misses on the second access, replacing the other value(same index),\nmisses one more time to read the first value,\
        \nvalue wrote - %d, value read2 - %d, value read3 - %d, time elapsed - %u\n\n", value, value2, value3, getTime());
    return 0;
}