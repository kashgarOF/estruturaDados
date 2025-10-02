#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_SALA 50

// --- ESTRUTURAS DE DADOS ---

// um ponteiro para outro Nó à esquerda, e um ponteiro para outro Nó à direita."
typedef struct No {
    char nome[MAX_NOME_SALA]; // O nome do cômodo (ex: "Biblioteca")
    struct No *esquerda;      // Ponteiro para o cômodo da esquerda (ou NULL se não houver)
    struct No *direita;       // Ponteiro para o cômodo da direita (ou NULL se não houver)
} No;

// --- MODULARIZAÇÃO DAS FUNÇÕES ---

No* criarSala(const char* nome);
void explorarSalas(No* salaAtual);


// Funções Utilitárias
void limparBufferUp(void);
void liberarMemoria(No* raiz);


//-------------- Função principal ----------------

int main() {
    // A raiz da nossa árvore (o ponto de partida)
    No* raiz = criarSala("Entrada");

    // Conectando as portas (ponteiros)
    raiz->esquerda = criarSala("Sala de Estar");
    raiz->direita = criarSala("Biblioteca");
    raiz->esquerda->esquerda = criarSala("Quarto");

    printf("Detetive, bem-vindo a mansao!\n");

    // --- Iniciando a Exploração ---
    explorarSalas(raiz);

    // --- Limpeza Final ---
    // Libera toda a memória alocada para a árvore antes de encerrar.
    liberarMemoria(raiz);
    printf("\nMemoria da mansao liberada. Jogo encerrado.\n");

    return 0;
}



//---------------------------- IMPLEMENTAÇÕES DAS FUNÇÕES --------------------------------------


// Função para alocar memória e criar um novo nó (sala) da árvore.
No* criarSala(const char* nome) {

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

     // 3. Retorna o endereço da sala recém-criada.
    return novaSala;
}


// Função recursiva para navegar pela mansão (árvore).
void explorarSalas(No* salaAtual) {

    // CONDIÇÃO DE PARADA 1: Chegou a um caminho que não existe.
    if(salaAtual == NULL) {
        printf("\n>> Nao ha um comodo nesse caminho.\n");
        return;
    }
    
    printf("\n-----------------------------------");
    printf("\nVoce esta em: %s\n", salaAtual->nome);

    // CONDIÇÃO DE PARADA 2: Chegou a um cômodo sem saídas.
    if(salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf(">> Este comodo nao tem mais saidas. Fim da exploracao neste caminho.\n");
        return;  
    }

    char escolha;
    printf("Para onde voce quer ir? ('e' para esquerda, 'd' para direita, 's' para sair): ");  
    scanf(" %c", &escolha);
    limparBufferUp();

    if (escolha == 'e') {
        // CHAMADA RECURSIVA: Manda um "clone" para a sala da esquerda.
        explorarSalas(salaAtual->esquerda);
    } else if (escolha == 'd') {
        // CHAMADA RECURSIVA: Manda um "clone" para a sala da direita.
        explorarSalas(salaAtual->direita);
    } else if (escolha == 's') {
        printf("Voce decidiu parar a exploracao por enquanto.\n");
    } else {
        printf("Opcao invalida! Tente novamente na mesma sala.\n");
        // CHAMADA RECURSIVA: Manda um "clone" para a sala ATUAL, para o jogador tentar de novo.
        explorarSalas(salaAtual);
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



/*---limpar buffer -> limpando o caminho para a proxima leitura---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}