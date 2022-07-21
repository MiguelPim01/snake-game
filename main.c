#include <stdio.h>
#define LINHA 100
#define COLUNA 100

typedef struct {
    char direcaoAtual; //Variavel para armazenar a direção que aponta a cabeca da cobra
    int xCabeca; //Coordenada x da cabeca da cobra
    int yCabeca; //Coordenada y da cabeca da cobra
    int colidiuParede; //Variavel sera 0 se nao colidiu com parede e 1 se colidiu
    int comeuComida; //Variavel sera 0 se nao comeu comida e 1 se comeu 
    int pegouDinheiro; //Variavel sera 0 se nao pegou dinheiro e 1 se pegou 
    int colidiuComCorpo; //Variavel sera 0 se nao colidiu com o corpo e 1 se colidiu
    int cobraViva; //Varivel que indica que a cobra esta viva se for 1 e 0 caso contrario
    int tamanho; //Variavel para indicar o tamanho atual da cobra
    int ultimoMovimento; /* Variavel para armazenar o ultimo movimento feito pela cobra:
                            (1 para cima, 2 para baixo, 3 para direira, 4 para esquerda e 0 se nao moveu) */
} tCobra;

typedef struct {
    char mapa[LINHA][COLUNA]; //Matriz que ira armazenar o mapa do jogo
    int mapaDeCalor[LINHA][COLUNA]; //Matriz para armazenar o mapa de calor
    int linhas; //Numero de linhas das matrizes
    int colunas; //Numero de colunas das matrizes
} tMapa;

tMapa LeMapa(FILE *pFileOut, FILE *pFile, tMapa mapa);
void PrintaMapa(tMapa mapa);

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
    
    /*###### LEITURA E INICIALIZAÇÃO DO MAPA DO JOGO ######*/
    /*#####################################################*/

    sprintf(caminho, "%s/saida/Inicializacao.txt", argv[1]);
    pFileMapaOut = fopen(caminho, "w");

    if (!pFileMapaOut){
        printf("ERRO: Falha na abertura do arquivo Inicializacao.txt. %s/saida/Incializacao.txt\n", argv[1]);
        return 1;
    }

    mapa = LeMapa(pFileMapaOut, pFileMapa, mapa);

    return 0;
}

tMapa LeMapa(FILE *pFileOut, FILE *pFile, tMapa mapa){
    int i, j;
    
    fscanf(pFile, "%d %d%*c", &mapa.linhas, &mapa.colunas);
    
    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            fscanf(pFile, "%c", &mapa.mapa[i][j]);
            fprintf(pFileOut, "%c", mapa.mapa[i][j]);
        }
        fprintf(pFileOut, "\n");
        fscanf(pFile, "%*c");
    }
    fclose(pFile);
    fclose(pFileOut);

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