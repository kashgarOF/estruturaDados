#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INFO 30
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_BUFF_NUM 32



/*--- Estrutura do territorio ---*/
struct territorio {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
};

/*---Limpa "\n" do fgets---*/
static void strip_newline(char *s) {
    if (!s) return;
    s[strcspn(s, "\n")] = '\0';
};

/*--- função criada usando void, no caso sem retorno, apenas executa sua ação: limpar buffer---*/
void limparBufferUp(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*--- Função principal ---*/
int main(void) 
{
    struct territorio dados[MAX_INFO];
    int totalInfo = 0;
    int opcao;

    do 
    {
        printf("=========================================\n");
        printf("           DADOS DOS TERRITORIOS         \n");
        printf("=========================================\n");
        printf("1 - Cadastrar novo Territorio\n");
        printf("2 - Listar todos os Territorios cadastrados\n");
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

                if (totalInfo >= MAX_INFO) {
                    printf("Quantidade maxima atingida! Não é possivel cadastrar mais territorios.");
                    printf("\nPressione enter para continuar....");
                    getchar();
                    break;
                }

                struct territorio *a = &dados[totalInfo]; // *ponteiro/atalho para -> total info
                char buf [MAX_BUFF_NUM]; //armazena array temp de string e converte para int

                printf("Digite o nome do territorio; ");
                if (!fgets(a->nome, MAX_NOME, stdin))
                    a->nome[0] = '\0';
                    strip_newline(a->nome);

                printf("\nDigite a cor do seu territorio: ");
                if (!fgets(a->cor, MAX_COR, stdin))
                    a->cor[0] = '\0';
                    strip_newline(a->cor);

                printf("Digite a quantidade de tropas do seu territorio: ");
                if (!fgets(buf, sizeof(buf), stdin))
                    buf[0] = '\0';
                    strip_newline(buf);
                    a->tropas = (int)strtol(buf, NULL, 10); //conversor de string para int

                totalInfo++;
                
                printf("\nTerritorio cadastrado com sucesso!\n");
                printf("\nPressione enter para continuar...");
                getchar();
            } break;

            case 2: {
                printf("\n----Lista de territorios cadastrados----\n\n");

                if (totalInfo == 0) {
                    printf("Nenhum territorio cadastrado ainda.\n");
                } else {
                    for (int i = 0; i < totalInfo; i++) {
                        struct territorio *a = &dados[i]; //atalho para legite
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

            case 0: {

                printf("\nSaindo do sistema...\n");
                break;

                default: //tratamento de erros
                    printf("\nOpcao invalida! Tente novamente.\n");
                    printf("\nPressione Enter para continuar...");
                    getchar();
                    break; 
            }
        } 

    } while (opcao != 0);

    return 0;
}