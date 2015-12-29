#include "util/rand.h"

/* Linear-Congruential Generator */
static int lcg_x;
const int lcg_A = 1664525;
const int lcg_C = 1013904223;

int rand_lcg8(void) {
    lcg_x = lcg_x * lcg_A + lcg_C;
    return (lcg_x & 0xFF000000) >> 24;
}

/* Xorshift Generator */

static unsigned int xor_x, xor_y, xor_z, xor_w;

unsigned int rand_xorshift32(void) {
    unsigned int t = xor_x ^ (xor_x << 11);
    xor_x = xor_y; xor_y = xor_z; xor_z = xor_w;
    xor_w = xor_w ^ (xor_w >> 19) ^ t ^ (t >> 8);
    return xor_w;
}

/* Seeding functions */

void seed_generators(int s) {
    lcg_x = s;
    for (int i = 0; i < 4; i++) {
        xor_x = (xor_x << 8) | rand_lcg8();
        xor_y = (xor_y << 8) | rand_lcg8();
        xor_z = (xor_z << 8) | rand_lcg8();
        xor_w = (xor_w << 8) | rand_lcg8();
    }
}

