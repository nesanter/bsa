#define IS_BOOTLOADER

#include "boot/config.h"
#include "version.h"
#include "boot/bootlib.h"

int main(void) {
    boot_uart_init();
    boot_uart_print("BOOTLOADER ");
    boot_uart_print(BOOTLOADER_VERSION);
    boot_uart_print("\r\n");

    while (1) {

    }
}
