# ELF-linker
Custom ELF Loader intended for pedagogic use

## Usage

```bash
./read_elf [options] FILE

    Display information about the given ELF file
    -H --help       Display this information
    -h              Display the ELF header
    -S              Display the sections headers
    -s              Display the symbol table
    -x <num|text>   Display the content of the section <num|text>
    -r              Display the relocation table
    -F file2        Fusion of file2 sections onto file
```

## Installation

Clone the project

```bash
git clone https://github.com/Karvyz/Prog5-linker
```

Go to the project directory

```bash
cd Prog5-linker
```

Install

```bash
./configure
make
sudo make install
```

Cleanup

```bash
make clean
make distclean
```

## Running Tests

To run tests, run the following command

```bash
make check
```


## Documentation

To access documentation you require [doxygen](https://www.doxygen.nl/download.html):

```bash
doxygen Documentation/Doxyfile
```

Open with your web browser

```bash
Documentation/html/index.html
```
