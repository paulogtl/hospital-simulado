#include <stdio.h>
#include "paciente.h"

/* ============================================================
   PILHA – Setor de Emergência (LIFO)
   Implementada com vetor e controle de topo.
   topo == -1  →  pilha vazia
   ============================================================ */

void pilha_inicializar(Pilha *p) {
    p->topo = -1;
}

int pilha_vazia(Pilha *p) {
    return (p->topo == -1);
}

int pilha_cheia(Pilha *p) {
    return (p->topo == MAX_EMERGENCIA - 1);
}

/* Retorna 1 em sucesso, 0 se cheia */
int pilha_push(Pilha *p, Paciente pac) {
    if (pilha_cheia(p)) {
        return 0;
    }
    p->topo++;
    p->dados[p->topo] = pac;
    return 1;
}

/* Retorna 1 em sucesso, 0 se vazia */
int pilha_pop(Pilha *p, Paciente *pac) {
    if (pilha_vazia(p)) {
        return 0;
    }
    *pac = p->dados[p->topo];
    p->topo--;
    return 1;
}

/* Exibe do topo para a base (ordem de atendimento) */
void pilha_exibir(Pilha *p) {
    if (pilha_vazia(p)) {
        printf("  [Emergencia vazia]\n");
        return;
    }
    printf("  Ordem de atendimento (topo -> base):\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("  [%d] ", p->topo - i + 1);
        exibir_paciente(&p->dados[i]);
    }
}
