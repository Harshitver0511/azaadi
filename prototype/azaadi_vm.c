// azaadi_vm.c
// Stack-based Virtual Machine for Azaadi bytecode

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "azaadi_bytecode.h"

#define STACK_SIZE 256

typedef struct {
    int32_t stack[STACK_SIZE];
    int sp;  // Stack pointer
    Instruction* instructions;
    int instruction_count;
    int ip;  // Instruction pointer
} VM;

void vm_init(VM* vm) {
    vm->sp = -1;
    vm->ip = 0;
}

void push(VM* vm, int32_t value) {
    if (vm->sp >= STACK_SIZE - 1) {
        fprintf(stderr, "Runtime Error: Stack overflow\n");
        exit(1);
    }
    vm->stack[++vm->sp] = value;
}

int32_t pop(VM* vm) {
    if (vm->sp < 0) {
        fprintf(stderr, "Runtime Error: Stack underflow\n");
        exit(1);
    }
    return vm->stack[vm->sp--];
}

void execute(VM* vm) {
    while (vm->ip < vm->instruction_count) {
        Instruction inst = vm->instructions[vm->ip++];
        
        switch (inst.opcode) {
            case OP_PUSH:
                push(vm, inst.operand);
                break;
                
            case OP_ADD: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, a + b);
                break;
            }
            
            case OP_SUB: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, a - b);
                break;
            }
            
            case OP_MUL: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                push(vm, a * b);
                break;
            }
            
            case OP_DIV: {
                int32_t b = pop(vm);
                int32_t a = pop(vm);
                if (b == 0) {
                    fprintf(stderr, "Runtime Error: Division by zero\n");
                    exit(1);
                }
                push(vm, a / b);
                break;
            }
            
            case OP_PRINT:
                printf("═══════════════════════════════════\n");
                printf("  Result: %d\n", vm->stack[vm->sp]);
                printf("═══════════════════════════════════\n");
                break;
                
            case OP_HALT:
                return;
                
            default:
                fprintf(stderr, "Runtime Error: Unknown opcode %d\n", inst.opcode);
                exit(1);
        }
    }
}

void load_bytecode(VM* vm, const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open bytecode file\n");
        exit(1);
    }
    
    // Read and verify header
    uint32_t magic;
    fread(&magic, sizeof(uint32_t), 1, f);
    if (magic != 0x415A4144) {
        fprintf(stderr, "Error: Invalid bytecode file\n");
        exit(1);
    }
    
    fread(&vm->instruction_count, sizeof(int), 1, f);
    
    // Allocate and read instructions
    vm->instructions = malloc(sizeof(Instruction) * vm->instruction_count);
    fread(vm->instructions, sizeof(Instruction), vm->instruction_count, f);
    
    fclose(f);
    printf("✓ Loaded %d instructions\n", vm->instruction_count);
}

int main(int argc, char* argv[]) {
    printf("╔════════════════════════════════════╗\n");
    printf("║   Azaadi VM v0.1 🖥️               ║\n");
    printf("╚════════════════════════════════════╝\n\n");
    
    if (argc != 2) {
        printf("Usage: %s <bytecode.azb>\n", argv[0]);
        printf("Example: %s program.azb\n\n", argv[0]);
        return 1;
    }
    
    VM vm;
    vm_init(&vm);
    
    printf("📂 Loading bytecode: %s\n", argv[1]);
    load_bytecode(&vm, argv[1]);
    printf("▶️  Executing...\n\n");
    execute(&vm);
    
    free(vm.instructions);
    
    return 0;
}