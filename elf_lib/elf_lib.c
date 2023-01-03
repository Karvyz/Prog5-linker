/**
 * @file     elf_lib.c
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf_lib/elf_lib.h"
#include "elf_lib/elf_utils.h"

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



void print_elf(FILE *fout, Elf32_Ehdr elf_h){
    fprintf(fout, "ELF Header:\n");
    fprintf(fout, "  Magic:   ");
    for(int i = 0; i<EI_NIDENT; i++){
        fprintf(fout, "%.2x ",elf_h.e_ident[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Class:                             ");
    if (elf_h.e_ident[EI_CLASS] == ELFCLASS32) fprintf(fout, "ELF32\n");
    else fprintf(fout, "None\n");
    fprintf(fout, "  Data:                              ");

    if (elf_h.e_ident[EI_DATA] == ELFDATA2MSB) fprintf(fout, "2's complement, big endian\n");
    else if(elf_h.e_ident[EI_DATA] == ELFDATA2LSB) fprintf(fout, "2's complement, little endian\n");

    print_elf_version(fout, elf_h.e_version);
    print_OS_ABI(fout, elf_h.e_ident[EI_OSABI]);
    fprintf(fout, "  ABI Version:                       %d\n", elf_h.e_ident[EI_ABIVERSION]);
    print_elf_type(fout, elf_h.e_type);
    print_elf_machine(fout, elf_h.e_machine);
    fprintf(fout, "  Version:                           0x%1.x\n", elf_h.e_version);
    fprintf(fout, "  Entry point address:               0x%.1x\n", elf_h.e_entry);
    fprintf(fout, "  Start of program headers:          %d (bytes into file)\n", elf_h.e_phoff);
    fprintf(fout, "  Start of section headers:          %d (bytes into file)\n", elf_h.e_shoff);
    fprintf(fout, "  Flags:                             %#x, Version5 EABI\n",elf_h.e_flags);
    fprintf(fout, "  Size of this header:               %d (bytes)\n", elf_h.e_ehsize);
    fprintf(fout, "  Size of program headers:           %d (bytes)\n", elf_h.e_phentsize);
    fprintf(fout, "  Number of program headers:         %d\n", elf_h.e_phnum);
    fprintf(fout, "  Size of section headers:           %d (bytes)\n", elf_h.e_shentsize);
    fprintf(fout, "  Number of section headers:         %d\n", elf_h.e_shnum);
    fprintf(fout, "  Section header string table index: %d\n", elf_h.e_shstrndx);
}

void read_sections(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH){
    // Go to the beginning of the section header table
    assert(fseek(f, elf_h.e_shoff, SEEK_SET) == 0);

    // Read the section header table
    for (int i = 0; i < elf_h.e_shnum; i++){
        assert(bread(&arr_elf_SH[i].sh_name, sizeof(arr_elf_SH[i].sh_name), 1, f));
        assert(bread(&arr_elf_SH[i].sh_type, sizeof(arr_elf_SH[i].sh_type), 1, f));
        assert(bread(&arr_elf_SH[i].sh_flags, sizeof(arr_elf_SH[i].sh_flags), 1, f));
        assert(bread(&arr_elf_SH[i].sh_addr, sizeof(arr_elf_SH[i].sh_addr), 1, f));
        assert(bread(&arr_elf_SH[i].sh_offset, sizeof(arr_elf_SH[i].sh_offset), 1, f));
        assert(bread(&arr_elf_SH[i].sh_size, sizeof(arr_elf_SH[i].sh_size), 1, f));
        assert(bread(&arr_elf_SH[i].sh_link, sizeof(arr_elf_SH[i].sh_link), 1, f));
        assert(bread(&arr_elf_SH[i].sh_info, sizeof(arr_elf_SH[i].sh_info), 1, f));
        assert(bread(&arr_elf_SH[i].sh_addralign, sizeof(arr_elf_SH[i].sh_addralign), 1, f));
        assert(bread(&arr_elf_SH[i].sh_entsize, sizeof(arr_elf_SH[i].sh_entsize), 1, f));
    }
}

/* Print the section header table */
void print_sections_header(FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH) {
    // TODO : Print the section header table
}

