#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "boot/loader2.h"
#include "proc/segments.h"
#include "boot/flags.h"
#include "ulib/util.h"
#include "version.h"

/* Stack placement:
 *
 * Currently, a "dumb" placement strategy is used
 * that always places the stack at the end of available
 * memory.
 *
 * A rudimentary double-check is made to ensure there is
 * sufficient space beneath the end of memory for the minium
 * stack size.
 *
 * Since the runtime hardly uses this stack, it can be small,
 * and this strategy should work just fine.
 */

#define STACK_PADDING (0x40)
#define MIN_STACK_SIZE (0x200 + STACK_PADDING)

extern struct boot_status * volatile boot_status;

struct load_info {
    unsigned int vaddr,
                 memsz,
                 initsz;
    enum load_seg seg;
};

const unsigned int BLOCK_SIZE = 1024;
char __attribute__((aligned(4),section(".rt_data"))) block_buffer [1024];

const unsigned int MAX_LOAD_HEADERS = 8;
struct load_info load_headers [8];

const unsigned int MAP_GRANULARITY = 16; // in bytes
unsigned int map_phys_address = 0;
void * map_virt_address = 0;
unsigned int init_phys_start_address = 0;
unsigned int init_phys_cur_address = 0;
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
    if (initspace & PAGE_MASK)
        init_pages = 1 + (initspace / PAGE_SIZE);
    else
        init_pages = initspace / PAGE_SIZE;

    if (progmaxaddr == 0)
        return 4;

    if (progmaxaddr & PAGE_MASK)
        map_phys_address = progmaxaddr + (PAGE_SIZE - (progmaxaddr & PAGE_MASK));
    else
        map_phys_address = progmaxaddr;

    if (map_phys_address + (map_pages * PAGE_SIZE) + (init_pages * PAGE_SIZE) > SEG_PHYS_PROG_END)
        return 1;

    for (int i = 0 ; i < map_pages ; i++) {
        int r = flash_page_erase(map_phys_address + PAGE_SIZE * i);
        if (r & 0x1000)
            return 2;
        else if (r & 0x2000)
            return 3;
        else if (r)
            return 4;
    }

    // then record actual initspace address
    init_phys_start_address = map_phys_address + map_pages * PAGE_SIZE;
    init_phys_cur_address = init_phys_start_address;

    // and create it
    for (int i = 0 ; i < init_pages ; i++) {
        int r = flash_page_erase(init_phys_start_address + PAGE_SIZE * i);
        if (r & 0x1000)
            return 2;
        else if (r & 0x2000)
            return 3;
        else if (r)
            return 4;
    }

    // and virtual addresses
    map_virt_address = (void*)(map_phys_address + 0x80000000);
    init_virt_start_address = (void*)(init_phys_start_address + 0x80000000);

    return 0;
}

