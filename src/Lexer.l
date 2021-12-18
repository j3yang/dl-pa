%{
#include <stdio.h>
#include <stdlib.h>
#include "Formula.h"
#include "Program.h"
#include "Parser.hpp"

int lexer_verbose = 0;
%}

%%

[a-zA-Z][a-zA-Z0-9]*"("[ ,a-zA-Z0-9]*")"   { if (lexer_verbose) printf("Found paramaterized atomic variable: %s\n", yytext); yylval.name = strdup(yytext); return TOKEN_ATOM; }
[a-zA-Z][a-zA-Z0-9]*                       { if (lexer_verbose) printf("Found atomic variable: %s\n", yytext); yylval.name = strdup(yytext); return TOKEN_ATOM; }

"\\or"           { if (lexer_verbose) printf("Found OR: %s\n", yytext); return TOKEN_OR; }
"\\neg"          { if (lexer_verbose) printf("Found NEG: %s\n", yytext); return TOKEN_NEG; }
"\\top"          { if (lexer_verbose) printf("Found TOP: %s\n", yytext); return TOKEN_TRUE; }
"("              { if (lexer_verbose) printf("Found left paranthesis: %s\n", yytext); return TOKEN_LPAREN; }
")"              { if (lexer_verbose) printf("Found right paranthesis: %s\n", yytext); return TOKEN_RPAREN; }

"{"              { if (lexer_verbose) printf("Found left brace: %s\n", yytext); return TOKEN_LBRACE; }
"}"              { if (lexer_verbose) printf("Found right brace: %s\n", yytext); return TOKEN_RBRACE; }

"<"              { if (lexer_verbose) printf("Found left angle bracket: %s\n", yytext); return TOKEN_LANGLE; }
">"              { if (lexer_verbose) printf("Found right angle bracket: %s\n", yytext); return TOKEN_RANGLE; }
"<-"             { if (lexer_verbose) printf("Found GETS: %s\n", yytext); return TOKEN_GETS; }
"?"              { if (lexer_verbose) printf("Found TEST: %s\n", yytext); return TOKEN_TEST; }
"\\seq"          { if (lexer_verbose) printf("Found SEQ: %s\n", yytext); return TOKEN_SEQ; }
"\\cup"          { if (lexer_verbose) printf("Found CUP: %s\n", yytext); return TOKEN_CUP; }
"\\star"         { if (lexer_verbose) printf("Found STAR: %s\n", yytext); return TOKEN_STAR; }

[ \r\n\t]*       { continue; /* Skip whitespace */ }

.                { if (lexer_verbose) printf("FOUND UNEXPECTED: %s\n", yytext); continue; /* Found unexpected character */}

%%

int yyerror(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    return 0;
}
