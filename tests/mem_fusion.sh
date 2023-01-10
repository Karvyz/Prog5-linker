f1=Examples_fusion/file1.o
f2=Examples_fusion/file2.o

valgrind -q --leak-check=full --show-leak-kinds=all ./read_elf -F $f1 $f2  1> tmp1 2> error.txt
if [ -s error.txt ]; then
    echo Memory leak on file $f2 with option -r $f1
    rm tmp1 error.txt
    exit 1
fi
rm tmp1 error.txt