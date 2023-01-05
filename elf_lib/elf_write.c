#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "elf_lib/elf_lib.h"
#include "elf_lib/elf_utils.h"
#include "elf_lib/Phase2.h"


int bwrite_a_supprimer(void *ptr, size_t size, size_t nmemb, FILE *f) {
    int i = 0;
    char tmp[nmemb][size];
    int total = 0;

    for(i = 0; i<nmemb; i++){
        for(int j = 0; j<size; j++){
            // on inverse les octets
            tmp[i][j] = ((char *)ptr)[i*size+j];
            printf("%d\n",tmp[i][j]);
        }
    }

    for(i = 0; i<nmemb; i++){ // pour chaque bloc
        for(int j = 0; j<size; j++){ // pour chaque octet
            if(fputc(tmp[nmemb-i-1][size-j-1], f) == EOF)
                if(total == 0)
                    return 0;
                else
                    return total;
            else
                total++;
        }
    }
    return total;
}




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

/*
new_header.e_ident[EI_MAG0] = ELFMAG0;
new_header.e_ident[EI_MAG1] = ELFMAG1;
new_header.e_ident[EI_MAG2] = ELFMAG2;
new_header.e_ident[EI_MAG3] = ELFMAG3;
new_header.e_ident[EI_CLASS] = ELFCLASS32;
new_header.e_ident[EI_DATA] = ELFDATA2MSB;
new_header.e_ident[EI_VERSION] = EV_CURRENT;
new_header.e_ident[EI_PAD] = 0;
new_header.e_type = elf_h.e_type;
new_header.e_machine = elf_h.e_machine;
new_header.e_version = elf_h.e_version;
new_header.e_entry = elf_h.e_entry;
new_header.e_phoff = elf_h.e_phoff;
new_header.e_shoff = sizeof(Elf32_Ehdr);
new_header.e_flags = elf_h.e_flags;
new_header.e_ehsize = elf_h.e_ehsize;
new_header.e_phentsize = elf_h.e_phentsize;
new_header.e_phnum = elf_h.e_phnum;
new_header.e_shentsize = elf_h.e_shentsize;
new_header.e_shnum = 0;
new_header.e_shstrndx = SHN_UNDEF;
*/


/*
Elf32_Ehdr elf_header;
    // Remplissez la structure elf_header avec les valeurs appropriées...
    if (fwrite(&elf_header, sizeof(Elf32_Ehdr), 1, out) != 1) {
        perror("fwrite");
        fclose(out);
        return;
    }

    for (int i = 0; i < elf_header.e_shnum; i++) {
        Elf32_Shdr* section = sections + i;
        // Allouez de l'espace pour la section
        void* data = malloc(section->sh_size);
        if (!data) {
            perror("malloc");
            fclose(out);
            return;
        }
        // Lis la section dans le fichier ELF
        fseek(file, section->sh_offset, SEEK_SET);
        if (fread(data, section->sh_size, 1, file) != 1) {
            perror("fread");
            free(data);
            fclose(out);
            return;
        }
        // Écrivez la section dans le fichier .o
        if (fwrite(data, section->sh_size, 1, out) != 1) {
            perror("fwrite");
            free(data);
            fclose(out);
            return;
    }
    free(data);
}

fclose(out);
 */