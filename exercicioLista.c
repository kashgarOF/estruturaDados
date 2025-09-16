#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 10
#define MAX_STR_LEN 30

/*---Limpa "\n" do fgets---*/
static void strip_newline(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

/*---limpar buffer -> limpando o caminho para a proxima leitura---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Struct que define UM item
typedef struct {
    char nome[MAX_STR_LEN];
    char tipo[20];
    int quantidade;
} Item;



//----------------------------------Estatico----------------------------------------------

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
void inicializarInventario (InventarioEstatico *lista);
void inserirItemEstatico(InventarioEstatico *inv, const char* nome, const char* tipo, int quantidade);
void removerItemEstatico (InventarioEstatico *lista, const char* texto);
void listarInventarioEstatico (const InventarioEstatico *inv);
void menuInventarioEstatico();
void ordenarInventarioEstatico(InventarioEstatico *inv);
int buscaBinariaPorNome(const InventarioEstatico *inv, const char* nomeBusca);





/*------------------------------------Encadeada-----------------------------------------------------------*/
// O mesmo 'Item' da lista estática será usado aqui
typedef struct No {
    Item dados; // O 'Nó' agora carrega a struct 'Item' inteira
    struct No *proximo;
} No;

typedef No* InventarioEncadeado;

//funções (encadeada)
void inicializarInventarioEncadeado (InventarioEncadeado *lista);
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
        limparBufferUp();

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
    inicializarInventario(&lista);
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
        printf("5. Buscar item por nome (Busca Rapida)\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBufferUp(); // Limpa o buffer

        switch (opcao) {
            case 1:
                //pedindo o nome
                printf("Digite o nome do item:");
                fgets(nome, MAX_STR_LEN, stdin);
                strip_newline(nome);

                //pedindo o tipo
                printf("Digite o tipo do item(Ex: arma, cura, ferramenta):");
                fgets(tipo, 20, stdin);
                strip_newline(tipo);

                //pedindo a quantidade
                printf("Digite a quantidade:");
                scanf("%d", &quantidade);                
                limparBufferUp();

                inserirItemEstatico(&lista, nome, tipo, quantidade);
                break;
            case 2:
                printf("Digite o item a remover: ");
                fgets(nome, MAX_STR_LEN, stdin);
                strip_newline(nome);
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
            case 5:

                printf("\nMODO DE USO: essa função so funciona se a lista estiver ordenada(funcao 4 do menu)\n");
                int opcaoCase5;

                do
                {
                    printf("\n--- Busque seu item ---\n");
                    printf("1.Buscar item\n");
                    printf("0.Voltar ao menu anterior\n");
                    scanf("%d", &opcaoCase5);
                    limparBufferUp();
                    
                    switch (opcaoCase5)
                    {
                    case 1:
                        char nomeBusca[MAX_STR_LEN];
                        printf("Digite o nome do item a buscar: ");
                        fgets(nomeBusca, MAX_STR_LEN, stdin);
                        strip_newline(nomeBusca);

                        // Chama a função e guarda o resultado
                        int indice = buscaBinariaPorNome(&lista, nomeBusca);

                        if (indice == -1)
                        {
                            printf(">> Item '%s' nao encontrado no inventario.\n", nomeBusca);
                        }
                        else
                        {
                            printf(">> Item encontrado na posicao [%d]!\n", indice);
                            printf("   Nome......: %s\n", lista.itens[indice].nome);
                            printf("   Tipo......: %s\n", lista.itens[indice].tipo);
                            printf("   Quantidade: %d\n", lista.itens[indice].quantidade);
                        }
                        break;

                    case 0:
                        break;

                    default:
                        perror("ERROR: Opcao invalida!");
                        break;
                    }

                } while (opcaoCase5 != 0);

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
    inicializarInventarioEncadeado(&lista);
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
        limparBufferUp(); // Limpa o buffer

        switch (opcao) {
            case 1:
                printf("Digite o nome do item:");
                fgets(nome, MAX_STR_LEN, stdin);
                strip_newline(nome);

                printf("Digite o tipo do item(Ex: arma, cura, ferramenta):");
                fgets(tipo, 20, stdin);
                strip_newline(tipo);

                printf("Digite a quantidade:");
                scanf("%d", &quantidade);                
                limparBufferUp();

                inserirItemEncadeado(&lista, nome, tipo, quantidade);
                break;
            case 2:
                printf("Digite o item a remover: ");
                fgets(nome, MAX_STR_LEN, stdin);
                strip_newline(nome);
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

void inicializarInventario (InventarioEstatico *lista) {
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

void removerItemEstatico (InventarioEstatico *lista, const char* texto) {
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

/*--------------------------REMOVER ESTATICA-----------------------------------*/

// Retorna o índice do item se encontrado, ou -1 se não encontrar.
int buscaBinariaPorNome(const InventarioEstatico *inv, const char* nomeBusca) {
    
    // 1. Define os limites da busca: do primeiro ao último item.
    int inicio = 0;
    int fim = inv->quantidade - 1;

    // 2. Enquanto houver uma área de busca válida...
    while (inicio <= fim) {
        // 3. Calcula o índice do meio.
        int meio = inicio + (fim - inicio) / 2;

        // 4. Compara o nome do meio com o nome buscado.
        int comparacao = strcmp(inv->itens[meio].nome, nomeBusca);

        if (comparacao == 0) {
            // ACHOU! Retorna o índice.
            return meio; 
        } 
        else if (comparacao < 0) {
            // O nome do meio vem ANTES do que buscamos. Joga fora a metade de baixo.
            inicio = meio + 1;
        } 
        else { // comparacao > 0
            // O nome do meio vem DEPOIS do que buscamos. Joga fora a metade de cima.
            fim = meio - 1;
        }
    }

    // 5. Se o loop terminar, o item não foi encontrado.
    return -1;
}

//--------------------------- SELECTION SORT - ESTATICA ----------------------------------------

// Ordena o inventário com base na quantidade (prioridade), do menor para o maior.
void selectionSortPorPrioridade(InventarioEstatico *inv) {
    if (inv->quantidade < 2) {
        return; // Não precisa ordenar
    }

    // Loop externo ('i'): Percorre o vetor para definir a posição que será preenchida.
    // A cada volta, ele coloca o menor item da parte "bagunçada" na posição 'i'.
    for (size_t i = 0; i < inv->quantidade - 1; i++) {
        
        // Supomos que o menor item é o primeiro da parte não ordenada.
        size_t indiceMenor = i;        

        // Loop interno ('j'): Sua única missão é ENCONTRAR o verdadeiro menor item
        // na parte restante do vetor (de i+1 até o fim). 
        for (size_t j = i + 1; j < inv->quantidade; j++) {
            // Se encontrarmos um item com quantidade menor que o nosso 'menor' atual...
            if (inv->itens[j].quantidade < inv->itens[indiceMenor].quantidade) {
                // ...atualizamos nosso índice do menor.
                indiceMenor = j;
            }
        }

        // Depois que o loop interno termina, se o menor item encontrado
        // não for o que já estava na posição 'i', nós fazemos UMA ÚNICA TROCA. [cite: 1147, 1186]
      if (indiceMenor != i) { // <-- CORREÇÃO AQUI
            trocarItens(&inv->itens[i], &inv->itens[indiceMenor]);
        }
    }
    printf("Inventario ordenado por prioridade (quantidade) com sucesso!\n");
}




//---------------- IMPLEMENTAÇÃO DAS FUNÇÕES - ENCADEADA ------------------------------

void inicializarInventarioEncadeado (InventarioEncadeado *lista) {
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