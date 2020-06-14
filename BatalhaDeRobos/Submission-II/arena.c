#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "arena.h"
#include "compila.tab.h"

FILE *display;

static void Erro(char *msg) {
  fprintf(stderr,"%s\n", msg);
}
static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Arena *a;
void cria_arena() {
  int i, j;
  a = (Arena *) malloc(sizeof(Arena));
  if (!a) Fatal("Memória insuficiente",4);

  a->tempo = 0;
  a->topo_ex = 0;
  a->topo_reg = 0;

  // vetor de ponteiros
  a->celulas = (Celula**)malloc(15 * sizeof(Celula*));
  if (!a->celulas) Fatal("Memória insuficiente",4);
  for (i = 0; i < 15; i++) {
    // aloca um vetor de Celulas para cada posição do vetor de ponteiros
    a->celulas[i] = (Celula*) malloc(15 * sizeof(Celula));

      // percorre o vetor de Celulas atual,
      // determinando caracteristicas de cada uma
      for (j = 0; j < 15; j++) {
        a->celulas[i][j].x = i;
        a->celulas[i][j].y = j;
        a->celulas[i][j].ocupado = 0; // ocupado = 0 significa sem ocupacao

        a->celulas[i][j].base = -1;
        // base = -1 significa sem base;
        // base = 0 time = 0;
        // base = 1 time = 1
      }
  }

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      a->celulas[i][j].terreno = 1;
      a->celulas[i][3+j].terreno = 3;
      a->celulas[i][6+j].terreno = 4;
      a->celulas[i][9+j].terreno = 1;
      a->celulas[i][12+j].terreno = 1;

      a->celulas[3+i][j].terreno = 3;
      a->celulas[3+i][3+j].terreno = 3;
      a->celulas[3+i][6+j].terreno = 1;
      a->celulas[3+i][9+j].terreno = 2;
      a->celulas[3+i][12+j].terreno = 1;

      a->celulas[6+i][j].terreno = 4;
      a->celulas[6+i][3+j].terreno = 1;
      a->celulas[6+i][6+j].terreno = 2;
      a->celulas[6+i][9+j].terreno = 1;
      a->celulas[6+i][12+j].terreno = 4;

      a->celulas[9+i][j].terreno = 1;
      a->celulas[9+i][3+j].terreno = 2;
      a->celulas[9+i][6+j].terreno = 1;
      a->celulas[9+i][9+j].terreno = 3;
      a->celulas[9+i][12+j].terreno = 3;

      a->celulas[12+i][j].terreno = 1;
      a->celulas[12+i][3+j].terreno = 1;
      a->celulas[12+i][6+j].terreno = 4;
      a->celulas[12+i][9+j].terreno = 3;
      a->celulas[12+i][12+j].terreno = 1;
    }
  }

  // cria cristais
  srand(time(NULL));
  for (i = 0; i < 9; i++) {
    int num1 = rand() % 14;
    int num2 = rand() % 14;

    // se a celula ja tiver cristais, so adiciona mais
    switch (a->celulas[num1][num2].terreno) {
      case 0:
        a->celulas[num1][num2].cristais += 1;
        break;
      case 1:
        a->celulas[num1][num2].cristais += 2;
        break;
      case 2:
        a->celulas[num1][num2].cristais += 5;
        break;
      case 3:
        a->celulas[num1][num2].cristais += 4;
        break;
      case 4:
        a->celulas[num1][num2].cristais += 3;
        break;
    }
  }
  // o comando abaixo envia para o arquivo em python
  // o tipo de terreno e a quantidade de cristais de cada célula da arena
  for (i = 0; i < 15; i++) {
    for (j = 0; j < 15; j++) {
      // envia info de cada celula para ser desenhada
      fprintf(display, "cel %d %d %d %d %d\n", i, j, a->celulas[i][j].terreno,
          a->celulas[i][j].cristais, a->celulas[i][j].base);
      fflush(display);
    }
  }
}

