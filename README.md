# Hospital Simulado
---
## Objetivo:
Desenvolver um sistema em linguagem C que simule o fluxo de pacientes em um
hospital utilizando Pilha, Fila e Fila Circular, implementadas com vetores (arrays).

---
## Requisitos Obrigatórios
1. Cadastro de Paciente
- Nome (string)
- Idade (int)
- Gravidade (1 a 5)
- Tipo de Atendimento (1-Emergência, 2-Consulta, 3-Exame)
2. Menu Principal
- Cadastrar novo paciente
- Atender paciente em cada setor
- Mostrar pacientes em cada setor
- Transferir paciente entre setores
- Relatórios
- Sair
3. Implementação das Estruturas
- Pilha (LIFO): Emergência
- Fila (FIFO): Consultas Agendadas
- Fila Circular: Exames/Laboratório
4. Regras de Negócio
- Pacientes com gravidade 4 ou 5 devem ser direcionados para Emergência
- Ao atender, exibir os dados do paciente removido
- Na fila circular, ao at
