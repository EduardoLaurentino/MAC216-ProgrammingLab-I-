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
  "SIS",
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
  m->exec = cria_pilha();
  m->pil = cria_pilha();
  m->saude = 1000;
  m->arma[0] = 30;
  m->arma[1] = 50;
  m->arma[2] = 80;
  m->arma[3] = 140;
  m->isCiclo = 0;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define rbp (m->rbp) //novo registrador
#define pil (m->pil)
#define exec (m->exec)
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
        tmp.valor = op1.valor  + op2.valor;
        empilha(pil, tmp);
      }
      break;
    case SUB:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.valor = op2.valor  - op1.valor;
        empilha(pil, tmp);
      }
      break;
    case MUL:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.valor = op1.valor  * op2.valor;
        empilha(pil, tmp);
      }
      break;
    case DIV:
      op1 = desempilha(pil);
      op2 = desempilha(pil);

      if (op1.t == NUM && op2.t == NUM) {
        tmp.t = NUM;
        tmp.valor = op2.valor  / op1.valor;
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
    rbp.valor = topo;
    ip.valor = arg.valor;
    continue;
  case RET:
    topo = rbp.valor;
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
    m->Mem[arg.valor + 6] = desempilha(pil); //o + 6 representa que os 6 primeiros espacos do vetor estarão guardando infos das 6 celulas vizinhas.
    break;
  case RCL:
    empilha(pil,m->Mem[arg.valor + 6]);
    break;
  case END:
    return;
  case PRN:
    tmp = desempilha(pil);
    printf("%d\n", tmp.t);
    printf("%d\n", tmp.valor);
    break;

  case STL:
      exec->val[arg.valor + rbp.valor - 1] = desempilha(pil); //Corrigido o erro em que STL desempilhava
      break;                                                  //da memoria. Agora desempilha da exec.
  case RCE:
      empilha(pil, exec->val[arg.valor + rbp.valor - 1]);    //Corrigido o erro em que RCE empilhava
      break;                                                 //na memoria. Agora empilha na exec.

  case ALC:
    topo = topo + arg.valor; //Aloca "arg" espaços na exec. (Implementado na Fase2)
    break;
  case FRE:
    topo = topo - arg.valor; //Desaloca "arg" espaços na exec. (Implementado na Fase2)
    break;

  case ATR:
    tmp = desempilha(pil); //desempilha a celula que esta no topo da pilha de dados. (estamos assumindo que é uma celula que estará no topo da pil)
    op1.t = tmp.t;         //Atribui o tipo do operando1 o mesmo do operando que foi desempilhado
    switch(arg.valor){     //tipos de arg.valor: 0 = terreno, 1 = cristais, 2 = ocupado, 3 = base
      case(0):
        op1.valor = tmp.val.cel.terreno;  // armazena no op1.valor o tipo de terreno presente na celula desempilhada
        break;
      case(1):
        op1.valor = tmp.val.cel.cristais;  // armazena no op1.valor a quantidade de cristais presente na celula desempilhada
        break;
      case(2):
        op1.valor = tmp.val.cel.ocupado;  // armazena no op1.valor 0: se celula desempilhada estiver desocupada ou 1: se celula desempilhada estiver ocupada
        break;
      case(3):
        op1.valor = tmp.val.cel.base;  // armazena no op1.valor: -1, se nao for base/ 0, se for base do exercito 0/ 1, se for base do exercito 1;
        break;
    }
    empilha(pil, op1) ; //empilha na pilha de dados o operando op1, cujo atributo requerido está armazenado no "op1.valor"
    break;

  case SIS: //caso de chamada do sistema, que preenche o operando temporário com o tipo de solicitação feita e, no valor, 0 se a operação não deu certo e 1 se foi bem sucedida (ver implementação do sistema em "arena.c")
    tmp.t = arg.t;
    tmp.valor = Sistema(arg, m);
    empilha(pil, tmp);
    break;
  }

  D(imprime(pil,5));
  D(puts("\n"));

  ip.valor++;
  }
}