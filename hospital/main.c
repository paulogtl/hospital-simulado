#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "paciente.h"

/* ============================================================
   Contador global de IDs
   ============================================================ */
static int contador_id = 1;

int proximo_id(void) {
    return contador_id++;
}

/* ============================================================
   Utilitários de exibição
   ============================================================ */
void exibir_separador(void) {
    printf("\n================================================\n");
}

void exibir_paciente(Paciente *p) {
    const char *setor;
    switch (p->tipo) {
        case 1: setor = "Emergencia";   break;
        case 2: setor = "Consulta";     break;
        case 3: setor = "Exame";        break;
        default: setor = "Indefinido";
    }
    printf("ID:%-4d | %-30s | Idade: %3d | Grav: %d | Setor: %s\n",
           p->id, p->nome, p->idade, p->gravidade, setor);
}

/* ============================================================
   Leitura segura de string (evita buffer overflow e lixo no
   stdin após entradas numéricas)
   ============================================================ */
static void ler_string(char *buf, int tamanho) {
    if (fgets(buf, tamanho, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';
    }
}

static void limpar_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int ler_inteiro(int min, int max) {
    int val;
    while (1) {
        if (scanf("%d", &val) == 1 && val >= min && val <= max) {
            limpar_stdin();
            return val;
        }
        limpar_stdin();
        printf("  Valor invalido. Digite um numero entre %d e %d: ", min, max);
    }
}

/* ============================================================
   Cadastro de paciente
   ============================================================ */
static Paciente cadastrar_paciente(void) {
    Paciente p;
    p.id = proximo_id();

    printf("\n  Nome do paciente: ");
    ler_string(p.nome, MAX_NOME);

    printf("  Idade: ");
    p.idade = ler_inteiro(0, 130);

    printf("  Gravidade (1=baixa ... 5=critica): ");
    p.gravidade = ler_inteiro(1, 5);

    /* Regra de negocio: gravidade 4 ou 5 -> Emergencia obrigatoria */
    if (p.gravidade >= 4) {
        printf("  [ALERTA] Gravidade %d detectada! Paciente direcionado automaticamente para Emergencia.\n",
               p.gravidade);
        p.tipo = 1;
    } else {
        printf("  Setor (1=Emergencia, 2=Consulta, 3=Exame): ");
        p.tipo = ler_inteiro(1, 3);
    }

    return p;
}

/* ============================================================
   Relatórios
   ============================================================ */
static void relatorio_geral(Pilha *em, Fila *co, FilaCircular *ex) {
    int total = em->topo + 1 + co->tamanho + ex->tamanho;

    exibir_separador();
    printf("           RELATORIO GERAL DO HOSPITAL\n");
    exibir_separador();
    printf("  Emergencia  (Pilha)         : %d paciente(s)\n", em->topo + 1);
    printf("  Consultas   (Fila)          : %d paciente(s)\n", co->tamanho);
    printf("  Exames      (Fila Circular) : %d/%d paciente(s)\n", ex->tamanho, MAX_EXAME);
    printf("  -----------------------------------------------\n");
    printf("  Total geral                 : %d paciente(s)\n", total);
    exibir_separador();
}

/* ============================================================
   Transferência entre setores
   ============================================================ */
static void transferir(Pilha *em, Fila *co, FilaCircular *ex) {
    exibir_separador();
    printf("         TRANSFERENCIA DE PACIENTE\n");
    exibir_separador();
    printf("  Setor de ORIGEM:\n");
    printf("  1 - Emergencia\n");
    printf("  2 - Consultas\n");
    printf("  3 - Exames\n");
    printf("  Escolha: ");
    int origem = ler_inteiro(1, 3);

    Paciente pac;
    int removido = 0;

    switch (origem) {
        case 1:
            if (!pilha_pop(em, &pac)) {
                printf("  [ERRO] Emergencia vazia!\n"); return;
            }
            removido = 1;
            break;
        case 2:
            if (!fila_dequeue(co, &pac)) {
                printf("  [ERRO] Fila de consultas vazia!\n"); return;
            }
            removido = 1;
            break;
        case 3:
            if (!fila_circ_dequeue(ex, &pac)) {
                printf("  [ERRO] Fila de exames vazia!\n"); return;
            }
            removido = 1;
            break;
    }

    if (!removido) return;

    printf("\n  Paciente retirado do setor de origem:\n  >> ");
    exibir_paciente(&pac);

    printf("\n  Setor de DESTINO:\n");
    printf("  1 - Emergencia\n");
    printf("  2 - Consultas\n");
    printf("  3 - Exames\n");
    printf("  Escolha: ");
    int destino = ler_inteiro(1, 3);

    /* Regra: gravidade 4/5 nao pode sair da emergencia para outro setor */
    if (pac.gravidade >= 4 && destino != 1) {
        printf("  [BLOQUEIO] Paciente com gravidade %d nao pode ser transferido para fora da Emergencia.\n",
               pac.gravidade);
        printf("  Retornando paciente para Emergencia...\n");
        destino = 1;
    }

    pac.tipo = destino;
    int ok = 0;
    switch (destino) {
        case 1: ok = pilha_push(em, pac);        break;
        case 2: ok = fila_enqueue(co, pac);       break;
        case 3: fila_circ_enqueue(ex, pac); ok = 1; break;
    }

    if (ok) {
        const char *nomes[] = {"", "Emergencia", "Consultas", "Exames"};
        printf("  Paciente transferido com sucesso para %s.\n", nomes[destino]);
    } else {
        printf("  [ERRO] Setor de destino cheio! Paciente permanece sem setor.\n");
    }
}

/* ============================================================
   Menu principal
   ============================================================ */
int main(void) {
    Pilha      emergencia;
    Fila       consultas;
    FilaCircular exames;

    pilha_inicializar(&emergencia);
    fila_inicializar(&consultas);
    fila_circ_inicializar(&exames);

    int opcao;
    int continuar = 1;

    while (continuar) {
        exibir_separador();
        printf("     HOSPITAL SIMULADO – SISTEMA DE PACIENTES\n");
        exibir_separador();
        printf("  1. Cadastrar novo paciente\n");
        printf("  2. Atender paciente – Emergencia (Pilha)\n");
        printf("  3. Atender paciente – Consultas  (Fila)\n");
        printf("  4. Atender paciente – Exames     (Fila Circular)\n");
        printf("  5. Mostrar todos os setores\n");
        printf("  6. Transferir paciente entre setores\n");
        printf("  7. Relatorio geral\n");
        printf("  0. Sair\n");
        printf("  Opcao: ");
        opcao = ler_inteiro(0, 7);

        switch (opcao) {
            /* ---- Cadastro ---- */
            case 1: {
                exibir_separador();
                printf("        CADASTRO DE NOVO PACIENTE\n");
                exibir_separador();
                Paciente novo = cadastrar_paciente();
                int ok = 0;
                switch (novo.tipo) {
                    case 1:
                        ok = pilha_push(&emergencia, novo);
                        if (!ok) printf("  [ERRO] Emergencia cheia!\n");
                        break;
                    case 2:
                        ok = fila_enqueue(&consultas, novo);
                        if (!ok) printf("  [ERRO] Fila de consultas cheia!\n");
                        break;
                    case 3:
                        fila_circ_enqueue(&exames, novo);
                        ok = 1;
                        break;
                }
                if (ok) {
                    const char *nomes[] = {"", "Emergencia", "Consultas", "Exames"};
                    printf("\n  Paciente cadastrado com sucesso no setor de %s!\n", nomes[novo.tipo]);
                    printf("  ID atribuido: %d\n", novo.id);
                }
                break;
            }

            /* ---- Atender Emergência ---- */
            case 2: {
                exibir_separador();
                printf("     ATENDIMENTO – EMERGENCIA (PILHA)\n");
                exibir_separador();
                Paciente atendido;
                if (pilha_pop(&emergencia, &atendido)) {
                    printf("  Paciente atendido:\n  >> ");
                    exibir_paciente(&atendido);
                } else {
                    printf("  [INFO] Nenhum paciente na Emergencia.\n");
                }
                break;
            }

            /* ---- Atender Consulta ---- */
            case 3: {
                exibir_separador();
                printf("     ATENDIMENTO – CONSULTAS (FILA)\n");
                exibir_separador();
                Paciente atendido;
                if (fila_dequeue(&consultas, &atendido)) {
                    printf("  Paciente atendido:\n  >> ");
                    exibir_paciente(&atendido);
                } else {
                    printf("  [INFO] Nenhum paciente na fila de Consultas.\n");
                }
                break;
            }

            /* ---- Atender Exame ---- */
            case 4: {
                exibir_separador();
                printf("     ATENDIMENTO – EXAMES (FILA CIRCULAR)\n");
                exibir_separador();
                Paciente atendido;
                if (fila_circ_dequeue(&exames, &atendido)) {
                    printf("  Paciente atendido:\n  >> ");
                    exibir_paciente(&atendido);
                } else {
                    printf("  [INFO] Nenhum paciente na fila de Exames.\n");
                }
                break;
            }

            /* ---- Mostrar todos os setores ---- */
            case 5: {
                exibir_separador();
                printf("  SETOR 1 – EMERGENCIA (Pilha | LIFO)\n");
                exibir_separador();
                pilha_exibir(&emergencia);

                exibir_separador();
                printf("  SETOR 2 – CONSULTAS AGENDADAS (Fila | FIFO)\n");
                exibir_separador();
                fila_exibir(&consultas);

                exibir_separador();
                printf("  SETOR 3 – EXAMES / LABORATORIO (Fila Circular | max %d)\n", MAX_EXAME);
                exibir_separador();
                fila_circ_exibir(&exames);
                break;
            }

            /* ---- Transferência ---- */
            case 6:
                transferir(&emergencia, &consultas, &exames);
                break;

            /* ---- Relatório ---- */
            case 7:
                relatorio_geral(&emergencia, &consultas, &exames);
                break;

            /* ---- Sair ---- */
            case 0:
                printf("\n  Encerrando sistema. Ate logo!\n\n");
                continuar = 0;
                break;
        }
    }

    return 0;
}
