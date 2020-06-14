/* Códigos das instruções */
typedef enum {
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MUL,
  DIV,
  JMP,
  JIT,
  JIF,
  CALL,
  RET,
  STS,
  RCS,
  EQ,
  GT,
  GE,
  LT,
  LE,
  NE,
  STO,
  RCL,
  END,
  PRN,
  ATR,
  SIS,
  ENTRY,
  LEAVE
} OpCode;

/* Tipos dos operandos */
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELULA,
  MOV,
  EXTR,
  POR,
  ATK,
} Tipo;

/* Tipo de terreno */
typedef enum {
  ESTRADA,
  TERRA,
  MONTANHA,
  AGUA,
  LAMA
} Terreno;

/* Célula */
typedef struct {
  Terreno terreno;
  int cristais;
  int ocupado;
  int base;
  int x; // x, y = posição da célula na matriz de celulas da arena
  int y;

  // posicao da maquina no vetor de registros
  // o mesmo que o atributo "registro" da maquina
  int maquina_no_local;
} Celula;

/* Operando */
typedef struct {
  Tipo t;
  union {
    int n;
    int ac;
    int v;
    Celula cel;
  } val;
  //int valor;
} OPERANDO;


/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
