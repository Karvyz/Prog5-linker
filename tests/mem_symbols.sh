#!/bin/bash
for file in Exemples_ELF32_Files/*.o
do
    valgrind -q --leak-check=full --show-leak-kinds=all ./read_elf -s $file 1> tmp1 2> error.txt
    if [ -s error.txt ]; then
        echo Memory leak on file $file with option -s
        rm tmp1 error.txt
        exit 1
    fi
    rm tmp1 error.txt
done