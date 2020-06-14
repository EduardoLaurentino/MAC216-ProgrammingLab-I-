D [0-9]
L [a-zA-Z_]
H [a-zA-Z_0-9]
E [Ee][+-]?{D}+

%{
#include <math.h>
#include "compila.tab.h"
%}


%%
{D}*[.]?{D}+{E}?	{ yylval.val = atof(yytext); return NUMt; }

ret(urn)?  { return RETt; }

move       { return MOVt; }
poe        { return POEt; }
extrai     { return EXTt; }
ataque     { return ATKt; }

aqui       { return 6; }
leste      { return 1; }
nordeste   { return 0; }
sudeste    { return 2; }
oeste      { return 4; }
sudoeste   { return 3; }
noroeste   { return 5; }

celula     { CEL; }
terreno    { return 0; }
cristais   { return 1; }
ocupado    { return 2; }
base       { return 3; }

if         { return IF; }
else       { return ELSE; }
while	   { return WHILE; }
func|def   { return FUNC; }
print      { return PRINT; }


{L}{H}* {
  strncpy(yylval.cod, yytext, 30);
  return ID;
}

[ \t]+	/* pula espacos em branco */

\n yylineno++;

#.*		/* comentarios simples */

[.]     { return PONTO; }
;		{ return EOL; }
,		{ return SEP; }
=		{ return ASGN; }
[(]		{ return OPEN; }
[)]		{ return CLOSE; }
[{]		{ return ABRE; }
[}]		{ return FECHA; }
[+]		{ return ADDt; }
[-]		{ return SUBt; }
[*]		{ return MULt; }
[/]		{ return DIVt; }
==		{ return EQt; }
!=		{ return NEt; }
"<"		{ return LTt; }
"<="		{ return LEt; }
">"		{ return GTt; }
">="		{ return GEt; }

.	{  fprintf(stderr, "Entrada ignorada\n-->%s\n", yytext); }

%%

/* Local Variables: */
/* mode: c */
/* End: */
