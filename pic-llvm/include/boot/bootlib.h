#ifndef BOOTLIB_H
#ifdef IS_BOOTLOADER

void boot_uart_init();
void boot_print(char *s);

#define TRANSFER_BUFFER_SIZE (1024)
#define TRANSFER_BUFFER_SIZE_WITH_HEADER (TRANSFER_BUFFER_SIZE+2)

extern int transfer_ready;
unsigned char transfer_buffer[TRANSFER_BUFFER_SIZE_WITH_HEADER];

void boot_transfer_init();
void boot_transfer_enable();
unsigned int boot_get_crc();

#define PAGE_SIZE (1024)
#define ROW_SIZE (128)

#else
#error "runtime cannot access bootlib"
#endif

#endif /* BOOTLIB_H */
