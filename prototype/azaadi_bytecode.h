// azaadi_bytecode.h
// Azaadi Bytecode Instruction Set

#ifndef AZAADI_BYTECODE_H
#define AZAADI_BYTECODE_H

#include <stdint.h>

// Bytecode instruction opcodes
typedef enum {
    OP_PUSH,    // Push value onto stack
    OP_ADD,     // Pop two values, add, push result
    OP_SUB,     // Pop two values, subtract, push result
    OP_MUL,     // Pop two values, multiply, push result
    OP_DIV,     // Pop two values, divide, push result
    OP_PRINT,   // Print top of stack
    OP_HALT     // Stop execution
} OpCode;

// Bytecode instruction structure
typedef struct {
    OpCode opcode;
    int32_t operand;  // Used for OP_PUSH
} Instruction;

#endif // AZAADI_BYTECODE_H