#define registros (a->registros)
#define tempo (a->tempo)
#define exercitos (a->exercitos)
#define celulas (a->celulas)
#define topo_ex (a->topo_ex)
#define topo_reg (a->topo_reg)
#define arma (m->arma)

void RegistroMaquina(Maquina *m) {
  // coloca o endereco da maquina virtual recebida no vetor de registros
  registros[topo_reg++] = m;
}

OPERANDO reconheceVizinhaca(Maquina* m, int j) {
  // armazena na memoria[0-5] as celulas da vizinhança
  switch (j) {
    OPERANDO temp;
    case 0: // Posicao Nordeste
      if (m->pos[0] == 0 || m->pos[1] == 15) {
        temp.t = NUM;
        temp.val.n = 0;
        return temp;
      }
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0] + 1][m->pos[1] + 1];
      return temp;
      break;
    case 1: // Posicao Leste
      if (m->pos[1] == 15) {
        temp.t = NUM;
        temp.val.n = 1;
        return temp;
      }
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0]][m->pos[1] + 1];
      return temp;
      break;
    case 2: // Posicao Sudeste
      if (m->pos[0] == 15 || m->pos[1] == 15) {
        temp.t = NUM;
        temp.val.n = 2;
        return temp;
      }
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0] - 1][m->pos[1] + 1];
      return temp;
      break;
    case 3: // Posicao Sudoeste
      if (m->pos[0] == 15 || m->pos[1] == 0) {
        temp.t = NUM;
        temp.val.n = 3;
        return temp;
      }
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0] - 1][m->pos[1] - 1];
      return temp;
      break;
    case 4: // Posicao Oeste
      if (m->pos[1] == 0) {
        temp.t = NUM;
        temp.val.n = 4;
        return temp;
      }
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0]][m->pos[1] - 1];
      return temp;
      break;
    case 5: // Posicao Noroeste
      if (m->pos[0] == 0 || m->pos[1] == 0) {
        temp.t = NUM;
        temp.val.n = 5;
        return temp;
      }
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0] + 1][m->pos[1] - 1];
      return temp;
      break;
    case 6: //Aqui
      temp.t = CELULA;
      temp.val.cel = celulas[m->pos[0]][m->pos[1]];
      return temp;
      break;
  }
}

// insere o novo exercito e cria uma base para ele
void InsereExercito() {
  int i;
  int j;
  // ativa exercito
  exercitos[topo_ex].ativo = 1;

  // o local  da base sera sempre (2,2) para o "Exercito 0"
  // e (9,9) para o "Exercito 1"
  int p;
  int q;
  if (topo_ex == 0) {
    exercitos[topo_ex].pos_celula_base[0] = 2;
    p = 2;
    exercitos[topo_ex].pos_celula_base[1] = 2;
    q = 2;
    celulas[2][2].base = 0;
  }
  else {
    exercitos[topo_ex].pos_celula_base[0] = 9;
    p = 9;
    exercitos[topo_ex].pos_celula_base[1] = 9;
    q = 9;
    celulas[9][9].base = 1;
  }

  // registra na celula o numero do exercito que tem base ali
  celulas[exercitos[topo_ex].pos_celula_base[0]]
      [exercitos[topo_ex].pos_celula_base[1]].base = topo_ex;

  // a posicao dos robos do exercito no vetor de registros
  // é colocado no vetor de robos do exercito
  for (i = 0; i < 3; i++) {
    exercitos[topo_ex].robos[i] = (topo_ex*3) + i;
    // coloca em cada robo o numero do seu proprio exercito e do registro na arena
    registros[exercitos[topo_ex].robos[i]]->exercito = topo_ex;
    registros[exercitos[topo_ex].robos[i]]->registro = (topo_ex*3)+i;

    // coloca na maquina qual a posicao dela na arena
    registros[exercitos[topo_ex].robos[i]]->pos[0]
        = exercitos[topo_ex].pos_celula_base[0]+i+1;

    registros[exercitos[topo_ex].robos[i]]->pos[1]
        = exercitos[topo_ex].pos_celula_base[1];

    // ativa ocupacao da celula
    celulas[exercitos[topo_ex].pos_celula_base[0]+i+1]
        [exercitos[topo_ex].pos_celula_base[1]].ocupado = 1;

    // coloca na celula a info da maquina que esta ocupando o local
    celulas[exercitos[topo_ex].pos_celula_base[0]+i+1]
        [exercitos[topo_ex].pos_celula_base[1]].maquina_no_local = (topo_ex*3)+i;

    for (j = 0; j < 7; j++) {
      registros[exercitos[topo_ex].robos[i]]->Mem[j]
          = reconheceVizinhaca(registros[exercitos[topo_ex].robos[i]], j);
    }
  }

  // os comandos abaixo irão mandar para o arquivo em python
  // as coordenadas da base do exército em questão, a imagem do robô
  // associado a este exército e as posições de todos os robôs
  // redesenha celula com nova cor/base
  fprintf(display, "cel %d %d %d %d %d\n", p, q, celulas[p][q].terreno,
      celulas[p][q].cristais, topo_ex);
  fflush(display);


  for (i = 0; i < 3; i++) {
    if (topo_ex == 0) {
      // envia imagem e posicao inicial dos robos para serem desenhados
      fprintf(display, "rob GILEAD_A.png %d %d %d\n",
          exercitos[0].robos[i], registros[exercitos[0].robos[i]]->pos[0],
          registros[exercitos[0].robos[i]]->pos[1]);
      fflush(display);
    }
    else {
      fprintf(display, "rob GILEAD_B.png %d %d %d\n",
          exercitos[1].robos[i], registros[exercitos[1].robos[i]]->pos[0],
          registros[exercitos[1].robos[i]]->pos[1]);
      fflush(display);
    }
  }
  topo_ex++;
}


