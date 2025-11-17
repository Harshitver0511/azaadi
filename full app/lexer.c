#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

typedef struct {
    const char *source;
    int pos;
    int line;
    int column;
    int length;
} Lexer;

Lexer *lexer_create(const char *source) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->pos = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->length = strlen(source);
    return lexer;
}

void lexer_free(Lexer *lexer) {
    free(lexer);
}

char lexer_current(Lexer *lexer) {
    if (lexer->pos >= lexer->length) return '\0';
    return lexer->source[lexer->pos];
}

char lexer_peek(Lexer *lexer) {
    if (lexer->pos + 1 >= lexer->length) return '\0';
    return lexer->source[lexer->pos + 1];
}

void lexer_advance(Lexer *lexer) {
    if (lexer->pos < lexer->length) {
        if (lexer->source[lexer->pos] == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
        lexer->pos++;
    }
}

void lexer_skip_whitespace(Lexer *lexer) {
    while (isspace(lexer_current(lexer))) {
        lexer_advance(lexer);
    }
}

void lexer_skip_comment(Lexer *lexer) {
    if (lexer_current(lexer) == '/' && lexer_peek(lexer) == '/') {
        while (lexer_current(lexer) != '\n' && lexer_current(lexer) != '\0') {
            lexer_advance(lexer);
        }
    } else if (lexer_current(lexer) == '/' && lexer_peek(lexer) == '*') {
        lexer_advance(lexer);
        lexer_advance(lexer);
        while (!(lexer_current(lexer) == '*' && lexer_peek(lexer) == '/')) {
            if (lexer_current(lexer) == '\0') break;
            lexer_advance(lexer);
        }
        lexer_advance(lexer);
        lexer_advance(lexer);
    }
}

Token *token_create(TokenType type, const char *value, int line, int col) {
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = col;
    return token;
}

void token_free(Token *token) {
    if (token->value) free(token->value);
    free(token);
}

Token *lexer_read_number(Lexer *lexer) {
    int start = lexer->pos;
    int line = lexer->line;
    int col = lexer->column;
    
    while (isdigit(lexer_current(lexer)) || lexer_current(lexer) == '.') {
        lexer_advance(lexer);
    }
    
    int length = lexer->pos - start;
    char *value = malloc(length + 1);
    strncpy(value, lexer->source + start, length);
    value[length] = '\0';
    
    Token *token = token_create(TOKEN_NUMBER, value, line, col);
    free(value);
    return token;
}

Token *lexer_read_identifier(Lexer *lexer) {
    int start = lexer->pos;
    int line = lexer->line;
    int col = lexer->column;
    
    while (isalnum(lexer_current(lexer)) || lexer_current(lexer) == '_') {
        lexer_advance(lexer);
    }
    
    int length = lexer->pos - start;
    char *value = malloc(length + 1);
    strncpy(value, lexer->source + start, length);
    value[length] = '\0';
    
    TokenType type = TOKEN_IDENTIFIER;
    if (strcmp(value, "int") == 0) type = TOKEN_INT;
    else if (strcmp(value, "char") == 0) type = TOKEN_CHAR;
    else if (strcmp(value, "float") == 0) type = TOKEN_FLOAT;
    else if (strcmp(value, "double") == 0) type = TOKEN_DOUBLE;
    else if (strcmp(value, "void") == 0) type = TOKEN_VOID;
    else if (strcmp(value, "if") == 0) type = TOKEN_IF;
    else if (strcmp(value, "else") == 0) type = TOKEN_ELSE;
    else if (strcmp(value, "while") == 0) type = TOKEN_WHILE;
    else if (strcmp(value, "for") == 0) type = TOKEN_FOR;
    else if (strcmp(value, "return") == 0) type = TOKEN_RETURN;
    else if (strcmp(value, "break") == 0) type = TOKEN_BREAK;
    else if (strcmp(value, "continue") == 0) type = TOKEN_CONTINUE;
    
    Token *token = token_create(type, value, line, col);
    free(value);
    return token;
}

Token *lexer_read_string(Lexer *lexer) {
    int line = lexer->line;
    int col = lexer->column;
    lexer_advance(lexer); // Skip opening quote
    
    int start = lexer->pos;
    while (lexer_current(lexer) != '"' && lexer_current(lexer) != '\0') {
        if (lexer_current(lexer) == '\\') lexer_advance(lexer);
        lexer_advance(lexer);
    }
    
    int length = lexer->pos - start;
    char *value = malloc(length + 1);
    strncpy(value, lexer->source + start, length);
    value[length] = '\0';
    
    lexer_advance(lexer); // Skip closing quote
    
    Token *token = token_create(TOKEN_STRING, value, line, col);
    free(value);
    return token;
}

Token *lexer_next_token(Lexer *lexer) {
    lexer_skip_whitespace(lexer);
    lexer_skip_comment(lexer);
    lexer_skip_whitespace(lexer);
    
    if (lexer_current(lexer) == '\0') {
        return token_create(TOKEN_EOF, NULL, lexer->line, lexer->column);
    }
    
    char c = lexer_current(lexer);
    int line = lexer->line;
    int col = lexer->column;
    
    if (isdigit(c)) return lexer_read_number(lexer);
    if (isalpha(c) || c == '_') return lexer_read_identifier(lexer);
    if (c == '"') return lexer_read_string(lexer);
    
    lexer_advance(lexer);
    
    switch (c) {
        case '+': return token_create(TOKEN_PLUS, "+", line, col);
        case '-': return token_create(TOKEN_MINUS, "-", line, col);
        case '*': return token_create(TOKEN_STAR, "*", line, col);
        case '/': return token_create(TOKEN_SLASH, "/", line, col);
        case '%': return token_create(TOKEN_PERCENT, "%", line, col);
        case '(': return token_create(TOKEN_LPAREN, "(", line, col);
        case ')': return token_create(TOKEN_RPAREN, ")", line, col);
        case '{': return token_create(TOKEN_LBRACE, "{", line, col);
        case '}': return token_create(TOKEN_RBRACE, "}", line, col);
        case '[': return token_create(TOKEN_LBRACKET, "[", line, col);
        case ']': return token_create(TOKEN_RBRACKET, "]", line, col);
        case ';': return token_create(TOKEN_SEMICOLON, ";", line, col);
        case ',': return token_create(TOKEN_COMMA, ",", line, col);
        case '&': return token_create(TOKEN_AMPERSAND, "&", line, col);
        case '|': return token_create(TOKEN_PIPE, "|", line, col);
        case '^': return token_create(TOKEN_CARET, "^", line, col);
        case '~': return token_create(TOKEN_TILDE, "~", line, col);
        case '!':
            if (lexer_current(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_NE, "!=", line, col);
            }
            return token_create(TOKEN_NOT, "!", line, col);
        case '=':
            if (lexer_current(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_EQ, "==", line, col);
            }
            return token_create(TOKEN_ASSIGN, "=", line, col);
        case '<':
            if (lexer_current(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_LE, "<=", line, col);
            }
            return token_create(TOKEN_LT, "<", line, col);
        case '>':
            if (lexer_current(lexer) == '=') {
                lexer_advance(lexer);
                return token_create(TOKEN_GE, ">=", line, col);
            }
            return token_create(TOKEN_GT, ">", line, col);
        default:
            return token_create(TOKEN_ERROR, NULL, line, col);
    }
}