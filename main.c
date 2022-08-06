#include <stdio.h>

#define LINHA_MAX 100
#define COLUNA_MAX 100
#define TAM_MAX_CORPO 100

#define PAREDE '#'
#define COMIDA '*'
#define DINHEIRO '$'
#define TUNEL '@'
#define CORPO 'o'
#define CABECA_DIREITA '>'
#define CABECA_ESQUERDA '<'
#define CABECA_CIMA '^'
#define CABECA_BAIXO 'v'
#define PARTE_MORTA 'X'


typedef struct {
    int qtdMovimentos;             /*### Struct para armazenar todas as estatisticas da cobra ###*/
    int qtdMovimentosSemPontuar;
    int qtdMovimentosParaBaixo;
    int qtdMovimentosParaCima;
    int qtdMovimentosParaEsquerda;
    int qtdMovimentosParaDireita;
} tEstatistica;

//Obtencao das variaveis do tipo tEstatistica
int ObtemQtdMovimentos(tEstatistica est);
int ObtemQtdMovimentosSemPontuar(tEstatistica est);
int ObtemQtdMovimentosDireita(tEstatistica est);
int ObtemQtdMovimentosEsquerda(tEstatistica est);
int ObtemQtdMovimentosCima(tEstatistica est);
int ObtemQtdMovimentosBaixo(tEstatistica est);

//Encrementacao das variaveis do tipo tEstatistica
tEstatistica AumentaQtdMov(tEstatistica est);
tEstatistica AumentaQtdMovSemPontuar(tEstatistica est);
tEstatistica AumentaQtdMovCima(tEstatistica est);
tEstatistica AumentaQtdMovBaixo(tEstatistica est);
tEstatistica AumentaQtdMovDireita(tEstatistica est);
tEstatistica AumentaQtdMovEsquerda(tEstatistica est);

typedef struct {
    tEstatistica estatistica; //Estatisticas da cobra
    char direcaoAtual; //Indica para onde a cabeca da cobra aponta ('>','^','<','v')
    int PosCorpo[TAM_MAX_CORPO][2]; //Armazena a coordenada de cada parte do corpo incluindo a cabeca (linha 0: coordenada da cabeca)
    int cobraViva; //É 1 se a cobra esta viva e 0 caso contrario
    int tamanho; //Tamanho da cobra
    int pontuacao; //Pontuação da cobra durante o jogo                          
} tCobra;

//Inicializa variaveis contidas em tCobra
tCobra InicializaCobra(FILE *pFile, tCobra cobra, int xCabeca, int yCabeca);

//Move a cobra em "cobra.PosCorpo[][]"
tCobra MoveCobra(tCobra cobra, char mov, int linhas, int colunas, int xCabeca, int yCabeca);

//Retorna: 1 (direita), 2 (cima), 3 (esquerda) e 4 (baixo)
int DirecaoDoMovimento(tCobra cobra, char mov);

//Obtem as coordenadas de uma parte do corpo da cobra
int ObtemPosYParteDoCorpo(tCobra cobra, int parteDoCorpo);
int ObtemPosXParteDoCorpo(tCobra cobra, int parteDoCorpo);

//Retorna a variavel "direcaoAtual" do tipo tCobra
char ObtemDirecaoAtualCobra(tCobra cobra);

//Retorna variavel "pontuacao" do tipo tCobra
int ObtemPontuacaoDaCobra(tCobra cobra);

//Aumenta a pontuacao da cobra (comida: +1pt, dinheiro: +10pts)
tCobra AumentaPontuacaoComida(tCobra cobra);
tCobra AumentaPontuacaoDinheiro(tCobra cobra);

//Faz variavel "cobraViva" receber 0
tCobra MorreCobra(tCobra cobra);

//Retorna variavel "cobraViva" do tipo tCobra
int CobraEstaViva(tCobra cobra);

//Retorna 1 se for a calda da cobra e 0 se nao for
int EhFimDaCobra(tCobra cobra, int x, int y);

//Retorna a estatistica da cobra
tEstatistica ObtemEstatisticaDaCobra(tCobra cobra);

//Acrescenta 1 as qtdMovimentosSemPontuar da estatistica da cobra
tCobra AumentaQtdMovSemPontuarDaCobra(tCobra cobra);

//Retorna tamanho da cobra
int ObtemTamanhoDaCobra(tCobra cobra);

typedef struct {
    tCobra cobra; //Cobra que andara pelo mapa
    char mapa[LINHA_MAX][COLUNA_MAX]; //Matriz que ira armazenar o mapa do jogo
    int mapaDeCalor[LINHA_MAX][COLUNA_MAX]; //Matriz para armazenar o mapa de calor
    int linhas; //Numero de linhas das matrizes
    int colunas; //Numero de colunas das matrizes
    int tunel1[2]; //Coordenada x e y do tunel 1
    int tunel2[2]; //Coordenada x e y do tunel 2
    int dxTunel; //Distancia eixo x do tunel 1 pro 2
    int dyTunel; //Distancia eixo y do tunel 1 pro 2
} tMapa;

