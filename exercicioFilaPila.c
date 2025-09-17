#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_FILA 5

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




//-------------- Função principal ----------------

int main() {

    srand(time(NULL));// Inicializa a semente para números aleatórios

    Fila filaDePecas; // 1. Cria a variável do tipo Fila

    // 2. Chama a função para "inaugurar" a fila, passando o endereço dela com o '&'
    inicializarFila(&filaDePecas);

    printf("Inserindo 3 pecas na fila...\n");
    inserir(&filaDePecas, gerarPeca());
    inserir(&filaDePecas, gerarPeca());
    inserir(&filaDePecas, gerarPeca());

    printf("Fila apos insercoes. Total de pecas: %d\n", filaDePecas.total);

     mostrarFila(&filaDePecas);

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