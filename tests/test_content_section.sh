#!/bin/bash
for file in Exemples_ELF32_Files/*.o
do
    for num_section in {-1..20}
    do
        ./read_elf -x $num_section $file 1> tmp1 2> error.txt
        arm-none-eabi-readelf -x $num_section $file 1> tmp2 2> error.txt

        for word in $(cat tmp1)
        do
            if ! grep -q "$word" tmp2; then
                rm tmp1 tmp2 error.txt
                exit 1
            fi
        done
        rm tmp1 tmp2 error.txt
    done
done