//Leitura e inicializacao das variaveis do tipo tMapa
tMapa LeMapaEInicializa(FILE *pFileOut, FILE *pFile, tMapa mapa);

//Inicializa o mapa de calor (1 aonde a cabeca da cobra esta e 0 no resto)
tMapa InicializaMapaDeCalor(tMapa mapa);

//TEMPORARIO
void PrintaEstadoDoMapa(tMapa mapa);

//Obter as coordenadas iniciais da cabeca da cobra
int ObtemPosXCabecaInicial(tMapa mapa);
int ObtemPosYCabecaInicial(tMapa mapa);

//Move a cobra contida no mapa
tMapa MoveCobraNoMapa(FILE *pFileResumo, tMapa mapa, char mov);

//Faz a retirada da cobra e seu corpo do mapa, os substituindo por espacos
tMapa RefrescaMapa(tMapa mapa);

//Printa a cobra com sua posicao atualizada no mapa
tMapa PrintaCobraNoMapa(tMapa mapa);

//Atualiza mapa de calor de acordo com a posicao da cabeca da cobra
tMapa AtualizaMapaDeCalor(tMapa mapa);

//Retorna 1 se a cobra morreu e 0 caso contrario
int EhFimDeJogoCobraMorta(tMapa mapa);

//Retorna 1 se for o fim do jogo (ou morreu ou comeu todas as comidas)
int EhFimDeJogo(tMapa mapa);

//Retorna a quantidade de comida no mapa
int TemComidaNoMapa(tMapa mapa);

//Printa o mapa no arquivo "saida.txt" a cada movimento feito pela cobra
//Obs: Tambem tem opcao de printar na saida padrao (basta tirar o comentario dos printfs)
void PrintaMapaNaSaida(tMapa mapa, char mov);

//Printa o mapaDeCalor no arquivo "heatmap.txt"
void FilePrintaHeatMap(FILE *pFile, tMapa mapa);

//Printa estatisticas da cobra no arquivo "estatisticas.txt"
void FilePrintaEstatisticas(FILE *pFile, tMapa mapa);

//Printa ranking no arquivo "ranking.txt"
void FilePrintaRanking(FILE *pFile, tMapa mapa);

int main(int argc, char *argv[])
{
    FILE *pFileMapa; //Arquivo de entrada
    FILE *pFileMapaOut; FILE *pFileResumo; FILE *pFileRanking; //Arquivos de saida (linha 1) 
    FILE *pFileEstatisticas; FILE *pFileHeatMap; //Arquivos de saida (linha 2)
    tMapa mapa;
    char caminho[1000];

    /*###### VERIFICAÇÃO DE ERROS NA LEITURA DO ARQUIVO "mapa.txt" ######*/
    
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

    sprintf(caminho, "%s/saida/inicializacao.txt", argv[1]);
    pFileMapaOut = fopen(caminho, "w");

    if (!pFileMapaOut){
        printf("ERRO: Falha na abertura do arquivo inicializacao.txt. %s/saida/incializacao.txt\n", argv[1]);
        return 1;
    }

    mapa = LeMapaEInicializa(pFileMapaOut, pFileMapa, mapa);
    mapa = InicializaMapaDeCalor(mapa);

    /*###### REALIZACAO DO JOGO ######*/

    
    //Abrindo arquivos e verificando erros
    //RESUMO:
    sprintf(caminho, "%s/saida/resumo.txt", argv[1]);
    pFileResumo = fopen(caminho, "w");
    if (!pFileResumo){
        printf("ERRO: Falha na abertura do arquivo resumo.txt. %s/saida/resumo.txt\n", argv[1]);
        return 1;
    }

    //Leitura dos movimentos e realizacao do jogo
    char mov;
    while (scanf("%c", &mov) == 1){
        //Move a cobra no mapa do jogo
        mapa = MoveCobraNoMapa(pFileResumo, mapa, mov);
        
        //Printa o estado atual do mapa no arquivo "saida.txt"
        PrintaMapaNaSaida(mapa, mov);

        //Atualiza o mapa de calor, somando 1 a posicao da cabeca da cobra
        mapa = AtualizaMapaDeCalor(mapa);

        if (EhFimDeJogo(mapa)){
            break;
        }

        scanf("%*[^\n]");
        scanf("%*c");
    }
    fclose(pFileResumo);

    //Gerando arquivos finais de saida do programa

    //HEAT MAP:
    sprintf(caminho, "%s/saida/heatmap.txt", argv[1]);
    pFileHeatMap = fopen(caminho, "w");
    if (!pFileHeatMap){
        printf("ERRO: Falha na abertura do arquivo heatmap.txt. %s/saida/heatmap.txt\n", argv[1]);
        return 1;
    }
    FilePrintaHeatMap(pFileHeatMap, mapa);

    //ESTATISTICAS:
    sprintf(caminho, "%s/saida/estatisticas.txt", argv[1]);
    pFileEstatisticas = fopen(caminho, "w");
    if (!pFileEstatisticas){
        printf("ERRO: Falha na abertura do arquivo estatisticas.txt. %s/saida/estatisticas.txt\n", argv[1]);
        return 1;
    }
    FilePrintaEstatisticas(pFileEstatisticas, mapa);

    //RANKING:
    sprintf(caminho, "%s/saida/ranking.txt", argv[1]);
    pFileRanking = fopen(caminho, "w");
    if (!pFileRanking){
        printf("ERRO: Falha na abertura do arquivo ranking.txt. %s/saida/ranking.txt\n", argv[1]);
        return 1;
    }
    FilePrintaRanking(pFileRanking, mapa);

    return 0;
}


