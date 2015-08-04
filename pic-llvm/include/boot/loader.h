#ifndef LOADER_H
#define LOADER_H

#ifdef IS_BOOTLOADER


typedef struct {
    unsigned char   e_ident[16];
    unsigned short  e_type;
    unsigned short  e_machine;
    unsigned int    e_version;
    unsigned int    e_entry;
    unsigned int    e_phoff;
    unsigned int    e_shoff;
    unsigned int    e_flags;
    unsigned short  e_ehsize;
    unsigned short  e_phentsize;
    unsigned short  e_phnum;
    unsigned short  e_shentsize;
    unsigned short  e_shnum;
    unsigned short  e_shstridx;
} elf32_header;

#define ET_EXEC (2)
#define EM_MIPS (8)
#define EV_CURRENT (1)

#define EI_0 (0x7F)
#define EI_1 ('E')
#define EI_2 ('L')
#define EI_3 ('F')
#define EI_4 (1)
#define EI_5 (1)
#define EI_6 (EV_CURRENT)

#define EF_EXPECTED (0)

typedef struct {
    unsigned int    p_type;
    unsigned int    p_offset;
    unsigned int    p_vaddr;
    unsigned int    p_paddr;
    unsigned int    p_filesz;
    unsigned int    p_memsz;
    unsigned int    p_flags;
    unsigned int    p_align;
} elf32_pheader;

#define PT_NULL (0)
#define PT_LOAD (1)
#define PT_DYNAMIC (2)
#define PT_INTERP (3)
#define PT_NOTE (4)
#define PT_SHLIB (5)
#define PT_PHDR (6)
#define PT_MIPS_REGINFO (0x7000000)

int load_user_program(unsigned int *entry, unsigned int *stack, unsigned int *gp);
void __attribute__((noreturn)) start_user_program(unsigned int entry, unsigned int stack, unsigned int gp);

#else
#error "runtime cannot access loader.h"
#endif

#endif /* LOADER_H */
