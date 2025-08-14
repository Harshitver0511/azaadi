%{
#include "compiler.tab.h"
#include <string.h>
%}
%%

"int"           { return INT; }
"print"         { return PRINT; }

[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.id = strdup(yytext); return ID; }

"="             { return ASSIGN; }
"+"             { return PLUS; }
";"             { return SEMI; }
"("             { return LPAREN; }
")"             { return RPAREN; }

[ \t\n]         ;   // ignore whitespace
.               { return *yytext; }

%%

int yywrap() {
    return 1;
}
