#ifndef BOOTLIB_H
#ifdef IS_BOOTLOADER

void boot_uart_init();
void boot_uart_change_baud(unsigned int baud);
void boot_print_flush();
void boot_print(char *s);
void boot_print_n(char *s, unsigned int len);
void boot_print_int(int n);
void boot_read_blocking(char *s, unsigned int length);
unsigned int boot_read_nonblocking(char *buffer, unsigned int length);
int boot_expect(char *s);

#define TRANSFER_BUFFER_SIZE (4096)
#define TRANSFER_BUFFER_HEADER_SIZE (3)
#define TRANSFER_BUFFER_SIZE_WITH_HEADER (TRANSFER_BUFFER_SIZE+TRANSFER_BUFFER_HEADER_SIZE)

//extern int transfer_ready;
//unsigned char transfer_buffer[TRANSFER_BUFFER_SIZE_WITH_HEADER];

void boot_transfer_init();
void boot_transfer_enable();
unsigned int boot_get_crc();

typedef enum {
    SIGNAL_BOOT = 0x1,
    SIGNAL_USER = 0x2
} boot_signal;

void boot_signal_init();
void boot_signal_set(boot_signal sig, unsigned int on);

int boot_run_read();

void boot_internal_error(int single);

typedef enum {
    NVM_WRITE_WORD = 0x1,
    NVM_WRITE_ROW = 0x3,
    NVM_PAGE_ERASE = 0x4,
    NVM_NOP = 0x0
} nvm_op;

unsigned int flash_write_word(unsigned int value, unsigned int dest_addr);
unsigned int flash_write_row(unsigned int src_addr, unsigned int dest_addr);
unsigned int flash_page_erase(unsigned int page_addr);

unsigned int physical_address(void *virt);

#define PAGE_SIZE (0x400)
#define ROW_SIZE (0x80)
#define PAGE_MASK (0x3FF)
#define ROW_MASK (0x7F)
#define ROWS_PER_PAGE (0x8)

typedef enum {
    REASON_POWER_ON = 0x001,
    REASON_BROWN_OUT = 0x002,
    REASON_WATCHDOG = 0x008,
    REASON_DEADMAN = 0x010,
    REASON_SOFTWARE = 0x020,
    REASON_MCLR = 0x080,
    REASON_CONFIG_MISMATCH = 0x200,
} reset_reason;

#define UNLOCK_KEY_A (0xAA996655)
#define UNLOCK_KEY_B (0x556699AA)

void soft_reset();

#else
#error "runtime cannot access bootlib"
#endif

#endif /* BOOTLIB_H */
