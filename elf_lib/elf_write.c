#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf_lib/elf_lib.h"
#include "elf_lib/elf_utils.h"
#include "elf_lib/Phase2.h"




void write_header(Elf32_Ehdr elf_h, FILE *f){

    unsigned char e_ident[EI_NIDENT];

    e_ident[EI_MAG0] = ELFMAG0;
    e_ident[EI_MAG1] = ELFMAG1;
    e_ident[EI_MAG2] = ELFMAG2;
    e_ident[EI_MAG3] = ELFMAG3;
    e_ident[EI_CLASS] = ELFCLASS32;
    e_ident[EI_DATA] = ELFDATA2MSB;
    e_ident[EI_VERSION] = EV_CURRENT;
    e_ident[EI_PAD] = 0;

    // Ecriture du header dans le fichier resultat
    fwrite(&e_ident, EI_NIDENT, 1, f);

    assert(bwrite(&elf_h.e_type, sizeof(elf_h.e_type), 1, f));
    assert(bwrite(&elf_h.e_machine, sizeof(elf_h.e_machine), 1, f));
    assert(bwrite(&elf_h.e_version, sizeof(elf_h.e_version), 1, f));
    assert(bwrite(&elf_h.e_entry, sizeof(elf_h.e_entry), 1, f));
    assert(bwrite(&elf_h.e_phoff, sizeof(elf_h.e_phoff), 1, f));
    assert(bwrite(&elf_h.e_shoff, sizeof(elf_h.e_shoff), 1, f));
    assert(bwrite(&elf_h.e_flags, sizeof(elf_h.e_flags), 1, f));
    assert(bwrite(&elf_h.e_ehsize, sizeof(elf_h.e_ehsize), 1, f));
    assert(bwrite(&elf_h.e_phentsize, sizeof(elf_h.e_phentsize), 1, f));
    assert(bwrite(&elf_h.e_phnum, sizeof(elf_h.e_phnum), 1, f));
    assert(bwrite(&elf_h.e_shentsize, sizeof(elf_h.e_shentsize), 1, f));
    assert(bwrite(&elf_h.e_shnum, sizeof(elf_h.e_shnum), 1, f));
    assert(bwrite(&elf_h.e_shstrndx, sizeof(elf_h.e_shstrndx), 1, f));

}



void write_main(Elf32_Ehdr elf_h){
    FILE* out = fopen("test_ecriture_elf.o", "wb");
    if (!out) {
        perror("fopen");
        return;
    }
    write_header(elf_h, out);
    fclose(out);

}

