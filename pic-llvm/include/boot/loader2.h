#ifndef LOADER2_H
#ifdef IS_BOOTLOADER

void preamble(void);
void load(void);

#else
#error "runtime cannot access loader2"
#endif
#endif /* LOADER2_H */

