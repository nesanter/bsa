#ifndef BOOTLIB_H
#ifdef IS_BOOTLOADER

void boot_uart_init();
void boot_print(char *s);

#define TRANSFER_BUFFER_SIZE (512)

int transfer_ready = 0;
unsigned char transfer_buffer_1[TRANSFER_BUFFER_SIZE];
unsigned char transfer_buffer_2[TRANSFER_BUFFER_SIZE];

void boot_transfer_init();
void boot_transfer_enable(int buffern);

#else
#error "runtime cannot access bootlib"
#endif

#endif /* BOOTLIB_H */