/* Etape 3 */

void print_section_content(FILE *f, FILE *fout, Elf32_Shdr *elf_SH) {
    if(elf_SH->sh_size == 0) {
        fprintf(fout, "Section '%s' has no data.\n", read_from_shstrtab(elf_SH->sh_name));
        return;
    }
    fprintf(fout, "\n");
    fprintf(fout, "Hex dump of section '%s':", read_from_shstrtab(elf_SH->sh_name));
    fprintf(fout, "\n");

    fseek(f, elf_SH->sh_offset, SEEK_SET);

    for (int i = 0; i < elf_SH->sh_size; i++) {
        if (i%4 == 0) fprintf(fout, " ");
        if (i%16 == 0) fprintf(fout, "\n0x%08x ", i);
        fprintf(fout, "%.2x", fgetc(f));
    }
    fprintf(fout, "\n");

}

/* Etape 4 */

void read_symbols(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int *nb_sym){
    // Check if there is a symbol table
    Elf32_Shdr SymTab;
    if(!get_section_by_name(".symtab", elf_h.e_shnum, arr_elf_SH,&SymTab)) {
        fprintf(stderr, "No symbol table found.\n");
        *nb_sym = 0;
        return;
    }
    // Go to the beginning of the symbol table
    assert(fseek(f, SymTab.sh_offset, SEEK_SET) == 0);

    // Read the symbol table
    int i = 0; // Count the number of symbols
    for (i = 0; i < SymTab.sh_size / sizeof(Elf32_Sym) ; i++){ // Read all the symbols
        assert(bread(&arr_elf_ST[i].st_name, sizeof(arr_elf_ST[i].st_name), 1, f));
        assert(bread(&arr_elf_ST[i].st_value, sizeof(arr_elf_ST[i].st_value), 1, f));
        assert(bread(&arr_elf_ST[i].st_size, sizeof(arr_elf_ST[i].st_size), 1, f));
        assert(bread(&arr_elf_ST[i].st_info, sizeof(arr_elf_ST[i].st_info), 1, f));
        assert(bread(&arr_elf_ST[i].st_other, sizeof(arr_elf_ST[i].st_other), 1, f));
        assert(bread(&arr_elf_ST[i].st_shndx, sizeof(arr_elf_ST[i].st_shndx), 1, f));
    }
    *nb_sym = i;
}

/* Print one symbol */
void print_symbol(FILE *fout, Elf32_Shdr *arr_elf_Sym, Elf32_Sym elf_Sym) {
    fprintf(fout, "\t%08x", elf_Sym.st_value);
    fprintf(fout, "\t   0");

    print_st_type(fout, elf_Sym.st_info);
    print_st_bind(fout, elf_Sym.st_info);
    print_st_visibility(fout, elf_Sym.st_other);
    print_st_shndx(fout, elf_Sym.st_shndx);

    if(ELF32_ST_TYPE(elf_Sym.st_info) == STT_SECTION) {
        fprintf(fout, " %s", read_from_shstrtab(arr_elf_Sym[elf_Sym.st_shndx].sh_name));
    } else {
        fprintf(fout, "\t%s", read_from_strtab(elf_Sym.st_name));
    }
}

/* Print the symbol table */
void print_symbols(FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int nb_sym) {
    fprintf(fout, "\nSymbol table '.symtab' contains %d entries:\n", nb_sym);
    fprintf(fout, "   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

    for (int i = 0; i < nb_sym; i++) {
        fprintf(fout, "   ");
        if(i > 9)
            fprintf(fout, " %d:", i);
        else
            fprintf(fout, "  %d:", i);
        print_symbol(fout, arr_elf_SH, arr_elf_ST[i]);
        fprintf(fout, "\n");
    }
}

/* Etape 5 */

/* Etape 6 */

void fusion_sections(FILE *f1, FILE *f2) {
    // TODO : Fusionner les deux fichiers elf en un seul
    int nb_sym1 = 0;
    int nb_sym2 = 0;

    Elf32_Ehdr elf_h1;
    Elf32_Ehdr elf_h2;


}

