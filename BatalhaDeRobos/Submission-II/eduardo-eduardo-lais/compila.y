/* Compilador */

%{
#include <stdio.h>
#include <math.h>
#include "symrec.h"
#include "acertos.h"
#include "instr.h"

int yylex();
void yyerror(char const *);
int compila(FILE *, INSTR *);
static int ip  = 0;					/* ponteiro de instruções */
static int mem = 6;					/* ponteiro da memória */
static INSTR *prog;
static int parmcnt = 0;		/* contador de parâmetros */

OPERANDO tmp;
int yylineno; //USAMOS PARA TRATAMENTO DE ERRO; INDICARÁ LINHA DE ERRO NO CÓDIGO

void AddInstr(OpCode op, OPERANDO valor) {
  prog[ip++] = (INSTR) {op,  {valor.t, {valor.val.n}}};
}
%}

/*  Declaracoes */
%union {
  double val;
  /* symrec *cod; */
  char cod[30];
}

/* %type  Expr */

%token <val> NUMt
%token <cod> ID
%token ADDt SUBt MULt DIVt ASGN OPEN CLOSE RETt EOL
%token EQt NEt LTt LEt GTt GEt ABRE FECHA SEP
%token IF ELSE WHILE FUNC PRINT PONTO CEL

%token MOVt POEt EXTt ATKt

%right ASGN
%left ADDt SUBt
%left MULt DIVt
%left NEG
%right PWR
%left LTt GTt LEt GEt EQt NEt


/* Gramatica */
%%

Programa: Comando
       | Programa Comando
	   ;

Comando: Expr EOL
       | Cond
       | Loop
       | Func
	   | PRINT Expr EOL { tmp.t = NUM; tmp.val.n = 0; AddInstr(PRN, tmp);}
       | RETt EOL {
                 tmp.t = NUM; tmp.val.n = 0;
		 	     AddInstr(LEAVE, tmp);
			     AddInstr(RET, tmp);
 			  }
       | RETt OPEN Expr CLOSE EOL {
                 tmp.t = NUM; tmp.val.n = 0;
		 	     AddInstr(LEAVE, tmp);
			     AddInstr(RET, tmp);
 		      }

       // NOVOS COMANDOS ~~ESPECÍFICOS~~
       | MOVt PONTO NUMt EOL { tmp.t = MOV; tmp.val.n = $3; AddInstr(SIS, tmp); }
       | POEt PONTO NUMt EOL { tmp.t = POR; tmp.val.n = $3; AddInstr(SIS, tmp); }
       | EXTt PONTO NUMt EOL { tmp.t = EXTR; tmp.val.n = $3; AddInstr(SIS, tmp); }
       | ATKt PONTO NUMt EOL { tmp.t = ATK; tmp.val.n = $3; AddInstr(SIS, tmp); }

	   /* | EOL {printf("--> %d\n", ip);} */
;

Expr: NUMt {  tmp.t = NUM; tmp.val.n = $1; AddInstr(PUSH, tmp);}
    | ID   {
	         symrec *s = getsym($1);
			 if (s==0) s = putsym($1); /* não definida */
             tmp.t = NUM; tmp.val.n = s->val;
			 AddInstr(RCL, tmp);
	       }
	| ID ASGN Expr {
	         symrec *s = getsym($1);
			 if (s==0) s = putsym($1); /* não definida */
             tmp.t = NUM; tmp.val.n = s->val;
			 AddInstr(STO, tmp);
 		 }
	 | CEL PONTO NUMt PONTO NUMt {
	                 tmp.t = NUM; tmp.val.n = $3;
	 		 AddInstr(PUSH,tmp);
	                 tmp.t = NUM; tmp.val.n = $5;
	 		 AddInstr(ATR, tmp);
        	 }
	| Chamada
    | Expr ADDt Expr { tmp.t = NUM; tmp.val.n = 0; AddInstr(ADD, tmp);}
	| Expr SUBt Expr { tmp.t = NUM; tmp.val.n = 0; AddInstr(SUB, tmp);}
	| Expr MULt Expr { tmp.t = NUM; tmp.val.n = 0; AddInstr(MUL, tmp);}
	| Expr DIVt Expr { tmp.t = NUM; tmp.val.n = 0; AddInstr(DIV, tmp);}
    | '-' Expr %prec NEG  { printf("  {CHS, 0},\n"); }
	| OPEN Expr CLOSE
	| Expr LTt Expr  { tmp.t = NUM; tmp.val.n = 0; AddInstr(LT, tmp);}
	| Expr GTt Expr  { tmp.t = NUM; tmp.val.n = 0; AddInstr(GT, tmp);}
	| Expr LEt Expr  { tmp.t = NUM; tmp.val.n = 0; AddInstr(LE, tmp);}
	| Expr GEt Expr  { tmp.t = NUM; tmp.val.n = 0; AddInstr(GE, tmp);}
	| Expr EQt Expr  { tmp.t = NUM; tmp.val.n = 0; AddInstr(EQ, tmp);}
	| Expr NEt Expr  { tmp.t = NUM; tmp.val.n = 0; AddInstr(NE, tmp);}
