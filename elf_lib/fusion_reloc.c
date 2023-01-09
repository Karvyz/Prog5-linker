#include "fusion_reloc.h"
#include "elf_utils.h"
#include "elf_lib.h"
#include <elf.h>
#include <string.h>
#include <stdlib.h>


void fusion_relocation(FILE *f1, FILE *f2, FILE *result, Elf32_Ehdr elf_h1, Elf32_Shdr* elf_SH1, Elf32_Sym *elf_Sym1, Elf32_Ehdr elf_h2, Elf32_Shdr* elf_SH2, Elf32_Sym *elf_Sym2){
    //initialisation des variables
    
                
                
    //parcours des sections de f1 
    for (int i = 0; i < elf_h1.e_shnum; i++) {
        //verification si le type de section de f1 est relocation
        if (elf_SH1[i].sh_type == SHT_REL){

            //Recuperation du nom de s1:
            char *name_section1 = read_from_shstrtab(elf_SH1[i].sh_name);
            //parcours de f2:
            for (int j; j < elf_h2.e_shnum; j++){
                //recuperation du nom de s2:
                char *name_section2 = read_from_shstrtab(elf_SH2[j].sh_name);
                //verification si le type de section de f2 est relocation et comparaison avec les nom dans f2
                if (elf_SH2[j].sh_type == SHT_REL && name_section1 == name_section2){
                    //concatenation r1 et r2

                }
                free(name_section2)

            }
            //si non :

                //ajout de r1 a la suite
            

    //ajout des r2 non fait à la suite
            
        
        
            free(name_section1);
        }
    }

}