/* FUNCOES DO TIPO tEstatistica */

tEstatistica InicializaEstatistica(tEstatistica est){
    est.qtdMovimentos = 0;
    est.qtdMovimentosSemPontuar = 0;
    est.qtdMovimentosParaBaixo = 0;
    est.qtdMovimentosParaCima = 0;
    est.qtdMovimentosParaEsquerda = 0;
    est.qtdMovimentosParaDireita = 0;

    return est;
}

int ObtemQtdMovimentos(tEstatistica est){
    return est.qtdMovimentos;
}

int ObtemQtdMovimentosSemPontuar(tEstatistica est){
    return est.qtdMovimentosSemPontuar;
}

int ObtemQtdMovimentosDireita(tEstatistica est){
    return est.qtdMovimentosParaDireita;
}

int ObtemQtdMovimentosEsquerda(tEstatistica est){
    return est.qtdMovimentosParaEsquerda;
}

int ObtemQtdMovimentosCima(tEstatistica est){
    return est.qtdMovimentosParaCima;
}

int ObtemQtdMovimentosBaixo(tEstatistica est){
    return est.qtdMovimentosParaBaixo;
}

tEstatistica AumentaQtdMov(tEstatistica est){
    est.qtdMovimentos++;
    return est;
}

tEstatistica AumentaQtdMovSemPontuar(tEstatistica est){
    est.qtdMovimentosSemPontuar++;
    return est;
}

tEstatistica AumentaQtdMovCima(tEstatistica est){
    est.qtdMovimentosParaCima++;
    return est;
}

tEstatistica AumentaQtdMovBaixo(tEstatistica est){
    est.qtdMovimentosParaBaixo++;
    return est;
}

tEstatistica AumentaQtdMovDireita(tEstatistica est){
    est.qtdMovimentosParaDireita++;
    return est;
}

tEstatistica AumentaQtdMovEsquerda(tEstatistica est){
    est.qtdMovimentosParaEsquerda++;
    return est;
}

/* FUNCOES DO TIPO tCobra */ 

tCobra InicializaCobra(FILE *pFile, tCobra cobra, int xCabeca, int yCabeca){

    cobra.direcaoAtual = CABECA_DIREITA;
    cobra.cobraViva = 1;
    cobra.tamanho = 1;
    cobra.pontuacao = 0;

    cobra.estatistica = InicializaEstatistica(cobra.estatistica);

    cobra.PosCorpo[0][0] = xCabeca;
    cobra.PosCorpo[0][1] = yCabeca;

    fprintf(pFile, "A cobra comecara o jogo na linha %d e coluna %d\n", cobra.PosCorpo[0][1]+1, cobra.PosCorpo[0][0]+1);
    fclose(pFile);

    return cobra;
}

