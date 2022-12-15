/**
 * @file     elf_lib.c
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf_lib/elf_lib.h"

void init_header(FILE *f, Elf32_Ehdr *elf_h){
    unsigned char e_ident[EI_NIDENT];
    assert(fread(&e_ident, EI_NIDENT, 1, f));

    // Check of magic number and file's class (ELF32)
    if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 ||
        e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_CLASS] != ELFCLASS32){
        fprintf(stderr, "Error, this is not a ELF32 file\n");
        exit(EXIT_FAILURE);
    }

    // Test if this file is in big endian
    if (e_ident[EI_DATA] != ELFDATA2MSB){
        fprintf(stderr, "Error, this file is not in big endian\n");
        exit(EXIT_FAILURE);
    }

    // Copy of e_ident in elf_h->e_ident
    memcpy(elf_h->e_ident, e_ident, EI_NIDENT);

    // Read of file's header
    // TODO
    // read in big endian

    assert(bread(&elf_h->e_type, sizeof(elf_h->e_type), 1, f));
    assert(bread(&elf_h->e_machine, sizeof(elf_h->e_machine), 1, f));
    assert(bread(&elf_h->e_version, sizeof(elf_h->e_version), 1, f));
    assert(bread(&elf_h->e_entry, sizeof(elf_h->e_entry), 1, f));
    assert(bread(&elf_h->e_phoff, sizeof(elf_h->e_phoff), 1, f));
    assert(bread(&elf_h->e_shoff, sizeof(elf_h->e_shoff), 1, f));
    assert(bread(&elf_h->e_flags, sizeof(elf_h->e_flags), 1, f));
    assert(bread(&elf_h->e_ehsize, sizeof(elf_h->e_ehsize), 1, f));
    assert(bread(&elf_h->e_phentsize, sizeof(elf_h->e_phentsize), 1, f));
    assert(bread(&elf_h->e_phnum, sizeof(elf_h->e_phnum), 1, f));
    assert(bread(&elf_h->e_shentsize, sizeof(elf_h->e_shentsize), 1, f));
    assert(bread(&elf_h->e_shnum, sizeof(elf_h->e_shnum), 1, f));
    assert(bread(&elf_h->e_shstrndx, sizeof(elf_h->e_shstrndx), 1, f));
}



void print_elf(FILE *f, Elf32_Ehdr elf_h){

}