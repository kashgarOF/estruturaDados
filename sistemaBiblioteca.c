
#include <stdio.h> // I/O: printf, scanf, fgets, getchar
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
    int situacao; //1 = aprovado, 0 = reprovado, 2 recuperacao, -1 indefinido
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

//minusculiza
static void str_tolower(char *s){
    for(; s && *s; ++s) {
        if(*s >= 'A' && *s <= 'Z') *s = (char)(*s - 'A' + 'a');
    }
}

//mapeia texto -> codigo da situação
static int parse_situacao(char *s){
    if (!s) return -1;
    strip_newline(s);
    str_tolower(s);

    //aceita pequenas variações
    if(strcmp(s, "aprovado") == 0 || strcmp(s, "aprovada") == 0) return 1;
    if(strcmp(s, "reprovado") == 0 || strcmp(s, "reprovada") == 0) return 0;
    if(strcmp(s, "recuperacao") == 0 || strcmp(s, "recuperação") == 0 || strcmp(s, "rec") == 0) return 2;

    return -1;
}

//converte codigo -> string amigavel
static const char* print_situacao(int cod){
    switch (cod) {
        case 1: return "Aprovado";
        case 0: return "Reprovado";
        case 2: return "Recuperacao";
        default : return "Indefinido";
    }
}

// Lista de alunos por filtro de situação, if filtro < 0 lista todos
static void listarAlunos(const struct Aluno *dados, int total, int filtroSituacao) {
    if (total == 0) {
        printf("Nenhum aluno cadastrado ainda\n");
        return;
    }

    int mostrados = 0;
    for (int i = 0; i < total; i++) {
        const struct Aluno *a = &dados[i];
        if (filtroSituacao >= 0 && a->situacao != filtroSituacao) continue;

        printf("===========================================\n");
        printf("Aluno %d\n", i + 1);
        printf("Matricula: %s\n", a->matricula);
        printf("Nome     : %s\n", a->nome);
        printf("Idade    : %d\n", a->idade);
        printf("Curso    : %s\n", a->curso);
        printf("Media    : %.2f\n", a->media);
        printf("Situacao : %s\n", print_situacao(a->situacao));
        mostrados++;
    }

    if(mostrados == 0) {
        printf("Nenhum aluno com a situacao solicitada.\n");
    }
}

/*--- FUNÇÃO PRINCIPAL ---*/

int main(void)
{
    struct Aluno *dados = (struct Aluno *) calloc(MAX_ALUNOS, sizeof(struct Aluno));
    if(!dados) {
        fprintf(stderr, "erro: sem memoria.\n");
        return 1;
    }

    int totalAlunos = 0;
    int opcao;

    do
    {

        printf("=========================================\n");
        printf("             DADOS DOS ALUNOS            \n");
        printf("=========================================\n");
        printf("1 - Cadastrar novo aluno\n");
        printf("2 - Listar todos os alunos cadastrados\n");
        printf("3 - Listar todos os alunos Aprovados\n");
        printf("4 - Listar todos os alunos Reprovados\n");
        printf("0 - Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) //tentar ler opcao, se não ler 1 intem valido, trata erro
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

            printf("Digite a situacao do aluno (Aprovado/Reprovado/Recuperacao.): ");
            if (!fgets(buf, sizeof(buf), stdin)) buf[0] = '\0';
            a->situacao = parse_situacao(buf);
            
            if (a->situacao == -1) {
                    printf("\nSituacao invalida. Use Aprovado, Reprovado ou Recuperacao.\n");
                    printf("Cadastro cancelado.\n");
                    printf("\nPressione enter para continuar...");
                    getchar();
                    break; // sai sem incrementar totalAlunos
            }
                

            totalAlunos++; //confirma o cadastro

            printf("\nAluno cadastrado com sucesso!\n");
            printf("\nPressione enter para continuar...");
            getchar(); //pausa
        } break;

        case 2:  //lista todos
        { printf("\n--- Lista de alunos cadastrados ---\n\n");

            listarAlunos(dados, totalAlunos, -1);
            printf("\nPressione enter para continuar....");
            getchar();
        } break;

       case 3: {//lista aprovados
            printf("\n--- Alunos Aprovados ---\n\n");
                listarAlunos(dados, totalAlunos, 1);
                // contador opcional
                int ap = 0;
                for (int i = 0; i < totalAlunos; i++) if (dados[i].situacao == 1) ap++;
                printf("\nTotal de aprovados: %d\n", ap);
                printf("\nPressione enter para continuar...");
                getchar();
       } break;

       case 4: { //lista reprovados
            printf("\n--- Alunos Reprovados ---\n\n");
                listarAlunos(dados, totalAlunos, 0);
                int rp = 0;
                for (int i = 0; i < totalAlunos; i++) if (dados[i].situacao == 0) rp++;
                printf("\nTotal de reprovados: %d\n", rp);
                printf("\nPressione enter para continuar...");
                getchar();
       }break;
       
        case 0: //encerra codigo
        {
            printf("\nSaindo do sistema...\n");
            break;

            default: //tratamento de erro
                printf("\nOpcao invalida! Tente novamente.\n");
                printf("\nPressione Enter para continuar...");
                getchar();
                break; 
        }
    }
    } while (opcao != 0);

    free(dados); //libera memoria alocada
    
    return 0;
}