%{
#include <stdlib.h>
#include <string.h>

#include "piga.h"
#include "ppp.yacc.h"

unsigned int linenum = 1;
%}

%%
[A-Z]+ {
/* 	printf("type: %s\n", yytext); */
	yylval.str = strdup(yytext);
	return TYPE;
}

[0-9!$]+ {
/* 	printf("bool: %s\n", yytext); */
	yylval.str = strdup(yytext);
	return BOOL;
}

([a-z0-9_]+:){2}[a-z0-9_]+ {
/* 	printf("sid: %s\n", yytext); */
	yylval.str = strdup(yytext);
	return SID;
}

[a-z_]+ {
/* 	printf("word: %s\n", yytext); */
	yylval.str = strdup(yytext);
	return WORD;
}

[ \t]		; /* ignore whitespace */
^\n		{ ++linenum; } /* ignore empty lines */
^#(.)*\n	{ ++linenum; } /* ignore comments: lines starting with # */
\n		{ ++linenum; return EOL; }
<<EOF>>		{ return ENDF; }
-\(		{ return AVPERMSTART; }
\)->		{ return AVPERMEND; }
[-:{}();]	{ return yytext[0]; }

. {
	fprintf(stderr,"LEX: Unrecognised field at line %d: '%s'.\n", linenum, yytext);
	return yytext[0];
}
%%