tCobra MoveCobra(tCobra cobra, char mov, int linhas, int colunas, int xCabeca, int yCabeca){
    int i;
    int aux1[2], aux2[2];
    cobra.estatistica = AumentaQtdMov(cobra.estatistica);
    switch (DirecaoDoMovimento(cobra, mov)){

        //MOVE COBRA PARA DIREITA
        case 1:
            cobra.estatistica = AumentaQtdMovDireita(cobra.estatistica);
            cobra.direcaoAtual = CABECA_DIREITA;
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];

            /* ------ Move a cabeca da cobra ------ */
            cobra.PosCorpo[0][0] = xCabeca;
            cobra.PosCorpo[0][1] = yCabeca;

            /* ------ Verifica se acabou o mapa ------ */
            if (cobra.PosCorpo[0][0] >= colunas){
                cobra.PosCorpo[0][0] = 0;
            }
            break;

        //MOVE COBRA PARA CIMA
        case 2:
            cobra.estatistica = AumentaQtdMovCima(cobra.estatistica);
            cobra.direcaoAtual = CABECA_CIMA;
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];

            /* ------ Move a cabeca da cobra ------ */
            cobra.PosCorpo[0][0] = xCabeca;
            cobra.PosCorpo[0][1] = yCabeca;

            /* ------ Verifica se acabou o mapa ------ */
            if (cobra.PosCorpo[0][1] < 0){
                cobra.PosCorpo[0][1] = linhas - 1;
            }
            break;

        //MOVE COBRA PARA ESQUERDA
        case 3:
            cobra.estatistica = AumentaQtdMovEsquerda(cobra.estatistica);
            cobra.direcaoAtual = CABECA_ESQUERDA;
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];

            /* ------ Move a cabeca da cobra ------ */
            cobra.PosCorpo[0][0] = xCabeca;
            cobra.PosCorpo[0][1] = yCabeca;

            /* ------ Verifica se acabou o mapa ------ */
            if (cobra.PosCorpo[0][0] < 0){
                cobra.PosCorpo[0][0] = colunas - 1;
            }
            break;

        //MOVE COBRA PARA BAIXO
        case 4:
            cobra.estatistica = AumentaQtdMovBaixo(cobra.estatistica);
            cobra.direcaoAtual = CABECA_BAIXO;
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];

            /* ------ Move a cabeca da cobra ------ */
            cobra.PosCorpo[0][0] = xCabeca;
            cobra.PosCorpo[0][1] = yCabeca;

            /* ------ Verifica se acabou o mapa ------ */
            if (cobra.PosCorpo[0][1] >= linhas){
                cobra.PosCorpo[0][1] = 0;
            }
            break;
    }

    /* ------ Move corpo da cobra ------ */
    for (i = 1; i < cobra.tamanho; i+=2){
        aux2[0] = cobra.PosCorpo[i][0];
        aux2[1] = cobra.PosCorpo[i][1];
        cobra.PosCorpo[i][0] = aux1[0];
        cobra.PosCorpo[i][1] = aux1[1];
        if (i + 1 == cobra.tamanho){
            break;
        }
        aux1[0] = cobra.PosCorpo[i+1][0];
        aux1[1] = cobra.PosCorpo[i+1][1];
        cobra.PosCorpo[i+1][0] = aux2[0];
        cobra.PosCorpo[i+1][1] = aux2[1];
    }

    return cobra;
}

int DirecaoDoMovimento(tCobra cobra, char mov){
    switch (cobra.direcaoAtual){
        case CABECA_DIREITA:
            if (mov == 'c'){
                return 1; //Movimento para direita
            }
            else if (mov == 'a'){
                return 2; //Movimento para cima
            }
            else if (mov == 'h'){
                return 4; //Movimento para baixo
            }
            break;
        case CABECA_ESQUERDA:
            if (mov == 'c'){
                return 3; //Movimento para esquerda
            }
            else if (mov == 'a'){
                return 4; //Movimento para baixo
            }
            else if (mov == 'h'){
                return 2; //Movimento para cima
            }
            break;
        case CABECA_BAIXO:
            if (mov == 'c'){
                return 4; //Movimento para baixo
            }
            else if (mov == 'a'){
                return 1; //Movimento para direita
            }
            else if (mov == 'h'){
                return 3; //Movimento para esquerda
            }
            break;
        case CABECA_CIMA:
            if (mov == 'c'){
                return 2; //Movimento para cima
            }
            else if (mov == 'a'){
                return 3; //Movimento para esquerda
            }
            else if (mov == 'h'){
                return 1; //Movimento para direita
            }
            break;
    }
    printf("ERRO: Movimento invalido\n");
}

int ObtemPosYParteDoCorpo(tCobra cobra, int parteDoCorpo){
    return cobra.PosCorpo[parteDoCorpo][1];
}

int ObtemPosXParteDoCorpo(tCobra cobra, int parteDoCorpo){
    return cobra.PosCorpo[parteDoCorpo][0];
}

char ObtemDirecaoAtualCobra(tCobra cobra){
    return cobra.direcaoAtual;
}

tCobra AumentaTamanhoCobra(tCobra cobra){
    cobra.tamanho++;
    return cobra;
}

int ObtemPosXCabeca(tCobra cobra){
    return cobra.PosCorpo[0][0];
}

int ObtemPosYCabeca(tCobra cobra){
    return cobra.PosCorpo[0][1];
}

int ObtemPontuacaoDaCobra(tCobra cobra){
    return cobra.pontuacao;
}

tCobra AumentaPontuacaoComida(tCobra cobra){
    cobra.pontuacao++;
    return cobra;
}

tCobra AumentaPontuacaoDinheiro(tCobra cobra){
    cobra.pontuacao += 10;
    return cobra;
}

tCobra MorreCobra(tCobra cobra){
    cobra.cobraViva = 0;
    return cobra;
}

int CobraEstaViva(tCobra cobra){
    return cobra.cobraViva;
}

int EhFimDaCobra(tCobra cobra, int x, int y){
    if (cobra.PosCorpo[cobra.tamanho-1][0] == x && cobra.PosCorpo[cobra.tamanho-1][1] == y){
        return 1;
    }
    return 0;
}

