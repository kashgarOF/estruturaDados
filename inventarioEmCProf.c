#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Resumo das Ferramentas Explicadas:
scanf: Para ler dados formatados (principalmente números) do teclado.

getchar: Para "limpar" o buffer do teclado, um truque essencial em C.

fgets: A forma segura de ler uma linha de texto.

strcspn: Uma ferramenta mais específica para encontrar a posição de um caractere e nos ajudar a remover o \n do final do fgets.

strcpy: Para copiar o conteúdo de uma string para outra.

strcmp: Para comparar se duas strings são iguais, ou qual vem antes na ordem alfabética.

strlen: Para medir o comprimento de uma string (e sabermos quanta memória alocar).

malloc: Para pedir memória ao sistema operacional.

free: Para devolver a memória que pedimos, evitando "vazamentos".*/



// --- PARTE 1: LISTA ESTÁTICA (VETOR DE STRINGS) ---

#define TAM_MAX 10      // Define o número máximo de itens que nossa lista pode conter.
#define MAX_STR_LEN 50  // Define o comprimento máximo de cada string/texto.

// Estrutura que representa a lista estática
typedef struct {
    char dados[TAM_MAX][MAX_STR_LEN]; // Matriz: 10 linhas (itens), 50 colunas (caracteres por item)
    int quantidade;                   // Nosso contador de controle. Ele informa quantos itens estão realmente armazenados na lista.
} ListaEstatica;

// Funções da Lista Estática
void inicializarListaEstatica(ListaEstatica *lista);
void inserirListaEstatica(ListaEstatica *lista, const char* texto);
void removerListaEstatica(ListaEstatica *lista, const char* texto);
void listarListaEstatica(const ListaEstatica *lista);
void menuListaEstatica();


// --- PARTE 2: LISTA ENCADEADA (DE STRINGS) ---

// Estrutura do nó (cada elemento da lista)
// char* dado: Este é um ponteiro. Ele não armazena o texto em si, mas sim o endereço de memória onde o texto está guardado.
// struct No *proximo: Este é o ponteiro que cria a "ligação", e alocamos o endereço do próximo nó na lista.
typedef struct No {
    char *dado;
    struct No *proximo;
} No;

// Um atalho. Em vez de escrever No* sempre que quisermos nos referir à nossa lista,
// podemos simplesmente usar "ListaEncadeada". O ponteiro é para o primeiro nó (a "cabeça" da lista).
typedef No* ListaEncadeada;

// Funções da Lista Encadeada
void inicializarListaEncadeada(ListaEncadeada *lista);
void inserirListaEncadeada(ListaEncadeada *lista, const char* texto);
void removerListaEncadeada(ListaEncadeada *lista, const char* texto);
void listarListaEncadeada(const ListaEncadeada lista);
void liberarListaEncadeada(ListaEncadeada *lista); // Função nova para limpar a memória
void menuListaEncadeada();


// --- FUNÇÃO PRINCIPAL (MAIN) E MENUS ---

