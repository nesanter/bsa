#ifndef BOOTLIB_H
#ifdef IS_BOOTLOADER

void boot_uart_init();
void boot_print(char *s);
void boot_read_blocking(char *s, unsigned int length);

#define TRANSFER_BUFFER_SIZE (1024)
#define TRANSFER_BUFFER_SIZE_WITH_HEADER (TRANSFER_BUFFER_SIZE+2)

extern int transfer_ready;
unsigned char transfer_buffer[TRANSFER_BUFFER_SIZE_WITH_HEADER];

void boot_transfer_init();
void boot_transfer_enable();
unsigned int boot_get_crc();

typedef enum {
    SIGNAL_BOOT = 0x1,
    SIGNAL_USER = 0x2
} boot_signal;

void boot_signal_init();
void boot_signal_set(boot_signal sig, unsigned int on);

void boot_internal_error();

#define PAGE_SIZE (1024)
#define ROW_SIZE (128)

#else
#error "runtime cannot access bootlib"
#endif

#endif /* BOOTLIB_H */
