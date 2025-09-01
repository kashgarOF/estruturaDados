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
typedef struct {
    char *nome;
    struct Inv *proximo;
}Inv;

typedef Inv* InventarioEncadeado;

//funções (encadeada)
void inicializarInvetarioEncadeado (InventarioEncadeado *lista);
void inserirItemEncadeado (InventarioEncadeado *lista, const char* texto);
void removerItemEncadeado (InventarioEncadeado *lista, const char* texto);
void listarInventarioEncadeado (const InventarioEncadeado *lista);
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
    InventarioEstatico lista;
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
                listarInventarioEncadeado(&lista);
                break;
            case 0:
                break;
            default:
                perror("ERROR - Opcao invalida, tente novamente ou pressione '0' para voltar ao menu principal\n");
                break;
        }
    } while (opcao != 0);
    
}

//IMPLEMENTAÇÃO - ESTATICA