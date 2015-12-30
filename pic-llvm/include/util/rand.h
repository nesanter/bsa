#ifndef UTIL_RAND_H
#define UTIL_RAND_H

int rand_lcg8(void);
unsigned int rand_xorshift32(void);
void seed_generators(int s, int weak);

#endif /* UTIL_RAND_H */
