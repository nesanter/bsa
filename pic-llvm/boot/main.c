#define IS_BOOTLOADER

#include "boot/config.h"
#include "version.h"
#include "boot/bootlib.h"
#include "boot/handler.h"
#include "ulib/util.h"

int main(void) {
    unsigned int errorepc;
    boot_handler_setup(&errorepc);
    boot_uart_init();
    boot_print("BOOTLOADER ");
    boot_print(BOOTLOADER_VERSION);
    boot_print("\r\n");

    if (errorepc) {
        boot_print("ERROR MAY HAVE OCCURED [");
        boot_print(tohex(errorepc, 8));
        boot_print("\r\n");
    }

    while (1) {

    }
}
