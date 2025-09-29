#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_FILA 5
#define MAX_PILHA 3

/*---limpar buffer -> limpando o caminho para a proxima leitura---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


/*----------------- PILHA ----------------------*/
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo; // unico controlador: indica o indice do ultimo elemento inserido
} Pilha;

void inicizlizarPilha (Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);


//Estrutura que define uma peça do jogo
typedef struct {
    char tipo;
    int id;
} Peca;

//estrutura que define a fila circular
typedef struct {
    Peca pecas[MAX_FILA]; // o vetor (array) onde as peças são guardadas
    int inicio; // Índice que aponta para o começo da fila
    int fim;    // Índice que aponta para o PRÓXIMO espaço livre no fim da fila
    int total;  // Contador de quantas peças estão na fila atualmente
} Fila;


void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void inserir(Fila *f, Peca p);
Peca gerarPeca();
void mostrarFila(Fila *f);
Peca remover(Fila *f);




//-------------- Função principal ----------------

int main() {
    srand(time(NULL)); // Inicializa para números aleatórios
    Fila filaDePecas;
    int opcao;

    inicializarFila(&filaDePecas);

    // Vamos pré-preencher a fila para que o jogo já comece com algumas peças
    printf("Iniciando o jogo e gerando as primeiras pecas...\n");
    for(int i = 0; i < 3; i++) { // Começa com 3 peças
        inserir(&filaDePecas, gerarPeca());
    }

    do {
        // O estado da fila é sempre exibido no início de cada turno
        printf("\n---------------------------------");
        mostrarFila(&filaDePecas);
        printf("---------------------------------\n");
        
        // Menu de opções para o jogador
        printf("Opcoes:\n");
        printf("1. Jogar peca (remover da fila)\n");
        printf("2. Gerar e adicionar nova peca (se houver espaco)\n");
        printf("0. Sair do Jogo\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limparBufferUp();

        switch(opcao) {
            case 1: { // Chaves para criar um escopo local
                if (!filaVazia(&filaDePecas)) {
                    Peca jogada = remover(&filaDePecas);
                    printf("\n>> Voce jogou a peca [%c%d]!\n", jogada.tipo, jogada.id);
                } else {
                    printf("\n>> Fila de pecas vazia! Nao ha pecas para jogar.\n");
                }
                break;
            }
            case 2: {
                if (!filaCheia(&filaDePecas)) {
                    Peca nova = gerarPeca();
                    inserir(&filaDePecas, nova);
                    printf("\n>> Nova peca [%c%d] entrou na fila!\n", nova.tipo, nova.id);
                } else {
                    printf("\n>> A fila de pecas esta cheia!\n");
                }
                break;
            }
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

    } while(opcao != 0);

    return 0;
}


//---------------------------- FUNÇÕES --------------------------------------

// Inicializa os ponteiros e o contador da fila.
// Garante que a fila comece vazia e em um estado conhecido. 
void inicializarFila( Fila *f){
   f->inicio = 0;
   f->fim = 0;
   f->total = 0;
}


// Retorna 1 (verdadeiro) se a fila estiver cheia, 0 (falso) caso contrário.
int filaCheia(Fila *f){
    return f->total == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void inserir(Fila *f, Peca p) {
    // 1. Verifica se a fila não está cheia
    if (filaCheia(f)) {
        printf("Fila cheia. Nao e possivel inserir.\n");
        return;
    }
    // 2. Adiciona a nova peça na posição indicada pelo 'fim'
    f->pecas[f->fim] = p;
    // 3. ATUALIZAÇÃO CIRCULAR: Move o 'fim' para a próxima posição
    f->fim = (f->fim + 1) % MAX_FILA;
    // 4. Incrementa o número total de itens na fila
    f->total++;
}


Peca gerarPeca() {
    Peca p;
    char tipos[] = "IOTLJSZ";
    p.id = rand() % 100; //Gera um ID aleatório entre 0 e 99
    p.tipo = tipos[rand() % strlen(tipos)]; // Sorteia uma letra da string 'tipos'
    return p;
}

void mostrarFila(Fila *f) {
    printf("\nFila de pecas: ");
    if (filaVazia(f)) {
        printf("[Vazia]\n");
        return;
    }
    
    // 'idx' começa no início da fila e avança de forma circular
    int idx = f->inicio;

    // O loop repete 'total' vezes para mostrar todos os elementos
    for (int i = 0; i < f->total; i++) {
        // Imprime a peça na posição 'idx'
        printf("[%c%d] ", f->pecas[idx].tipo, f->pecas[idx].id);
        
        // Move 'idx' para a próxima posição de forma circular
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");

}

// Remove o elemento do início da fila (dequeue) e o retorna.
Peca remover(Fila *f) {
    Peca pecaRemovida;
    // 1. Verificação de segurança
    if(filaVazia(f)) {
        printf("Fila vazia. Nao e possivel remover\n");
        // Cria uma "peça de erro" para retornar
        pecaRemovida.id =  -1;
        pecaRemovida.tipo = ' ';
        return pecaRemovida;
    }
    // 2. Guarda uma cópia da peça que está no início da fila.
    pecaRemovida = f->pecas[f->inicio];
    // 3. Move o 'início' para a próxima posição.
    f->inicio = (f->inicio + 1) % MAX_FILA;
    // 4. Decrementa o número total de itens.
    f->total--;
    // 5. Retorna a peça que foi removida.
    return pecaRemovida;
}


/*----------------------------- P I L H A S ----------------------------------*/

// Prepara a pilha para uso, marcando-a como vazia.
void inicizlizarPilha (Pilha *p) {
    p->topo = -1;
}

// Retorna 1 (verdadeiro) se a pilha estiver vazia.
int pilhaVazia(Pilha *p) {
    return p->topo == -1; 
}

// Retorna 1 (verdadeiro) se a pilha estiver cheia.
int pilhaCheia(Pilha *p) {
    // Como os índices vão de 0 a MAX_PILHA-1, a pilha está cheia quando o topo atinge o último índice.
    return p->topo == MAX_PILHA -1;
}