#include "pilha.h"

#define MAXMEM 100
#define MAXFRM 30

typedef struct {
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int bp[MAXFRM];
  int ib;

  int pos[2];
  // pos[0] = coordenada x na matriz;
  // pos[1] = coordenada y na matriz;

  int cristais;
  int registro; // guarda a posição no vetor de registro da arena
  int energia; // começará com 1000 pontos de energia
  int saude; // representa a vida de cada robo. Inicializaremos com 1000

  //int arma[4];
  // arma[0] gasta pouca energia e tira 'arma[1]' de saude do adversario;
  // arma[2] gasta mais energia, porem retira 'arma[3]' de saude do adversario
  // No caso, arma[0] e arma[1] representam 1 tipo de arma
  // e arma[2] e arma[3] guardam valores do segundo tipo de arma.

  int isCiclo;
  int exercito;
} Maquina;

Maquina *cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);

int new_frame(Maquina *m, int pos);

int del_frame(Maquina *m);


