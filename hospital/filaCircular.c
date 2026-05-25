#include <stdio.h>
#include "paciente.h"

/* ============================================================
   FILA CIRCULAR – Exames / Laboratório
   Capacidade fixa: MAX_EXAME (8 pacientes).
   Ao atingir a capacidade, o paciente mais antigo é removido
   automaticamente para dar lugar ao novo.
   ============================================================ */

void fila_circ_inicializar(FilaCircular *fc) {
    fc->inicio  = 0;
    fc->fim     = 0;
    fc->tamanho = 0;
}

int fila_circ_vazia(FilaCircular *fc) {
    return (fc->tamanho == 0);
}

/* Insere paciente. Se cheia, remove o mais antigo primeiro. */
void fila_circ_enqueue(FilaCircular *fc, Paciente pac) {
    if (fc->tamanho == MAX_EXAME) {
        /* Remove automaticamente o paciente mais antigo */
        Paciente removido = fc->dados[fc->inicio];
        fc->inicio = (fc->inicio + 1) % MAX_EXAME;
        fc->tamanho--;
        printf("  [AVISO] Fila circular cheia! Paciente mais antigo removido automaticamente:\n");
        printf("  >> ");
        exibir_paciente(&removido);
    }
    fc->dados[fc->fim] = pac;
    fc->fim = (fc->fim + 1) % MAX_EXAME;
    fc->tamanho++;
}

/* Retorna 1 em sucesso, 0 se vazia */
int fila_circ_dequeue(FilaCircular *fc, Paciente *pac) {
    if (fila_circ_vazia(fc)) {
        return 0;
    }
    *pac = fc->dados[fc->inicio];
    fc->inicio = (fc->inicio + 1) % MAX_EXAME;
    fc->tamanho--;
    return 1;
}

/* Exibe da frente para o fim (ordem de atendimento) */
void fila_circ_exibir(FilaCircular *fc) {
    if (fila_circ_vazia(fc)) {
        printf("  [Fila de exames vazia]\n");
        return;
    }
    printf("  Ordem de atendimento (primeiro -> ultimo) [%d/%d vagas]:\n",
           fc->tamanho, MAX_EXAME);
    for (int i = 0; i < fc->tamanho; i++) {
        int idx = (fc->inicio + i) % MAX_EXAME;
        printf("  [%d] ", i + 1);
        exibir_paciente(&fc->dados[idx]);
    }
}
