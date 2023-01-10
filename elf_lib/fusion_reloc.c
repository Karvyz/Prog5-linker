#include "fusion_reloc.h"
#include "elf_utils.h"
#include "elf_lib.h"
#include <elf.h>
#include <string.h>
#include <stdlib.h>

/*
Elf32_Addr offset_section_sortie(){
    
}*/


Elf32_Addr new_offset_reloc(Elf32_Addr old_offset){
    Elf32_Addr new_offset;
    //TODO : a modifie si chagement de offset
    new_offset = old_offset;
    return new_offset;
}

Elf32_Word new_info_reloc(Elf32_Word old_info){
    //TODO : a modifie
    /*Elf32_Word new_info;
    int new_symb;
    int symb = ELF32_R_SYM(old_info);
    int typint = ELF32_R_TYPE(old_info);

    //TODO recherche nouveau indice du symbole
    new_info = (new_symb << 8);
    */
    return old_info;
}

void lecture_ecriture_reloc(Elf32_Shdr elf_SH, FILE *f, FILE *result){
    for (int i_sym = 0; i_sym < elf_SH.sh_size / sizeof(Elf32_Rel); i_sym++){
        Elf32_Rel relocation;
        fseek(f, elf_SH.sh_offset, SEEK_SET);
        //lecture dans le fichier 1
        bread(&relocation.r_offset, sizeof(relocation.r_offset), 1, f);
        bread(&relocation.r_info, sizeof(relocation.r_info), 1, f);

        //fseek(result, offset_section_sortie(), SEEK_SET); //TODO : obtenir offset section
        //ecriture dans le fichier 1
        //bwrite(relocation.r_offset, sizeof(relocation.r_offset), 1, result);
        //bwrite(new_info_reloc(relocation.r_info), sizeof(relocation.r_info), 1, result);
        
    }
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

            //Recuperation du nom de s1:
            char *name_section1 = read_from_shstrtab(elf_SH1[i].sh_name);
            //parcours de f2:
            for (int j = 0; j < elf_h2.e_shnum; j++){
                //recuperation du nom de s2:
                char *name_section2 = read_from_shstrtab(elf_SH2[j].sh_name);
                //verification si le type de section de f2 est relocation et comparaison avec les nom dans f2
                if (elf_SH2[j].sh_type == SHT_REL && name_section1 == name_section2){

                    //concatenation r1 et r2
                    lecture_ecriture_reloc(elf_SH1[i], f1, result);

                    //marquer la sections du f1 comme traite
                    tab_base_check1[i]=1; 
                    lecture_ecriture_reloc(elf_SH2[j], f2, result);

                    //marquer la sections du f2 comme traite
                    tab_base_check2[j]=1; 

                }
                else{ //marquer la sections dans f2 comme traite car pas de reloc
                    tab_base_check2[j]=1; 
                }
                free(name_section2);

            }
            if (tab_base_check1[i] == 0){
                //ajouter r1
                lecture_ecriture_reloc(elf_SH1[i], f1, result);
            }
            //si non :
            
                //ajout de r1 a la suite
            
            free(name_section1);
        } else{
            //marquer la sections dans f1 comme traite car pas de reloc
            tab_base_check1[i]=1; 
        }

    }
    //ajouter des relocations non traité de r2
    for (int parcours = 0; parcours < elf_h2.e_shnum; parcours++){
        if (tab_base_check2[parcours] == 0){
            //ajouter r2

        }

    }

}