void RemoveExercito(int num_ex) {
  int i;
  // desativa exercito
  exercitos[num_ex].ativo = 0;

  // retira a base
  celulas[exercitos[num_ex].pos_celula_base[0]]
      [exercitos[num_ex].pos_celula_base[1]].base = -1;

  int xbase = exercitos[num_ex].pos_celula_base[0];
  int ybase = exercitos[num_ex].pos_celula_base[1];

  // redesenha celula sem a cor/base
  fprintf(display, "cel %d %d %d %d %d\n",
      xbase, ybase, celulas[xbase][ybase].terreno,
      celulas[xbase][ybase].cristais, celulas[xbase][ybase].base);
  fflush(display);

  // retira a ocupacao das celulas onde os robos do exercito estavam
  for (i = 0; i < 3; i++)
    celulas[registros[exercitos[num_ex].robos[i]]->pos[0]]
        [registros[exercitos[num_ex].robos[i]]->pos[1]].ocupado = 0;

    int x = registros[exercitos[num_ex].robos[i]]->pos[0];
    int y = registros[exercitos[num_ex].robos[i]]->pos[1];

    // redesenha celula sem o robo
    fprintf(display, "cel %d %d %d %d %d\n", x, y,
        celulas[x][y].terreno, celulas[x][y].cristais, celulas[x][y].base);
    fflush(display);
}

void destroi_arena() {
  free(celulas);
  free(a);
  fprintf(display, "fim\n");
  fflush(display);
}

// Verifica se existe pelo menos 1 robo de 1 exercito vivo:
int verifica_exercito_ativo(Exercito exerc) {

  // coordenada X da base do exercito em questao
  int XcoordBaseExerc = exerc.pos_celula_base[0];
  // coordenada Y da base do exercito em questao
  int YcoordBaseExerc = exerc.pos_celula_base[1];

  // Se a base do exercito em questao tiver 5 cristais, jogo acaba
  if (celulas[XcoordBaseExerc][YcoordBaseExerc].cristais >= 5) return 0;
  int i;
  int cont;

  // varredura de todos os robos ate encontrar pelo menos 1 com saude > 0
  for (i = 0; i < 3; i++) {
    int x = registros[exerc.robos[i]]->pos[0];
    int y = registros[exerc.robos[i]]->pos[1];

    if (registros[exerc.robos[i]]->saude > 0) cont = 1;
    else {
      // a celula em que o robo morreu se torna desocupada (desocupado=0)
      celulas[x][y].ocupado = 0;

      //envia info da celula para ser redesenhada sem o robo
      fprintf(display, "cel %d %d %d %d %d\n", x, y,
          celulas[x][y].terreno, celulas[x][y].cristais, celulas[x][y].base);
      fflush(display);
    }
  }

  // caso haja 1 robo vivo, return 1 = "ativo"
  if (cont == 1) return 1;
  // caso nao haja nenhum robo com vida (energia = 0), retornar 0 = "inativo"
  else return 0;
}


