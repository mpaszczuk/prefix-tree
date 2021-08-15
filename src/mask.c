#include "mask.h"
#include "ip.h"
#include <stdint.h>

/* Generate bitmask filed with '1' of the same length as ip->bitmask */
unsigned int get_bitmask(char mask) {
    if (mask == IP_LENGTH) {
        return UINT32_MAX;
    }
    return (unsigned int)((1 << mask) - 1) << (IP_LENGTH - mask);
}

/*Get bit of value at position bit_number*/
unsigned int get_bit(unsigned int value, unsigned char bit_number) {
    return (value & (1 << (UINT32_LEN - bit_number - 1))) >> (UINT32_LEN - bit_number - 1);
}