tEstatistica ObtemEstatisticaDaCobra(tCobra cobra){
    return cobra.estatistica;
}

tCobra AumentaQtdMovSemPontuarDaCobra(tCobra cobra){
    cobra.estatistica = AumentaQtdMovSemPontuar(cobra.estatistica);
    return cobra;
}

int ObtemTamanhoDaCobra(tCobra cobra){
    return cobra.tamanho;
}

/* FUNCOES DO TIPO tMapa */

tMapa LeMapaEInicializa(FILE *pFileOut, FILE *pFile, tMapa mapa){
    int i, j, cont=0;
    
    //Le as linhas e colunas do mapa
    fscanf(pFile, "%d %d%*[^\n]", &mapa.linhas, &mapa.colunas);
    fscanf(pFile, "%*c");
    
    //Le as informacoes do mapa e armazena na matriz "mapa" do tipo tMapa
    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            fscanf(pFile, "%c", &mapa.mapa[i][j]);
            if (mapa.mapa[i][j] == TUNEL && !cont){
                mapa.tunel1[0] = j;
                mapa.tunel1[1] = i;
                cont = 1;
            }
            else if (mapa.mapa[i][j] == TUNEL){
                mapa.tunel2[0] = j;
                mapa.tunel2[1] = i;
            }

            //Printa no arquivo "Incializacao.txt"
            fprintf(pFileOut, "%c", mapa.mapa[i][j]);
        }
        //Printa no arquivo "Incializacao.txt"
        fprintf(pFileOut, "\n");

        fscanf(pFile, "%*[^\n]");
        fscanf(pFile, "%*c");
    }
    fclose(pFile);

    if (cont){
        mapa.dxTunel = mapa.tunel1[0] - mapa.tunel2[0];
        mapa.dyTunel = mapa.tunel1[1] - mapa.tunel2[1];
    }

    //Inicializa cobra e fecha arquivo "Inicializacao.txt"
    mapa.cobra = InicializaCobra(pFileOut, mapa.cobra, ObtemPosXCabecaInicial(mapa), ObtemPosYCabecaInicial(mapa));

    return mapa;
}

tMapa InicializaMapaDeCalor(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == CABECA_DIREITA){
                mapa.mapaDeCalor[i][j] = 1;
            }
            else {
                mapa.mapaDeCalor[i][j] = 0;
            }
        }
    }

    return mapa;
}

void PrintaEstadoDoMapa(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            printf("%c", mapa.mapa[i][j]);
        }
        printf("\n");
    }
}

int ObtemPosXCabecaInicial(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            //Verificar aonde se encontra a cabeça da cobra
            if (mapa.mapa[i][j] == CABECA_DIREITA){
                return j;
            }
        }
    }
}

int ObtemPosYCabecaInicial(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            //Verificar aonde se encontra a cabeça da cobra
            if (mapa.mapa[i][j] == CABECA_DIREITA){
                return i;
            }
        }
    }
}

