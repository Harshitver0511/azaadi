#include <stdio.h>

extern int yylex();
extern FILE *yyin;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename.c>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    yylex();
    fclose(yyin);
    return 0;
}
