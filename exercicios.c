#include <stdio.h>      // Para entrada e saída (printf, scanf, fgets, getchar)
#include <stdlib.h>     // Para alocação de memória (calloc, realloc, free) e conversão (strtol), aleatoriedade (rand, srand)
#include <string.h>     // Para manipulação de strings (strncpy, strcspn)
#include <time.h>       // Para usar o tempo atual como "semente" do gerador aleatório (time)

//#define MAX_TERRITORIO 30
#define MAX_NOME 30
#define MAX_COR 10
#define MAX_BUFF_NUM 32
#define MAX_MISSAO 120

/*--- Estrutura do territorio ---*/
struct Territorio {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
};

struct Jogador {
    char nome[32];
    char cor[MAX_COR];  // mesma largura usada nos territórios
    char *missao;       // armazenada via malloc (dinâmico)
};

static const char *MISSOES[] = {
    "Controlar pelo menos 3 territorios da sua cor",
    "Controlar pelo menos 2 territorios da sua cor",
    "Somar 15 ou mais tropas na sua cor",
    "Eliminar todas as tropas da cor Azul",
    "Eliminar todas as tropas da cor Vermelha"
};

static const int TOTAL_MISSOES = sizeof(MISSOES)/sizeof(MISSOES[0]);

//vetor(array)
struct Territorio *mapa = NULL; //null -> aponta para lugar nenhum
int capacidade = 0; //é o tamanho alocado
int usados = 0; // quantos da "capacidade" foram usados


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


static int rolar_d6(void) {
    return (rand() % 6) + 1;
}

//lista de atac/def
static void lista_compacta(const struct Territorio *v, int n) {
    for (int i = 0; i < n; i++) {
        printf("[%d] %-20s | dono=%-10s | tropas=%d\n",
        i, v[i].nome, v[i].cor, v[i].tropas);
    }
}

//le indice com validação basica (0...limite-1)
static int ler_indice(const char *prompt, int limite) {
    char buf[64];
    long idx;
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof buf, stdin)) return -1;
        idx = strtol(buf, NULL, 10);
        if (idx >= 0 && idx < limite) return (int)idx;
        puts("Indice invalido. Tente novamente.");
    }
}

//regras minimas atac
static int pode_atacar(const struct Territorio *atac, const struct Territorio *def) {
    //pelo menos 2 tropas para atac
    if (atac->tropas < 2) return 0;
    //não pode atacar territorio da mesma cor
    if (strncmp(atac->cor, def->cor, MAX_COR) == 0) return 0;
    //defensor com 0 ja vai cair direto
    return 1;
}

void atacar(struct Territorio *atac, struct Territorio *def){
    //checar pre-cond 
    if(atac == NULL || def == NULL) return;
    if(atac == def) {
        puts("Não pode atacar o mesmo territorio.");
        return;
    }

    //se não puder atacar, sai
    if(atac->tropas < 2) {
        puts("Atacante não possui tropas suficientes (min.: 2).");
        return;
    }

    printf("\n-- BATALHA --\n");
    printf("Atacante: %s (cor=%s, tropas=%d)\n", atac->nome, atac->cor, atac->tropas);
    printf("Defensor: %s (cor=%s, tropas=%d)\n", def->nome, def->cor, def->tropas);

    //se o def ja estiver sem tropas, captura
    if (def->tropas <= 0) {
        printf("Defensor sem tropas, captura direta!\n");
        //ocupa com 1 tropa
        if (atac->tropas >= 2) {
            def->tropas = 1;
            atac->tropas -= 1;
            //troca cor
            strncpy(def->cor, atac->cor, MAX_COR);
            def->cor[MAX_COR-1] = '\0';
            printf("Territorio %s capturado! Novo dodo: %s\n", def->nome, def->cor);
        } else {
            puts("Mas o atacante não tem tropas para ocupar, Captura fracassou!");
        }
        return;
    }

    // 1 rodada: 1 dado cada
    int d_atac = rolar_d6();
    int d_def = rolar_d6();
    printf("ROlagem: Atacante=%d x defensor=%d\n", d_atac, d_def);

    if (d_atac > d_def) {
        def->tropas -= 1;
        printf("Defensor perde 1 tropa. Tropas def=%d\n", def->tropas);
    } else {
        atac->tropas -= 1;
        printf("Atacante perde 1 tropa. Tropas atac=%d\n", atac->tropas);
    }

    //verifica captura
    if (def->tropas <= 0) {
        if (atac->tropas >= 2) {
            def->tropas = 1;
            atac->tropas -=1;
            strncpy(def->cor, atac->cor, MAX_COR);
            def->cor[MAX_COR-1] = '\0';
            printf("Territorio %s capturado! Novo dono: %s\n", def->nome, def->cor);
            printf("Ocupacao: 1 Tropa movida para o territorio conquistado.\n");
        } else {
            //chegou aqui sem tropas suficientes para ocupar
            puts("defensor zerou, mas atacante nao possui tropas para ocupar, captura falhou!");
            def->tropas = 1;
        }
    }

    puts("-- FIM DA RODADA --");
}


