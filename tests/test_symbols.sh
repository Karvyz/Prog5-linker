#!/bin/bash
for file in Exemples_ELF32_Files/*.o
do
    ./read_elf -s $file > tmp1
    arm-none-eabi-readelf -s $file > tmp2
    if ! cmp -s tmp1 tmp2; then
        echo error on $file with arg -s
        rm tmp1 tmp2
        exit 1
    fi
    rm tmp1 tmp2
done