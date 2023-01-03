#!/bin/bash

cmp_elf() {
    ./read_elf $1 $2 > tmp1
    arm-none-eabi-readelf $1 $2 > tmp2
    if ! cmp -s tmp1 tmp2; then
        printf 'Test fail on file "%s" with the argument "%s"\n' "$file" "$1"
    fi
    rm tmp1 tmp2
}


for file in Exemples_ELF32_Files/*.o
do
    cmp_elf -h $file
    cmp_elf -s $file
done