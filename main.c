#include <stdio.h>

#define LINHA_MAX 100
#define COLUNA_MAX 100
#define TAM_MAX_CORPO 100


typedef struct {
    char mapa[LINHA_MAX][COLUNA_MAX]; //Matriz que ira armazenar o mapa do jogo
    int mapaDeCalor[LINHA_MAX][COLUNA_MAX]; //Matriz para armazenar o mapa de calor
    int linhas; //Numero de linhas das matrizes
    int colunas; //Numero de colunas das matrizes
} tMapa;

tMapa LeMapa(FILE *pFileOut, FILE *pFile, tMapa mapa);
void PrintaMapa(tMapa mapa);

typedef struct {
    char direcaoAtual; //Variavel para armazenar a direção que aponta a cabeca da cobra
    int PosCorpo[TAM_MAX_CORPO][2]; //Armazena a coordenada de cada parte do corpo incluindo a cabeca (linha 0: coordenada da cabeca)
    int colidiuParede; //Variavel sera 0 se nao colidiu com parede e 1 se colidiu
    int comeuComida; //Variavel sera 0 se nao comeu comida e 1 se comeu 
    int pegouDinheiro; //Variavel sera 0 se nao pegou dinheiro e 1 se pegou 
    int colidiuComCorpo; //Variavel sera 0 se nao colidiu com o corpo e 1 se colidiu
    int cobraViva; //Varivel que indica que a cobra esta viva se for 1 e 0 caso contrario
    int tamanho; //Variavel para indicar o tamanho atual da cobra                           
} tCobra;

tCobra InicializaCobra(FILE *pFile, tCobra cobra, tMapa mapa);

typedef struct {
    int qtdMovimentos;             /*### Struct que ira armazenar todas as estatisticas da partida ###*/
    int qtdMovimentosSemPontuar;
    int qtdMovimentosParaBaixo;
    int qtdMovimentosParaCima;
    int qtdMovimentosParaEsquerda;
    int qtdMovimentosParaDireita;
} tEstatistica;

int main(int argc, char *argv[])
{
    FILE *pFileMapa; FILE *pFileMovimentos; //Arquivos de entrada
    FILE *pFileMapaOut; //Arquivos de saida
    tMapa mapa;
    tCobra cobra;
    char caminho[1000];

    /*###### VERIFICAÇÃO DE ERROS NA LEITURA DO ARQUIVO "mapa.txt" ######*/
    /*###################################################################*/
    
    if (argc <= 1){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 1;
    }
    
    sprintf(caminho, "%s/mapa.txt", argv[1]);
    pFileMapa = fopen(caminho, "r");

    if (!pFileMapa){
        printf("ERRO: Problema na leitura do arquivo mapa.txt. %s/mapa.txt\n", argv[1]);
        return 1;
    }
    
    /*###### LEITURA E INICIALIZAÇÃO DO MAPA DO JOGO (Criacao do arquivo "Inicializacao.txt") ######*/
    /*##############################################################################################*/

    sprintf(caminho, "%s/saida/Inicializacao.txt", argv[1]);
    pFileMapaOut = fopen(caminho, "w");

    if (!pFileMapaOut){
        printf("ERRO: Falha na abertura do arquivo Inicializacao.txt. %s/saida/Incializacao.txt\n", argv[1]);
        return 1;
    }

    mapa = LeMapa(pFileMapaOut, pFileMapa, mapa);

    cobra = InicializaCobra(pFileMapaOut, cobra, mapa);

    return 0;
}

tCobra InicializaCobra(FILE *pFile, tCobra cobra, tMapa mapa){
    int i, j, flag=0;

    cobra.direcaoAtual = '>';
    cobra.colidiuComCorpo = 0;
    cobra.colidiuParede = 0;
    cobra.comeuComida = 0;
    cobra.cobraViva = 1;
    cobra.pegouDinheiro = 0;
    cobra.tamanho = 1;

    //Achar a posicao inicial da cabeca da cobra
    for (i = 0; i < mapa.linhas; i++){
        if (flag){
            break;
        }
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == cobra.direcaoAtual){
                cobra.PosCorpo[0][0] = j + 1;
                cobra.PosCorpo[0][1] = i + 1;
                flag = 1;
                break;
            }
        }
    }
    fprintf(pFile, "A cobra comecara o jogo na linha %d e coluna %d", cobra.PosCorpo[0][1], cobra.PosCorpo[0][0]);
    fclose(pFile);

    return cobra;
}

tMapa LeMapa(FILE *pFileOut, FILE *pFile, tMapa mapa){
    int i, j;
    
    //Le as linhas e colunas do mapa
    fscanf(pFile, "%d %d%*[^\n]", &mapa.linhas, &mapa.colunas);
    fscanf(pFile, "%*c");
    
    //Le as informacoes do mapa e armazena na matriz "mapa" do tipo tMapa
    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            fscanf(pFile, "%c", &mapa.mapa[i][j]);

            //Printa no arquivo "Incializacao.txt"
            fprintf(pFileOut, "%c", mapa.mapa[i][j]);
        }
        //Printa no arquivo "Incializacao.txt"
        fprintf(pFileOut, "\n");

        fscanf(pFile, "%*[^\n]");
        fscanf(pFile, "%*c");
    }
    fclose(pFile);

    return mapa;
}

void PrintaMapa(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            printf("%c", mapa.mapa[i][j]);
        }
        printf("\n");
    }
}