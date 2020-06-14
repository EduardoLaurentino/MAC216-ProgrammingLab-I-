#include "pilha.h"

#define MAXMEM 100

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog; 
  OPERANDO ip;
  OPERANDO rbp;
  int cristais;
  int exercito;
  int registro;
  int energia;
  int pos[2];

} Maquina;

Maquina *cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
