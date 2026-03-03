#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
// Cada sala possui um nome e até dois caminhos (esquerda e direita)
typedef struct Sala {
    char nome[50];            // Nome da sala
    struct Sala *esquerda;    // Ponteiro para sala à esquerda
    struct Sala *direita;     // Ponteiro para sala à direita
} Sala;

// Função criarSala()
// Cria dinamicamente uma sala com o nome fornecido
// Inicializa os ponteiros esquerda e direita como NULL
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala)); // Aloca memória para a sala
    strcpy(nova->nome, nome);                  // Copia o nome para a sala
    nova->esquerda = NULL;                     // Inicializa sem filhos
    nova->direita = NULL;
    return nova;                               // Retorna ponteiro para a nova sala
}

// Função explorarSalas()
// Permite ao jogador navegar pela mansão interativamente
// O jogador escolhe ir para esquerda (e), direita (d) ou sair (s)
void explorarSalas(Sala* atual) {
    char escolha;
    while (atual != NULL) {
        // Exibe o nome da sala atual
        printf("\nVocê está na %s.\n", atual->nome);

        // Caso seja um nó-folha (sem filhos), encerra a exploração
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim da exploração! Você chegou a um cômodo sem saídas.\n");
            break;
        }

        // Menu de opções para o jogador
        printf("Escolha: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);

        // Navegação para a esquerda
        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda; // Move para a sala à esquerda
            } else {
                printf("Não há sala à esquerda!\n");
            }
        }
        // Navegação para a direita
        else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita; // Move para a sala à direita
            } else {
                printf("Não há sala à direita!\n");
            }
        }
        // Encerrar exploração
        else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        }
        // Caso o jogador digite algo inválido
        else {
            printf("Opção inválida!\n");
        }
    }
}

// Função main()
// Monta manualmente o mapa da mansão como uma árvore binária
// Inicia a exploração a partir do Hall de entrada
int main() {
    // Criação das salas
    Sala* hall = criarSala("Hall de entrada");
    Sala* salaEstar = criarSala("Sala de estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* jardim = criarSala("Jardim");
    Sala* biblioteca = criarSala("Biblioteca");

    // Conexões da árvore (estrutura fixa da mansão)
    hall->esquerda = salaEstar;       // À esquerda do Hall fica a Sala de estar
    hall->direita = cozinha;          // À direita do Hall fica a Cozinha
    salaEstar->esquerda = jardim;     // À esquerda da Sala de estar fica o Jardim
    salaEstar->direita = biblioteca;  // À direita da Sala de estar fica a Biblioteca

    // Início da exploração interativa
    explorarSalas(hall);

    return 0; // Fim do programa
}
