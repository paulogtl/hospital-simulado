#include <stdio.h>
#include "paciente.h"

/* ============================================================
   FILA – Consultas Agendadas (FIFO)
   Implementada com vetor, índices inicio/fim e contador tamanho.
   ============================================================ */

void fila_inicializar(Fila *f) {
    f->inicio  = 0;
    f->fim     = 0;
    f->tamanho = 0;
}

int fila_vazia(Fila *f) {
    return (f->tamanho == 0);
}

int fila_cheia(Fila *f) {
    return (f->tamanho == MAX_CONSULTA);
}

/* Retorna 1 em sucesso, 0 se cheia */
int fila_enqueue(Fila *f, Paciente pac) {
    if (fila_cheia(f)) {
        return 0;
    }
    f->dados[f->fim] = pac;
    f->fim = (f->fim + 1) % MAX_CONSULTA;
    f->tamanho++;
    return 1;
}

/* Retorna 1 em sucesso, 0 se vazia */
int fila_dequeue(Fila *f, Paciente *pac) {
    if (fila_vazia(f)) {
        return 0;
    }
    *pac = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_CONSULTA;
    f->tamanho--;
    return 1;
}

/* Exibe da frente para o fim (ordem de atendimento) */
void fila_exibir(Fila *f) {
    if (fila_vazia(f)) {
        printf("  [Fila de consultas vazia]\n");
        return;
    }
    printf("  Ordem de atendimento (primeiro -> ultimo):\n");
    for (int i = 0; i < f->tamanho; i++) {
        int idx = (f->inicio + i) % MAX_CONSULTA;
        printf("  [%d] ", i + 1);
        exibir_paciente(&f->dados[idx]);
    }
}
