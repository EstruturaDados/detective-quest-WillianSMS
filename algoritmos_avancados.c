#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];            // Nome da sala
    char pista[50];           // Pista associada ao cômodo
    struct Sala *esquerda;    // Ponteiro para sala à esquerda
    struct Sala *direita;     // Ponteiro para sala à direita
} Sala;

// Estrutura da árvore BST de pistas
typedef struct PistaNode {
    char pista[50];           // Nome da pista
    struct PistaNode *esq;    // Filho à esquerda
    struct PistaNode *dir;    // Filho à direita
} PistaNode;

// Função criarSala()
// Cria dinamicamente um cômodo com nome e pista
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função inserirPista()
// Insere uma nova pista na árvore BST
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esq = nova->dir = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

// Função exibirPistas()
// Exibe todas as pistas em ordem alfabética (in-order traversal)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

// Função explorarSalasComPistas()
// Permite navegação pela mansão e coleta de pistas
PistaNode* explorarSalasComPistas(Sala* atual, PistaNode* arvorePistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na %s.\n", atual->nome);

        // Se houver pista na sala, coleta e insere na BST
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            arvorePistas = inserirPista(arvorePistas, atual->pista);
        }

        // Menu de opções
        printf("Escolha: esquerda (e), direita (d), sair (s): ");
        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda!\n");
        } else if (escolha == 'd') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há sala à direita!\n");
        } else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }
    return arvorePistas;
}

// Função main()
// Monta o mapa da mansão e inicia a exploração
int main() {
    // Criação das salas com pistas
    Sala* hall = criarSala("Hall de Entrada", "Luvas sujas");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala* cozinha = criarSala("Cozinha", "Faca ensanguentada");
    Sala* jardim = criarSala("Jardim", "Pegadas na terra");
    Sala* biblioteca = criarSala("Biblioteca", "Carta rasgada");

    // Conexões da árvore da mansão
    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;

    // Exploração e coleta de pistas
    PistaNode* arvorePistas = NULL;
    arvorePistas = explorarSalasComPistas(hall, arvorePistas);

    // Exibição final das pistas coletadas
    printf("\nPistas coletadas (em ordem alfabética):\n");
    exibirPistas(arvorePistas);

    return 0;
}
