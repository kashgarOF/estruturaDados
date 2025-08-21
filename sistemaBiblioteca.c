
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
        printf("0 - Sair\n")
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
                fgets(dados[totalAlunos].matricula, 20, stdin);

                dados[totalAlunos].matricula[strcspn(dados[totalAlunos].matricula, "\n")] = '\0';
            }
            
            break;
        
        default:
            break;
        }

    } while ();
    
}