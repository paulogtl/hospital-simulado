#ifndef PACIENTE_H
#define PACIENTE_H

#define MAX_NOME 100
#define MAX_EMERGENCIA 50
#define MAX_CONSULTA 50
#define MAX_EXAME 8

/* ============================================================
   Estrutura do Paciente
   ============================================================ */
typedef struct {
    int id;
    char nome[MAX_NOME];
    int idade;
    int gravidade;   /* 1 a 5 */
    int tipo;        /* 1=Emergência, 2=Consulta, 3=Exame */
} Paciente;

/* ============================================================
   Pilha – Emergência (LIFO)
   ============================================================ */
typedef struct {
    Paciente dados[MAX_EMERGENCIA];
    int topo;
} Pilha;

/* ============================================================
   Fila – Consultas Agendadas (FIFO)
   ============================================================ */
typedef struct {
    Paciente dados[MAX_CONSULTA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

/* ============================================================
   Fila Circular – Exames/Laboratório (FIFO circular, max 8)
   ============================================================ */
typedef struct {
    Paciente dados[MAX_EXAME];
    int inicio;
    int fim;
    int tamanho;
} FilaCircular;

/* ============================================================
   Protótipos – Pilha
   ============================================================ */
void pilha_inicializar(Pilha *p);
int  pilha_cheia(Pilha *p);
int  pilha_vazia(Pilha *p);
int  pilha_push(Pilha *p, Paciente pac);
int  pilha_pop(Pilha *p, Paciente *pac);
void pilha_exibir(Pilha *p);

/* ============================================================
   Protótipos – Fila
   ============================================================ */
void fila_inicializar(Fila *f);
int  fila_cheia(Fila *f);
int  fila_vazia(Fila *f);
int  fila_enqueue(Fila *f, Paciente pac);
int  fila_dequeue(Fila *f, Paciente *pac);
void fila_exibir(Fila *f);

/* ============================================================
   Protótipos – Fila Circular
   ============================================================ */
void fila_circ_inicializar(FilaCircular *fc);
int  fila_circ_vazia(FilaCircular *fc);
void fila_circ_enqueue(FilaCircular *fc, Paciente pac);
int  fila_circ_dequeue(FilaCircular *fc, Paciente *pac);
void fila_circ_exibir(FilaCircular *fc);

/* ============================================================
   Protótipos – Utilitários / Menu
   ============================================================ */
void exibir_paciente(Paciente *p);
void exibir_separador(void);
int  proximo_id(void);

#endif /* PACIENTE_H */

