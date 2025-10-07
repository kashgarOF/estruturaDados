#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_SALA 50

// --- ESTRUTURAS DE DADOS ---

// um ponteiro para outro Nó à esquerda, e um ponteiro para outro Nó à direita."
typedef struct No {
    char nome[MAX_NOME_SALA]; // O nome do cômodo (ex: "Biblioteca")
    char* pista;
    struct No *esquerda;      // Ponteiro para o cômodo da esquerda (ou NULL se não houver)
    struct No *direita;       // Ponteiro para o cômodo da direita (ou NULL se não houver)
} No;

typedef struct PistaNo {
    char pista[MAX_NOME_SALA];
    struct PistaNo *esquerda;
    struct PistaNo *direita;
} PistaNo;



// --- MODULARIZAÇÃO DAS FUNÇÕES ---

No* criarSala(const char* nome, const char* pista);
void explorarSalas(No* salaAtual, PistaNo** raizPistas);
PistaNo* inserirPista(PistaNo* raizPistas, const char* pista);
void exibirPistas(PistaNo* raizPistas); 
void liberarMemoriaPistas(PistaNo* raiz); 


// Funções Utilitárias
void limparBufferUp(void);
void liberarMemoria(No* raiz);


//-------------- Função principal ----------------

int main() {
    // --- Montando o Mapa da Mansão com Pistas Escondidas ---
    No* raizMapa = criarSala("Hall de Entrada", "Um casaco molhado no cabide.");

    // CONSTRUÇÃO DO MAPA
    raizMapa->esquerda = criarSala("Sala de Estar", "Uma lareira recem-apagada.");
    raizMapa->direita = criarSala("Biblioteca", NULL); // Sala sem pista
    raizMapa->esquerda->esquerda = criarSala("Quarto", "Uma janela quebrada.");
    raizMapa->direita->direita = criarSala("Cozinha", "Faca de cozinha desaparecida.");

    // Agora a árvore de pistas (o caderno do detetive)
    PistaNo* raizPistas = NULL;

    printf("Detetive, bem-vindo a mansao!\n");
    
    // Inicia a exploração, passando o mapa COMPLETO e o caderno de pistas
    explorarSalas(raizMapa, &raizPistas);

    // --- Exibindo o resultado final ---
    printf("\n--- PISTAS COLETADAS (em ordem alfabetica) ---\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(raizPistas);
    }

    // --- Limpeza Final ---
    liberarMemoria(raizMapa);
    liberarMemoriaPistas(raizPistas);
    printf("\nMemorias liberadas. Jogo encerrado.\n");

    return 0;
}



//---------------------------- IMPLEMENTAÇÕES DAS FUNÇÕES --------------------------------------


// Função para alocar memória e criar um novo nó (sala) da árvore.
No* criarSala(const char* nome, const char* pista) {

    // 1. Pede ao sistema memória suficiente para um novo 'No'.
    No* novaSala = (No*) malloc(sizeof(No));

    // Verificação de segurança: O sistema concedeu a memória?
    if (novaSala == NULL) {
        perror("Erro ao alocar memoria para a nova sala");
        exit(1);
    }

    // 2. Preenche os dados da nova sala.
    strcpy(novaSala->nome, nome);   // Copia o nome para dentro da struct.
    novaSala->esquerda = NULL;  // A porta da esquerda começa fechada.
    novaSala->direita = NULL;   // A porta da direita começa fechada.

    if (pista != NULL) {
        //aloca memoria para a pista e copia
        novaSala->pista = (char*) malloc(strlen(pista) + 1);
        strcpy(novaSala->pista, pista);
    } else {
        //se não houver pista...
        novaSala->pista = NULL;
    }

     // 3. Retorna o endereço da sala recém-criada.
    return novaSala;
}