;

/*Cond: IF OPEN  Expr {
               salva_end(ip);
               tmp.t = NUM; tmp.val.n = 0;
               AddInstr(JIF, tmp);
         }
         CLOSE  Bloco {
           prog[pega_end()].op.val.n = ip;
           tmp.t = NUM; tmp.val.n = ip;
           salva_end(ip);
           AddInstr(JMP, tmp);
         };

Cond: Cond ELSE {
           prog[pega_end()].op.val.n = ip;
        }*/


If:   IF OPEN Expr {
  	  	 	   salva_end(ip);
               tmp.t = NUM; tmp.val.n = 0;
               AddInstr(DUP, tmp);
			   AddInstr(JIF, tmp);
 		 }
		 CLOSE  Bloco {
		   prog[pega_end()+1].op.val.n = ip;
        }

Cond: If
      | If {
         salva_end(ip-1);
         tmp.t = NUM; tmp.val.n = 0;
         AddInstr(JIT, tmp);
      } ELSE Bloco{
        prog[pega_end()+1].op.val.n = ip;
      }

Loop: WHILE OPEN  { salva_end(ip); }
	  		Expr  { salva_end(ip); tmp.t = NUM; tmp.val.n = 0; AddInstr(JIF, tmp); }
	  		CLOSE Bloco {
			  int ip2 = pega_end();
			  tmp.t = NUM; tmp.val.n = 0; AddInstr(JMP, tmp);
			  prog[ip2].op.val.n = ip;
			};

Bloco: ABRE Comandos FECHA ;

Comandos: Comando
    | Comandos Comando
	;

Func: FUNC ID
	  {
		salva_end(ip);
		tmp.t = NUM; tmp.val.n = 0; AddInstr(JMP, tmp);
		symrec *s = getsym($2);
		if (s==0) s = putsym($2);
		else {
		  yyerror("Função definida duas vezes\n");
		  YYABORT;
		}
		s->val = ip;
	  } OPEN
	  {
		newtab(0);
	  }
	  Args CLOSE  Bloco
	  {
		tmp.t = NUM; tmp.val.n = 0; AddInstr(LEAVE, tmp);
		tmp.t = NUM; tmp.val.n = 0; AddInstr(RET, tmp);
		prog[pega_end()].op.val.n = ip;
		deltab();
	  }
	  ;

Args:
	| ID {
	  	 putsym($1);
	  }
    | Args SEP ID {
	  	 putsym($3);
	  }
	;

Chamada: ID OPEN
		 {
			 parmcnt = 0;
			 /* posição da memória mais avançada */
		 }
		 ListParms
		 {
		   symrec *s = getsym($1);
		   if (s == 0) {
			 yyerror("Função não definida\n");
			 YYABORT;
		   }
           tmp.t = NUM; tmp.val.n = 0;
		   AddInstr(ENTRY, tmp);
		   /* Cópia dos parâmetros */
		   while (parmcnt > 0){
             tmp.t = NUM; tmp.val.n = --parmcnt;
			 AddInstr(STO, tmp);
           }
           tmp.t = NUM; tmp.val.n = s->val;
		   AddInstr(CALL, tmp);
		 }
	  	 CLOSE ;


ListParms:
	| Expr { parmcnt++;}
	| Expr { parmcnt++;} SEP ListParms
;

%%
extern FILE *yyin;

void yyerror(char const *msg) {
  fprintf(stderr,"ERRO na linha %d: %s", yylineno, msg); //TRATAMENTO DE ERRO: INDICA LINHA DO ERRO
}

int compilador(FILE *cod, INSTR *dest) {
  int r;
  yyin = cod;
  prog = dest;
  cleartab();
  ip = 0;
  r = yyparse();
  tmp.t = NUM; tmp.val.n = 0;
  AddInstr(END, tmp);
  return r;
}
