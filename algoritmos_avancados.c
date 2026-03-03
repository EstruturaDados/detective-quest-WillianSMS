#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

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

// Estrutura da tabela hash (pista -> suspeito)
typedef struct HashItem {
    char pista[50];
    char suspeito[50];
    struct HashItem* prox;    // Encadeamento para colisões
} HashItem;

HashItem* tabela[TAM_HASH];   // Tabela hash global

// ---------------- Funções auxiliares ----------------

// criarSala() – cria dinamicamente um cômodo
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// inserirPista() – insere uma pista coletada na árvore BST
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

// Função hash() – calcula índice da tabela hash
int hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAM_HASH;
}

// inserirNaHash() – insere associação pista/suspeito na tabela hash
void inserirNaHash(const char* pista, const char* suspeito) {
    int idx = hash(pista);
    HashItem* novo = (HashItem*) malloc(sizeof(HashItem));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

// encontrarSuspeito() – consulta o suspeito correspondente a uma pista
char* encontrarSuspeito(const char* pista) {
    int idx = hash(pista);
    HashItem* atual = tabela[idx];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// explorarSalas() – navega pela árvore e ativa o sistema de pistas
PistaNode* explorarSalas(Sala* atual, PistaNode* arvorePistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na %s.\n", atual->nome);

        // Exibe pista da sala
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

// verificarSuspeitoFinal() – conduz à fase de julgamento final
int contarPistasSuspeito(PistaNode* raiz, const char* acusado) {
    if (raiz == NULL) return 0;
    int contador = 0;
    char* suspeito = encontrarSuspeito(raiz->pista);
    if (suspeito != NULL && strcmp(suspeito, acusado) == 0)
        contador++;
    contador += contarPistasSuspeito(raiz->esq, acusado);
    contador += contarPistasSuspeito(raiz->dir, acusado);
    return contador;
}

void verificarSuspeitoFinal(PistaNode* arvorePistas, const char* acusado) {
    int qtd = contarPistasSuspeito(arvorePistas, acusado);
    if (qtd >= 2)
        printf("\nAcusação válida! %s é o culpado.\n", acusado);
    else
        printf("\nAcusação fraca. %s não pode ser condenado.\n", acusado);
}

// ---------------- Função principal ----------------
int main() {
    // Monta a mansão (árvore binária de salas)
    Sala* hall = criarSala("Hall de Entrada", "Luvas sujas");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo quebrado");
    Sala* cozinha = criarSala("Cozinha", "Faca ensanguentada");
    Sala* jardim = criarSala("Jardim", "Pegadas na terra");
    Sala* biblioteca = criarSala("Biblioteca", "Carta rasgada");

    hall->esquerda = salaEstar;
    hall->direita = cozinha;
    salaEstar->esquerda = jardim;
    salaEstar->direita = biblioteca;

    // Cria tabela hash de pistas -> suspeitos
    inserirNaHash("Luvas sujas", "Carlos");
    inserirNaHash("Copo quebrado", "Ana");
    inserirNaHash("Faca ensanguentada", "Carlos");
    inserirNaHash("Pegadas na terra", "Bruno");
    inserirNaHash("Carta rasgada", "Ana");

    // Exploração
    PistaNode* arvorePistas = NULL;
    arvorePistas = explorarSalas(hall, arvorePistas);

    // Julgamento final
    char acusado[50];
    printf("\nDigite o nome do suspeito que deseja acusar: ");
    scanf(" %[^\n]", acusado);

    verificarSuspeitoFinal(arvorePistas, acusado);

    return 0;
}
