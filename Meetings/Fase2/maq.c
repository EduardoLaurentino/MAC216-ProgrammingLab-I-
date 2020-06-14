#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

/* #define DEBUG */

#ifdef DEBUG
#  define D(X) X
char *CODES[] = {
  "PUSH",
  "POP",
  "DUP",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "JMP",
  "JIT",
  "JIF",
  "CALL",
  "RET",
  "EQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "NE",
  "STO",
  "RCL",
  "END",
  "PRN",
  "STL",
  "RCE",
  "ATR",
};

#else
#  define D(X)
#endif

static void Erro(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Maquina *cria_maquina(INSTR *p) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  
  m->ip.valor = 0;
  m->rbp.valor = 0; //novo registrador
  m->prog = p;
  m->energia = 1000;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define rbp (m->rbp) //novo registrador
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)
#define topo (exec->topo) // topo da pilha de execução

void exec_maquina(Maquina *m, int n) {
  int i;

  for (i = 0; i < n; i++) {
    OpCode   opc = prg[ip.valor].instr;
    OPERANDO arg = prg[ip.valor].op;

    OPERANDO tmp;
    OPERANDO op1;
    OPERANDO op2;

    D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg, arg));

    switch (opc) {
      OPERANDO tmp;
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
        tmp.t = NUM;
        tmp.valor = op1.valor + op2.valor;
        empilha(pil, tmp);
      }      
      break;
    case SUB:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.valor = op2.valor - op1.valor;
        empilha(pil, tmp);
      }      
      break;
    case MUL:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.valor = op1.valor * op2.valor;
        empilha(pil, tmp);
      }      
      break;
    case DIV:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.valor = op2.valor / op1.valor;
        empilha(pil, tmp);
      }      
      break;
      
    case JMP:
      ip = arg;
      continue;
    case JIT:
      if (desempilha(pil).valor != 0) {
        ip = arg;
        continue;
      }
      break;
    case JIF:
      if (desempilha(pil).valor == 0) {
        ip = arg;
        continue;
      }
      break;
  
    case CALL:
      empilha(exec, ip);
      empilha(exec, rbp); 
      ip = arg;
      rbp.valor = topo; 
      continue;
    case RET:
      rbp = desempilha(exec);
      ip = desempilha(exec);
      break;
    
    case EQ:
      op1 = desempilha(pil);
      op2 = desempilha(pil);
    
      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        if (op1.valor == op2.valor) tmp.valor = 1;
        else tmp.valor = 0;
        empilha(pil, tmp);
      }
      break;
  
    case GT:
      op1 = desempilha(pil);
      op2 = desempilha(pil);
    
      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        if (op1.valor < op2.valor) tmp.valor = 1;
        else tmp.valor = 0;
        empilha(pil, tmp);
      }
      break;
    case GE:
      op1 = desempilha(pil);
      op2 = desempilha(pil);
    
      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        if (op1.valor <= op2.valor) tmp.valor = 1;
        else tmp.valor = 0;
        empilha(pil, tmp);
      }
      break;
    case LT:
      op1 = desempilha(pil);
      op2 = desempilha(pil);
      
      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        if (op1.valor > op2.valor) tmp.valor = 1;
        else tmp.valor = 0;
        empilha(pil, tmp);
      }
      break;
    case LE:
      op1 = desempilha(pil);
      op2 = desempilha(pil);
    
      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        if (op1.valor >= op2.valor) tmp.valor = 1;
        else tmp.valor = 0;
        empilha(pil, tmp);
      }
      break;
    case NE:
      op1 = desempilha(pil);
      op2 = desempilha(pil);
    
      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        if (op1.valor != op2.valor) tmp.valor = 1;
        else tmp.valor = 0;
        empilha(pil, tmp);
      }
      break;

    case STO:
      m->Mem[arg.valor] = desempilha(pil);
      break;
    case RCL:
      empilha(pil, m->Mem[arg.valor]);
      break;
    case END:
      return;
    case PRN:
      tmp = desempilha(pil);
      printf("%d\n", tmp.t);
      printf("%d\n", tmp.valor);
      break;
	  
    case STL:
      exec->val[arg.valor + rbp.valor -1] = desempilha(pil);
      break;
    case RCE:
      empilha(pil, exec->val[arg.valor + rbp.valor -1]);
      break; 

    case ALC:
      topo = topo + arg.valor;
      break;
    case FRE:
      for(i = 0; i < arg.valor; i++)
        desempilha(exec);
      break;

    case SIS:
      tmp.valor = Sistema(desempilha(pil), m);
      tmp.t = NUM;
      empilha(pil, tmp);
      break;
    }
    
  D(imprime(pil,5));
  D(puts("\n"));

  ip.valor++;
  }
}