int set_initializer(unsigned int phys_addr, unsigned int * data) {
    // calculate place in map
    unsigned int slot = (phys_addr - SEG_PHYS_RAM_START) / MAP_GRANULARITY;
    unsigned int map_byte = slot >> 5; // word aligned
    unsigned int mask = 0xFFFFFFFF ^ (1 << (slot & 0x1F));

//    boot_print(tohex(mask, 8));

    if (flash_write_word(mask, map_phys_address + (map_byte << 2))) {
        return 1;
    }

    // record initial data
    for (int i = 0 ; i < (MAP_GRANULARITY >> 2) ; i++) {
        if (flash_write_word(data[i], init_phys_cur_address + 4 * i)) {
            return 2;
        }
    }

    init_phys_cur_address += MAP_GRANULARITY;

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
#ifdef SLOW_PORT_SWITCH
                asm volatile ("mtc0 $zero, $9");
                do {
                    asm volatile ("mfc0 %0, $9" : "=r" (count));
                } while (count < 40000000);
                boot_internal_error(1);
#endif
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
    // clear persistant info
//    boot_print(tohex((unsigned int)boot_status, 8));
    if ((unsigned int)boot_status & PAGE_MASK) {
        boot_print("PG");
        boot_internal_error(1);
        soft_reset();
    }
    int rrr = flash_page_erase(physical_address(boot_status));
    if (rrr == 1)
        boot_print("N1");
    else if (rrr & 0x1000)
        boot_print("N2");
    else if (rrr & 0x2000)
        boot_print("N3");
    else if (rrr)
        boot_print("N?");
    if (rrr) {
        boot_internal_error(1);
        soft_reset();
    }

    // get entry
    unsigned int entry;
    if (boot_expect("ENTR")) {
        boot_print("NO");
        boot_internal_error(1);
        soft_reset();
    }
    
    boot_print("OK");

    boot_read_blocking((char*)&entry, 4);
    
    boot_print("OK");

    // get pre-info (also double check safety of dumb stack placement)
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
            if (physical_address((void*)load_headers[n_phdrs].vaddr) + load_headers[n_phdrs].memsz > (SEG_PHYS_RAM_END - MIN_STACK_SIZE)) {
                boot_print(">S");
                boot_internal_error(1);
                soft_reset();
            }
        } else {
            progspace += load_headers[n_phdrs].memsz;
            unsigned int phys = physical_address((void*)load_headers[n_phdrs].vaddr) + load_headers[n_phdrs].memsz;
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
    else if (r)
        boot_print("??");

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

    unsigned int ptr;
    for (int i = 0 ; i < n_phdrs ; i++) {
        if (load_headers[i].seg & SEG_IS_INITIALIZED) {

        } else {
            ptr = physical_address((void*)load_headers[i].vaddr);
            ptr -= ptr & PAGE_MASK;

            for (int j = 0 ; j < load_headers[i].initsz ; j += PAGE_SIZE) {
                int r = flash_page_erase(ptr + j);
                if (r & 0x1000)
                    boot_print("V");
                if (r & 0x2000)
                    boot_print("X");
                if (r) {
                    boot_print("EP");
                    boot_internal_error(1);
                    soft_reset();
                }
            }

        }
    }

    boot_print("OK");

    unsigned int block_buffer_phys = physical_address(block_buffer);

    // get data
    unsigned int n_blocks, n = 0;
    while (!boot_expect("DATA")) {
        boot_read_blocking((char*)&n_blocks, 4);
        if (load_headers[n].seg & SEG_IS_INITIALIZED) {
            ptr = physical_address((void*)load_headers[n].vaddr);
            unsigned int memsz = 0;
            for (int i = 0 ; i < n_blocks ; i++) {
                boot_read_blocking(block_buffer, BLOCK_SIZE);
                for (int j = 0 ; j < BLOCK_SIZE ; j += MAP_GRANULARITY) {
                    unsigned int * word = (unsigned int *)(&block_buffer[j]);
                    for (int k = 0 ; k < MAP_GRANULARITY >> 2; k++) {
                        if (word[k]) {
                            int r = set_initializer(ptr + j, word);
                            if (r == 1)
                                boot_print("I1");
                            else if (r == 2)
                                boot_print("I2");
                            else if (r)
                                boot_print("I?");
                            if (r) {
                                boot_print("IN");
                                boot_internal_error(1);
                                soft_reset(1);
                            }
                            break;
                        }
                    }
                }
                ptr += BLOCK_SIZE;
                boot_print("OK");
            }
        } else {
            ptr = physical_address((void*)load_headers[n].vaddr);
            for (int i = 0 ; i < n_blocks ; i++) {
                boot_read_blocking(block_buffer, BLOCK_SIZE);

                unsigned int sz = BLOCK_SIZE;
                if (ptr & ROW_MASK) {
                    for (int j = 0 ; j < ROW_SIZE - (ptr & ROW_MASK); j += 4) {
                        if (flash_write_word(*(unsigned int*)(&block_buffer[j]), ptr + j)) {
                            boot_print("FW");
                        }
                    }
                    sz -= ROW_SIZE - (ptr & ROW_MASK);
                    ptr += (ptr & ROW_MASK) + ROW_SIZE;
                }

                for (int j = 0 ; j < sz ; j += ROW_SIZE) {
                    if (flash_write_row(block_buffer_phys + j, ptr + j)) {
                        boot_print("FR");
                        boot_internal_error(1);
                        soft_reset();
                    }
                }

                char * check_ptr = (char*)load_headers[n].vaddr + i * BLOCK_SIZE;
                for (int j = 0 ; j < BLOCK_SIZE ; j++) {
                    if (check_ptr[j] != block_buffer[j]) {
                        boot_print("DC");
                        boot_internal_error(1);
                        soft_reset();
                    }
                }

                ptr += sz;
                boot_print("OK");
            }
        }
        boot_print("DONE");
        n++;
    }

    if (boot_expect("ALLCLEAR")) {
        boot_print("ED");
        boot_internal_error(1);
        soft_reset();
    }

    if (n_phdrs != n) {
        boot_print("NO");
        boot_internal_error(1);
        soft_reset();
    }

    // write persistant info
    if (flash_write_word((unsigned int)map_virt_address, physical_address(&boot_status->init_map)) ||
        flash_write_word((unsigned int)init_virt_start_address, physical_address(&boot_status->init_data)) ||
        flash_write_word(entry, physical_address(&boot_status->runtime_entry))) {
        boot_print("NO");
        boot_internal_error(1);
        soft_reset();
    }

    boot_print("OK");

    // done!
    
    soft_reset();
}

