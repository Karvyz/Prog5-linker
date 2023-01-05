#!/bin/bash

cmp_elf() {
    ./read_elf $1 $2 > tmp1
    arm-none-eabi-readelf $1 $2 > tmp2
    if ! cmp -s tmp1 tmp2; then
        printf 'Test fail on file "%s" with the argument "%s"\n' "$file" "$1"
    fi
    rm tmp1 tmp2
}

cmp_elfx() {
    for num_section in {0..5}
    do
        ./read_elf -x $num_section $file > tmp1
        arm-none-eabi-readelf -x $num_section $file > tmp2

        for word in $(cat tmp1)
        do
            if ! grep -q "$word" tmp2; then
                printf 'Test fail on file "%s" with the argument "%s"\n' "$word" "$1"
                
            fi
        done
        rm tmp1 tmp2
    done
}


for file in Exemples_ELF32_Files/*.o
do
    cmp_elf -h $file
    cmp_elf -s $file
    cmp_elfx
    cmp_elf -S $file
done