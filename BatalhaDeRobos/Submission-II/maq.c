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
  "ATR",
  "SIS",
  "ENTRY",
  "LEAVE"
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

  m->ip = 0;
  m->prog = p;
  m->pil.topo = 0;
  m->ib = 0;

  //m->exec = cria_pilha();
  //m->pil = cria_pilha();

  m->energia = 1000;
  m->saude = 1000;
  //m->arma[0] = 30;
  //m->arma[1] = 50;
  //m->arma[2] = 80;
  //m->arma[3] = 140;
  m->isCiclo = 0;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

int new_frame(Maquina *m, int n) {
  int ibc = m->ib;
  if (ibc < MAXFRM-1) {
    m->bp[++m->ib] = n+ibc;
    return m->ib;
  }
  return -1;
}

int del_frame(Maquina *m) {
  if (m->ib > 0) return --m->ib;
  return -1;
}


/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)
#define topo2 (pil->topo)
//#define topo (exec->topo) // topo da pilha de execução

void exec_maquina(Maquina *m, int n) {
  int i;

  for (i = 0; i < n; i++) {
    OpCode   opc = prg[ip].instr;
    OPERANDO arg = prg[ip].op;

    OPERANDO tmp;
    OPERANDO op1;
    OPERANDO op2;

    D(printf("%3d: %-5.5s %d\n", ip, CODES[opc], arg.val.n));

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
        tmp.val.n = op1.val.n  + op2.val.n;
        empilha(pil, tmp);
      }
      break;
    case SUB:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.val.n = op2.val.n  - op1.val.n;
        empilha(pil, tmp);
      }
      break;
    case MUL:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.val.n = op1.val.n  * op2.val.n;
        empilha(pil, tmp);
      }
      break;
    case DIV:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.val.n = op2.val.n  / op1.val.n;
        empilha(pil, tmp);
      }
      break;

    case JMP:
      ip = arg.val.n;
      continue;
    case JIT:
      if (desempilha(pil).val.n != 0) {
        ip = arg.val.n;
        continue;
      }
      break;
    case JIF:
      if (desempilha(pil).val.n == 0) {
        ip = arg.val.n;
        continue;
      }
      break;

  case CALL:
    op1.t = NUM;
    op1.val.n = ip;
    empilha(exec, op1);
    ip = arg.val.n;
    continue;

  case RET:
    ip = desempilha(exec).val.n;
    break;

  case EQ:
    op1 = desempilha(pil);
    op2 = desempilha(pil);

    if (op1.t == NUM && op2.t == NUM) {
      tmp.t = NUM;
      if (op1.val.n == op2.val.n) tmp.val.n = 1;
      else tmp.val.n = 0;
      empilha(pil, tmp);
    }
    break;
  case GT:
    op1 = desempilha(pil);
    op2 = desempilha(pil);

    if (op1.t == NUM && op2.t == NUM) {
      tmp.t = NUM;
      if (op1.val.n < op2.val.n) tmp.val.n = 1;
      else tmp.val.n = 0;
      empilha(pil, tmp);
    }
    break;
  case GE:
    op1 = desempilha(pil);
    op2 = desempilha(pil);

    if (op1.t == NUM && op2.t == NUM) {
      tmp.t = NUM;
      if (op1.val.n <= op2.val.n) tmp.val.n = 1;
      else tmp.val.n = 0;
      empilha(pil, tmp);
    }
    break;
  case LT:
    op1 = desempilha(pil);
    op2 = desempilha(pil);

    if (op1.t == NUM && op2.t == NUM) {
      tmp.t = NUM;
      if (op1.val.n > op2.val.n) tmp.val.n = 1;
      else tmp.val.n = 0;
      empilha(pil, tmp);
    }
    break;
  case LE:
    op1 = desempilha(pil);
    op2 = desempilha(pil);

    if (op1.t == NUM && op2.t == NUM) {
      tmp.t = NUM;
      if (op1.val.n >= op2.val.n) tmp.val.n = 1;
      else tmp.val.n = 0;
      empilha(pil, tmp);
    }
    break;
  case NE:
    op1 = desempilha(pil);
    op2 = desempilha(pil);

    if (op1.t == NUM && op2.t == NUM) {
      tmp.t = NUM;
      if (op1.val.n != op2.val.n) tmp.val.n = 1;
      else tmp.val.n = 0;
      empilha(pil, tmp);
    }
    break;

  case STO:
    m->Mem[arg.val.n+m->bp[m->ib]] = desempilha(pil);
    break;
  case RCL:
    empilha(pil,m->Mem[arg.val.n+m->bp[m->ib]]);
    break;

  case END:
    topo2 = 0;
    return;
  case PRN:
    printf("%d\n", desempilha(pil).val.n);
    break;
  case ENTRY:
    new_frame(m, arg.val.n);
    break;
  case LEAVE:
    del_frame(m);
    break;

  case ATR:
    // desempilha a celula que esta no topo da pilha de dados
    // (estamos assumindo que é uma celula que estará no topo da pil)
    tmp = desempilha(pil);
    op1.t = tmp.t;
    // atribui ao tipo do operando1 o mesmo do operando que foi desempilhado

    switch (arg.val.n) {
      // tipos de arg.val.n: 0 = terreno, 1 = cristais, 2 = ocupado, 3 = base
      case (0):
        op1.val.n = m->Mem[tmp.val.n].val.cel.terreno;
        // armazena no op1.val.n o tipo de terreno presente na celula desempilhada
        break;
      case (1):
        op1.val.n =  m->Mem[tmp.val.n].val.cel.cristais;
        // armazena no op1.val.n a quantidade de cristais presente na celula desempilhada
        break;
      case (2):
        op1.val.n =  m->Mem[tmp.val.n].val.cel.ocupado;
        // armazena no op1.val.n 0: se celula desempilhada estiver desocupada
        // ou 1: se celula desempilhada estiver ocupada
        break;
      case (3):
        op1.val.n =  m->Mem[tmp.val.n].val.cel.base;
        // armazena no op1.val.n: -1, se nao for base
        // 0, se for base do exercito 0
        // 1, se for base do exercito 1
        break;
    }
    empilha(pil, op1);
    // empilha na pilha de dados o operando op1,
    // cujo atributo requerido está armazenado no "op1.val.n"
    break;


  // caso de chamada do sistema, que preenche o operando temporário com o tipo
  // de solicitação feita e, no val.n, 0 se a operação não deu certo e 1 se foi
  // bem sucedida (ver implementação do sistema em "arena.c")
  case SIS:
    tmp.t = arg.t;
    tmp.val.n = Sistema(arg, m);
    empilha(pil, tmp);
    break;
  }

  D(imprime(pil,5));
  D(puts("\n"));

  ip++;
  }
}
