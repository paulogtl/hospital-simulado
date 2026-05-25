#include "paciente.h"

/* ============================================================
   FILA DE PRIORIDADE COM PONTEIRO — Emergencia
   Ordenada por gravidade decrescente (5=critica tem prioridade
   maxima, 1=baixa tem prioridade minima).
   A insercao mantem a lista sempre ordenada (O(n)),
   garantindo que fila_prio_remover seja O(1).
   Empate de gravidade: FIFO (quem chegou primeiro e atendido
   primeiro — o novo e inserido APOS os de mesma gravidade).
   ============================================================ */

void fila_prio_inicializar(FilaPrioridade *fp) {
    fp->inicio   = NULL;
    fp->tamanho  = 0;
}

int fila_prio_vazia(FilaPrioridade *fp) {
    return (fp->inicio == NULL);
}

/* Insere mantendo ordem decrescente de gravidade */
void fila_prio_inserir(FilaPrioridade *fp, Paciente pac) {
    NoPrio *novo = (NoPrio *)malloc(sizeof(NoPrio));
    if (!novo) {
        printf("  [ERRO] Memoria insuficiente!\n");
        return;
    }
    novo->paciente = pac;
    novo->prox     = NULL;

    /* Caso 1: lista vazia ou novo tem gravidade maior que o primeiro */
    if (fila_prio_vazia(fp) || pac.gravidade > fp->inicio->paciente.gravidade) {
        novo->prox  = fp->inicio;
        fp->inicio  = novo;
        fp->tamanho++;
        return;
    }

    /* Caso 2: encontra a posicao correta (apos todos de mesma ou maior gravidade) */
    NoPrio *atual = fp->inicio;
    while (atual->prox != NULL && atual->prox->paciente.gravidade >= pac.gravidade) {
        atual = atual->prox;
    }
    novo->prox   = atual->prox;
    atual->prox  = novo;
    fp->tamanho++;
}

/* Remove o paciente de maior prioridade (inicio da lista) */
int fila_prio_remover(FilaPrioridade *fp, Paciente *pac) {
    if (fila_prio_vazia(fp)) return 0;
    NoPrio *temp = fp->inicio;
    *pac         = temp->paciente;
    fp->inicio   = temp->prox;
    free(temp);
    fp->tamanho--;
    return 1;
}

/* Exibe em ordem de prioridade (maior gravidade primeiro) */
void fila_prio_exibir(FilaPrioridade *fp) {
    if (fila_prio_vazia(fp)) {
        printf("  [Emergencia vazia]\n");
        return;
    }
    printf("  Ordem de atendimento (maior gravidade primeiro):\n");
    NoPrio *atual = fp->inicio;
    int     pos   = 1;
    while (atual != NULL) {
        printf("  [%d] ", pos++);
        exibir_paciente(&atual->paciente);
        atual = atual->prox;
    }
}

/* Libera todos os nos */
void fila_prio_liberar(FilaPrioridade *fp) {
    NoPrio *atual = fp->inicio;
    while (atual != NULL) {
        NoPrio *temp = atual;
        atual        = atual->prox;
        free(temp);
    }
    fp->inicio  = NULL;
    fp->tamanho = 0;
}