tMapa MoveCobraNoMapa(FILE *pFileResumo, tMapa mapa, char mov){
    int xCabeca, yCabeca;
    tEstatistica est;

    est = ObtemEstatisticaDaCobra(mapa.cobra);

    xCabeca = ObtemPosXCabeca(mapa.cobra);
    yCabeca = ObtemPosYCabeca(mapa.cobra);

    //Verificação de onde a cabeca da cobra estara indo (#, o, *, $, @) 
    switch (DirecaoDoMovimento(mapa.cobra, mov)){
        case 1:
            if ((xCabeca + 1) >= mapa.colunas){
                xCabeca = 0;
            }
            else {
                xCabeca += 1;
            }
            //TRATANDO OS TUNEIS (direita):
            if (mapa.mapa[yCabeca][xCabeca] == TUNEL){
                //Indo do tunel 1 para o tunel 2:
                if (xCabeca == mapa.tunel1[0] && yCabeca == mapa.tunel1[1]){
                    yCabeca -= mapa.dyTunel;
                    xCabeca -= mapa.dxTunel;
                    xCabeca++; //Devido ao movimento ser pra direita
                    if (xCabeca >= mapa.colunas){
                        xCabeca = 0;
                    }
                }
                //Indo do tunel 2 para o tunel 1:
                else {
                    yCabeca += mapa.dyTunel;
                    xCabeca += mapa.dxTunel;
                    xCabeca++; //Devido ao movimento ser pra direita
                    if (xCabeca >= mapa.colunas){
                        xCabeca = 0;
                    }
                }
            }
            if (xCabeca == ObtemPosXCabeca(mapa.cobra) && yCabeca == ObtemPosYCabeca(mapa.cobra) && ObtemTamanhoDaCobra(mapa.cobra) > 1){
                mapa.cobra = MorreCobra(mapa.cobra);
            }
            switch (mapa.mapa[yCabeca][xCabeca]){
                case PAREDE:
                    mapa.cobra = MorreCobra(mapa.cobra);
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    break;    
                case CORPO:
                    if (!EhFimDaCobra(mapa.cobra, xCabeca, yCabeca)){
                        mapa.cobra = MorreCobra(mapa.cobra);
                        fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    }
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
                case COMIDA:
                    mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                    mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                    if (TemComidaNoMapa(mapa) == 1){
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d, terminando o jogo\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    else {
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    break;
                case DINHEIRO:
                    mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) gerou dinheiro\n", ObtemQtdMovimentos(est)+1, mov);
                    break;
                default:
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
            }
            break;
        case 2:
            if ((yCabeca - 1) < 0){
                yCabeca = mapa.linhas - 1;
            }
            else {
                yCabeca -= 1;
            }
            //TRATANDO OS TUNEIS (cima):
            if (mapa.mapa[yCabeca][xCabeca] == TUNEL){
                //Indo do tunel 1 para o tunel 2:
                if (xCabeca == mapa.tunel1[0] && yCabeca == mapa.tunel1[1]){
                    yCabeca -= mapa.dyTunel;
                    yCabeca--; //Devido ao movimento ser pra cima
                    xCabeca -= mapa.dxTunel;
                    if (yCabeca < 0){
                        yCabeca = mapa.linhas - 1;
                    }
                }
                //Indo do tunel 2 para o tunel 1:
                else {
                    yCabeca += mapa.dyTunel;
                    yCabeca--; //Devido ao movimento ser pra cima
                    xCabeca += mapa.dxTunel;
                    if (yCabeca < 0){
                        yCabeca = mapa.linhas - 1;
                    }
                }
            }
            if (xCabeca == ObtemPosXCabeca(mapa.cobra) && yCabeca == ObtemPosYCabeca(mapa.cobra) && ObtemTamanhoDaCobra(mapa.cobra) > 1){
                mapa.cobra = MorreCobra(mapa.cobra);
            }
            switch (mapa.mapa[yCabeca][xCabeca]){
                case PAREDE:
                    mapa.cobra = MorreCobra(mapa.cobra);
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    break;    
                case CORPO:
                    if (!EhFimDaCobra(mapa.cobra, xCabeca, yCabeca)){
                        mapa.cobra = MorreCobra(mapa.cobra);
                        fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    }
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
                case COMIDA:
                    mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                    mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                    if (TemComidaNoMapa(mapa) == 1){
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d, terminando o jogo\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    else {
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    break;
                case DINHEIRO:
                    mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) gerou dinheiro\n", ObtemQtdMovimentos(est)+1, mov);
                    break;
                default:
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
            }
            break;
        case 3:
            if ((xCabeca - 1) < 0){
                xCabeca = mapa.colunas - 1;
            }
            else {
                xCabeca -= 1;
            }
            //TRATANDO OS TUNEIS (esquerda):
            if (mapa.mapa[yCabeca][xCabeca] == TUNEL){
                //Indo do tunel 1 para o tunel 2:
                if (xCabeca == mapa.tunel1[0] && yCabeca == mapa.tunel1[1]){
                    yCabeca -= mapa.dyTunel;
                    xCabeca -= mapa.dxTunel;
                    xCabeca--; //Devido ao movimento ser pra esquerda
                    if (xCabeca < 0){
                        xCabeca = mapa.colunas - 1;
                    }
                }
                //Indo do tunel 2 para o tunel 1:
                else {
                    yCabeca += mapa.dyTunel;
                    xCabeca += mapa.dxTunel;
                    xCabeca--; //Devido ao movimento ser pra esquerda
                    if (xCabeca < 0){
                        xCabeca = mapa.colunas - 1;
                    }
                }
            }
            if (xCabeca == ObtemPosXCabeca(mapa.cobra) && yCabeca == ObtemPosYCabeca(mapa.cobra) && ObtemTamanhoDaCobra(mapa.cobra) > 1){
                mapa.cobra = MorreCobra(mapa.cobra);
            }
            switch (mapa.mapa[yCabeca][xCabeca]){
                case PAREDE:
                    mapa.cobra = MorreCobra(mapa.cobra);
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    break;    
                case CORPO:
                    if (!EhFimDaCobra(mapa.cobra, xCabeca, yCabeca)){
                        mapa.cobra = MorreCobra(mapa.cobra);
                        fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    }
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
                case COMIDA:
                    mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                    mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                    if (TemComidaNoMapa(mapa) == 1){
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d, terminando o jogo\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    else {
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    break;
                case DINHEIRO:
                    mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) gerou dinheiro\n", ObtemQtdMovimentos(est)+1, mov);
                    break;
                default:
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
            }
            break;
        case 4:
            if ((yCabeca + 1) >= mapa.linhas){
                yCabeca = 0;
            }
            else {
                yCabeca += 1;
            }
            //TRATANDO OS TUNEIS (baixo):
            if (mapa.mapa[yCabeca][xCabeca] == TUNEL){
                //Indo do tunel 1 para o tunel 2:
                if (xCabeca == mapa.tunel1[0] && yCabeca == mapa.tunel1[1]){
                    yCabeca -= mapa.dyTunel;
                    yCabeca++; //Devido ao movimento ser pra baixo
                    xCabeca -= mapa.dxTunel;
                    if (yCabeca >= mapa.linhas){
                        yCabeca = 0;
                    }
                }
                //Indo do tunel 2 para o tunel 1:
                else {
                    yCabeca += mapa.dyTunel;
                    yCabeca++; //Devido ao movimento ser pra baixo
                    xCabeca += mapa.dxTunel;
                    if (yCabeca >= mapa.linhas){
                        yCabeca = 0;
                    }
                }
            }
            if (xCabeca == ObtemPosXCabeca(mapa.cobra) && yCabeca == ObtemPosYCabeca(mapa.cobra) && ObtemTamanhoDaCobra(mapa.cobra) > 1){
                mapa.cobra = MorreCobra(mapa.cobra);
            }
            switch (mapa.mapa[yCabeca][xCabeca]){
                case PAREDE:
                    mapa.cobra = MorreCobra(mapa.cobra);
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    break;    
                case CORPO:
                    if (!EhFimDaCobra(mapa.cobra, xCabeca, yCabeca)){
                        mapa.cobra = MorreCobra(mapa.cobra);
                        fprintf(pFileResumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", ObtemQtdMovimentos(est)+1, mov);
                    }
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
                case COMIDA:
                    mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                    mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                    if (TemComidaNoMapa(mapa) == 1){
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d, terminando o jogo\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    else {
                        fprintf(pFileResumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", ObtemQtdMovimentos(est)+1, mov, ObtemTamanhoDaCobra(mapa.cobra));
                    }
                    break;
                case DINHEIRO:
                    mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                    fprintf(pFileResumo, "Movimento %d (%c) gerou dinheiro\n", ObtemQtdMovimentos(est)+1, mov);
                    break;
                default:
                    mapa.cobra = AumentaQtdMovSemPontuarDaCobra(mapa.cobra);
                    break;
            }
            break;
    }
    
    mapa.cobra = MoveCobra(mapa.cobra, mov, mapa.linhas, mapa.colunas, xCabeca, yCabeca);

    mapa = RefrescaMapa(mapa);
    mapa = PrintaCobraNoMapa(mapa);

    return mapa;
}

tMapa RefrescaMapa(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == CABECA_DIREITA || mapa.mapa[i][j] == CABECA_ESQUERDA || mapa.mapa[i][j] == CABECA_BAIXO || mapa.mapa[i][j] == CABECA_CIMA ||
                mapa.mapa[i][j] == CORPO){
                    mapa.mapa[i][j] = ' ';
                }
        }
    }
    return mapa;
}

tMapa PrintaCobraNoMapa(tMapa mapa){
    int i;

    if (CobraEstaViva(mapa.cobra)){
        for (i = 0; i < ObtemTamanhoDaCobra(mapa.cobra); i++){
            if (!i){
                mapa.mapa[ObtemPosYParteDoCorpo(mapa.cobra, i)][ObtemPosXParteDoCorpo(mapa.cobra, i)] = ObtemDirecaoAtualCobra(mapa.cobra);
            }
            else {
                mapa.mapa[ObtemPosYParteDoCorpo(mapa.cobra, i)][ObtemPosXParteDoCorpo(mapa.cobra, i)] = CORPO;
            }
        }
    }
    else {
        for (i = 0; i < ObtemTamanhoDaCobra(mapa.cobra); i++){
            mapa.mapa[ObtemPosYParteDoCorpo(mapa.cobra, i)][ObtemPosXParteDoCorpo(mapa.cobra, i)] = PARTE_MORTA;
        }
    }
    return mapa;
}

tMapa AtualizaMapaDeCalor(tMapa mapa){
    mapa.mapaDeCalor[ObtemPosYCabeca(mapa.cobra)][ObtemPosXCabeca(mapa.cobra)] += 1;
    return mapa;
}

int EhFimDeJogoCobraMorta(tMapa mapa){
    if (CobraEstaViva(mapa.cobra)){
        return 0;
    }
    return 1;
}

int EhFimDeJogo(tMapa mapa){
    if (EhFimDeJogoCobraMorta(mapa)){
        return 1;
    }
    else if (!TemComidaNoMapa(mapa)){
        return 1;
    }
    return 0;
}

int TemComidaNoMapa(tMapa mapa){
    int i, j, cont=0;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == COMIDA){
                cont++;
            }
        }
    }
    return cont;
}

