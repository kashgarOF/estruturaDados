#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 10
#define MAX_STR_LEN 30


//Estatico
typedef struct {
    char nomes [TAM_MAX][MAX_STR_LEN];
    char tipo [MAX_STR_LEN];
    int quantidade;
} InventarioEstatico;

// funções (modularização)
void inicializarInvetario (InventarioEstatico *lista);
void inserirItem (InventarioEstatico *lista, const char* texto);
void removerItem (InventarioEstatico *lista, const char* texto);
void listarInventarioEstatico (const InventarioEstatico *lista);
void menuInventarioEstatico();


//encadeado
typedef struct Inv {
    char *nome;
    struct Inv *proximo;
}Inv;

typedef Inv* InventarioEncadeado;

//funções (encadeada)
void inicializarInvetarioEncadeado (InventarioEncadeado *lista);
void inserirItemEncadeado (InventarioEncadeado *lista, const char* texto);
void removerItemEncadeado (InventarioEncadeado *lista, const char* texto);
void listarInventarioEncadeado (const InventarioEncadeado lista);
void liberarInventarioEncadeado (InventarioEncadeado *lista);
void menuInventarioEncadeado();


// função principal

int main() {
    int opcao;
    do
    {
        printf("\n--- TIPOS DE INVENTARIO ---\n");
        printf("1. Inventario estatico\n");
        printf("2. Inventario Encadeado\n");
        printf("0. Sair do inventario\n");
        printf("Escolha uma opcao: \n");
        scanf("%d", &opcao);
        getchar(); //limpa buffer(\n)

        switch (opcao)
        {
        case 1:
            menuInventarioEstatico();
            break;
        case 2:
            menuInventarioEncadeado();
            break;
        case 0:
            printf("Saindo do inventario!\n");
            break;
        default:
            perror("ERROR - Opcao invalida, tente novamente ou pressione '0' para sair do inventario\n");
            break;
        }
    } while (opcao != 0);
    return 0;
    
}

