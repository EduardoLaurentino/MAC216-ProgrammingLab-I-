#include "maq.h"

#define MAXMEM 100

//cada exercito tem no maximo 3 robos
typedef struct {
  int ativo;
  int robos[3]; //enderecos dos robos do exercito no vetor de registros da arena
  int pos_celula_base[2];
} Exercito;

typedef struct {
  Celula **celulas; //ponteiro para matriz de celulas
  Exercito exercitos[2]; //5 exercitos no total, maximo de 100 robos
  Maquina *registros[100];
  int topo_ex; //local do vetor para inserir novos exercitos
  int topo_reg; //local do vetor para inserir novas maquinas
  int tempo;
} Arena;

Arena *cria_arena();
void RegistroMaquina(Arena *a, Maquina *m);
void InsereExercito(Arena *a);
void RemoveExercito(Arena *a, int num_ex);

void Atualiza(Arena *a, int rodadas);
int Sistema(OPERANDO op, Maquina *m);


int verifica_exercito_ativo(Arena *a, Exercito exerc); // 0 = nenhum robo de um certo exercito esta vivo; 1, caso contrario.

int verifica_continuidade(Arena *a); //"int" para usarmos retornos 0 ou 1, simulando booleana

void escalonador(Arena *a, int quant_rod);


void destroi_arena(Arena *a);