// Contagem de territorios da cor e soma de tropas (ajudam na verificação)
static int contar_territorios_cor(const struct Territorio *mapa, int n, const char *cor) {
    int c = 0;
    for (int i = 0; i < n; ++i)
        if (strncmp(mapa[i].cor, cor, MAX_COR) == 0) c++;
    return c;
}

static int somar_tropas_cor(const struct Territorio *mapa, int n, const char *cor) {
    int t = 0;
    for (int i = 0; i < n; ++i)
        if (strncmp(mapa[i].cor, cor, MAX_COR) == 0) t += mapa[i].tropas;
    return t;
}

// Sorteio e cópia (destino já deve estar alocado com MAX_MISSAO)
void atribuirMissao(char *destino, const char *missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    strcpy(destino, missoes[idx]);
}

void exibirMissao(const char *missao, const char *nomeJogador) {
    printf("\nMissao de %s: %s\n(Guarde essa informacao. Nao sera exibida novamente neste turno.)\n",
           nomeJogador, missao);
}

// Verificação simples baseada no texto da missão
int verificarMissao(const char *missao, const struct Territorio *mapa, int n, const char *corJog) {
    int qt = contar_territorios_cor(mapa, n, corJog);
    int tp = somar_tropas_cor(mapa, n, corJog);

    if (strstr(missao, "Controlar pelo menos 3 territorios") && qt >= 3) return 1;
    if (strstr(missao, "Controlar pelo menos 2 territorios") && qt >= 2) return 1;
    if (strstr(missao, "Somar 15 ou mais tropas") && tp >= 15) return 1;

    if (strstr(missao, "Eliminar todas as tropas da cor Azul")) {
        if (somar_tropas_cor(mapa, n, "Azul") == 0) return 1;
    }
    if (strstr(missao, "Eliminar todas as tropas da cor Vermelha")) {
        if (somar_tropas_cor(mapa, n, "Vermelha") == 0) return 1;
    }
    return 0;
}


