#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "ast.h"
#include "bytecode.h"
#include "codegen.c"
#include "parser.c"


// Forward declarations
extern void *lexer_create(const char *source);
extern void lexer_free(void *lexer);
extern void *parser_create(void *lexer);
extern ASTNode *parser_parse(void *parser);
extern Bytecode *codegen_generate(ASTNode *ast);

char *read_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return NULL;
    }
    
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    char *content = malloc(size + 1);
    fread(content, 1, size, f);
    content[size] = '\0';
    
    fclose(f);
    return content;
}

void compile(const char *input_file, const char *output_file) {
    printf("Compiling %s...\n", input_file);
    
    char *source = read_file(input_file);
    if (!source) return;
    
    // Lexical analysis
    void *lexer = lexer_create(source);
    
    // Parsing
    void *parser = parser_create(lexer);
    ASTNode *ast = parser_parse(parser);
    
    // Code generation
    Bytecode *bc = codegen_generate(ast);
    
    // Write bytecode
    bytecode_write_file(bc, output_file);
    
    printf("Compilation successful!\n");
    
    // Cleanup
    ast_free(ast);
    bytecode_free(bc);
    lexer_free(lexer);
    free(source);
}

int main(int argc, char **argv) {
    printf("====================================\n");
    printf("   Project Azaadi Compiler v1.0\n");
    printf("====================================\n\n");
    
    if (argc < 2) {
        printf("Usage: %s <input.c> [output.azb]\n", argv[0]);
        printf("\nExamples:\n");
        printf("  %s program.c           # Compiles to program.azb\n", argv[0]);
        printf("  %s program.c out.azb   # Compiles to out.azb\n", argv[0]);
        return 1;
    }
    
    const char *input_file = argv[1];
    char output_file[256];
    
    if (argc >= 3) {
        strcpy(output_file, argv[2]);
    } else {
        // Generate output filename
        strcpy(output_file, input_file);
        char *dot = strrchr(output_file, '.');
        if (dot) *dot = '\0';
        strcat(output_file, ".azb");
    }
    
    compile(input_file, output_file);
    
    return 0;
}