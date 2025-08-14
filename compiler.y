%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *out;

int yylex(void);
void yyerror(const char *s);
%}

%union {
    int num;
    char *id;
}

%token <num> NUMBER
%token <id> ID
%token INT PRINT
%token ASSIGN PLUS SEMI LPAREN RPAREN

%type <id> expr

%%

program:
    program statement
    | statement
    ;

statement:
    INT ID ASSIGN expr SEMI {
        fprintf(out, "%s", $4);      // $4 is <id> (string with bytecode)
        fprintf(out, "STORE %s\n", $2); // $2 is <id>
    }
    | PRINT LPAREN ID RPAREN SEMI {
        fprintf(out, "LOAD %s\n", $3);
        fprintf(out, "PRINT\n");
    }
    ;

expr:
    NUMBER {
        char *code = malloc(50);
        sprintf(code, "PUSH %d\n", $1);
        $$ = code;
    }
    | ID PLUS ID {
        char *code = malloc(100);
        sprintf(code, "LOAD %s\nLOAD %s\nADD\n", $1, $3);
        $$ = code;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    out = fopen("program.byte", "w");
    if (!out) {
        perror("program.byte");
        return 1;
    }
    yyparse();
    fclose(out);
    return 0;
}