/*------------------ FUNÇÃO PRINCIAL ---------------------------*/
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

    mapa = calloc(capacidade, sizeof *mapa);//alloca a "estante" inicial
        if (!mapa) { // se calloc falhar ele não devolve um endereço valido, e avisa o erro e o motivo
            perror("calloc: Cannot allocate memory");
            return 1;
        }

        int numJogadores = 0;
        printf("Quantos jogadores? ");
        if (!fgets(buf, sizeof buf, stdin))
            return 1;
        numJogadores = (int)strtol(buf, NULL, 10);
        if (numJogadores <= 0 || numJogadores > 6)
            numJogadores = 2; // default simples

        struct Jogador *J = calloc(numJogadores, sizeof *J);
        if (!J)
        {
            perror("calloc jogadores");
            free(mapa);
            return 1;
        }

        for (int j = 0; j < numJogadores; ++j)
        {
            printf("\nJogador %d - nome: ", j + 1);
            if (!fgets(J[j].nome, sizeof J[j].nome, stdin))
                J[j].nome[0] = '\0';
            strip_newline(J[j].nome);

            printf("Jogador %d - cor (deve bater com os territorios): ", j + 1);
            if (!fgets(J[j].cor, sizeof J[j].cor, stdin))
                J[j].cor[0] = '\0';
            strip_newline(J[j].cor);

            J[j].missao = malloc(MAX_MISSAO);
            if (!J[j].missao)
            {
                perror("ERRO:malloc-missao");
            }

            atribuirMissao(J[j].missao, MISSOES, TOTAL_MISSOES);
            exibirMissao(J[j].missao, J[j].nome); // exibe uma vez no início
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
        printf("4 - Dobra a capacidade\n");
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

                if (capacidade == usados) {
                    printf("Quantidade maxima atingida! Nao e possivel cadastrar mais territorios.");
                    printf("\nPressione enter para continuar....");
                    getchar();
                    break;
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
                if (usados < 2) {
                    puts("Precisa de pelo menos 2 territorios cadastrados.");
                    printf("Pressione enter para continuar...");
                    getchar();
                    break;
                }

                printf("\n-- SELECIONE ATACANTE E DEFENSOR --\n");
                lista_compacta(mapa, usados);

                int i_atac = ler_indice("Indice do ATACANTE: ", usados);
                int i_def = ler_indice("Indice do DEFENSOR: ", usados);

                if (i_atac == i_def) {
                    puts("Atacante e defensor nao podem ser o mesmo.");
                    printf("Pressione enter para continuar...");
                    getchar();
                    break;
                }

                struct Territorio *atac = &mapa[i_atac];
                struct Territorio *def = &mapa[i_def];

                if (!pode_atacar(atac, def)) {
                    puts("Ataque nao permitido (Tropas insuficientes ou mesma cor).");
                    printf("Pressione Enter para continuar...");
                    getchar();
                    break;
                }

                atacar(atac, def);

                printf("\nEstado atualizado:\n");
                lista_compacta(mapa, usados);

                int houveVencedor = 0;
                for (int j = 0; j < numJogadores; ++j)
                {
                    if (verificarMissao(J[j].missao, mapa, usados, J[j].cor))
                    {
                        printf("\n>>> VITORIA: %s cumpriu a missao!\n", J[j].nome);
                        houveVencedor = 1;
                    }
                }
                if (houveVencedor)
                {
                    printf("Pressione Enter para encerrar o jogo...");
                    getchar();
                    opcao = 0; // encerra o loop principal
                    break;
                }

                printf("Pressione Enter para continuar...");
                getchar();


            } break;

            case 4: { //aumenta capacidade
                if (usados == capacidade) {
                    // Dobra o tamanho da capacidade
                    int nova = capacidade ? capacidade * 2 : 1;
                    
                    // Tenta alocar uma estante nova, maior.
                    struct Territorio *tmp = realloc(mapa, nova * sizeof *tmp);
                    
                    if(!tmp) { // Se não conseguir (faltou memória no PC)
                        perror("realloc");
                        printf("Falha ao crescer o vetor. Registro nao cadastrado\n");
                        printf("Pressione enter para continuar...");
                        break;} //volta ao menu sem mexer em mapa
                        
                    mapa = tmp;
                    capacidade = nova;
                }
                
                printf("foi adicionado mais capacidade, agora voce pode cadastrar mais territorios!\n");
                printf("Pressiona enter para continuar...");
                getchar();
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

    //libera memoria
    for (int j = 0; j < numJogadores; ++j) 
        free(J[j].missao);
    free(J);
    free(mapa);

    return 0;
}