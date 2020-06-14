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
  Exercito exercitos[2]; //2 exercitos no total, maximo de 6 robos
  Maquina *registros[6]; // 3 robos por exercito, totalizando 6 robos.
  int topo_ex; //local do vetor para inserir novos exercitos
  int topo_reg; //local do vetor para inserir novas maquinas
  int tempo;
} Arena;

void cria_arena();
void RegistroMaquina(Arena *a, Maquina *m);
void InsereExercito(Arena *a);
void RemoveExercito(Arena *a, int num_ex);

int Sistema(OPERANDO op, Maquina *m);

int verifica_exercito_ativo(Exercito exerc); // 0 = nenhum robo de um certo exercito esta vivo; 1, caso contrario.

int verifica_continuidade(void); //"int" para usarmos retornos 0 ou 1, simulando booleana

void escalonador(Arena *a, int quant_rod);

void destroi_arena(Arena *a);

int verifica_exercito_ativo(Exercito exerc); // 0 = nenhum robo de um certo exercito esta vivo; 1, caso contrario.

int verifica_ocupacao(int x, int y);

int celula_existe(int x, int y);

int retira_energia_movimento(Maquina *m, Terreno terreno);

int retira_energia_extracao_e_por(Maquina *m, Terreno terreno);

int movimentacao(Maquina *m, int i, int j);

int extracao(Maquina *m, int i, int j);

int poe_cristal(Maquina *m, int i, int j);

int atacar(Maquina *m, int i, int j, int n);



