CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Compiler sources
COMPILER_SRC = compiler_main.c lexer.c parser.c ast.c bytecode.c codegen.c
COMPILER_OBJ = $(COMPILER_SRC:.c=.o)
COMPILER_BIN = azaadicc

# VM sources
VM_SRC = vm_main.c vm.c bytecode.c
VM_OBJ = $(VM_SRC:.c=.o)
VM_BIN = azaadi

.PHONY: all clean test

all: $(COMPILER_BIN) $(VM_BIN)

$(COMPILER_BIN): $(COMPILER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(VM_BIN): $(VM_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(COMPILER_OBJ) $(VM_OBJ) $(COMPILER_BIN) $(VM_BIN) *.azb

test: all
	@echo "=== Running Test ==="
	./$(COMPILER_BIN) test.c test.azb
	./$(VM_BIN) test.azb

help:
	@echo "Project Azaadi - Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build compiler and VM (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  test      - Run test program"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make              # Build everything"
	@echo "  make clean        # Clean build"
	@echo "  make test         # Test the system"