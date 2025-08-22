
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para strcspn()

#define MAX_ALUNOS 100
#define TAM_STRING 100

/*------ DEF STRUCT ------*/

struct Aluno {
    int matricula[8];
    char nome[TAM_STRING];
    int idade[3];
    char curso[TAM_STRING];
    float media[4];
    char situacao[20];
};

/*FUNÇÃO PARA LIMPAR BUFFER NA ENTRADA*/

void limparBufferUp() {
    int c;
    while ((c=getchar()) != '\n' && c != EOF);
}

/*--- FUNÇÃO PRINCIPAL ---*/

int main() {
    struct Aluno dados[MAX_ALUNOS];
    int totalAlunos = 0;
    int opcao;

    do    {
      

        printf("=========================================\n");
        printf("    DADOS DOS ALUNOS       \n");
        printf("=========================================\n");
        printf("1 - Cadastrar novo aluno\n");
        printf("2 - Listar todos os alunos cadastrados\n");
        printf("0 - Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferUp();

        switch (opcao) {
        case 1: /* cadastro */
        printf("--- Cadastro de um novo aluno ---");

            if (totalAlunos < MAX_ALUNOS) {
                printf("Digite a matricula do aluno: ");
                fgets(dados[totalAlunos].matricula, 8, stdin);

                printf("Digite o nome do aluno: ");
                fgets(dados[totalAlunos].nome, TAM_STRING, stdin);

                printf("Digite a idade do aluno: ");
                fgets(dados[totalAlunos].idade, 3, stdin);

                printf("Digite o curso do aluno: ");
                fgets(dados[totalAlunos].curso, TAM_STRING, stdin);

                printf("Digite a media do aluno: ");
                fgets(dados[totalAlunos].media, 4, stdin);
            
                printf("Digite a situacao do aluno: ");
                fgets(dados[totalAlunos].situacao, 20, stdin);

                dados[totalAlunos].matricula[strcspn(dados[totalAlunos].matricula, "\n")] = '\0';
                dados[totalAlunos].nome[strcspn(dados[totalAlunos].nome, "\n")] = '\0';
                dados[totalAlunos].idade[strcspn(dados[totalAlunos].idade, "\n")] = '\0';
                dados[totalAlunos].curso[strcspn(dados[totalAlunos].curso, "\n")] = '\0';
                dados[totalAlunos].media[strcspn(dados[totalAlunos].media, "\n")] = '\0';
                dados[totalAlunos].situacao[strcspn(dados[totalAlunos].situacao, "\n")] = '\0';

                limparBufferUp();

                totalAlunos++;

                printf("\nAluno cadastrado com sucesso!\n");

            } else {
                printf("Quantidade maxima de alunos cadastrado! Nao e possivel cadastrar mais alunos.");

            }
            
            printf("\n Pressione Enter para continuar...");
            getchar(); /*Pausa para o user ler a mensagem antes de voltar*/
            break;

        case 2: /*Listagem de alunos*/
            printf("--- Lista de alunos cadastrados ---\n\n");

            if (totalAlunos == 0) {
                printf("Nenhum aluno cadastrado ainda. \n");
            } else {
                for (int i = 0; i < totalAlunos; i++) {
                    printf("===========================================\n");
                    printf("Aluno %d\n", i + 1);
                    printf("Matricula: %d\n", dados[i].matricula);
                    printf("Matricula: %s\n", dados[i].nome);
                    printf("Matricula: %d\n", dados[i].idade);
                    printf("Matricula: %s\n", dados[i].curso);
                    printf("Matricula: %d\n", dados[i].media);
                    printf("Matricula: %s\n", dados[i].situacao);
                }
                printf("===============================================\n");
            }

            printf("\nPressione enter para continuar....");
            getchar();
            break;

        case 0: /*SAIR*/
            printf("\nSaindo do sistema....\n");
            break;

        default: /*Opção invalida - caso seja digitado algo fora das opções dadas*/
            printf("\nOpcao invalida! Tente novamente.\n");
            printf("\nPressione enter para continuar...");
            getchar();
            break;
        }

    } while (opcao != 0);

    return 0; /*Fecho!*/
    
}