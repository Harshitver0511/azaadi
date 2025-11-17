# Makefile for Project Azaadi

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

all: azaadi-compiler azaadi-vm

azaadi-compiler: azaadi_compiler.c azaadi_bytecode.h
	$(CC) $(CFLAGS) -o azaadi-compiler azaadi_compiler.c

azaadi-vm: azaadi_vm.c azaadi_bytecode.h
	$(CC) $(CFLAGS) -o azaadi-vm azaadi_vm.c

clean:
	rm -f azaadi-compiler azaadi-vm *.azb

test: all
	@echo "\n╔════════════════════════════════════════════════════╗"
	@echo "║          Testing Azaadi Compiler & VM            ║"
	@echo "╚════════════════════════════════════════════════════╝\n"
	
	@echo "Test 1: Compiling test.c"
	@echo "-------------------------"
	./azaadi-compiler test.c test.azb
	@echo ""
	./azaadi-vm test.azb
	@echo "\n"

.PHONY: all clean test