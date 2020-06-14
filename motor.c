#include <stdio.h>
#include "maq.h"

/*INSTR programa[] = {
  {PUSH, 3},
  {PUSH, 6},
  {CALL, 5},
  {PRN,  0},
  {END,  0},
  {ADD,  0},
  {RET,  0}
};*/

INSTR fibonacci[] =  {
  /* inicializa variáveis */
  {PUSH,  1},  /*  0 */
  {DUP,   0},  /*  1 */
  {STL,   0},  /*  2 x */
  {STL,   1},  /*  3 y */

  /* for i = 10 */
  {PUSH, 10},  /*  4 */
  {STL,   2},  /*  5 i*/

  /* início do laço */
  {RCE,   0},  /*  6 */
  {RCE,   1},  /*  7 y */
  {DUP,   0},  /*  8 */
  {STL,   0},  /*  9 x' = y */
  {ADD,   0},  /* 10 x+y */
  {DUP,   0},  /* 11 */
  {STL,   1},  /* 12 y = x+y */
  {PRN,   0},  /* 13 (x+y) */
  {RCE,   2},  /* 14 i */
  {PUSH,  1},  /* 15 */
  {SUB,   0},  /* 16 */
  {DUP,   0},  /* 17 */
  {STL,   2},  /* 18 i = i - 1 */
  {PUSH,  0},  /* 19 */
  {EQ,    0},  /* 20 i == 0 ?*/
  {JIF,   6},  /* 21 (LOOP) */
  {END,   0},  /* 22 */

};

int main(int ac, char **av) {
  //Maquina *maq = cria_maquina(programa);
  Maquina *maq2 = cria_maquina(fibonacci);
  //exec_maquina(maq, 7);
  exec_maquina(maq2, 40);
  //puts("---");
  //exec_maquina(maq, 20);
  //puts("---");
  exec_maquina(maq2, 10000);
  //destroi_maquina(maq);
  destroi_maquina(maq2);
  return 0;
}