int verifica_continuidade() {
  // não continua se excedeu o numero maximo de rodadas
  int i;
  int quant = 0;
  int exerc_vencedor = -1;

  for (i = 0; i < 2; i++) { // dois exercitos apenas
    // verificar se existem exercitos ativos
    exercitos[i].ativo = verifica_exercito_ativo(exercitos[i]);
    // guarda o numero do exercito que ta ativo.
    if (exercitos[i].ativo == 1) { quant++; exerc_vencedor = i; }
  }

  // se os dois exercitos estao ativos, o jogo continua(retorna -1)
  if (quant == 2) return -1;
  // caso haja apenas 1, retorna o numero do unico exercito ativo (vencedor)
  else return exerc_vencedor;
}


void escalonador(int quant_rod) {
  int i, j;
  int verifica_cont;

  for (i = 0; i < quant_rod; i++) {
    // funcao "verifica_continuidade()" retorna -1: caso o jogo continue;
    // 0: caso exerc 0 vença e 1: caso o exerc 1 vença.
    verifica_cont = verifica_continuidade();

    if (verifica_cont < 0) {
      //faz todas os robos executarem 10 instruções por rodadas
      for (j = 0; j < 6; j++) {
        if (registros[j]->isCiclo == 0) exec_maquina(registros[j], 1);
        else registros[j]->isCiclo -= 1;
      }
    }
    else {
      //Printa o exercito vencedor.
      printf("Vencedor: Exército %d\n",verifica_cont);
      //Jogo acaba.
      break;
    }
  }
}


int verifica_ocupacao(int x, int y) {
  // verifica disponibilidade de conquista da célula (se existir)
  // indisponibilidade ~= estar ocupado
  if (x < 0 || y < 0 || x >= 15 || y >= 15) return 1;
  else return celulas[x][y].ocupado;
}

int celula_existe(int x, int y) {
  // indisponibilidade ~= não atacavel, não pode atacar e nao perde energia
  if (x < 0 || y < 0 || x >= 15 || y >= 15) return 0;
  else return 1;
}

int retira_energia_movimento(Maquina *m, Terreno terreno) {
  switch (terreno) {
    case ESTRADA:
      // Consegue se movimentar no seu proprio turno,
      // pois ESTRADA é um terreno de facil movimentação.
      if (m->energia >= 10) { m->energia -= 10; return 1; }
      else return 0;
      break;
    case TERRA:
      m->isCiclo = 1;
      // Demora apenas 1 turno para se movimentar caso o terreno seja do tipo TERRA
      if (m->energia >= 15) { m->energia -= 15; return 1; }
      else return 0;
      break;
    case LAMA:
      m->isCiclo = 1;
      // Demora apenas 1 turno para se movimentar caso o terreno seja do tipo LAMA
      if (m->energia >= 20) { m->energia -= 20; return 1; }
      else return 0;
      break;
    case AGUA:
      m->isCiclo = 2;
      // Demora 2 turnos para se movimentar caso o terreno seja do tipo AGUA
      if (m->energia >= 25) { m->energia -= 25; return 1; }
      else return 0;
      break;
    case MONTANHA:
      m->isCiclo = 2;
      // Demora 2 turnos para se movimentar caso o terreno seja do tipo MONTANHA
      if (m->energia >= 30) { m->energia -= 30; return 1; }
      else return 0;
      break;
  }
}

