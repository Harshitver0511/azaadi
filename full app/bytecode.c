#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bytecode.h"

Bytecode *bytecode_create() {
    Bytecode *bc = malloc(sizeof(Bytecode));
    bc->count = 0;
    bc->capacity = 8;
    bc->code = malloc(sizeof(Instruction) * bc->capacity);
    
    bc->constant_count = 0;
    bc->constant_capacity = 8;
    bc->constants = malloc(sizeof(double) * bc->constant_capacity);
    
    bc->string_count = 0;
    bc->strings = NULL;
    
    return bc;
}

void bytecode_free(Bytecode *bc) {
    free(bc->code);
    free(bc->constants);
    for (int i = 0; i < bc->string_count; i++) {
        free(bc->strings[i]);
    }
    free(bc->strings);
    free(bc);
}

void bytecode_emit(Bytecode *bc, Opcode op, int32_t operand) {
    if (bc->count >= bc->capacity) {
        bc->capacity *= 2;
        bc->code = realloc(bc->code, sizeof(Instruction) * bc->capacity);
    }
    
    bc->code[bc->count].op = op;
    bc->code[bc->count].operand = operand;
    bc->count++;
}

int bytecode_add_constant(Bytecode *bc, double value) {
    if (bc->constant_count >= bc->constant_capacity) {
        bc->constant_capacity *= 2;
        bc->constants = realloc(bc->constants, sizeof(double) * bc->constant_capacity);
    }
    
    bc->constants[bc->constant_count] = value;
    return bc->constant_count++;
}

int bytecode_add_string(Bytecode *bc, const char *str) {
    bc->string_count++;
    bc->strings = realloc(bc->strings, sizeof(char*) * bc->string_count);
    bc->strings[bc->string_count - 1] = strdup(str);
    return bc->string_count - 1;
}

void bytecode_write_file(Bytecode *bc, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s for writing\n", filename);
        return;
    }
    
    // Write magic number
    uint32_t magic = 0x415A4144; // "AZAD"
    fwrite(&magic, sizeof(uint32_t), 1, f);
    
    // Write constants
    fwrite(&bc->constant_count, sizeof(int), 1, f);
    fwrite(bc->constants, sizeof(double), bc->constant_count, f);
    
    // Write strings
    fwrite(&bc->string_count, sizeof(int), 1, f);
    for (int i = 0; i < bc->string_count; i++) {
        int len = strlen(bc->strings[i]);
        fwrite(&len, sizeof(int), 1, f);
        fwrite(bc->strings[i], sizeof(char), len, f);
    }
    
    // Write instructions
    fwrite(&bc->count, sizeof(int), 1, f);
    fwrite(bc->code, sizeof(Instruction), bc->count, f);
    
    fclose(f);
    printf("Bytecode written to %s\n", filename);
}

Bytecode *bytecode_read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s for reading\n", filename);
        return NULL;
    }
    
    // Read and verify magic number
    uint32_t magic;
    fread(&magic, sizeof(uint32_t), 1, f);
    if (magic != 0x415A4144) {
        fprintf(stderr, "Error: Invalid bytecode file\n");
        fclose(f);
        return NULL;
    }
    
    Bytecode *bc = bytecode_create();
    
    // Read constants
    fread(&bc->constant_count, sizeof(int), 1, f);
    bc->constant_capacity = bc->constant_count;
    bc->constants = realloc(bc->constants, sizeof(double) * bc->constant_capacity);
    fread(bc->constants, sizeof(double), bc->constant_count, f);
    
    // Read strings
    fread(&bc->string_count, sizeof(int), 1, f);
    bc->strings = malloc(sizeof(char*) * bc->string_count);
    for (int i = 0; i < bc->string_count; i++) {
        int len;
        fread(&len, sizeof(int), 1, f);
        bc->strings[i] = malloc(len + 1);
        fread(bc->strings[i], sizeof(char), len, f);
        bc->strings[i][len] = '\0';
    }
    
    // Read instructions
    fread(&bc->count, sizeof(int), 1, f);
    bc->capacity = bc->count;
    bc->code = realloc(bc->code, sizeof(Instruction) * bc->capacity);
    fread(bc->code, sizeof(Instruction), bc->count, f);
    
    fclose(f);
    return bc;
}

const char *opcode_name(Opcode op) {
    switch (op) {
        case OP_HALT: return "HALT";
        case OP_PUSH: return "PUSH";
        case OP_POP: return "POP";
        case OP_ADD: return "ADD";
        case OP_SUB: return "SUB";
        case OP_MUL: return "MUL";
        case OP_DIV: return "DIV";
        case OP_MOD: return "MOD";
        case OP_NEG: return "NEG";
        case OP_EQ: return "EQ";
        case OP_NE: return "NE";
        case OP_LT: return "LT";
        case OP_LE: return "LE";
        case OP_GT: return "GT";
        case OP_GE: return "GE";
        case OP_AND: return "AND";
        case OP_OR: return "OR";
        case OP_NOT: return "NOT";
        case OP_LOAD: return "LOAD";
        case OP_STORE: return "STORE";
        case OP_JMP: return "JMP";
        case OP_JZ: return "JZ";
        case OP_CALL: return "CALL";
        case OP_RET: return "RET";
        case OP_PRINT: return "PRINT";
        default: return "UNKNOWN";
    }
}

void bytecode_print(Bytecode *bc) {
    printf("=== Azaadi Bytecode ===\n\n");
    
    printf("Constants:\n");
    for (int i = 0; i < bc->constant_count; i++) {
        printf("  [%d] %f\n", i, bc->constants[i]);
    }
    
    printf("\nInstructions:\n");
    for (int i = 0; i < bc->count; i++) {
        printf("  %04d: %-8s %d\n", i, 
               opcode_name(bc->code[i].op), 
               bc->code[i].operand);
    }
}