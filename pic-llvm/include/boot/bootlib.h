#ifndef BOOTLIB_H
#ifdef IS_BOOTLOADER

void boot_uart_init();
void boot_uart_print(char *s);

#else
#error "runtime cannot access bootlib"
#endif

#endif /* BOOTLIB_H */