void PrintaMapaNaSaida(tMapa mapa, char mov){
    int i, j;

    //Funcao tambem tem a opcao de printar na saida padrao (basta apenas tirar o comentario dos printfs)

    //fprintf(pFile, "\n");
    //fprintf(pFile, "Estado do jogo apos o movimento '%c':\n", mov);
    printf("\n");
    printf("Estado do jogo apos o movimento '%c':\n", mov);
    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            //fprintf(pFile, "%c", mapa.mapa[i][j]);
            printf("%c", mapa.mapa[i][j]);
        }
        //fprintf(pFile, "\n");
        printf("\n");
    }
    //fprintf(pFile, "Pontuacao: %d\n", ObtemPontuacaoDaCobra(mapa.cobra));
    printf("Pontuacao: %d\n", ObtemPontuacaoDaCobra(mapa.cobra));
    if (!CobraEstaViva(mapa.cobra)){
        //fprintf(pFile, "Game over!\nPontuacao final: %d", ObtemPontuacaoDaCobra(mapa.cobra));
        printf("Game over!\nPontuacao final: %d\n", ObtemPontuacaoDaCobra(mapa.cobra));
    }
    else if (!TemComidaNoMapa(mapa)){
        //fprintf(pFile, "Voce venceu!\nPontuacao final: %d", ObtemPontuacaoDaCobra(mapa.cobra));
        printf("Voce venceu!\nPontuacao final: %d\n", ObtemPontuacaoDaCobra(mapa.cobra));
    }
}

