
#include <stdio.h> //padrão
#include <stdlib.h> // conversões numericas (strtol/strtof)
#include <string.h> // Para strcspn(), entre outras manipulações

#define MAX_ALUNOS 100
#define TAM_STRING 100
#define TAM_MATRICULA 20
#define TAM_BUF_NUM 32 // buffer temporário para ler números como texto


/*------ DEF DA STRUCT ------*/
struct Aluno
{
    char matricula[TAM_MATRICULA];
    char nome[TAM_STRING];
    int idade;
    char curso[TAM_STRING];
    float media;
    char situacao[TAM_MATRICULA];
};

/*------ remove "\n" que vem do fgets ------*/
static void strip_newline(char *s)
{
    if (!s) //protege contra ponteiro null
        return;
    s[strcspn(s, "\n")] = '\0';
}

/*------ descarta lixo ate fim da linha, otimo apos scanf ------*/
void limparBufferUp(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/*--- FUNÇÃO PRINCIPAL ---*/

int main(void)
{
    struct Aluno dados[MAX_ALUNOS];
    int totalAlunos = 0;
    int opcao;

    do
    {

        printf("=========================================\n");
        printf("             DADOS DOS ALUNOS            \n");
        printf("=========================================\n");
        printf("1 - Cadastrar novo aluno\n");
        printf("2 - Listar todos os alunos cadastrados\n");
        printf("0 - Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) //tentar ler opcao, se não lef 1 intem valido, trata erro
        {
            printf("\nOpcao invalida! Tente novamente.\n");
            limparBufferUp();
            continue; //volta ao menu
        };

        limparBufferUp();

        switch (opcao) {
        case 1: 
        { /* cadastro */
            printf("--- Cadastro de um novo aluno ---");

            if (totalAlunos >= MAX_ALUNOS)
            {
                printf("Quantidade maxima atingida! Nao e possivel cadastrar mais alunos.\n");
                printf("\nPressione enter para continuar....");
                getchar();
                break;
            }

            struct Aluno *a = &dados[totalAlunos]; /*ponteiro, "atalho" para apontar -> endereço estabelecido*/
            char buf[TAM_BUF_NUM]; /*array temporario para ler fgets e depois converter*/

            printf("Digite a matricula do aluno: ");
            if (!fgets(a->matricula, TAM_MATRICULA, stdin))
                a->matricula[0] = '\0';
            strip_newline(a->matricula);

            printf("Digite o nome do aluno: ");
            if (!fgets(a->nome, TAM_STRING, stdin))
                a->nome[0] = '\0';
            strip_newline(a->nome);

            printf("Digite a idade do aluno: ");
            if (!fgets(buf, sizeof(buf), stdin))
                buf[0] = '\0';
            strip_newline(buf);
            a->idade = (int)strtol(buf, NULL, 10); /* --strtol-- converte string -> long(123). --strtof-- converte string -> float(0.12).*/

            printf("Digite o curso do aluno: ");
            if (!fgets(a->curso, TAM_STRING, stdin))
                a->curso[0] = '\0';
            strip_newline(a->curso);

            printf("Digite a media do aluno (ex: 7.5): ");
            if (!fgets(buf, sizeof(buf), stdin))
                buf[0] = '\0';
            strip_newline(buf);
            a->media = strtof(buf, NULL); // converte para float

            printf("Digite a situacao do aluno (Aprovado/Reprovado/Rec.): ");
            if (!fgets(a->situacao, TAM_MATRICULA, stdin))
                a->situacao[0] = '\0';
            strip_newline(a->situacao);

            totalAlunos++; //confirma o cadastro

            printf("\nAluno cadastrado com sucesso!\n");
            printf("\nPressione enter para continuar...");
            getchar(); //pausa
        }
        break;

        case 2: 
        { printf("\n--- Lista de alunos cadastrados ---\n\n");

            if (totalAlunos ==0) {
                printf("Nenhum aluno cadastrado ainda.\n");
            } else 
            {
                for (int i = 0; i < totalAlunos; i++) {
                    struct Aluno *a = &dados[i]; // outro atalho para legit...
                    printf("===========================================\n");
                        printf("Aluno %d\n", i + 1);
                        printf("Matricula: %s\n", a->matricula);
                        printf("Nome     : %s\n", a->nome);
                        printf("Idade    : %d\n", a->idade);
                        printf("Curso    : %s\n", a->curso);
                        printf("Media    : %.2f\n", a->media);
                        printf("Situacao : %s\n", a->situacao);
                }
                printf("================================================\n");
            }

            printf("\nPressione enter para continuar...");
            getchar();
        } break;

        case 0: //encerra codigo

            printf("\nSaindo do sistema...\n");
            break;

            default: //tratamento de erro
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("\nPressione Enter para continuar...");
                getchar();
                break; 
        }

        
    } while (opcao != 0);
    
    return 0;
}