#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#define MAX_TERRITORIO 30
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_BUFF_NUM 32
//int g_num_territorios = 0; //tamanho real do mapa



/*--- Estrutura do territorio ---*/
struct Territorio {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
};

/*---Limpa "\n" do fgets---*/
static void strip_newline(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
}

/*--- função criada usando void, no caso sem retorno, apenas executa sua ação: limpar buffer---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct Territorio *mapa = NULL;
int capacidade = 0; //é o tamanho alocado
int usados = 0; // quantos registros validos





/*--- Função principal ---*/
int main(void) 
{
    // semente pro rand()
    srand((unsigned)time(NULL));

    printf("Quantos territorios deseja cadastrar? ");
    char buf[64];
    if (!fgets(buf, sizeof(buf), stdin)) return 1; //return 1 sinaliza falha
    
    long n = strtol(buf, NULL, 10);
    if (n <= 0 || n > 100000) { //noção basica
        fprintf(stderr, "Quantidade invalida.\n"); //saida de erro padrão
        return 1;
    }

    int opcao;
    capacidade = (int)n;

    mapa = calloc(capacidade, sizeof *mapa);
        if (!mapa) { // se calloc falhar ele não devolve um endereço valido, e avisa o erro e o motivo
            perror("calloc: Cannot allocate memory");
            return 1;
        }

    do 
    { //menu
        printf("\n=========================================\n");
        printf("     capacidade: %d | Usados: %d\n", capacidade, usados);
        printf("=========================================\n");
        printf("           DADOS DOS TERRITORIOS         \n");
        printf("=========================================\n");
        printf("1 - Cadastrar novo Territorio\n");
        printf("2 - Mapa mundial (atual)\n");
        printf("3 - Campo de batalha\n");
        printf("0 - Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {//tenta ler, se for diferente de 1 interio ele quebra
           printf("\nOpcao invalida! Tente novamente...\n");
           limparBufferUp();
           continue; //volta ao menu 
        };

        limparBufferUp();

        switch (opcao) 
        {
            case 1: { /*---Cadastro---*/
                printf("--- Cadastro de um novo territorio ---");

                /*if (g_num_territorios = 100) {
                    printf("Quantidade maxima atingida! Não é possivel cadastrar mais territorios.");
                    printf("\nPressione enter para continuar....");
                    getchar();
                    break;
                }*/

                if (usados == capacidade) {
                    int nova = capacidade ? capacidade * 2 : 1;
                    struct Territorio *tmp = realloc(mapa, nova * sizeof *tmp);
                    if(!tmp) { perror("realloc");
                        printf("Falha ao crescer o vetor. Registro nao cadastrado\n");
                        printf("Pressione enter para continuar...");
                        break;} //volta ao menu sem mexer em mapa
                    mapa = tmp;
                    capacidade = nova;
                }

                struct Territorio *a = &mapa[usados];
                char buf [MAX_BUFF_NUM]; //armazena array temp de string e converte para int

                printf("\nDigite o nome do territorio: ");
                if (!fgets(a->nome, MAX_NOME, stdin)) {a->nome[0] = '\0'; }                    
                    strip_newline(a->nome);

                printf("\nDigite a cor do seu territorio: ");
                if (!fgets(a->cor, MAX_COR, stdin)) {a->cor[0] = '\0';}                    
                    strip_newline(a->cor);

                printf("Digite a quantidade de tropas do seu territorio: ");
                if (!fgets(buf, sizeof(buf), stdin)) {buf[0] = '\0';}                     
                    strip_newline(buf);

                    a->tropas = (int)strtol(buf, NULL, 10); //conversor de string para int
                if (a->tropas < 0) a->tropas = 0;

                usados++;
                
                printf("\nTerritorio cadastrado com sucesso!\n");
                printf("\nPressione enter para continuar...");
                getchar();
            } break;

            case 2: {
                printf("\n----Lista de territorios cadastrados----\n\n");

                if (usados == 0) {
                    printf("Nenhum territorio cadastrado ainda.\n");
                } else {
                    for (int i = 0; i < usados; i++) {
                        struct Territorio *a = &mapa[i]; //atalho para legite
                        printf("===========================================\n");
                        printf("Territorio %d\n", i + 1);
                        printf("Nome     : %s\n", a->nome);
                        printf("Cor      : %s\n", a->cor);
                        printf("Tropas   : %d\n", a->tropas);
                    }
                    printf("================================================\n");  
                }

                printf("\nPressione enter para continuar....");
                getchar();
            } break;

            case 3: {//batalha

            } break;
           
            case 0: //sair

                printf("\nSaindo do sistema...\n"); 
                break;

            default: //tratamento de erros
                    printf("\nOpcao invalida! Tente novamente.\n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                break; 
        } 

    } while (opcao != 0);

    free(mapa); //liberar memoria allocada

    return 0;
}