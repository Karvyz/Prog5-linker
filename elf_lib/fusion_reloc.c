#include "fusion_reloc.h"
#include "elf_utils.h"
#include "elf_lib.h"
#include <elf.h>
#include <string.h>
#include <stdlib.h>


void fusion_relocation(FILE *f1, FILE *f2, FILE *result, Elf32_Ehdr elf_h1, Elf32_Shdr* elf_SH1, Elf32_Sym *elf_Sym1, Elf32_Ehdr elf_h2, Elf32_Shdr* elf_SH2, Elf32_Sym *elf_Sym2){
    //initialisation des variables
    
    //parcours des sections de f1 et verification si le type de section de f1 est relocation

        //parcours et verification si le type de section de f2 est relocation 
        //et comparaison avec les nom dans f2, si oui:

            //concatenation r1 et r2

        //si non :

            //ajout de r1 a la suite
            

    //ajout des r2 non fait à la suite
            
        
        

        

}