void FilePrintaHeatMap(FILE *pFile, tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (j == (mapa.colunas - 1)){
                fprintf(pFile, "%d", mapa.mapaDeCalor[i][j]);
            }
            else {
                fprintf(pFile, "%d ", mapa.mapaDeCalor[i][j]);
            }
        }
        fprintf(pFile, "\n");
    }

    fclose(pFile);
}

void FilePrintaEstatisticas(FILE *pFile, tMapa mapa){
    tEstatistica est;

    est = ObtemEstatisticaDaCobra(mapa.cobra);

    fprintf(pFile, "Numero de movimentos: %d\n", ObtemQtdMovimentos(est));
    fprintf(pFile, "Numero de movimentos sem pontuar: %d\n", ObtemQtdMovimentosSemPontuar(est));
    fprintf(pFile, "Numero de movimentos para baixo: %d\n", ObtemQtdMovimentosBaixo(est));
    fprintf(pFile, "Numero de movimentos para cima: %d\n", ObtemQtdMovimentosCima(est));
    fprintf(pFile, "Numero de movimentos para esquerda: %d\n", ObtemQtdMovimentosEsquerda(est));
    fprintf(pFile, "Numero de movimentos para direita: %d\n", ObtemQtdMovimentosDireita(est));

    fclose(pFile);
}

void FilePrintaRanking(FILE *pFile, tMapa mapa){
    int i, j, cont=0;
    int vet[mapa.linhas*mapa.colunas][3];

    /*
    Armazenar todos os valores onde a cobra passou pelo menos uma vez em uma matriz
    coluna 0: LINHA - coluna 1: COLUNA - coluna 2: VALOR NO HEATMAP
    */

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapaDeCalor[i][j] > 0){
                vet[cont][0] = i;
                vet[cont][1] = j;
                vet[cont][2] = mapa.mapaDeCalor[i][j];
                cont++;
            }
        }
    }

    //ORDENAR A MATRIZ DECRESCENTE:
    int aux[3], pos, flag=0;
    for (i = 0; i < cont; i++){

        aux[0] = vet[i][0]; 
        aux[1] = vet[i][1]; 
        aux[2] = vet[i][2];
        flag = 0;
        for (j = (i + 1); j < cont; j++){
            //Analizando valor no heatmap
            if (aux[2] < vet[j][2]){
                aux[0] = vet[j][0]; 
                aux[1] = vet[j][1]; 
                aux[2] = vet[j][2];
                pos = j;
                flag = 1;
            }
            else if (aux[2] == vet[j][2]){
                //Analizando linhas
                if (aux[0] > vet[j][0]){
                    aux[0] = vet[j][0]; 
                    aux[1] = vet[j][1]; 
                    aux[2] = vet[j][2];
                    pos = j;
                    flag = 1;
                }
                else if (aux[0] == vet[j][0]){
                    //Analizando colunas
                    if (aux[1] > vet[j][1]){
                        aux[0] = vet[j][0]; 
                        aux[1] = vet[j][1]; 
                        aux[2] = vet[j][2];
                        pos = j;
                        flag = 1;
                    }
                }
            }
        }//Fim do for "j"
        if (flag){
            vet[pos][0] = vet[i][0];
            vet[pos][1] = vet[i][1];
            vet[pos][2] = vet[i][2];
            vet[i][0] = aux[0];
            vet[i][1] = aux[1];
            vet[i][2] = aux[2];
        }
    }//Fim do for "i"

    //PRINTAR NO ARQUIVO:
    for (i = 0; i < cont; i++){
        fprintf(pFile, "(%d, %d) - %d\n", vet[i][0], vet[i][1], vet[i][2]);
    }

    fclose(pFile);
}