int main() {
    int opcao;
    do {
        printf("\n--- MANIPULACAO DE LISTAS (TEXTO) ---\n");
        printf("1. Lista Estatica\n");
        printf("2. Lista Encadeada\n");
        printf("0. Sair do Programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do teclado (consome o '\n' do scanf)

        switch (opcao) {
            case 1:
                menuListaEstatica();
                break;
            case 2:
                menuListaEncadeada();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
    return 0;
}

void menuListaEstatica() {
    ListaEstatica lista;
    inicializarListaEstatica(&lista);
    int opcao;
    char texto[MAX_STR_LEN];

    do {
        printf("\n--- MENU LISTA ESTATICA ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                printf("Digite o texto a inserir: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0; // Remove o '\n' do final
                inserirListaEstatica(&lista, texto);
                break;
            case 2:
                printf("Digite o texto a remover: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0;
                removerListaEstatica(&lista, texto);
                break;
            case 3:
                listarListaEstatica(&lista);
                break;
            case 0:
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}

void menuListaEncadeada() {
    ListaEncadeada lista;
    inicializarListaEncadeada(&lista);
    int opcao;
    char texto[MAX_STR_LEN];

    do {
        printf("\n--- MENU LISTA ENCADEADA ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Digite o texto a inserir: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0;
                inserirListaEncadeada(&lista, texto);
                break;
            case 2:
                printf("Digite o texto a remover: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0;
                removerListaEncadeada(&lista, texto);
                break;
            case 3:
                listarListaEncadeada(lista);
                break;
            case 0:
                liberarListaEncadeada(&lista); // Limpa toda a memória antes de sair
                break;
            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (opcao != 0);
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES - LISTA ESTÁTICA ---

void inicializarListaEstatica(ListaEstatica *lista) {
    lista->quantidade = 0;
}

void inserirListaEstatica(ListaEstatica *lista, const char* texto) {
    if (lista->quantidade >= TAM_MAX) {
        printf("Erro: Lista cheia! Nao e possivel inserir.\n");
        return;
    }
    // Copia o conteúdo da string 'texto' para a posição correta na nossa matriz de dados.
    strcpy(lista->dados[lista->quantidade], texto);
    lista->quantidade++;
    printf("Texto \"%s\" inserido com sucesso.\n", texto);
}

void removerListaEstatica(ListaEstatica *lista, const char* texto) {
    int i, pos = -1;
    
    // Para comparar strings, usamos strcmp(). Retorna 0 se forem iguais.
    // A função percorre os itens em uso na lista (de 0 até quantidade).
    for (i = 0; i < lista->quantidade; i++) {
        if (strcmp(lista->dados[i], texto) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Erro: Texto \"%s\" nao encontrado na lista.\n", texto);
        return;
    }

    // Para preencher a lacuna, movemos todos os itens subsequentes uma posição para a esquerda.
    // Deslocar os elementos posteriores para a esquerda usando strcpy.
    for (i = pos; i < lista->quantidade - 1; i++) {
        strcpy(lista->dados[i], lista->dados[i + 1]);
    }

    lista->quantidade--; // Atualização do contador
    printf("Texto \"%s\" removido com sucesso.\n", texto);
}

void listarListaEstatica(const ListaEstatica *lista) {
    printf("\nItens da Lista Estatica:\n");
    if (lista->quantidade == 0) {
        printf("A lista estatica esta vazia.\n");
        return;
    }
    for (int i = 0; i < lista->quantidade; i++) {
        printf("[%d]: %s\n", i, lista->dados[i]);
    }
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES - LISTA ENCADEADA ---

void inicializarListaEncadeada(ListaEncadeada *lista) {
    *lista = NULL;
}

void inserirListaEncadeada(ListaEncadeada *lista, const char* texto) {
    // 1. Alocação da memória para o NOVO NÓ
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocacao de memoria para o no.\n");
        return;
    }

    // 2. Alocar memória para a STRING DENTRO DO NÓ
    // strlen conta os caracteres, e o +1 é vital para o caractere nulo ('\0') que finaliza toda string em C.
    novoNo->dado = (char*) malloc(strlen(texto) + 1);
    if (novoNo->dado == NULL) {
        printf("Erro: Falha na alocacao de memoria para o texto.\n");
        free(novoNo); // Libera o nó que já foi alocado
        return;
    }

    // 3. Copiar o texto para a memória recém-alocada
    strcpy(novoNo->dado, texto);

    // 4. Ligar o novo nó à lista
    // O ponteiro proximo do novo nó passa a apontar para o que era o antigo primeiro nó da lista.
    novoNo->proximo = *lista;
    
    // A cabeça da lista (*lista) é atualizada para apontar para o novoNo. O novoNo é agora o primeiro item.
    *lista = novoNo;

    printf("Texto \"%s\" inserido com sucesso.\n", texto);
}

void removerListaEncadeada(ListaEncadeada *lista, const char* texto) {
    No *atual = *lista;
    No *anterior = NULL;

    // Percorremos a lista com dois ponteiros, atual e anterior, para ser removido.
    while (atual != NULL && strcmp(atual->dado, texto) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Erro: Texto \"%s\" nao encontrado na lista.\n", texto);
        return;
    }

    // Se o nó a ser removido for o primeiro (anterior == NULL)
    if (anterior == NULL) {
        *lista = atual->proximo;
    } else { // Se estiver no meio/fim, o ponteiro proximo do anterior "pula" o atual
        anterior->proximo = atual->proximo;
    }

    // Liberação de Memória (Muito Importante!)
    free(atual->dado); // Libera a memória da string DENTRO do nó.
    free(atual);       // AGORA podemos liberar o nó.
    printf("Texto \"%s\" removido com sucesso.\n", texto);
}

void listarListaEncadeada(const ListaEncadeada lista) {
    No *temp = lista;
    printf("\nItens da Lista Encadeada:\n");
    if (temp == NULL) {
        printf("A lista encadeada esta vazia.\n");
        return;
    }
    int i = 0;
    while (temp != NULL) {
        printf("[%d]: %s\n", i, temp->dado);
        temp = temp->proximo;
        i++;
    }
}

void liberarListaEncadeada(ListaEncadeada *lista) {
    No *atual = *lista;
    No *proximo;

    // Percorre todos os nós da lista, um por um, e aplica a mesma lógica de liberação dupla (free)
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual->dado); // Libera a string
        free(atual);       // Libera o nó
        atual = proximo;
    }
    *lista = NULL; // Garante que a lista aponte para NULL após ser liberada
}