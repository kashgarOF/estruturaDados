#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 10
#define MAX_STR_LEN 30

// Struct que define UM item
typedef struct {
    char nome[MAX_STR_LEN];
    char tipo[20];
    int quantidade;
} Item;

//Estatico

// Struct do inventário que guarda um VETOR de Itens
typedef struct {
    Item itens[TAM_MAX]; 
    size_t quantidade;  
} InventarioEstatico;

/*--------------------------Bubble Sort - TROCAR ESTATICA-----------------------------------*/

// Ferramenta auxiliar para o Bubble Sort
// Recebe o ENDEREÇO de dois Itens e troca o conteúdo entre eles.
void trocarItens(Item *itemA, Item *itemB) {
    Item temp = *itemA; // 1. Guarda uma cópia do item A em uma variável temporária.
    *itemA = *itemB;    // 2. Copia o conteúdo do item B por cima do item A.
    *itemB = temp;      // 3. Copia o conteúdo guardado em 'temp' por cima do item B.
}


// funções (modularização) 

/*--------------------------- estatica --------------------------*/
void inicializarInvetario (InventarioEstatico *lista);
void inserirItemEstatico(InventarioEstatico *inv, const char* nome, const char* tipo, int quantidade);
void removerItem (InventarioEstatico *lista, const char* texto);
void listarInventarioEstatico (const InventarioEstatico *inv);
void menuInventarioEstatico();
void ordenarInventarioEstatico(InventarioEstatico *inv);


/*-----------------------encadeada---------------------------------*/
// O mesmo 'Item' da lista estática será usado aqui
typedef struct No {
    Item dados; // O 'Nó' agora carrega a struct 'Item' inteira
    struct No *proximo;
} No;

typedef No* InventarioEncadeado;

