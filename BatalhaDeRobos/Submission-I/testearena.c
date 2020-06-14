#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

INSTR programa[] = {
  {PUSH, {NUM, 3}},
  {PUSH, {NUM, 6}},
  {CALL, {NUM, 5}},
  {PRN,  {NUM, 0}},
  {END,  {NUM, 0}},
  {ADD,  {NUM, 0}},
  {RET,  {NUM, 0}}
};

int main(int ac, char **av) {
  display = popen("./apres", "w");
  Arena *a = cria_arena();

  testes_arena(a);

  //jogador 1 com 3 robos no exercito
  Maquina *maq0 = cria_maquina(programa);
  RegistroMaquina(a, maq0);

  Maquina *maq1 = cria_maquina(programa);
  RegistroMaquina(a, maq1);

  Maquina *maq2 = cria_maquina(programa);
  RegistroMaquina(a, maq2);

  InsereExercito(a);

  //jogador 2 com 3 robos no exercito
  Maquina *maq3 = cria_maquina(programa);
  RegistroMaquina(a, maq3);

  Maquina *maq4 = cria_maquina(programa);
  RegistroMaquina(a, maq4);

  Maquina *maq5 = cria_maquina(programa);
  RegistroMaquina(a, maq5);

  InsereExercito(a);

  testes_insercao(a);

  RemoveExercito(a, 1);
  RemoveExercito(a, 0);

  testes_remocao(a);

  destroi_maquina(maq0);
  destroi_maquina(maq1);
  destroi_maquina(maq2);
  destroi_maquina(maq3);
  destroi_maquina(maq4);
  destroi_maquina(maq5);

  destroi_arena(a);
  return 0;
}
