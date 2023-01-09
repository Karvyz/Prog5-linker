#include "fusion_reloc.h"
#include "elf_utils.h"
#include "elf_lib.h"
#include <elf.h>
#include <string.h>
#include <stdlib.h>

Elf32_Addr new_offset_reloc(Elf32_Addr old_offset){
    Elf32_Addr new_offset;


    return new_offset;
}

Elf32_Word new_info_reloc(Elf32_Word old_info){

    Elf32_Word new_info;
    int symb = ELF32_R_SYM(old_info);
    int typint = ELF32_R_TYPE(old_info);
    
    return new_info

}


void fusion_relocation(FILE *f1, FILE *f2, FILE *result, Elf32_Ehdr elf_h1, Elf32_Shdr* elf_SH1, Elf32_Sym *elf_Sym1, Elf32_Ehdr elf_h2, Elf32_Shdr* elf_SH2, Elf32_Sym *elf_Sym2){
    //TODO recuperer offset de section pour le fichier final et fseek avant concatenation
    //TODO supprimer tab_base_check1 qui est inutile normalement


    //initialisation des variables
    int tab_base_check1[elf_h1.e_shnum];
    memset(tab_base_check1, 0, sizeof(tab_base_check1));
    int tab_base_check2[elf_h2.e_shnum];
    memset(tab_base_check2, 0, sizeof(tab_base_check2));
    //parcours des sections de f1 
    for (int i = 0; i < elf_h1.e_shnum; i++) {
        //verification si le type de section de f1 est relocation
        if (elf_SH1[i].sh_type == SHT_REL){
            Elf32_Rel relocation1;
            fseek(f1, elf_SH1[i].sh_offset, SEEK_SET);

            //Recuperation du nom de s1:
            char *name_section1 = read_from_shstrtab(elf_SH1[i].sh_name);
            //parcours de f2:
            for (int j; j < elf_h2.e_shnum; j++){
                //recuperation du nom de s2:
                char *name_section2 = read_from_shstrtab(elf_SH2[j].sh_name);
                //verification si le type de section de f2 est relocation et comparaison avec les nom dans f2
                if (elf_SH2[j].sh_type == SHT_REL && name_section1 == name_section2){
                    Elf32_Rel relocation2;
                    //concatenation r1 et r2
                    for (int i_sym = 0; i_sym < elf_SH1[i].sh_size / sizeof(Elf32_Rel); i_sym++){
                        //lecture dans le fichier 1
                        assert(bread(relocation1.r_offset, sizeof(relocation1.r_offset), 1, f1));
                        assert(bread(relocation1.r_info, sizeof(relocation1.r_info), 1, f1));

                        //ecriture dans le fichier 1
                        assert(bwrite(new_offset_reloc(relocation1.r_offset), sizeof(relocation1.r_offset), 1, result));
                        assert(bwrite(new_info_reloc(relocation1.r_info), sizeof(relocation1.r_info), 1, result));
                        
                        //marquer la sections comme traite
                        tab_base_check1[i]=1; 
                    }
                    fseek(f2, elf_SH2[j].sh_offset, SEEK_SET);
                    for (int j_sym = 0; j_sym < elf_SH1[j].sh_size / sizeof(Elf32_Rel); j_sym++){
                        //lecture dans le fichier 2
                        assert(bread(relocation2.r_offset, sizeof(relocation2.r_offset), 1, f2));
                        assert(bread(relocation2.r_info, sizeof(relocation2.r_info), 1, f2));
                        
                        //ecriture dans le fichier 1
                        assert(bwrite(new_offset_reloc(relocation2.r_offset), sizeof(relocation2.r_offset), 1, result));
                        assert(bwrite(new_info_reloc(relocation2.r_info), sizeof(relocation2.r_info), 1, result));
                    
                        //marquer la sections comme traite
                        tab_base_check2[j]=1; 

                    }

                }
                else{ //marquer la sections dans f2 comme traite car pas de reloc
                    tab_base_check2[j]=1; 
                }
                free(name_section2);

            }
            //si non :

                //ajout de r1 a la suite
            

    //ajout des r2 non fait à la suite
            
        
            
            free(name_section1);
        } else{
            //marquer la sections dans f1 comme traite car pas de reloc
            tab_base_check1[i]=1; 
        }

    }

}