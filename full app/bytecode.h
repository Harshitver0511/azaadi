#ifndef BYTECODE_H
#define BYTECODE_H

#include <stdint.h>

typedef enum {
    OP_HALT,        // Stop execution
    OP_PUSH,        // Push constant to stack
    OP_POP,         // Pop from stack
    OP_ADD,         // Add top two values
    OP_SUB,         // Subtract
    OP_MUL,         // Multiply
    OP_DIV,         // Divide
    OP_MOD,         // Modulo
    OP_NEG,         // Negate top value
    OP_EQ,          // Equal comparison
    OP_NE,          // Not equal
    OP_LT,          // Less than
    OP_LE,          // Less or equal
    OP_GT,          // Greater than
    OP_GE,          // Greater or equal
    OP_AND,         // Logical AND
    OP_OR,          // Logical OR
    OP_NOT,         // Logical NOT
    OP_LOAD,        // Load variable
    OP_STORE,       // Store to variable
    OP_JMP,         // Unconditional jump
    OP_JZ,          // Jump if zero
    OP_CALL,        // Call function
    OP_RET,         // Return from function
    OP_PRINT        // Print (built-in)
} Opcode;

typedef struct {
    Opcode op;
    int32_t operand;
} Instruction;

typedef struct {
    Instruction *code;
    int count;
    int capacity;
    
    double *constants;
    int constant_count;
    int constant_capacity;
    
    char **strings;
    int string_count;
} Bytecode;

Bytecode *bytecode_create();
void bytecode_free(Bytecode *bc);
void bytecode_emit(Bytecode *bc, Opcode op, int32_t operand);
int bytecode_add_constant(Bytecode *bc, double value);
int bytecode_add_string(Bytecode *bc, const char *str);
void bytecode_write_file(Bytecode *bc, const char *filename);
Bytecode *bytecode_read_file(const char *filename);
void bytecode_print(Bytecode *bc);

#endif