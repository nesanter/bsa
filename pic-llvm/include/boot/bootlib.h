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

typedef enum {
    NVM_WRITE_WORD = 0x8001,
    NVM_WRITE_ROW = 0x8003,
    NVM_PAGE_ERASE = 0x8004,
    NVM_NOP = 0x8000
} nvm_op;

unsigned int flash_write_word(unsigned int value, unsigned int dest_addr);
unsigned int flash_write_row(unsigned int src_addr, unsigned int dest_addr);
unsigned int flash_unlock_erase(unsigned int page_addr);


#define PAGE_SIZE (1024)
#define ROW_SIZE (128)

#else
#error "runtime cannot access bootlib"
#endif

#endif /* BOOTLIB_H */