// Função recursiva para navegar pela mansão (árvore).
void explorarSalas(No* salaAtual, PistaNo** raizPistas) {
    if (salaAtual == NULL) {
        printf("\n>> Nao ha um comodo nesse caminho.\n");
        return;
    }

    printf("\n-----------------------------------");
    printf("\nVoce esta em: %s\n", salaAtual->nome);

    // Verifica se a sala ATUAL tem uma pista
    if (salaAtual->pista != NULL) {
        printf(">> Pista encontrada: %s\n", salaAtual->pista);
        // Adiciona a pista encontrada à árvore de pistas
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
    } else {
        printf(">> Nenhuma pista encontrada aqui.\n");
    }

    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf(">> Este comodo nao tem mais saidas.\n");
        return;
    }

    char escolha;
    printf("Para onde voce quer ir? ('e' para esquerda, 'd' para direita, 's' para sair): ");  
    scanf(" %c", &escolha);
    limparBufferUp();

    if (escolha == 'e') {
        // CHAMADA RECURSIVA: Manda um "clone" para a sala da esquerda.
        explorarSalas(salaAtual->esquerda, raizPistas);
    } else if (escolha == 'd') {
        // CHAMADA RECURSIVA: Manda um "clone" para a sala da direita.
        explorarSalas(salaAtual->direita, raizPistas);
    } else if (escolha == 's') {
        printf("Voce decidiu parar a exploracao por enquanto.\n");
    } else {
        printf("Opcao invalida! Tente novamente na mesma sala.\n");
        // CHAMADA RECURSIVA: Manda um "clone" para a sala ATUAL, para o jogador tentar de novo.
        explorarSalas(salaAtual, raizPistas);
    }
}



// Função recursiva (pós-ordem) para liberar toda a memória alocada para a árvore.
void liberarMemoria(No* raiz) {
    // Condição de parada: se o nó não existe, não há nada a fazer.
    if (raiz == NULL) {
        return;
    }

    // Libera primeiro os filhos (galhos) antes de liberar o pai (tronco).
    liberarMemoria(raiz->esquerda);
    liberarMemoria(raiz->direita);
    
    // Agora que os filhos já foram liberados, podemos liberar o nó atual.
    free(raiz);
}



// Função recursiva para inserir uma nova pista na Árvore de Busca Binária (BST)
PistaNo* inserirPista(PistaNo* raizPistas, const char* pista) {
    // Caso Base 1: Se a árvore/sub-árvore está vazia, encontramos o local de inserção.
    if (raizPistas == NULL) {
        PistaNo* novoNo = (PistaNo*) malloc(sizeof(PistaNo));
        strcpy(novoNo->pista, pista);
        novoNo->esquerda = novoNo->direita = NULL;
        return novoNo; // Retorna o novo nó para ser "conectado" pelo nó pai.
    }

    // Compara a nova pista com a pista na raiz da sub-árvore atual
    int comparacao = strcmp(pista, raizPistas->pista);

    // Navegação recursiva
    if (comparacao < 0) {
        // Se a nova pista é "menor" (vem antes), insere na sub-árvore da esquerda.
        raizPistas->esquerda = inserirPista(raizPistas->esquerda, pista);
    } else if (comparacao > 0) {
        // Se a nova pista é "maior" (vem depois), insere na sub-árvore da direita.
        raizPistas->direita = inserirPista(raizPistas->direita, pista);
    }
    // Se comparacao == 0, a pista já existe, então não fazemos nada.

    return raizPistas;
}


// Função para exibir as pistas em ordem alfabética (percurso em-ordem)
void exibirPistas(PistaNo* raizPistas) {
    if (raizPistas != NULL) {
        exibirPistas(raizPistas->esquerda);
        printf("- %s\n", raizPistas->pista);
        exibirPistas(raizPistas->direita);
    }
}

// Função para liberar a memória da árvore de pistas
void liberarMemoriaPistas(PistaNo* raiz) {
    if (raiz == NULL) return;
    liberarMemoriaPistas(raiz->esquerda);
    liberarMemoriaPistas(raiz->direita);
    free(raiz);
}


/*---limpar buffer -> limpando o caminho para a proxima leitura---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}