void run(void) {
    // check for valid entry
    if (boot_status->runtime_entry == (void*)0 || boot_status->runtime_entry == (void*)0xFFFFFFFF) {
        boot_internal_error(1);
        soft_reset();
    }

    // load pre-init data
    unsigned int * init_map = boot_status->init_map;
    unsigned int * init_data = boot_status->init_data;
    unsigned int * dest = (unsigned int *)SEG_K0_RAM_START;
    unsigned int map_words = (SEG_RAM_SIZE / MAP_GRANULARITY) >> 5;

    /*
    boot_print(tohex((unsigned int)init_map, 8));
    boot_print(tohex((unsigned int)init_data, 8));
    boot_print(tohex((unsigned int)dest, 8));
    */

    for (int i = 0; i < map_words ; i++) {
        unsigned int word = init_map[i];
        for (int j = 0 ; j < 32 ; j++) {
            if (!(word & 0x1)) {
                for (int k = 0 ; k < MAP_GRANULARITY >> 2 ; k++) {
                    dest[k] = init_data[k];
                    /*
                    boot_print("@");
                    boot_print(tohex((unsigned int)&dest[k], 8));
                    boot_print(" = ");
                    boot_print(tohex((unsigned int)&init_data[k], 8));
                    boot_print(" (");
                    boot_print(tohex(init_data[k], 8));
                    boot_print(")\r\n");
                    */
                }
                init_data += (MAP_GRANULARITY >> 2);
            } else {
                
                for (int k = 0 ; k < MAP_GRANULARITY >> 2 ; k++) {
                    dest[k] = 0;
                }
                
            }
            dest += (MAP_GRANULARITY >> 2);
            word >>= 1;
        }
    }

    // enter runtime
    void * stack_ptr = (void*)(SEG_K1_RAM_END - STACK_PADDING);
    void * gp = 0; // does not actually need initialization
    void * entry = boot_status->runtime_entry;
    asm volatile ("move $sp, %0; \
                   move $gp, %1; \
                   move $t0, %2; \
                   jr $t0" : "+r" (stack_ptr),
                             "+r" (gp),
                             "+r" (entry));
}

void dump_map(void) {
    unsigned int * init_map = boot_status->init_map;
    unsigned int map_words = (SEG_RAM_SIZE / MAP_GRANULARITY) >> 5;

    for (int i = 0; i < map_words ; i += 2) {
        unsigned int word = init_map[i];
        for (int j = 0 ; j < 32 ; j++) {
            if (word & 0x1)
                boot_print("1");
            else
                boot_print("0");
            word >>= 1;
        }
        word = init_map[i+1];
        for (int j = 0 ; j < 32 ; j++) {
            if (word & 0x1)
                boot_print("1");
            else
                boot_print("0");
            word >>= 1;
        }
        boot_print("\r\n");
    }
}