void menuInventarioEstatico() {
    InventarioEstatico lista;
    inicializarInvetario(&lista);
    int opcao;
    char texto[MAX_STR_LEN];

    do
    {
        printf("\n--- MENU INVENTARIO ESTATICA ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                printf("Digite o item a inserir: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0; // Remove o '\n' do final
                inserirItem(&lista, texto);
                break;
            case 2:
                printf("Digite o item a remover: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0;
                removerItem(&lista, texto);
                break;
            case 3:
                listarInventarioEstatico(&lista);
                break;
            case 0:
                break;
            default:
                perror("ERROR - Opcao invalida, tente novamente ou pressione '0' para voltar ao menu principal\n");
                break;
        }
    } while (opcao != 0);
    
}

void menuInventarioEncadeado() {
    InventarioEncadeado lista;
    inicializarInvetarioEncadeado(&lista);
    int opcao;
    char texto[MAX_STR_LEN];

    do
    {
        printf("\n--- MENU INVENTARIO ENCADEADO ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                printf("Digite o item a inserir: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0; // Remove o '\n' do final
                inserirItemEncadeado(&lista, texto);
                break;
            case 2:
                printf("Digite o item a remover: ");
                fgets(texto, MAX_STR_LEN, stdin);
                texto[strcspn(texto, "\n")] = 0;
                removerItemEncadeado(&lista, texto);
                break;
            case 3:
                listarInventarioEncadeado(lista);
                break;
            case 0:
            liberarInventarioEncadeado(&lista);
                break;
            default:
                perror("ERROR - Opcao invalida, tente novamente ou pressione '0' para voltar ao menu principal\n");
                break;
        }
    } while (opcao != 0);
    
}

//IMPLEMENTAÇÃO - ESTATICA

void inicializarInvetario (InventarioEstatico *lista) {
    lista->quantidade = 0;
}


void inserirItem (InventarioEstatico *lista, const char* texto) {
    if (lista->quantidade >= TAM_MAX) {
        printf("Erro: lista cheia! Nao e possivel inserir mais.\n");
        return;
    }
    //copia o conteudo do "texto" para a posição correta na matriz nome
    strcpy(lista->nomes[lista->quantidade], texto);
    lista->quantidade++;
    printf("Item \"%s\" Inserido com sucesso.\n", texto);
}

void removerItem (InventarioEstatico *lista, const char* texto) {
    int i, pos = -1;

    //vamos usar strcmp() para comparar strings, retona 0 se forem iguais.
    // a função percorre os itens em uso na lista de 0 ate quantidade.
    for (i = 0; i < lista->quantidade; i++) {
        if (strcmp(lista->nomes[i], texto) ==0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Erro: Item \"%s\" nao encontrado no invetario.\n", texto);
        return;
    }

    // Deslocar os elementos posteriores para a esquerda usando strcpy.
    for (i = pos; i < lista->quantidade - 1; i++) {
        strcpy(lista->nomes[i], lista->nomes[i + 1]);
    }

    lista->quantidade--; //atualização do contador
    printf("Item \"%s\" removido com sucesso.\n", texto);
}

void listarInventarioEstatico (const InventarioEstatico *lista) {
    printf("\nItens do Invetario Estatico:\n");
    if (lista->quantidade == 0) {
        printf("O invetario estatico esta vazio.\n");
        return;
    }
    for (int i = 0; i < lista->quantidade; i++) {
        printf("[%d]: %s\n", i, lista->nomes[i]);
    }
}

//---- IMPLEMENTAÇÃO DAS FUNÇÕES - ENCADEADA ----

void inicializarInvetarioEncadeado (InventarioEncadeado *lista) {
    *lista = NULL;
}

void inserirItemEncadeado (InventarioEncadeado *lista, const char* texto) {
    //Alocação da memória para o NOVO inv
    Inv* novoInv = (Inv*) malloc(sizeof(Inv));
    if (novoInv == NULL) {
        printf("Erro: Falha na alocacao de memoria para o Invetario.\n");
        return;
    }

    // Alocar memória para a STRING DENTRO DO inv
    // strlen conta os caracteres, e o +1 é vital para o caractere nulo ('\0') que finaliza toda string em C.
    novoInv->nome = (char*) malloc(strlen(texto) + 1);
    if (novoInv->nome == NULL) {
        printf("Erro: Falha na alocacao de memoria para o Inventario.\n");
        free(novoInv); // Libera o inv que já foi alocado
        return;
    }

        //Copiar o texto para a memória recém-alocada
    strcpy(novoInv->nome, texto);

    //Ligar o novo inv à lista
    // O ponteiro proximo do novo inv passa a apontar para o que era o antigo primeiro inv da lista.
    novoInv->proximo = *lista;
    
    // A cabeça da lista (*lista) é atualizada para apontar para o novoInv. O novoinv é agora o primeiro item.
    *lista = novoInv;

    printf("Item \"%s\" inserido com sucesso.\n", texto);
}

void removerItemEncadeado (InventarioEncadeado *lista, const char* texto) {
    Inv *atual = *lista;
    Inv *anterior = NULL;

    // Percorremos a lista com dois ponteiros, atual e anterior, para ser removido.
    while (atual != NULL && strcmp(atual->nome, texto) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Erro: Item \"%s\" nao encontrado na Inventario.\n", texto);
        return;
    }

    // Se o item a ser removido for o primeiro (anterior == NULL)
    if (anterior == NULL) {
        *lista = atual->proximo;
    } else { // Se estiver no meio/fim, o ponteiro proximo do anterior "pula" o atual
        anterior->proximo = atual->proximo;
    }

    // Liberação de Memória (Muito Importante!)
    free(atual->nome); // Libera a memória da string DENTRO do inv.
    free(atual);       // AGORA podemos liberar o inv.
    printf("Item \"%s\" removido com sucesso.\n", texto);
}

void listarInventarioEncadeado (const InventarioEncadeado lista) {
    Inv *temp = lista;
    printf("\nItens do inventario Encadeado:\n");
    if (temp == NULL) {
        printf("O iventario esta vazio.\n");
        return;
    }
    int i = 0;
    while (temp != NULL) {
        printf("[%d]: %s\n", i, temp->nome);
        temp = temp->proximo;
        i++;
    }
}

void liberarInventarioEncadeado (InventarioEncadeado *lista) {
    Inv *atual = *lista;
    Inv *proximo;

    // Percorre todos os itens da lista, um por um, e aplica a mesma lógica de liberação dupla (free)
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual->nome); // Libera a string
        free(atual);       // Libera o item
        atual = proximo;
    }
    *lista = NULL; // Garante que a lista aponte para NULL após ser liberada
}