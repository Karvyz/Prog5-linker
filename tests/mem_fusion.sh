f1=Examples_fusion/file1.o
f2=Examples_fusion/file2.o
    valgrind -q ./read_elf -F $f1 $f2  1> tmp1 2> error.txt
    if [ -s error.txt ]; then
        echo error memoire lors de la fusion $f1 $f2
        rm tmp1 error.txt
        exit 1
    fi
    rm tmp1 error.txt