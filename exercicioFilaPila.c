#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_FILA 5
#define MAX_PILHA 3


// --- ESTRUTURAS DE DADOS ---

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

//estrutura de pilha
typedef struct {
    Peca pecas[MAX_PILHA];
    int topo; // unico controlador: indica o indice do ultimo elemento inserido
} Pilha;




// --- MODULARIZAÇÃO DAS FUNÇÕES ---

// Funções da Fila
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void inserir(Fila *f, Peca p);
Peca gerarPeca();
void mostrarFila(Fila *f);
Peca remover(Fila *f);


// Funções da Pilha
void inicializarPilha(Pilha *p);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void push (Pilha *p, Peca peca);
Peca pop(Pilha *p);
void mostrarPilha(Pilha *p);


// Funções hibridas
void trocarPecas(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);



// Funções Utilitárias

/*---limpar buffer -> limpando o caminho para a proxima leitura---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



//-------------- Função principal ----------------

int main() {
    srand(time(NULL)); // Inicializa para números aleatórios
    Fila filaDePecas;
    Pilha pilhaDeReserva;
    int opcao;

    inicializarFila(&filaDePecas);
    inicializarPilha(&pilhaDeReserva);

    // Pré-preenche a fila para o jogo começar
    printf("Iniciando o jogo e gerando as primeiras pecas...\n");
    for(int i = 0; i < MAX_FILA; i++) {
        inserir(&filaDePecas, gerarPeca());
    }

    do {
        // Exibe o estado atual do jogo
        printf("\n=================================\n");
        mostrarFila(&filaDePecas);
        mostrarPilha(&pilhaDeReserva);
        printf("=================================\n");
        
        // Menu
        printf("Opcoes:\n");
        printf("1. Jogar peca (da Fila)\n");
        printf("2. Reservar peca (Fila -> Pilha)\n");
        printf("3. Usar peca reservada (da Pilha)\n");
        printf("4. Trocar peca (Fila <-> Pilha)\n"); // Deixando mais claro
        printf("5. Troca Multipla (3 da Fila <-> 3 da Pilha)\n"); // Deixando mais claro
        printf("0. Sair do Jogo\n");
        printf("Escolha uma opcao: ");
        
        scanf("%d", &opcao);
        limparBufferUp();

        switch(opcao) {
            case 1: { // Jogar peça da Fila
                if (!filaVazia(&filaDePecas)) {
                    Peca jogada = remover(&filaDePecas);
                    printf("\n>> Voce jogou a peca [%c%d]!\n", jogada.tipo, jogada.id);
                    // Regra do jogo: sempre que uma peça sai, uma nova entra
                    inserir(&filaDePecas, gerarPeca());
                } else {
                    printf("\n>> Fila de pecas vazia!\n");
                }
                break;
            }
            case 2: { // Reservar peça (Fila -> Pilha)
                // Verifica se a PILHA tem espaço E se a FILA tem peças
                if (!pilhaCheia(&pilhaDeReserva) && !filaVazia(&filaDePecas)) {
                    Peca pecaParaReservar = remover(&filaDePecas); // 1. Tira da fila
                    push(&pilhaDeReserva, pecaParaReservar);      // 2. Coloca na pilha
                    printf("\n>> Peca [%c%d] reservada!\n", pecaParaReservar.tipo, pecaParaReservar.id);
                    // Regra do jogo: nova peça entra na fila
                    inserir(&filaDePecas, gerarPeca());
                } else {
                    printf("\n>> Nao foi possivel reservar! (Pilha cheia ou Fila vazia)\n");
                }
                break;
            }
            case 3: { // Usar peça da Pilha
                if (!pilhaVazia(&pilhaDeReserva)) {
                    Peca pecaUsada = pop(&pilhaDeReserva); // Tira da pilha
                    printf("\n>> Voce usou a peca reservada [%c%d]!\n", pecaUsada.tipo, pecaUsada.id);
                } else {
                    printf("\n>> Pilha de reserva esta vazia!\n");
                }
                break;
            }
            case 4: {
                trocarPecas(&filaDePecas, &pilhaDeReserva);
                break;
            }
            case 5: {
                trocaMultipla(&filaDePecas, &pilhaDeReserva);
                break;
            }
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                perror("\nOpcao invalida! Tente novamente.\n");
                break;
        }

    } while(opcao != 0);

    return 0;
}


//---------------------------- IMPLEMENTAÇÕES DAS FUNÇÕES --------------------------------------

// Inicializa os ponteiros e o contador da fila.
// Garante que a fila comece vazia e em um estado conhecido. 
void inicializarFila( Fila *f){
   f->inicio = 0;
   f->fim = 0;
   f->total = 0;
}

// ------------------------ Funções da FILA -------------------------------

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
void inicializarPilha (Pilha *p) {
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


// Insere um elemento no topo da pilha
void push (Pilha *p, Peca peca) {
    // 1. Usa nossa função auxiliar para conferir se há espaço.
    if (pilhaCheia(p)) {
        printf("Pilha de reserva cheia. Nao e possivel inserir.\n");
        return;
    }

    // 2. PRIMEIRO, avança o topo para a próxima posição livre.
    p->topo++;

    // 3. AGORA, insere a peça na nova posição do topo.
    p->pecas[p->topo] = peca;

}


// Remove o elemento do topo da pilha (pop) e retorna.
Peca pop(Pilha *p) {
    Peca pecaRemovida;

    // 1. Usa nossa função auxiliar para verificar se há algo para remover.
    if (pilhaVazia(p)) {
        printf("Pilha de reserva vazia. Nao ha pecas para remover. \n");
        pecaRemovida.id = -1;
        pecaRemovida.tipo = ' ';
        return pecaRemovida;
    }

    // 2. PRIMEIRO, copia a peça que está no topo.
    pecaRemovida = p->pecas[p->topo];

    // 3. AGORA, retrocede o topo para a posição anterior.
    p->topo--;

    // 4. Retorna a peça que foi removida.
    return pecaRemovida;
}


void mostrarPilha(Pilha *p) {

    printf("\nTopo -> base: ");
    if(pilhaVazia(p)){
        printf("[Vazia]\n");
        return;
    }
    
    //começa no topo e percorre as peças decrementando
    for (int i = p->topo; i>= 0; i--) {
        printf("[%c%d] ", p->pecas[i].tipo, p->pecas[i].id);
    }

    printf("\n");
}



void trocarPecas(Fila *f, Pilha *p) {
    // 1. Verificação de segurança: A troca só é possível se AMBAS tiverem peças.
    if(filaVazia(f) || pilhaVazia(p)) {
        printf("\n>> Troca nao permitida! Fila ou pilha vazia.\n");
        return;
    }
    // 2. Lógica da troca usando uma variável temporária
    Peca temp = f->pecas[f->inicio];    // Guarda a peça da Fila
    f->pecas[f->inicio] = p->pecas[p->topo];     // Coloca a peça da Pilha na Fila
    p->pecas[p->topo] = temp;   // Coloca a peça guardada na Pilha

    printf("\n>> Troca realizada com sucesso!\n");
}


void trocaMultipla(Fila *f, Pilha *p) {
    
    // 1. Verificação de segurança
    // A pilha precisa ter 3 itens (topo no índice 2) e a fila também (total >= 3)
    if (f->total < 3 || p->topo < 2) {
        printf("\n>> Troca multipla nao permitida! Fila ou Pilha nao tem 3 pecas.\n");
        return;
    }

    printf("\n>> Realizando troca multipla!\n");

    // 2. Loop simples que roda EXATAMENTE 3 vezes (para i = 0, 1, 2)
    for (int i = 0; i < 3; i++) {
        // 3. Cálculo dos índices
        // Para a Fila, calculamos o índice a partir do 'inicio' de forma circular
        int indiceFila = (f->inicio + i) % MAX_FILA;
        
        // Para a Pilha, calculamos o índice a partir do 'topo', descendo
        int indicePilha = p->topo - i;

        // 4. A mesma lógica da função trocarPecas
        Peca temp = f->pecas[indiceFila];
        f->pecas[indiceFila] = p->pecas[indicePilha];
        p->pecas[indicePilha] = temp;
    }
}