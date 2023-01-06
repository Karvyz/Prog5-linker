#!/bin/bash
for file in Exemples_ELF32_Files/*.o
do
    for num_section in {-1..20}
    do
        valgrind./read_elf -x $num_section $file 1> tmp1 2> error.txt
        if [ -s error.txt ]; then
            rm tmp1 error.txt
            exit 1
        done
        rm tmp1 error.txt
    done
done