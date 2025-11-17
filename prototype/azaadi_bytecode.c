// azaadi_compiler.c
// Compiler that reads C files and generates Azaadi bytecode

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "azaadi_bytecode.h"

#define MAX_INSTRUCTIONS 1024
#define MAX_SOURCE 10000

typedef struct {
    Instruction instructions[MAX_INSTRUCTIONS];
    int count;
} BytecodeProgram;

// Token types
typedef enum {
    TOK_NUMBER,
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_SEMICOLON,
    TOK_RETURN,
    TOK_INT,
    TOK_MAIN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
    char text[32];
} Token;

// Global state
char* source;
int pos = 0;
BytecodeProgram program;
Token current_token;

void emit(OpCode op, int32_t operand) {
    if (program.count >= MAX_INSTRUCTIONS) {
        fprintf(stderr, "Error: Too many instructions\n");
        exit(1);
    }
    program.instructions[program.count].opcode = op;
    program.instructions[program.count].operand = operand;
    program.count++;
}

// Skip whitespace and comments
void skip_whitespace() {
    while (source[pos]) {
        if (isspace(source[pos])) {
            pos++;
        } else if (source[pos] == '/' && source[pos+1] == '/') {
            // Skip single-line comment
            while (source[pos] && source[pos] != '\n') pos++;
        } else if (source[pos] == '/' && source[pos+1] == '*') {
            // Skip multi-line comment
            pos += 2;
            while (source[pos] && !(source[pos] == '*' && source[pos+1] == '/')) pos++;
            if (source[pos]) pos += 2;
        } else {
            break;
        }
    }
}

// Check if string matches at current position
int match_keyword(const char* keyword) {
    int len = strlen(keyword);
    if (strncmp(&source[pos], keyword, len) == 0 && !isalnum(source[pos + len])) {
        return 1;
    }
    return 0;
}

Token next_token() {
    Token tok;
    memset(&tok, 0, sizeof(Token));
    
    skip_whitespace();
    
    if (!source[pos]) {
        tok.type = TOK_EOF;
        return tok;
    }
    
    // Keywords
    if (match_keyword("return")) {
        tok.type = TOK_RETURN;
        pos += 6;
        return tok;
    }
    
    if (match_keyword("int")) {
        tok.type = TOK_INT;
        pos += 3;
        return tok;
    }
    
    if (match_keyword("main")) {
        tok.type = TOK_MAIN;
        pos += 4;
        return tok;
    }
    
    // Numbers
    if (isdigit(source[pos])) {
        tok.type = TOK_NUMBER;
        tok.value = 0;
        while (isdigit(source[pos])) {
            tok.value = tok.value * 10 + (source[pos] - '0');
            pos++;
        }
        return tok;
    }
    
    // Single character tokens
    char c = source[pos++];
    switch (c) {
        case '+': tok.type = TOK_PLUS; break;
        case '-': tok.type = TOK_MINUS; break;
        case '*': tok.type = TOK_STAR; break;
        case '/': tok.type = TOK_SLASH; break;
        case '(': tok.type = TOK_LPAREN; break;
        case ')': tok.type = TOK_RPAREN; break;
        case '{': tok.type = TOK_LBRACE; break;
        case '}': tok.type = TOK_RBRACE; break;
        case ';': tok.type = TOK_SEMICOLON; break;
        default:
            fprintf(stderr, "Error: Unknown character '%c'\n", c);
            exit(1);
    }
    
    return tok;
}

void parse_expression();

void parse_factor() {
    if (current_token.type == TOK_NUMBER) {
        emit(OP_PUSH, current_token.value);
        current_token = next_token();
    } else if (current_token.type == TOK_LPAREN) {
        current_token = next_token();
        parse_expression();
        if (current_token.type != TOK_RPAREN) {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(1);
        }
        current_token = next_token();
    } else {
        fprintf(stderr, "Error: Expected number or '('\n");
        exit(1);
    }
}

