#include <stdio.h>
#include "maq.h"

INSTR programa[] = { 
  {PUSH, 3}, 
  {PUSH, 6}, 
  {CALL, 5}, 
  {PRN,  0}, 
  {END,  0}, 
  {ADD,  0}, 
  {RET,  0} 
};

INSTR prog[] = {
  {PUSH, 1},
  {DUP, 0},
  {STO, 0},
  {STO, 1},

  {PUSH, 10},
  {STO, 2},

  {RCL, 0},
  {RCL, 1},
  {DUP, 0},
  {STO, 0},
  {ADD, 0},
  {DUP, 0},
  {STO, 1},
  {PRN, 0},
  {RCL, 2},
  {PUSH, 1},
  {SUB, 0},
  {DUP, 0},
  {STO, 2},
  {PUSH, 0},
  {EQ, 0},
  {JIF, 8},

  {END, 0},
};

int main(int ac, char **av) { 
 Maquina *maq = cria_maquina(programa); 
 Maquina *maq2 = cria_maquina(prog); 
 exec_maquina(maq, 3);
 exec_maquina(maq2, 40);
 puts("---");
 exec_maquina(maq, 10);
 puts("---");
 exec_maquina(maq2, 10000);
 destroi_maquina(maq);
 destroi_maquina(maq2);
 return 0;}
