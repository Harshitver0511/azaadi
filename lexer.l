%{
#include <stdio.h>
#include <string.h>
%}

%%
"int"|"float"|"char"|"double"|"void"                { printf("KEYWORD(%s)\n", yytext); }
"if"|"else"|"for"|"while"|"do"|"return"              { printf("KEYWORD(%s)\n", yytext); }
[0-9]+                                               { printf("NUMBER(%s)\n", yytext); }
\"([^\\\"]|\\.)*\"                                   { printf("STRING(%s)\n", yytext); }
[ \t\n]+                                             { /* skip whitespace */ }
[a-zA-Z_][a-zA-Z0-9_]*                               { printf("IDENTIFIER(%s)\n", yytext); }
"=="|"!="|"<="|">="|"<"|">"                          { printf("OPERATOR(%s)\n", yytext); }
"="|"+"|"-"|"*"|"/"|"%"                              { printf("OPERATOR(%s)\n", yytext); }
.                                                    { printf("UNKNOWN(%s)\n", yytext); }
%%

int yywrap() { return 1; }
