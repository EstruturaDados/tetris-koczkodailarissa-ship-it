#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// ============================================================================
//         DESAFIO TETRIS STACK – CONTROLE DE PEÇAS
// ============================================================================
//
// OBJETIVOS:
// - Nível Novato: Fila circular de peças futuras.
// - Nível Aventureiro: Pilha de reserva de peças.
// - Nível Mestre: Integração estratégica entre fila e pilha.
// - Modularização total em funções.
// - Uso de struct, fila circular, pilha e boas práticas.
//
// ============================================================================

// --- Constantes Globais ---
#define MAX_FILA 5
#define MAX_PILHA 3

// --- Estrutura de Dados ---
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;
} Peca;

// --- Variáveis globais ---
Peca fila[MAX_FILA];
int frente = 0, tras = 0, qtdFila = 0;
Peca pilha[MAX_PILHA];
int topo = -1;
int contadorId = 1;

// --- Protótipos ---
void inicializarFila();
bool filaCheia();
bool filaVazia();
void enqueue(Peca p);
Peca dequeue();
void mostrarFila();

void inicializarPilha();
bool pilhaCheia();
bool pilhaVazia();
void push(Peca p);
Peca pop();
void mostrarPilha();

Peca gerarPeca();
void trocarFrenteComTopo();
void trocarTresComPilha();

void exibirMenu();

// --- Função Principal ---
int main() {
    srand(time(NULL));
    inicializarFila();
    inicializarPilha();

    int opcao;
    do {
        exibirMenu();
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia()) {
                    Peca jogada = dequeue();
                    printf("Jogou peça %c (id %d)\n", jogada.tipo, jogada.id);
                    enqueue(gerarPeca());
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            }
            case 2: { // Reservar peça
                if (!filaVazia() && !pilhaCheia()) {
                    Peca reservada = dequeue();
                    push(reservada);
                    printf("Reservou peça %c (id %d)\n", reservada.tipo, reservada.id);
                    enqueue(gerarPeca());
                } else {
                    printf("Não foi possível reservar!\n");
                }
                break;
            }
            case 3: { // Usar peça reservada
                if (!pilhaVazia()) {
                    Peca usada = pop();
                    printf("Usou peça %c (id %d)\n", usada.tipo, usada.id);
                } else {
                    printf("Pilha vazia!\n");
                }
                break;
            }
            case 4: trocarFrenteComTopo(); break;
            case 5: trocarTresComPilha(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }

        mostrarFila();
        mostrarPilha();
        printf("\n-----------------------------\n");

    } while (opcao != 0);

    return 0;
}

// --- Implementações ---
// Fila
void inicializarFila() {
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(gerarPeca());
    }
}
bool filaCheia() { return qtdFila == MAX_FILA; }
bool filaVazia() { return qtdFila == 0; }
void enqueue(Peca p) {
    if (!filaCheia()) {
        fila[tras] = p;
        tras = (tras + 1) % MAX_FILA;
        qtdFila++;
    }
}
Peca dequeue() {
    Peca p = fila[frente];
    frente = (frente + 1) % MAX_FILA;
    qtdFila--;
    return p;
}
void mostrarFila() {
    printf("\nFila: ");
    for (int i = 0, idx = frente; i < qtdFila; i++) {
        printf("[%c-%d] ", fila[idx].tipo, fila[idx].id);
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

// Pilha
void inicializarPilha() { topo = -1; }
bool pilhaCheia() { return topo == MAX_PILHA - 1; }
bool pilhaVazia() { return topo == -1; }
void push(Peca p) {
    if (!pilhaCheia()) pilha[++topo] = p;
}
Peca pop() {
    return pilha[topo--];
}
void mostrarPilha() {
    printf("Pilha: ");
    for (int i = topo; i >= 0; i--) {
        printf("[%c-%d] ", pilha[i].tipo, pilha[i].id);
    }
    printf("\n");
}

// Utilitários
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = contadorId++;
    return p;
}

// Mestre
void trocarFrenteComTopo() {
    if (!filaVazia() && !pilhaVazia()) {
        Peca temp = fila[frente];
        fila[frente] = pilha[topo];
        pilha[topo] = temp;
        printf("Troca realizada entre frente da fila e topo da pilha!\n");
    } else {
        printf("Não foi possível trocar!\n");
    }
}
void trocarTresComPilha() {
    if (qtdFila >= 3 && topo == 2) {
        for (int i = 0; i < 3; i++) {
            int idx = (frente + i) % MAX_FILA;
            Peca temp = fila[idx];
            fila[idx] = pilha[i];
            pilha[i] = temp;
        }
        printf("Troca realizada entre 3 da fila e 3 da pilha!\n");
    } else {
        printf("Condições insuficientes para troca!\n");
    }
}

void exibirMenu() {
    printf("\n===== MENU TETRIS STACK =====\n");
    printf("1 - Jogar peça\n");
    printf("2 - Reservar peça\n");
    printf("3 - Usar peça reservada\n");
    printf("4 - Trocar frente da fila com topo da pilha\n");
    printf("5 - Trocar 3 primeiros da fila com 3 da pilha\n");
    printf("0 - Sair\n");
}

