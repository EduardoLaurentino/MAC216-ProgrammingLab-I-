#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

     .
	 .
	 .

	 
void exec_maquina(Maquina *m, int n) {


     .
	 .
	 .


	switch (opc) {
	  OPERANDO tmp;
	  OPERANDO op1;
	  OPERANDO op2;
	  OPERANDO res;
	  
	case PUSH:
	  empilha(pil, arg);
	  break;
	case POP:
	  desempilha(pil);
	  break;
	case DUP:
	  tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);
	  break;
	case ADD:
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n  + op2.val.n;
		empilha(pil, res);
	  }
	  break;
     .
	 .
	 .

}
