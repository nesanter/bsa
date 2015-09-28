#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "boot/loader2.h"
#include "proc/segments.h"
#include "version.h"

struct load_info {
    unsigned int vaddr,
                 memsz,
                 initsz;
    enum load_seg seg; 
};

const unsigned int BLOCK_SIZE = 1024;
char __attribute__((section(".rt_data"))) block_buffer [1024];

const unsigned int MAX_LOAD_HEADERS = 8;
struct load_info load_headers [8];

const unsigned int MAP_GRANULARITY = 16; // in bytes
unsigned int map_phys_address = 0;
void * map_virt_address = 0;
unsigned int init_phys_start_address = 0;
void * init_virt_start_address = 0;

int create_initialization_map(unsigned int initspace, unsigned int progspace, unsigned int progmaxaddr) {
    // first create the map itself
    unsigned int map_bytes = SEG_RAM_SIZE / MAP_GRANULARITY;
    unsigned int map_pages;
    if (map_bytes % PAGE_SIZE == 0) {
        map_pages = map_bytes / PAGE_SIZE;
    } else {
        map_pages = 1 + (map_bytes / PAGE_SIZE);
    }

    unsigned int init_pages;
    if (initspace % PAGE_SIZE == 0)
        init_pages = initspace / PAGE_SIZE;
    else
        init_pages = 1 + (initspace / PAGE_SIZE);

    if (progmaxaddr % PAGE_SIZE == 0)
        map_phys_address = progmaxaddr;
    else
        map_phys_address = progmaxaddr + (PAGE_SIZE - progmaxaddr % PAGE_SIZE);

    if (map_phys_address + (map_pages * PAGE_SIZE) + (init_pages * PAGE_SIZE) > SEG_PHYS_PROG_END)
        return 1;

    for (int i = 0 ; i < map_pages ; i++) {
        int r = flash_page_erase(map_phys_address + PAGE_SIZE * i);
        if (r & 0x1000)
            return 2;
        else if (r & 0x2000)
            return 3;
    }

    // then record actual initspace address
    init_phys_start_address = map_phys_address + map_pages * PAGE_SIZE;

    // and create it
    for (int i = 0 ; i < init_pages ; i++) {
        int r = flash_page_erase(init_phys_start_address + PAGE_SIZE * i);
        if (r & 0x1000)
            return 2;
        else if (r & 0x2000)
            return 3;
    }

    // and virtual addresses
    map_virt_address = (void*)(map_phys_address + 0xC0000000);
    init_virt_start_address = (void*)(init_phys_start_address + 0xC0000000);

    return 0;
}

// triggered after "BSA PREAMBLE" is received
void preamble(void) {

    unsigned int cmd, new_baud = DEFAULT_BAUD, count;
    while (1) {
        boot_read_blocking((char*)&cmd, 4);
        switch (cmd) {
            case 0x3F524556U:
                boot_print_int(BOOTLOADER_VERSION);
                if (boot_expect("OK")) {
                    boot_internal_error(1);
                    soft_reset();
                }
                break;
            case 0x44554142U:
                boot_read_blocking((char*)&new_baud, 4);
                // could check baud here
                boot_print("OK");
                break;
            case 0x3F5A5342U:
                boot_print_int(BLOCK_SIZE);
                if (boot_expect("OK")) {
                    boot_internal_error(1);
                    soft_reset();
                }
                break;
            case 0x454E4F44U:
                boot_print("OK");
                boot_print_flush();
                boot_uart_change_baud(new_baud);
                asm volatile ("mtc0 $zero, $9");
                do {
                    asm volatile ("mfc0 %0, $9" : "=r" (count));
                } while (count < 40000000);
                boot_internal_error(1);
                boot_print("BOOTLOADER READY");
                if (boot_expect("OK")) {
                    boot_internal_error(1);
                    soft_reset();
                }
                return;
            default:
                boot_print("NO");
                boot_internal_error(1);
                soft_reset();
                break;
        }
    }
}

