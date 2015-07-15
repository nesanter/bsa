#define IS_BOOTLOADER

#include "boot/loader.h"
#include "boot/bootlib.h"
#include "ulib/util.h"

int verify_elf_header(elf32_header *ehdr) {
    if (ehdr->e_ident[0] != EI_0 ||
        ehdr->e_ident[1] != EI_1 ||
        ehdr->e_ident[2] != EI_2 ||
        ehdr->e_ident[3] != EI_3) {
        boot_print("Bad ELF magic\r\n");
        return 1;
    }
    if (ehdr->e_ident[4] != EI_4 ||
        ehdr->e_ident[5] != EI_5 ||
        ehdr->e_ident[6] != EI_6 ||
        ehdr->e_type != ET_EXEC ||
        ehdr->e_machine != EM_MIPS ||
        ehdr->e_version != EV_CURRENT) {
        boot_print("Incorrect ELF header\r\n");
        return 1;
    }
    if (ehdr->e_flags != EF_EXPECTED) {
        boot_print("Warning: unexpected ELF flags\r\n");
    }
    return 0;
}

void boot_copy_to_memory(unsigned int address, unsigned char *data, unsigned int nbytes) {
    boot_print("Copying 0x");
    boot_print(tohex(nbytes, 8));
    boot_print(" bytes to 0x");
    boot_print(tohex(address, 8));
    boot_print("\r\n");
}

void boot_copy_zero_to_memory(unsigned int address, unsigned int nbytes) {
    boot_print("Zeroing 0x");
    boot_print(tohex(nbytes, 8));
    boot_print(" bytes at 0x");
    boot_print(tohex(address, 8));
    boot_print("\r\n");
}

int load_user_program(unsigned int *entry) {
    unsigned char *current_buffer = 0;
    int phase = 0, current_offset = 0, n, count = 0, i;
    int read_sz, write_sz, transfer_size;

    elf32_header ehdr;
    elf32_pheader tmp;
    elf32_pheader phdr[8];

    int n_phdrs = 0;
    
    current_buffer = &transfer_buffer[1];

    while (1) {
        boot_transfer_enable();
        while (!transfer_ready);
        transfer_ready = 0;
        transfer_size = *(unsigned short*)transfer_buffer;
        if (transfer_size == 0) {
            boot_print("Warning: recieved transfer of empty block\r\n");
        }
        current_offset += transfer_size;
        //process buffer
        switch (phase) {
            case 0: // read ELF header
                ehdr = *(elf32_header*)current_buffer;
                if (verify_elf_header(&ehdr)) {
                    return 1;
                }
                *entry = ehdr.e_entry;
                n = ehdr.e_phoff;
                //fall through
            case 1: // read program headers (most likely right after ELF header)
                while (n < current_offset) {
                    if (current_offset - n > ehdr.e_phentsize) {
                        // program header is split accross buffer boundary
                        // this situation is not yet handled
                        boot_print("Encountered situation not currently handled by bootloader\r\n");
                        return 1;
                    }
                    phdr[n_phdrs] = *(elf32_pheader*)(&current_buffer[n]);
                    if (phdr[n_phdrs].p_type == PT_LOAD) {
                        n_phdrs++;
                    }
                    count++;
                    n += ehdr.e_phentsize;
                    if (count == ehdr.e_phnum) {
                        if (n_phdrs == 0) {
                            boot_print("Found no useful program headers\r\n");
                        }
                        phase = 2;
                        break;
                    }
                }
                if (phase == 1)
                    break;
                // fall through
            case 2: // sort useful program headers
                for (i = 1; i < n_phdrs;) {
                    if (phdr[i].p_offset < phdr[i-1].p_offset) {
                        tmp = phdr[i-1];
                        phdr[i-1] = phdr[i];
                        phdr[i] = phdr[i-1];
                    } else {
                        i++;
                    }
                }
                phase = 3;
                count = 0;
                n = phdr[0].p_offset;
                read_sz = phdr[0].p_filesz;
                write_sz = phdr[0].p_memsz;
                // fall through
            case 3: // process headers
                while (read_sz < phdr[count].p_filesz) {
                    if (n >= current_offset) {
                        break;
                    }
                    i = phdr[count].p_filesz + phdr[count].p_offset - n;
                    if (i > current_offset) {
                        i = current_offset - n;
                    }
                    boot_copy_to_memory(phdr[count].p_vaddr + write_sz, &current_buffer[n], i);
                    write_sz += i;
                    read_sz += i;
                    n += i;
                }
                while (read_sz == phdr[count].p_filesz && write_sz < phdr[count].p_memsz) {
                    boot_copy_zero_to_memory(phdr[count].p_vaddr + write_sz, phdr[count].p_memsz - write_sz);
                }
                count++;
                if (count == n_phdrs) {
                    phase = 4;
                } else {
                    break;
                }
                // fall through
            case 4: // finish
                boot_print("CRC ");
                boot_print(tohex(boot_get_crc(), 8));
                boot_print("\r\n");
                return 0;
        }
    }
}


