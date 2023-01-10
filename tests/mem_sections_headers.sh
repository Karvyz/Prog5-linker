#!/bin/bash
for file in Exemples_ELF32_Files/*.o
do
    valgrind -q ./read_elf -S $file 1> tmp1 2> error.txt
    if [ -s error.txt ]; then
        rm tmp1 error.txt
        exit 1
    fi
    rm tmp1 error.txt
done