#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

#define aqui 0
#define norte 1
#define nordeste 2
#define sudeste 3
#define sul 3
#define sudoeste 4
#define noroeste 5

INSTR programa1[] = {
  {SIS, {MOV, norte}},
  {SIS, {MOV, norte}},
  {SIS, {MOV, nordeste}},
  {SIS, {MOV, norte}},
  {SIS, {MOV, norte}},
  {SIS, {MOV, nordeste}},
  {SIS, {MOV, noroeste}},
  {SIS, {MOV, aqui}},
};

INSTR programa2[] = {
  {SIS, {MOV, sul}},
  {SIS, {MOV, sul}},
  {SIS, {MOV, sudeste}},
  {SIS, {MOV, sudeste}},
  {SIS, {MOV, sul}},
  {SIS, {MOV, sudoeste}},
  {SIS, {MOV, sul}},
  {SIS, {MOV, aqui}},
};

INSTR programa3[] = {
  {SIS, {MOV, sudoeste}},
  {SIS, {MOV, sudoeste}},
  {SIS, {MOV, noroeste}},
  {SIS, {MOV, sudoeste}},
  {SIS, {MOV, sudoeste}},
  {SIS, {MOV, noroeste}},
  {SIS, {MOV, sudoeste}},
  {SIS, {MOV, aqui}},
};


void testes_insercao(Arena *a) {
  int i, j;
  printf("Dados dos robos:\n\n");
  for (i = 0; i < 6; i++) {
    printf("Robo: %d\n", i);
    printf("Registro: %d\n", a->registros[i]->registro);
    printf("Energia: %d\n", a->registros[i]->energia);
    printf("Cristais: %d\n", a->registros[i]->cristais);
    printf("Posicao: %d ", a->registros[i]->pos[0]);
    printf("%d\n", a->registros[i]->pos[1]);
    printf("Exercito: %d\n\n", a->registros[i]->exercito);
  }
  printf("\n");

  printf("Matriz/Localizacao dos robos na arena apos insercao:\n\n");
  for (i = 0; i < 100; i++) {
    for (j = 0; j < 100; j++) {
      if (a->celulas[i][j].ocupado == 0) printf(".");
      else printf("%d", a->celulas[i][j].ocupado);
    }
    printf("\n");
  }
  printf("\n");

  printf("Dados dos exercitos:\n\n");
  for (i = 0; i < 2; i++) {
    printf("Ativo: %d\n", a->exercitos[i].ativo);
    printf("Posicao da base: X %d Y %d\n", a->exercitos[i].pos_celula_base[0], a->exercitos[i].pos_celula_base[1]);

    for (j = 0; j < 3; j++) {
      printf("Numero do exercito nos robos: %d\n", a->registros[a->exercitos[i].robos[j]]->exercito);
    }
  }
  printf("\n");
}

void testes_movimentacao(Arena *a) {
  int i, j;
  printf("Dados dos robos após movimentacao:\n\n");
  for (i = 0; i < 6; i++) {
    printf("Robo: %d\n", i);
    printf("Registro: %d\n", a->registros[i]->registro);
    printf("Energia: %d\n", a->registros[i]->energia);
    printf("Cristais: %d\n", a->registros[i]->cristais);
    printf("Posicao: %d ", a->registros[i]->pos[0]);
    printf("%d\n", a->registros[i]->pos[1]);
    printf("Exercito: %d\n\n", a->registros[i]->exercito);
  }
  printf("\n");

  printf("Matriz/Localizacao dos robos na arena apos movimentacao:\n\n");
  for (i = 0; i < 100; i++) {
    for (j = 0; j < 100; j++) {
      if (a->celulas[i][j].ocupado == 0) printf(".");
      else printf("%d", a->celulas[i][j].ocupado);
    }
    printf("\n");
  }
  printf("\n");

  printf("Dados dos exercitos:\n\n");
  for (i = 0; i < 2; i++) {
    printf("Ativo: %d\n", a->exercitos[i].ativo);
    printf("Posicao da base: X %d Y %d\n", a->exercitos[i].pos_celula_base[0], a->exercitos[i].pos_celula_base[1]);

    for (j = 0; j < 3; j++) {
      printf("Numero do exercito nos robos: %d\n", a->registros[a->exercitos[i].robos[j]]->exercito);
    }
  }
  printf("\n");
}


int main(int ac, char **av) {
  Arena *a = cria_arena();

  //jogador 1 com 3 robos no exercito
  Maquina *maq0 = cria_maquina(programa1);
  RegistroMaquina(a, maq0);

  Maquina *maq1 = cria_maquina(programa2);
  RegistroMaquina(a, maq1);

  Maquina *maq2 = cria_maquina(programa3);
  RegistroMaquina(a, maq2);

  InsereExercito(a);

  //jogador 2 com 3 robos no exercito
  Maquina *maq3 = cria_maquina(programa3);
  RegistroMaquina(a, maq3);

  Maquina *maq4 = cria_maquina(programa1);
  RegistroMaquina(a, maq4);

  Maquina *maq5 = cria_maquina(programa2);
  RegistroMaquina(a, maq5);

  InsereExercito(a);

  testes_insercao(a);

  escalonador(a, 8); //altere o numero de rodadas de 10 para 1 para ver uma movimentação menor

  printf("=================================\n\n");

  testes_movimentacao(a);

  RemoveExercito(a, 1);
  RemoveExercito(a, 0);

  destroi_maquina(maq0);
  destroi_maquina(maq1);
  destroi_maquina(maq2);
  destroi_maquina(maq3);
  destroi_maquina(maq4);
  destroi_maquina(maq5);

  destroi_arena(a);
  return 0;
}

