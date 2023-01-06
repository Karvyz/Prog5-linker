#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf_lib/elf_lib.h"
#include "elf_lib/elf_utils.h"
#include "elf_lib/Phase2.h"




void write_header(FILE *f, Elf32_Ehdr elf_h){
    unsigned char e_ident[EI_NIDENT];

    e_ident[EI_MAG0] = ELFMAG0;
    e_ident[EI_MAG1] = ELFMAG1;
    e_ident[EI_MAG2] = ELFMAG2;
    e_ident[EI_MAG3] = ELFMAG3;
    e_ident[EI_CLASS] = ELFCLASS32;
    e_ident[EI_DATA] = ELFDATA2MSB;
    e_ident[EI_VERSION] = EV_CURRENT;
    e_ident[EI_PAD] = 0;

    //ecriture des nombres magiques en little endian
    fwrite(&e_ident, EI_NIDENT, 1, f);


    //ecriture du reste du header en big endian
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
    printf("ecriture du header ok\n");
}

void write_sections(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH){
    // Go to the beginning of the section header table
    assert(fseek(f, elf_h.e_shoff, SEEK_SET) == 0);

    // Read the section header table
    for (int i = 0; i < elf_h.e_shnum; i++){
        assert(bwrite(&arr_elf_SH[i].sh_name, sizeof(arr_elf_SH[i].sh_name), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_type, sizeof(arr_elf_SH[i].sh_type), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_flags, sizeof(arr_elf_SH[i].sh_flags), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_addr, sizeof(arr_elf_SH[i].sh_addr), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_offset, sizeof(arr_elf_SH[i].sh_offset), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_size, sizeof(arr_elf_SH[i].sh_size), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_link, sizeof(arr_elf_SH[i].sh_link), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_info, sizeof(arr_elf_SH[i].sh_info), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_addralign, sizeof(arr_elf_SH[i].sh_addralign), 1, f));
        assert(bwrite(&arr_elf_SH[i].sh_entsize, sizeof(arr_elf_SH[i].sh_entsize), 1, f));
    }
    printf("ecriture des section ok\n");
}

void write_sections_header(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH){
    //TODO section name à recupèrer
    //Recherchez et ecrire les noms de chaque section
	//fseek(f, arr_elf_SH[elf_h.e_shstrndx].sh_offset, SEEK_SET);
    //read_from_shstrtab(arr_elf_SH[i].sh_name,read_section_names(fin, arr_elf_SH[elf_h.e_shstrndx])));
	//assert(fwrite(arr_elf_SH->sh_name, arr_elf_SH[elf_h.e_shstrndx].sh_size, 1, fout));
     
}



void write_main(Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH){
    FILE* out = fopen("test_ecriture_elf.o", "wb");
    if (!out) {
        perror("fopen");
        return;
    }
    write_header(out, elf_h);
    write_sections(out, elf_h, arr_elf_SH);
    write_sections_header(out, elf_h, arr_elf_SH);
    fclose(out);
}

