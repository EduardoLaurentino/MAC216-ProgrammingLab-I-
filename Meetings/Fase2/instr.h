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
  STL,
  RCE,
  ALC,
  FRE,
  ATR,
  SIS,
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
  int x;
  int y;
  int cristais;
  int ocupado;
  int base;
  int maquina_no_local; //posicao da maquina no vetor de registros
} Celula;

/* Operando */
typedef struct {
  Tipo t;
  int valor;
  /*union {
    int n;
    int ac;
    int v;
    Celula cel;
  } val;*/
} OPERANDO;


/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
