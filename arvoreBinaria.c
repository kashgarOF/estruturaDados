#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME_SALA 50
#define TAMANHO_TABELA 100 // O tamanho da nossa tabela hash

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


// Estrutura para cada entrada na lista encadeada da tabela hash
typedef struct HashNo {
    char pista[MAX_NOME_SALA];
    char suspeito[MAX_NOME_SALA];
    struct HashNo* proximo;
} HashNo;

HashNo* tabelaHash[TAMANHO_TABELA];


// --- MODULARIZAÇÃO DAS FUNÇÕES ---

No* criarSala(const char* nome, const char* pista);
void explorarSalas(No* salaAtual, PistaNo** raizPistas);
PistaNo* inserirPista(PistaNo* raizPistas, const char* pista);
void exibirPistas(PistaNo* raizPistas); 
void liberarMemoriaPistas(PistaNo* raiz);
unsigned int funcaoHash(const char* chave);
void inserirNaHash(const char* pista, const char* suspeito);
const char* encontrarSuspeito(const char* pista);
void verificarAcusacao(PistaNo* raizPistas, const char* suspeitoAcusado);
void liberarMemoriaHash();
void contarEvidencias(PistaNo* raizPistas, const char* suspeitoAcusado, int* contador);


// Funções Utilitárias
void limparBufferUp(void);
void liberarMemoria(No* raiz);


//-------------- Função principal ----------------

int main() {
    // 1. Inicializa todas as estruturas
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabelaHash[i] = NULL;
    }
    PistaNo* raizPistas = NULL;

    // 2. Monta o Mapa da Mansão
    No* raizMapa = criarSala("Hall de Entrada", "Um casaco molhado no cabide.");
    raizMapa->esquerda = criarSala("Sala de Estar", "Uma lareira recem-apagada.");
    raizMapa->direita = criarSala("Biblioteca", NULL);
    raizMapa->esquerda->esquerda = criarSala("Quarto", "Uma janela quebrada.");
    raizMapa->direita->direita = criarSala("Cozinha", "Faca de cozinha desaparecida.");

    // 3. Popula a Tabela Hash com as "verdades" do crime
    inserirNaHash("Um casaco molhado no cabide.", "Sr. Mostarda");
    inserirNaHash("Uma lareira recem-apagada.", "Sra. Branca");
    inserirNaHash("Uma janela quebrada.", "Sr. Mostarda");
    inserirNaHash("Faca de cozinha desaparecida.", "Cozinheiro");

    // 4. Inicia a Exploração
    printf("Detetive, bem-vindo a mansao!\n");
    explorarSalas(raizMapa, &raizPistas);

    // 5. Exibe as Pistas Coletadas
    printf("\n--- PISTAS COLETADAS (em ordem alfabetica) ---\n");
    if (raizPistas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(raizPistas);
    }
    printf("--------------------------------------------\n");

    // 6. Fase de Acusação
    if (raizPistas != NULL) {
        char suspeitoAcusado[MAX_NOME_SALA];
        printf("\nChegou a hora da verdade, detetive.\n");
        printf("Quem voce acusa? (Sr. Mostarda, Sra. Branca, Cozinheiro): ");
        fgets(suspeitoAcusado, MAX_NOME_SALA, stdin);
        limparBufferUp(); // Limpa o buffer após o fgets
        suspeitoAcusado[strcspn(suspeitoAcusado, "\n")] = 0; // Remove o \n

        int contador = 0;
        contarEvidencias(raizPistas, suspeitoAcusado, &contador);

        printf("\n--- VEREDITO ---\n");
        printf("Voce coletou %d pista(s) contra %s.\n", contador, suspeitoAcusado);

        if (contador >= 2) {
            printf("Evidencias suficientes! O culpado e %s! Caso encerrado!\n", suspeitoAcusado);
        } else {
            printf("Evidencias insuficientes. O culpado escapou desta vez...\n");
        }
    }

    // 7. Limpeza Final de TODA a memória
    liberarMemoria(raizMapa);
    liberarMemoriaPistas(raizPistas);
    liberarMemoriaHash();
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


// Função Hash: Transforma uma string (chave) em um índice para a tabela.
unsigned int funcaoHash(const char* chave) {
    unsigned long int hash = 5381;
    int c;

    while ((c = *chave++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash % TAMANHO_TABELA;
}



// Insere uma associação Pista -> Suspeito na Tabela Hash usando encadeamento
void inserirNaHash(const char* pista, const char* suspeito) {
    // 1. Usa a função hash para descobrir em qual índice a pista deve ir.
    unsigned int indice = funcaoHash(pista);

    // 2. Cria um novo "elo" (nó) para guardar a associação.
    HashNo* novoNo = (HashNo*) malloc(sizeof(HashNo));
    if (novoNo == NULL) {
        perror("Erro ao alocar memoria para no da tabela hash");
        return;
    }
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);

    // 3. Adiciona o novo elo NO INÍCIO da corrente naquele índice.
    // A lógica é a mesma de inserir no início de uma lista encadeada.
    novoNo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNo;
}



// Procura por uma pista na Tabela Hash e retorna o suspeito associado.
// Retorna NULL se a pista não for encontrada.
const char* encontrarSuspeito(const char* pista) {
    // 1. Descobre em qual índice a pista deveria estar.
    unsigned int indice = funcaoHash(pista);

    // 2. Pega o primeiro elo da corrente nesse índice.
    HashNo* atual = tabelaHash[indice];

    // 3. Percorre a corrente (lista encadeada).
    while (atual != NULL) {
        // Compara a pista do nó atual com a pista que estamos procurando.
        if (strcmp(atual->pista, pista) == 0) {
            // ENCONTROU! Retorna o nome do suspeito.
            return atual->suspeito;
        }
        // Se não for este, pula para o próximo elo da corrente.
        atual = atual->proximo;
    }

    // Se o loop terminar, significa que percorremos a corrente inteira e não encontrámos.
    return NULL;
}


// Função recursiva que percorre a árvore de pistas coletadas e conta as evidências
// contra o suspeito acusado.
void contarEvidencias(PistaNo* raizPistas, const char* suspeitoAcusado, int* contador) {
    if (raizPistas == NULL) {
        return;
    }
    
    // Procura o suspeito associado à pista atual na tabela hash
    const char* suspeitoEncontrado = encontrarSuspeito(raizPistas->pista);
    
    // Se um suspeito foi encontrado E o nome dele é igual ao do acusado...
    if (suspeitoEncontrado != NULL && strcmp(suspeitoEncontrado, suspeitoAcusado) == 0) {
        (*contador)++; // Incrementa o contador de evidências
    }
    
    // Continua a busca nos galhos da árvore de pistas
    contarEvidencias(raizPistas->esquerda, suspeitoAcusado, contador);
    contarEvidencias(raizPistas->direita, suspeitoAcusado, contador);
}


// Libera toda a memória alocada para a Tabela Hash
void liberarMemoriaHash() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        HashNo* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNo* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
}



/*---limpar buffer -> limpando o caminho para a proxima leitura---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}