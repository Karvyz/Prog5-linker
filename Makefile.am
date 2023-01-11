ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS=-Wall -g #-Werror
AM_CFLAGS+=-D DEBUG
AM_CFLAGS+=-D WARNING
# Uncomment if performance when running with -DDEBUG is an issue
# Warning, if uncommented, issuing calls to debug functions during options
# parsing might result in debug flag incorrectly set to 0 for some files
#AM_CFLAGS+=-D CACHE_DEBUG_FLAG

LDADD=-lpthread

AUTOMAKE_OPTIONS=subdir-objects
SUBDIRS=
SUBDIRS+=. Examples_fusion

bin_PROGRAMS=
bin_PROGRAMS+=Options_and_debug_example read_elf

read_elf_SOURCES=elf_lib/elf_lib.h elf_lib/elf_lib.c elf_lib/elf_utils.h elf_lib/elf_utils.c elf_read/elf_read.c elf_lib/Phase2.h elf_lib/Phase2.c elf_lib/fusion_reloc.h elf_lib/fusion_reloc.c elf_lib/elf_write.h elf_lib/elf_write.c


COMMON=debug.h debug.c util.h util.c

EXTRA_DIST=filtre.pl
Options_and_debug_example_SOURCES=$(COMMON) Options_and_debug_example.c

EXTRA_DIST+=

TESTS = ./tests/test_headers.sh ./tests/test_section_headers.sh ./tests/test_realocations.sh ./tests/test_symbols.sh ./tests/test_content_section.sh ./tests/mem_headers.sh ./tests/mem_realocations.sh ./tests/mem_symbols.sh ./tests/mem_sections_headers.sh ./tests/mem_content_section.sh