//funções (encadeada)
void inicializarInvetarioEncadeado (InventarioEncadeado *lista);
void inserirItemEncadeado (InventarioEncadeado *lista, const char* nome, const char* tipo, int quantidade);
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
    char nome[MAX_STR_LEN];
    char tipo[20]; //conforme a struct item
    int quantidade;

    do
    {
        printf("\n--- MENU INVENTARIO ESTATICA ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. ordenar itens\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch (opcao) {
            case 1:
                //pedindo o nome
                printf("Digite o nome do item:");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;

                //pedindo o tipo
                printf("Digite o tipo do item(Ex: arma, cura, ferramenta):");
                fgets(tipo, 20, stdin);
                tipo[strcspn(tipo, "\n")] = 0;

                //pedindo a quantidade
                printf("Digite a quantidade:");
                scanf("%d", &quantidade);                
                getchar();

                inserirItemEstatico(&lista, nome, tipo, quantidade);
                break;
            case 2:
                printf("Digite o item a remover: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerItem(&lista, nome);
                break;
            case 3:
                listarInventarioEstatico(&lista);
                break;
            case 4:
            printf("-----Ordenar itens------\n");
                ordenarInventarioEstatico(&lista);
                //chamando a listagem em seguida para saber como ficou
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
    char nome[MAX_STR_LEN];
    char tipo[20];
    int quantidade;

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
                printf("Digite o nome do item:");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("Digite o tipo do item(Ex: arma, cura, ferramenta):");
                fgets(tipo, 20, stdin);
                tipo[strcspn(tipo, "\n")] = 0;

                printf("Digite a quantidade:");
                scanf("%d", &quantidade);                
                getchar();

                inserirItemEncadeado(&lista, nome, tipo, quantidade);
                break;
            case 2:
                printf("Digite o item a remover: ");
                fgets(nome, MAX_STR_LEN, stdin);
                nome[strcspn(nome, "\n")] = 0;
                removerItemEncadeado(&lista, nome);
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

/*--------------------------INSERIR ESTATICA-----------------------------------*/

void inserirItemEstatico(InventarioEstatico *inv, const char *nome, const char *tipo, int quantidade)
{

    if (inv->quantidade >= TAM_MAX)
    {
        printf("Erro: Inventario estatico cheio!\n");
        return;
    }

    Item *novoItem = &inv->itens[inv->quantidade];

    // 3. Copiamos os dados recebidos para dentro da struct nesse espaço livre.
    strcpy(novoItem->nome, nome);
    strcpy(novoItem->tipo, tipo);
    novoItem->quantidade = quantidade;

    // 4. Incrementamos o contador de itens, como antes.
    inv->quantidade++;

    printf("Item '%s' (Tipo: %s, Qtd: %d) inserido com sucesso.\n", nome, tipo, quantidade);
}

/*--------------------------REMOVER ESTATICA-----------------------------------*/

void removerItem (InventarioEstatico *lista, const char* texto) {
    int i, pos = -1;

    //vamos usar strcmp() para comparar strings, retona 0 se forem iguais.
    // a função percorre os itens em uso na lista de 0 ate quantidade.
       for (size_t i = 0; i < lista->quantidade; i++) {
        if (strcmp(lista->itens[i].nome, texto) == 0) {
            pos = (int)i;
            break;
        }
    }

        if (pos == -1) {
        printf("Erro: Item \"%s\" nao encontrado no invetario.\n", texto);
        return;
    }

    // Deslocar os elementos posteriores para a esquerda usando strcpy.
  //Usando '=' para copiar a struct inteira
    for (size_t i = (size_t)pos; i < lista->quantidade - 1; i++) {
        lista->itens[i] = lista->itens[i + 1];
    }

    lista->quantidade--; //atualização do contador
    printf("Item \"%s\" removido com sucesso.\n", texto);
}

/*--------------------------LISTAR ESTATICA-----------------------------------*/

void listarInventarioEstatico(const InventarioEstatico *inv) {
    printf("\n--- Itens no Inventario Estatico ---\n");
    
    if (inv->quantidade == 0) {
        printf("O inventario esta vazio.\n");
        return;
    }

    //size_t para o contador 'i'
    for (size_t i = 0; i < inv->quantidade; i++) {
        printf("------------------------------------\n");
        // %zu é o formato correto para imprimir uma variável do tipo size_t
        printf("Item [%zu]:\n", i); 
        
        //Acessando cada campo da struct 'Item'
        printf("  Nome......: %s\n", inv->itens[i].nome);
        printf("  Tipo......: %s\n", inv->itens[i].tipo);
        printf("  Quantidade: %d\n", inv->itens[i].quantidade);
    }
    printf("------------------------------------\n");
}

/*--------------------------ORDENAR ESTATICA-----------------------------------*/
void ordenarInventarioEstatico(InventarioEstatico *inv) {
    if (inv->quantidade < 2) {
        printf("Nao ha itens suficientes para ordenar.\n");
        return; // Não faz nada se a lista estiver vazia ou com 1 item
    }

    // Loop externo: controla as passagens
    for (size_t i = 0; i < inv->quantidade - 1; i++) {
        // Loop interno: faz as comparações e trocas
        // O "- i" é uma otimização: a cada passagem, o maior item já borbulhou para o final
        // então não precisamos verificar os itens que já estão no lugar certo.
        for (size_t j = 0; j < inv->quantidade - 1 - i; j++) {
            
            // Compara o nome do item atual com o nome do próximo item
            // strcmp > 0 significa que a primeira string vem DEPOIS da segunda na ordem alfabética
            if (strcmp(inv->itens[j].nome, inv->itens[j + 1].nome) > 0) {
                // Se estiverem fora de ordem, troca os itens de lugar
                trocarItens(&inv->itens[j], &inv->itens[j + 1]);
            }
        }
    }
    printf("Inventario ordenado por nome com sucesso!\n");
}


//---------------- IMPLEMENTAÇÃO DAS FUNÇÕES - ENCADEADA ------------------------------

void inicializarInvetarioEncadeado (InventarioEncadeado *lista) {
    *lista = NULL;
}

/*--------------------------INSERIR ENCADEADA-----------------------------------*/

void inserirItemEncadeado(InventarioEncadeado *lista, const char* nome, const char* tipo, int quantidade) {
    
    // Alocar memória para o "Nó"
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocacao de memoria para o No.\n");
        return;
    }

    // Preenchendo os dados do Item DENTRO do Nó.
    strcpy(novoNo->dados.nome, nome);
    strcpy(novoNo->dados.tipo, tipo);
    novoNo->dados.quantidade = quantidade;

    // para ligar o novo nó na lista
    novoNo->proximo = *lista;
    *lista = novoNo;

    printf("Item '%s' inserido com sucesso na lista encadeada.\n", nome);
}

/*--------------------------REMOVER ENCADEADA-----------------------------------*/

void removerItemEncadeado (InventarioEncadeado *lista, const char* nome) {
    No *atual = *lista;
    No *anterior = NULL;

    //O loop agora compara o 'nome' recebido
    // com o campo 'nome' que está DENTRO da struct 'dados'.
    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }
    // Se não encontrou o item, sai da função.
    if (atual == NULL) {
        printf("Erro: Item \"%s\" nao encontrado na Inventario.\n", nome);
        return;
    }

    // para remover o nó da "corrente"
    // não muda nada, pois ela só se importa com os engates (ponteiros).
    if (anterior == NULL) {
        *lista = atual->proximo;// O item a ser removido era o primeiro
    } else { 
        anterior->proximo = atual->proximo;// O item estava no meio ou no fim.
    }

    //Na versão anterior, precisávamos de dois 'free', um para o nome e outro para o nó.
    // Agora, como a struct 'Item' inteira vive DENTRO do Nó, precisamos de apenas UM 'free'.
    free(atual);
    printf("Item \"%s\" removido com sucesso.\n", nome);
}

/*--------------------------LISTAR ENCADEADA-----------------------------------*/


void listarInventarioEncadeado (const InventarioEncadeado lista) {
    No *atual = lista;
    printf("\n---- Itens do inventario Encadeado -----\n");
    if (atual == NULL) {
        printf("O iventario esta vazio.\n");
        return;
    }
    int i = 0;
    while (atual != NULL) {
   printf("------------------------------------\n");
        printf("Item [%d]:\n", i);
        
        printf("  Nome......: %s\n", atual->dados.nome);
        printf("  Tipo......: %s\n", atual->dados.tipo);
        printf("  Quantidade: %d\n", atual->dados.quantidade);

        // Avança para o próximo nó
        atual = atual->proximo;
        i++;
    }
    printf("------------------------------------\n");
}

/*--------------------------LIBERAR ENCADEADA-----------------------------------*/


void liberarInventarioEncadeado (InventarioEncadeado *lista) {
    No *atual = *lista;
    No *proximo;

    // Percorre todos os itens da lista, um por um, e aplica a mesma lógica de liberação dupla (free)
    while (atual != NULL) {
        proximo = atual->proximo;  //guarda o endereço do próximo vagão.
        free(atual);       // Libera o vagão atual. TUDO que está dentro dele (a struct Item inteira) vai junto.
        atual = proximo;  //Pula para o próximo vagão.
    }
    *lista = NULL; // Garante que a lista fique segura e vazia.
}