int retira_energia_extracao_e_por(Maquina *m, Terreno terreno) {
  switch (terreno) {
    case AGUA:
      // Consegue retirar cristal no seu proprio turno,
      // pois AGUA é um terreno facil de se extrair.
      if (m->energia >= 25) { m->energia -= 5; return 1; }
      else return 0;
      break;
    case LAMA:
      m->isCiclo = 1;
      // Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo LAMA
      if (m->energia >= 27) { m->energia -= 7; return 1; }
      else return 0;
      break;
    case TERRA:
      m->isCiclo = 1;
      // Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo TERRA
      if (m->energia >= 29) { m->energia -= 9; return 1; }
      else return 0;
      break;
    case ESTRADA:
      m->isCiclo = 2;
      // Demora 2 turnos para retirar cristal caso o terreno seja do tipo ESTRADA
      if (m->energia >= 32) { m->energia -= 12; return 1; }
      else return 0;
      break;
    case MONTANHA:
      m->isCiclo = 2;
      // Demora 2 turnos para retirar cristal caso o terreno seja do tipo MONTANHA
      if (m->energia >= 35) { m->energia -= 15; return 1; }
      else return 0;
      break;
  }
}

#define x (m->pos[0])
#define y (m->pos[1])

#define aqui 6
#define nordeste 0
#define leste 1
#define sudeste 2
#define sudoeste 3
#define oeste 4
#define noroeste 5

int movimentacao(Maquina *m, int i, int j) {
  // verifica se a célula para a qual quer ir existe e esta vazia
  // e se o robo tem energia para ir, já subtraindo energia caso sim
  if (verifica_ocupacao(i, j) == 0 &&
    retira_energia_movimento(m, celulas[i][j].terreno) == 1) {

    // muda o status da celula onde tava para desocupada
    celulas[m->pos[0]][m->pos[1]].ocupado = 0;

    int posxvelho = x;
    int posyvelho = y;

    fprintf(display, "cel %d %d %d %d %d\n",posxvelho, posyvelho,
        celulas[posxvelho][posyvelho].terreno,
        celulas[posxvelho][posyvelho].cristais,
        celulas[posxvelho][posyvelho].base);
    fflush(display);

    x = i;
    y = j; // atualiza posicao do robo
    celulas[i][j].maquina_no_local = m->registro;
    celulas[i][j].ocupado = 1;

    fprintf(display, "%d %d %d %d %d\n", m->registro, posxvelho, posyvelho, x, y);
    fflush(display);

    switch (celulas[i][j].terreno) {
      case ESTRADA:
        // Consegue se movimentar no seu proprio turno,
        // pois ESTRADA é um terreno de facil movimentação.
        break;
      case TERRA:
        m->isCiclo = 1;
        // Demora apenas 1 turno para se movimentar caso o terreno seja do tipo TERRA
        break;
      case LAMA:
        m->isCiclo = 1;
        // Demora apenas 1 turno para se movimentar caso o terreno seja do tipo LAMA
        break;
      case AGUA:
        m->isCiclo = 2;
        // Demora 2 turnos para se movimentar caso o terreno seja do tipo AGUA
      case MONTANHA:
        m->isCiclo = 2;
        // Demora 2 turnos para se movimentar caso o terreno seja do tipo MONTANHA
        break;
    }
    for (j = 0; j < 7; j++) {
      registros[exercitos[topo_ex].robos[i]]->Mem[j]
          = reconheceVizinhaca(registros[exercitos[topo_ex].robos[i]], j);
    }
    return 1;
  }
  else return 0;
}