enum load_seg get_segment(unsigned int vaddr) {
    if (vaddr >= SEG_K0_RAM_START && vaddr < SEG_K0_RAM_END) {
        return SEG_K0_RAM;
    } else if (vaddr >= SEG_K0_PROG_START && vaddr < SEG_K0_PROG_END) {
        return SEG_K0_PROG;
    } else if (vaddr >= SEG_K1_RAM_START && vaddr < SEG_K1_RAM_END) {
        return SEG_K1_RAM;
    } else if (vaddr >= SEG_K1_PROG_START && vaddr < SEG_K1_PROG_END) {
        return SEG_K1_PROG;
    }
    return SEG_UNKNOWN;
}

// triggered after "LOAD" is received
void load(void) {
    // get entry
    unsigned int entry;
    if (boot_expect("ENTR")) {
        boot_print("NO");
        boot_internal_error(1);
        soft_reset();
    }

    boot_read_blocking((char*)&entry, 4);
    boot_print("OK");

    // get pre-info
    unsigned int n_phdrs = 0, initspace = 0, ramspace = 0, progspace = 0, progmaxaddr = 0;
    while (!boot_expect("PHDR")) {
        if (n_phdrs >= MAX_LOAD_HEADERS) {
            boot_print("NO");
            boot_internal_error(1);
            soft_reset();
        }
        boot_read_blocking((char*)&load_headers[n_phdrs].vaddr, 4);
        boot_read_blocking((char*)&load_headers[n_phdrs].memsz, 4);
        boot_read_blocking((char*)&load_headers[n_phdrs].initsz, 4);

        load_headers[n_phdrs].seg = get_segment(load_headers[n_phdrs].vaddr);

        if (load_headers[n_phdrs].seg & SEG_IS_INITIALIZED) {
            initspace += load_headers[n_phdrs].initsz;
            ramspace += load_headers[n_phdrs].memsz;
        } else {
            progspace += load_headers[n_phdrs].memsz;
            unsigned int phys = physical_address((void*)load_headers[n_phdrs].vaddr);
            if (phys > progmaxaddr)
                progmaxaddr = phys;
        }

        boot_print("OK");
        n_phdrs++;
    }

    int r = create_initialization_map(initspace, progspace, progmaxaddr);
    if (r == 1)
        boot_print("NO");
    else if (r == 2)
        boot_print("LV");
    else if (r == 3)
        boot_print("WE");

    if (r) {
        boot_internal_error(1);
        soft_reset();
    }

    /*
    for (int i = 0; i < n_phdrs; i++) {
        if (load_headers[i].seg & SEG_IS_INITIALIZED) {
            mark_as_initialized(map, load_headers[i].vaddr, load_headers[i].initsz);
        }
    }
    */

    boot_print("OK");

    // get data
    unsigned int n_blocks, n = 0;
    unsigned int ptr;
    while (!boot_expect("DATA")) {
        if (load_headers[n].seg & SEG_IS_INITIALIZED) {
            // todo
        } else {
            ptr = physical_address((void*)load_headers[n].vaddr);
            boot_read_blocking((char*)&n_blocks, 4);
            for (int i = 0 ; i < n_blocks ; i++) {
                boot_read_blocking(block_buffer, BLOCK_SIZE);

                for (int j = 0 ; j < BLOCK_SIZE ; j += PAGE_SIZE) {
                    flash_page_erase(ptr + j);
                }
                for (int j = 0 ; j < BLOCK_SIZE ; j += ROW_SIZE) {
                    if (flash_write_row(block_buffer_phys, ptr + j)) {
                        boot_print("FL");
                        boot_internal_error(1);
                        soft_reset();
                    }
                }

                char * check_ptr = (char*)load_headers[n].vaddr;
                for (int j = 0 ; j < BLOCK_SIZE ; j++) {
                    if (check_ptr[j] != block_buffer[j]) {
                        boot_print("DC");
                        boot_internal_error(1);
                        soft_reset();
                    }
                }

                ptr += BLOCK_SIZE;
                boot_print("OK");
            }
        }
        boot_print("DONE");
        n++;
    }
    
    if (boot_expect("ALLCLEAR")) {
        boot_internal_error(1);
        soft_reset();
    }

    if (n_phdrs != n)
        boot_print("OK");
    else
        boot_print("NO");
    
    // done!
}

