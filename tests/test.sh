#!/bin/bash

cmp_elf() {
    ./read_elf $1 $2 > tmp1
    readelf $1 $2 > tmp2
    if ! cmp -s tmp1 tmp2; then
        printf 'Test fail with the argument "%s"\n' "$1"
    fi
    rm tmp1 tmp2
}


cmp_elf -h $1
cmp_elf -S $1
cmp_elf -s $1
cmp_elf -r $1