void parse_term() {
    parse_factor();
    
    while (current_token.type == TOK_STAR || current_token.type == TOK_SLASH) {
        TokenType op = current_token.type;
        current_token = next_token();
        parse_factor();
        
        if (op == TOK_STAR) {
            emit(OP_MUL, 0);
        } else {
            emit(OP_DIV, 0);
        }
    }
}

void parse_expression() {
    parse_term();
    
    while (current_token.type == TOK_PLUS || current_token.type == TOK_MINUS) {
        TokenType op = current_token.type;
        current_token = next_token();
        parse_term();
        
        if (op == TOK_PLUS) {
            emit(OP_ADD, 0);
        } else {
            emit(OP_SUB, 0);
        }
    }
}

void parse_return_statement() {
    if (current_token.type != TOK_RETURN) {
        fprintf(stderr, "Error: Expected 'return'\n");
        exit(1);
    }
    current_token = next_token();
    
    parse_expression();
    
    if (current_token.type != TOK_SEMICOLON) {
        fprintf(stderr, "Error: Expected ';'\n");
        exit(1);
    }
    current_token = next_token();
    
    emit(OP_PRINT, 0);
}

void parse_main_function() {
    // int main() {
    if (current_token.type != TOK_INT) {
        fprintf(stderr, "Error: Expected 'int'\n");
        exit(1);
    }
    current_token = next_token();
    
    if (current_token.type != TOK_MAIN) {
        fprintf(stderr, "Error: Expected 'main'\n");
        exit(1);
    }
    current_token = next_token();
    
    if (current_token.type != TOK_LPAREN) {
        fprintf(stderr, "Error: Expected '('\n");
        exit(1);
    }
    current_token = next_token();
    
    if (current_token.type != TOK_RPAREN) {
        fprintf(stderr, "Error: Expected ')'\n");
        exit(1);
    }
    current_token = next_token();
    
    if (current_token.type != TOK_LBRACE) {
        fprintf(stderr, "Error: Expected '{'\n");
        exit(1);
    }
    current_token = next_token();
    
    // Parse return statement
    parse_return_statement();
    
    if (current_token.type != TOK_RBRACE) {
        fprintf(stderr, "Error: Expected '}'\n");
        exit(1);
    }
    current_token = next_token();
}

void compile(const char* source_code) {
    source = strdup(source_code);
    pos = 0;
    program.count = 0;
    
    current_token = next_token();
    parse_main_function();
    
    emit(OP_HALT, 0);
    
    free(source);
}

char* read_file(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open source file '%s'\n", filename);
        exit(1);
    }
    
    char* buffer = malloc(MAX_SOURCE);
    size_t size = fread(buffer, 1, MAX_SOURCE - 1, f);
    buffer[size] = '\0';
    
    fclose(f);
    return buffer;
}

void save_bytecode(const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file for writing\n");
        exit(1);
    }
    
    // Write header
    uint32_t magic = 0x415A4144; // "AZAD" in hex
    fwrite(&magic, sizeof(uint32_t), 1, f);
    fwrite(&program.count, sizeof(int), 1, f);
    
    // Write instructions
    fwrite(program.instructions, sizeof(Instruction), program.count, f);
    
    fclose(f);
    printf("✓ Compiled successfully to %s (%d instructions)\n", filename, program.count);
}

int main(int argc, char* argv[]) {
    printf("╔════════════════════════════════════╗\n");
    printf("║   Azaadi Compiler v0.1 🚀         ║\n");
    printf("╚════════════════════════════════════╝\n\n");
    
    if (argc != 3) {
        printf("Usage: %s <source.c> <output.azb>\n", argv[0]);
        printf("Example: %s test.c output.azb\n\n", argv[0]);
        return 1;
    }
    
    printf("📄 Reading source file: %s\n", argv[1]);
    char* source_code = read_file(argv[1]);
    
    printf("⚙️  Compiling...\n");
    compile(source_code);
    
    save_bytecode(argv[2]);
    
    free(source_code);
    
    return 0;
}