int extracao(Maquina *m, int i, int j) {
  // verifica se a célula para a qual quer ir existe e esta vazia,
  // se sim tenta extrair (e nisso ja perde energia) mas só consegue se tiver cristais
  // não pode extrair se já tiver mais de dois
  if (verifica_ocupacao(i, j) == 0 &&
      retira_energia_extracao_e_por(m, celulas[i][j].terreno) == 1
      && celulas[i][j].cristais > 0 && m->cristais < 3) {

    celulas[i][j].cristais -= 1;
    m->cristais += 1;
    // perda de energia maior por fazer extração em célula vizinha (fazer extração
    // em céula vizinha ao invés de onde se está tem custo energético maior)
    m->energia -= 20;

    switch (celulas[i][j].terreno) {
      case AGUA:
        // Consegue retirar cristal no seu proprio turno,
        // pois AGUA é um terreno facil de se extrair.
        break;
      case LAMA:
        m->isCiclo = 1;
        // Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo LAMA
        break;
      case TERRA:
        m->isCiclo = 1;
        // Demora apenas 1 turno para retirar cristal caso o terreno seja do tipo TERRA
        break;
      case ESTRADA:
        m->isCiclo = 2;
        // Demora 2 turnos para retirar cristal caso o terreno seja do tipo ESTRADA
        break;
      case MONTANHA:
        m->isCiclo = 2;
        // Demora 2 turnos para retirar cristal caso o terreno seja do tipo MONTANHA
        break;
    }
    fprintf(display, "cel %d %d %d %d %d\n", x, y,
        celulas[i][j].terreno, celulas[i][j].cristais, celulas[i][j].base);
    fflush(display);
    if (i == x && j == y) {
        fprintf(display, "%d %d %d %d %d\n", m->registro, x, y, x, y);
        fflush(display);
    }
    return 1;
  }
  else return 0;
}

int por_cristal(Maquina *m, int i, int j) {
  // verifica se a célula para a qual quer ir existe
  // e esta vazia e tbm se tem cristais na maquina
  if (verifica_ocupacao(i, j) == 0 &&
      retira_energia_extracao_e_por(m, celulas[i][j].terreno) == 1
      && m->cristais > 0) {

    celulas[i][j].cristais += 1;
    m->cristais -= 1;
    m->energia -= 20;
    fprintf(display, "cel %d %d %d %d %d\n", x, y,
        celulas[i][j].terreno, celulas[i][j].cristais, celulas[i][j].base);
    fflush(display);
  }
  else return 0;
}

int atacar(Maquina *m, int i, int j) {
  int perde;
  // caso nao esteja em um ciclo e escolher a arma tipo 1 (a mais forte),
  // ele fica ocupado por 1 turno.
  // se a arma(n) for a do tipo 1, o oponente perde 'arma[3]' de saude.
  //if (n == 1) { m->isCiclo = 1; perde = 3; }

  // se a arma(n) for a do tipo 0, o oponente perde 'arma[1]' de saude
  //if (n == 0) perde = 1;

  if (celula_existe(i, j) == 1 && m->energia >= 50) {
    // tem robo pra ser atacado
    if (celulas[i][j].ocupado == 1) {
      // retira energia do robo atacado
      registros[celulas[i][j].maquina_no_local]->saude -= 80;
      // perde energia por atacar com sucesso
      m->energia -= 50;
      return 1;
    }
    else {
      // perde energia simplesmente por atacar,
      // pra impedir que fique atacando descriteriosamente
      m->energia -= 50;
      return 0; // não atacou
    }
  }
  else return 0;
}

