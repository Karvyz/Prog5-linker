/**
 * @file     elf_lib.c
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf_lib.h"
#include "elf_utils.h"

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
        // assert(bread(&arr_elf_SH[i].sh_offset, sizeof(arr_elf_SH[i].sh_offset), 1, f));
        bread(&arr_elf_SH[i].sh_offset, sizeof(arr_elf_SH[i].sh_offset), 1, f);

        assert(bread(&arr_elf_SH[i].sh_size, sizeof(arr_elf_SH[i].sh_size), 1, f));
        assert(bread(&arr_elf_SH[i].sh_link, sizeof(arr_elf_SH[i].sh_link), 1, f));
        assert(bread(&arr_elf_SH[i].sh_info, sizeof(arr_elf_SH[i].sh_info), 1, f));
        assert(bread(&arr_elf_SH[i].sh_addralign, sizeof(arr_elf_SH[i].sh_addralign), 1, f));
        assert(bread(&arr_elf_SH[i].sh_entsize, sizeof(arr_elf_SH[i].sh_entsize), 1, f));
    }
}

/* Print the section header table */

void print_sections_header(FILE *fin, FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, char *shstrtab){
    //Intro
    fprintf(fout,"There are %d section headers, starting at offset 0x%x:\n\n",elf_h.e_shnum,elf_h.e_shoff);

    // Les noms de chaque section
    fprintf(fout,"Section Headers:\n");
    fprintf(fout, "  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

    char * section_name = NULL;
    
    for (int i = 0; i < elf_h.e_shnum; i++) {
        // Numéro sections
        fprintf(fout,"  [%2d] ",i);

        // Nom de la section
        section_name = read_from_shstrtab(arr_elf_SH[i].sh_name, shstrtab);
        int k = 0;
        for (; section_name[k] != '\0'; k++) {}
        if (k > 15) {
            strcpy(&section_name[12], "[...]\0");
        }
        fprintf(fout,"%-18s", section_name);

        // Gestion du type
        switch (arr_elf_SH[i].sh_type) {
	      case SHT_PROGBITS:
		    fprintf(fout, "%-16s", "PROGBITS");
		    break;
	      case SHT_SYMTAB:
            fprintf(fout, "%-16s", "SYMTAB");
            break;
	      case SHT_STRTAB:
            fprintf(fout, "%-16s", "STRTAB");
            break;
	      case SHT_RELA:
            fprintf(fout, "%-16s", "RELA");
            break;
          case SHT_HASH:
            fprintf(fout, "%-16s", "HASH");
            break;
	      case SHT_DYNAMIC:
            fprintf(fout, "%-16s", "DYNAMIC");
            break;	      
          case SHT_NOTE:
            fprintf(fout, "%-16s", "NOTE");
            break;
          case SHT_NOBITS:
            fprintf(fout, "%-16s", "NOBITS");
            break;
	      case SHT_REL:
            fprintf(fout, "%-16s", "REL");
            break;
	      case SHT_SHLIB:
            fprintf(fout, "%-16s", "SHLIB");
            break;
	      case SHT_DYNSYM:
            fprintf(fout, "%-16s", "DYNSYM");
            break;	      
          case SHT_LOPROC:
            fprintf(fout, "%-16s", "LOPROC");
            break;
	      case SHT_HIPROC:
            fprintf(fout, "%-16s", "HIPROC");
            break;
	      case SHT_LOUSER:
            fprintf(fout, "%-16s", "LOUSER");
            break;
	      case SHT_HIUSER:
            fprintf(fout, "%-16s", "HIUSER");
            break;
          case SHT_ARM_ATTRIBUTES:
            fprintf(fout, "%-16s", "ARM_ATTRIBUTES");
            break;
	      case SHT_NULL:
            fprintf(fout, "%-16s", "NULL");
            break;
	    }

        // Affichage de Addr ,Offset ,Size et ES
        fprintf(fout, "%08x ", arr_elf_SH[i].sh_addr);
        fprintf(fout, "%06x ", arr_elf_SH[i].sh_offset);
        fprintf(fout, "%06x ", arr_elf_SH[i].sh_size);
        fprintf(fout, "%02x ", arr_elf_SH[i].sh_entsize);



    ///////////////////// Gestion des flags///////////////////////////////
        char f[] = "\0\0\0";
        int j = 0;
        if (arr_elf_SH[i].sh_flags & SHF_WRITE) {f[j] = 'W'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_ALLOC) {f[j] = 'A'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_EXECINSTR) {f[j] = 'X'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_MERGE) {f[j] = 'M'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_STRINGS) {f[j] = 'S'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_INFO_LINK) {f[j] = 'I'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_LINK_ORDER) {f[j] = 'L'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_GROUP) {f[j] = 'G'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_TLS) {f[j] = 'T'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_COMPRESSED) {f[j] = 'C'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_MASKOS) {f[j] = 'o'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_MASKPROC) {f[j] = 'p'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_ORDERED) {f[j] = 'O'; j++;}
        if (arr_elf_SH[i].sh_flags & SHF_EXCLUDE) {f[j] = 'E'; j++;}
        fprintf(fout, "%3s ", f);
    /////////////////////////////////////////////////////////////////////

        // Affichage de Lk , Inf et Al
        fprintf(fout, "%2d", arr_elf_SH[i].sh_link);
        fprintf(fout, "%4d", arr_elf_SH[i].sh_info);
        fprintf(fout, "%3d", arr_elf_SH[i].sh_addralign);

        // Retour fin de ligne
        fprintf(fout,"\n");
        free(section_name);
    }
    // Legende des Flags
    fprintf(fout,"Key to Flags:\n  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n");
    fprintf(fout,"  L (link order), O (extra OS processing required), G (group), T (TLS),\n");
    fprintf(fout,"  C (compressed), x (unknown), o (OS specific), E (exclude),\n");
    fprintf(fout,"  D (mbind), y (purecode), p (processor specific)\n");
}

/* Etape 3 */

void print_section_content(FILE *f, FILE *fout, Elf32_Shdr *elf_SH, char *shstrtab) {
    // Uniquement affichage du nom de la section si elle est vide
    if(elf_SH->sh_size == 0) {
        char* tmp = read_from_shstrtab(elf_SH->sh_name, shstrtab);
        if (tmp == NULL) {return;}
        fprintf(fout, "Section '%s' has no data to dump.", tmp);
        free(tmp);
        return;
    }

    // Affichage du contenu de la section
    fprintf(fout, "\n");
    char* tmp = read_from_shstrtab(elf_SH->sh_name, shstrtab);
    if (tmp == NULL) {return;}
    fprintf(fout, "Hex dump of section '%s':", tmp);
    free(tmp);

    fseek(f, elf_SH->sh_offset, SEEK_SET);

    for (int i = 0; i < elf_SH->sh_size; i++) {
        if (i%4 == 0) fprintf(fout, " ");
        if (i%16 == 0) fprintf(fout, "\n  0x%08x ", i);
        fprintf(fout, "%.2x", fgetc(f));
    }
    fprintf(fout, "\n");

}

/* Etape 4 */

void    read_symbols(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int *nb_sym){
    // Check s'il y a une table des symboles
    Elf32_Shdr *SymTab = NULL;
    SymTab = malloc(sizeof(Elf32_Shdr));
    char *tmp = NULL;
    tmp = read_section_names(f, arr_elf_SH[elf_h.e_shstrndx]);
    if(!get_section_by_name(".symtab", elf_h.e_shnum, arr_elf_SH,SymTab, tmp)) {
        fprintf(stderr, "No symbol table found.\n");
        *nb_sym = 0;
        free(tmp);
        return;
    }
    free(tmp);
    // Go to le début de la table des symboles
    assert(fseek(f, SymTab->sh_offset, SEEK_SET) == 0);

    // Lecture de la table des symboles
    int i = 0; // Count the number of symbols
    for (i = 0; i < SymTab->sh_size / sizeof(Elf32_Sym) ; i++){
        // Lecture de tous les symboles
        assert(bread(&arr_elf_ST[i].st_name, sizeof(arr_elf_ST[i].st_name), 1, f));
        assert(bread(&arr_elf_ST[i].st_value, sizeof(arr_elf_ST[i].st_value), 1, f));
        assert(bread(&arr_elf_ST[i].st_size, sizeof(arr_elf_ST[i].st_size), 1, f));
        assert(bread(&arr_elf_ST[i].st_info, sizeof(arr_elf_ST[i].st_info), 1, f));
        assert(bread(&arr_elf_ST[i].st_other, sizeof(arr_elf_ST[i].st_other), 1, f));
        assert(bread_abs(&arr_elf_ST[i].st_shndx, sizeof(arr_elf_ST[i].st_shndx), 1, f));
    }
    *nb_sym = i;
    free(SymTab);
}

/* Print one symbol */
void print_symbol(FILE *fout, Elf32_Shdr *arr_elf_Sym, Elf32_Sym elf_Sym, const char *shstrtab, char *symstrtab) {
    // Affichage de toutes les données du symbole
    fprintf(fout, " %08x", elf_Sym.st_value);
    fprintf(fout, " %5d", elf_Sym.st_size);

    print_st_type(fout, elf_Sym.st_info);
    print_st_bind(fout, elf_Sym.st_info);
    print_st_visibility(fout, elf_Sym.st_other);
    print_st_shndx(fout, elf_Sym.st_shndx);

    if(ELF32_ST_TYPE(elf_Sym.st_info) == STT_SECTION) {
        char* tmp = read_from_shstrtab(arr_elf_Sym[elf_Sym.st_shndx].sh_name, shstrtab);
        if (tmp == NULL) {return;} 
        fprintf(fout, " %s", tmp);
        free(tmp);
    } else {
        char* tmp = read_from_strtab(elf_Sym.st_name, symstrtab);
        if (tmp == NULL) {return;} 
        fprintf(fout, " %s", tmp);
        free(tmp);
    }
}

/* Print the symbol table */
void print_symbols(FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int nb_sym, const char *shstrtab, char *symstrtab) {
    fprintf(fout, "\nSymbol table '.symtab' contains %d entries:\n", nb_sym);
    fprintf(fout, "   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

    // Affichage de la table des symboles
    for (int i = 0; i < nb_sym; i++) {
        fprintf(fout, "   ");
        fprintf(fout, "%3d:", i);
        print_symbol(fout, arr_elf_SH, arr_elf_ST[i], shstrtab, symstrtab);
        fprintf(fout, "\n");
    }
}

/* Etape 5 */

void print_relocation(Elf32_Ehdr elf_h, Elf32_Shdr* elf_SH, Elf32_Sym *elf_Sym, FILE *file, char* shstrtab, char* symstrtab){
    int bool = 1; // si il n'y a pas de relocation dans le fichier, reste à 1.
    // Parcours des sections en cherchant les relocalisations
    for (int i = 0; i < elf_h.e_shnum; i++) {
        //regarde si la section est de type relocation
        if (elf_SH[i].sh_type == SHT_REL) {
            //indique qu'il y a au moins une relocation.
            bool = 0;
            //recupere le nom de la section
            char* tmp = read_from_shstrtab(elf_SH[i].sh_name, shstrtab);
            //affiche les informations concernant la sectian
            printf("\nRelocation section '%s' at offset 0x%x contains %ld entr%s:\n", (tmp != NULL) ? tmp : "", elf_SH[i].sh_offset, elf_SH[i].sh_size / sizeof(Elf32_Rel), (elf_SH[i].sh_size / sizeof(Elf32_Rel) < 2) ? "y" : "ies");
            if (tmp != NULL) {
                free(tmp);
            }
            printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
            //avance dans le fichier jusqu'à la position de la section i
            fseek(file, elf_SH[i].sh_offset, SEEK_SET);
            Elf32_Rel relocations;
            
            //parcours la table de relocation
            for (int j = 0; j < elf_SH[i].sh_size / sizeof(Elf32_Rel); j++) {
                //lis les informations de relocation et les stockes dans une Elf32_rel temporaire
                //le r_offset contient la position du symbole relocalisé
                bread(&relocations.r_offset, sizeof(relocations.r_offset), 1, file);
                //r_info contient le type et l'index (dans la table des symboles) du symbole relocalisé
                assert(bread(&relocations.r_info, sizeof(relocations.r_info), 1, file));
                //utilisation de macro pour séparer les informations
                int symb = ELF32_R_SYM(relocations.r_info);
                int typint = ELF32_R_TYPE(relocations.r_info);
                //convertion de typint en char correspondant
                char* typechar = revert_define_type_relocation(typint);
                //recuperation du nom du symbole
                char * nomSymb;
                if(ELF32_ST_TYPE(elf_Sym[symb].st_info) == STT_SECTION) {
                    nomSymb = read_from_shstrtab(elf_SH[elf_Sym[symb].st_shndx].sh_name, shstrtab);
                } else {
                    nomSymb = read_from_strtab(elf_Sym[symb].st_name, symstrtab);
                }
                //affichage des lignes
                printf("%08x  %08x %-17s",
                        relocations.r_offset,
                        relocations.r_info, 
                        typechar);
                if (typint == 1 || typint == 2 || typint == 28 || typint == 29) {
                    printf(" %08x   %s", elf_Sym[symb].st_value, nomSymb);
                }
                printf("\n");
                //liberer la mémoire
                free(typechar);
                free(nomSymb);
            }
        }
    }
    //si aucun relocation, affiche ce message
    if (bool) {
        printf("\nThere are no relocations in this file.\n");
    }
}
