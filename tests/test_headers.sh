#!/bin/bash
for file in Exemples_ELF32_Files/*.o
do
    ./read_elf -h $file > tmp1
    arm-none-eabi-readelf -h $file > tmp2
    if ! cmp -s tmp1 tmp2; then
        echo Test failed on file $file with option -h
        rm tmp1 tmp2
        exit 1
    fi
    rm tmp1 tmp2
done