int Sistema(OPERANDO op, Maquina *m){
  int dir = op.val.n;

  if (op.t == MOV) {
    // é a operacao de descanso, que recarrega um pouco de energia
    if (dir == aqui) {
      // aumenta a energia e retorna o sucesso do procedimento
      // só pode aumentar energia se ainda estiver vivo (energia > 0)
      if (m->energia < 800 && m->energia > 0) { m->energia += 20; return 1; }

      // se não conseguir aumentar energia
      //uma vez que ficou com menos de 1000, nao pode voltar a ter 1000
      return 0;
    }
    else if (dir == leste)    return movimentacao(m, x, y + 1);
    else if (dir == nordeste) return movimentacao(m, x-1, y+1);
    else if (dir == sudeste)  return movimentacao(m, x, y+1);
    else if (dir == oeste)    return movimentacao(m, x, y - 1);
    else if (dir == sudoeste) return movimentacao(m, x, y-1);
    else if (dir == noroeste) return movimentacao(m, x-1, y-1);
  }

  else if (op.t == EXTR){
    if (dir == aqui){
      // só de tentar extrair já perde energia,
      // mas só vai conseguir se tiver cristais disponiveis
      if (retira_energia_extracao_e_por(m, celulas[x][y].terreno)
          == 1 && celulas[x][y].cristais > 0) {

        celulas[x][y].cristais -= 1;
        m->cristais += 1;
        return 1;
      }
      else return 0;
    }
    else if (dir == leste)    return extracao(m, x, y + 1);
    else if (dir == nordeste) return extracao(m, x-1, y+1);
    else if (dir == sudeste)  return extracao(m, x, y+1);
    else if (dir == oeste)    return extracao(m, x, y - 1);
    else if (dir == sudoeste) return extracao(m, x, y-1);
    else if (dir == noroeste) return extracao(m, x-1, y-1);
  }

  else if (op.t == POR){
    if (dir == aqui){
      // verifica se tem cristais pra por e só coloca se tiver energia,
      // mas perde energia só de tentar
      if (retira_energia_extracao_e_por(m, celulas[x][y].terreno)
          == 1 && m->cristais > 0) {

        celulas[x][y].cristais += 1;
        m->cristais -= 1;
        return 1;
      }
      else return 0;
    }
    else if (dir == leste)    return por_cristal(m, x, y + 1);
    else if (dir == nordeste) return por_cristal(m, x-1, y+1);
    else if (dir == sudeste)  return por_cristal(m, x, y+1);
    else if (dir == oeste)    return por_cristal(m, x, y - 1);
    else if (dir == sudoeste) return por_cristal(m, x, y-1);
    else if (dir == noroeste) return por_cristal(m, x-1, y-1);
  }

  else if (op.t == ATK) {
    if (dir == aqui) {
      if (m->energia >= 30) {
        m->energia -= 30;
        return 1;
      }
      else return 0; // sem energia suficiente
    }
    else if (dir == leste)    return atacar(m, x, y + 1);
    else if (dir == nordeste) return atacar(m, x-1, y+1);
    else if (dir == sudeste)  return atacar(m, x, y+1);
    else if (dir == oeste)    return atacar(m, x, y - 1);
    else if (dir == sudoeste) return atacar(m, x, y-1);
    else if (dir == noroeste) return atacar(m, x-1, y-1);
  }
}


INSTR programa1[2000];
INSTR programa2[2000];

int compilador(FILE *, INSTR *);

int main(int ac, char **av) {

  display = popen("./apres", "w");



  if (display == NULL) {
    fprintf(stderr,"Não encontrei o programa de exibição\n");
    return 1;
  }

  cria_arena();

  FILE *p = stdin;
  int res;
  ac--; av++;
  if (ac>0) p = fopen(*av, "r");

  res = compilador(p, programa1);
  if (res) return 1;

  fclose(p);
  av++;

  if (ac>1) p = fopen(*av, "r");

  res= compilador(p, programa2);
  if (res) return 1;

  fclose(p);


  // jogador 1 com 3 robos no exercito
  Maquina *maq0 = cria_maquina(programa1);
  RegistroMaquina(maq0);

  Maquina *maq1 = cria_maquina(programa1);
  RegistroMaquina(maq1);

  Maquina *maq2 = cria_maquina(programa2);
  RegistroMaquina(maq2);

  InsereExercito();

  // jogador 2 com 3 robos no exercito
  Maquina *maq3 = cria_maquina(programa2);
  RegistroMaquina(maq3);

  Maquina *maq4 = cria_maquina(programa2);
  RegistroMaquina(maq4);

  Maquina *maq5 = cria_maquina(programa2);
  RegistroMaquina(maq5);

  InsereExercito();

  escalonador(20);

  destroi_maquina(maq0);
  destroi_maquina(maq1);
  destroi_maquina(maq2);
  destroi_maquina(maq3);
  destroi_maquina(maq4);
  destroi_maquina(maq5);

  destroi_arena();
  return 0;
}
