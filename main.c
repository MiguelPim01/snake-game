#include <stdio.h>

#define LINHA_MAX 100
#define COLUNA_MAX 100
#define TAM_MAX_CORPO 100


typedef struct {
    char direcaoAtual; //Variavel para armazenar a direção que aponta a cabeca da cobra
    int PosCorpo[TAM_MAX_CORPO][2]; //Armazena a coordenada de cada parte do corpo incluindo a cabeca (linha 0: coordenada da cabeca)
    int colidiuParede; //Variavel sera 0 se nao colidiu com parede e 1 se colidiu
    int comeuComida; //Variavel sera 0 se nao comeu comida e 1 se comeu 
    int pegouDinheiro; //Variavel sera 0 se nao pegou dinheiro e 1 se pegou 
    int colidiuComCorpo; //Variavel sera 0 se nao colidiu com o corpo e 1 se colidiu
    int cobraViva; //Varivel que indica que a cobra esta viva se for 1 e 0 caso contrario
    int tamanho; //Variavel para indicar o tamanho atual da cobra
    int pontuacao;                           
} tCobra;

tCobra InicializaCobra(FILE *pFile, tCobra cobra, int xCabeca, int yCabeca);
tCobra MoveCobra(tCobra cobra, char mov, int linhas, int colunas);
int DirecaoDoMovimento(tCobra cobra, char mov); //Retorna: 1 (direita), 2 (cima), 3 (esquerda) e 4 (baixo)
int ObtemPosYParteDoCorpo(tCobra cobra, int parteDoCorpo);
int ObtemPosXParteDoCorpo(tCobra cobra, int parteDoCorpo);
char ObtemDirecaoAtualCobra(tCobra cobra);
int TamanhoCobra(tCobra cobra);
int ObtemPontuacaoDaCobra(tCobra cobra);
tCobra AumentaPontuacaoComida(tCobra cobra);
tCobra AumentaPontuacaoDinheiro(tCobra cobra);
tCobra MorreCobra(tCobra cobra);
int CobraEstaViva(tCobra cobra);
int EhFimDaCobra(tCobra cobra, int x, int y);

typedef struct {
    tCobra cobra;
    char mapa[LINHA_MAX][COLUNA_MAX]; //Matriz que ira armazenar o mapa do jogo
    int mapaDeCalor[LINHA_MAX][COLUNA_MAX]; //Matriz para armazenar o mapa de calor
    int linhas; //Numero de linhas das matrizes
    int colunas; //Numero de colunas das matrizes
    int qtdMovimentos;
} tMapa;

tMapa LeMapaEInicializa(FILE *pFileOut, FILE *pFile, tMapa mapa);
tMapa InicializaMapaDeCalor(tMapa mapa);
void PrintaEstadoDoMapa(tMapa mapa);
int ObtemPosXCabecaInicial(tMapa mapa);
int ObtemPosYCabecaInicial(tMapa mapa);
tMapa MoveCobraNoMapa(FILE *pFile, tMapa mapa, char mov);
tMapa RefrescaMapa(tMapa mapa);
tMapa PrintaCobraNoMapa(tMapa mapa);
tMapa AtualizaMapaDeCalor(tMapa mapa);
void FilePrintaMapaNaSaida(FILE *pFile, tMapa mapa, char mov);
int EhFimDeJogoCobraMorta(tMapa mapa);
tCobra ObtemCobra(tMapa mapa);
int EhFimDeJogo(tMapa mapa);
int TemComidaNoMapa(tMapa mapa);
void FilePrintaHeatMap(FILE *pFile, tMapa mapa);

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
    FILE *pFileMapaOut; FILE *pFileResumo; FILE *pFileSaida; FILE *pFileRanking; //Arquivos de saida (linha 1) 
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

    sprintf(caminho, "%s/saida/Inicializacao.txt", argv[1]);
    pFileMapaOut = fopen(caminho, "w");

    if (!pFileMapaOut){
        printf("ERRO: Falha na abertura do arquivo Inicializacao.txt. %s/saida/Incializacao.txt\n", argv[1]);
        return 1;
    }

    mapa = LeMapaEInicializa(pFileMapaOut, pFileMapa, mapa);
    mapa = InicializaMapaDeCalor(mapa);

    /*###### REALIZACAO DO JOGO ######*/

    
    //Abrindo arquivos e verificando erros
    sprintf(caminho, "%s/movimentos.txt", argv[1]);
    pFileMovimentos = fopen(caminho, "r");
    if (!pFileMovimentos){
        printf("ERRO: Falha na leitura do arquivo movimentos.txt. %s/movimentos.txt\n", argv[1]);
        return 1;
    }
    sprintf(caminho, "%s/saida/resumo.txt", argv[1]);
    pFileResumo = fopen(caminho, "w");
    if (!pFileResumo){
        printf("ERRO: Falha na abertura do arquivo resumo.txt. %s/saida/resumo.txt\n", argv[1]);
        return 1;
    }
    sprintf(caminho, "%s/saida/saida.txt", argv[1]);
    pFileSaida = fopen(caminho, "w");
    if (!pFileSaida){
        printf("ERRO: Falha na abertura do arquivo saida.txt. %s/saida/saida.txt\n", argv[1]);
        return 1;
    }

    //Leitura dos movimentos e realizacao do jogo
    char mov;
    while (!feof(pFileMovimentos)){
        fscanf(pFileMovimentos, "%c", &mov); //Obtendo o movimento que a cobra ira realizar
        fscanf(pFileMovimentos, "%*[^\n]");
        fscanf(pFileMovimentos, "%*c");

        //Move a cobra no mapa do jogo
        mapa = MoveCobraNoMapa(pFileResumo, mapa, mov);
        
        //Printa o estado atual do mapa no arquivo "saida.txt"
        FilePrintaMapaNaSaida(pFileSaida, mapa, mov);

        //Atualiza o mapa de calor, somando 1 a posicao da cabeca da cobra
        mapa = AtualizaMapaDeCalor(mapa);

        if (EhFimDeJogo(mapa)){
            break;
        }
    }
    if (!EhFimDeJogoCobraMorta(mapa)){
        fprintf(pFileSaida, "Voce venceu!\nPontuacao final: %d", ObtemPontuacaoDaCobra(ObtemCobra(mapa)));
    }
    fclose(pFileSaida);
    fclose(pFileMovimentos);
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
    fclose(pFileHeatMap);

    return 0;
}

/* FUNCOES DO TIPO tCobra */

tCobra InicializaCobra(FILE *pFile, tCobra cobra, int xCabeca, int yCabeca){

    cobra.direcaoAtual = '>';
    cobra.colidiuComCorpo = 0;
    cobra.colidiuParede = 0;
    cobra.comeuComida = 0;
    cobra.cobraViva = 1;
    cobra.pegouDinheiro = 0;
    cobra.tamanho = 1;
    cobra.pontuacao = 0;

    cobra.PosCorpo[0][0] = xCabeca;
    cobra.PosCorpo[0][1] = yCabeca;

    fprintf(pFile, "A cobra comecara o jogo na linha %d e coluna %d", cobra.PosCorpo[0][1]+1, cobra.PosCorpo[0][0]+1);
    fclose(pFile);

    return cobra;
}

tCobra MoveCobra(tCobra cobra, char mov, int linhas, int colunas){
    int i, deltaX, deltaY;
    int aux1[2], aux2[2];
    switch (DirecaoDoMovimento(cobra, mov)){
        //MOVE COBRA PARA DIREITA
        case 1:
            deltaX = 1;
            deltaY = 0;
            cobra.direcaoAtual = '>';
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];
            //Move a cabeca da cobra
            cobra.PosCorpo[0][0] += deltaX;
            cobra.PosCorpo[0][1] += deltaY;
            if (cobra.PosCorpo[0][0] >= colunas){
                cobra.PosCorpo[0][0] = 0;
            }
            //Move corpo da cobra
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
            break;
        //MOVE COBRA PARA CIMA
        case 2:
            deltaX = 0;
            deltaY = -1;
            cobra.direcaoAtual = '^';
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];
            //Move a cabeca da cobra
            cobra.PosCorpo[0][0] += deltaX;
            cobra.PosCorpo[0][1] += deltaY;
            if (cobra.PosCorpo[0][1] < 0){
                cobra.PosCorpo[0][1] = linhas - 1;
            }
            //Move corpo da cobra
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
            break;
        //MOVE COBRA PARA ESQUERDA
        case 3:
            deltaX = -1;
            deltaY = 0;
            cobra.direcaoAtual = '<';
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];
            //Move a cabeca da cobra
            cobra.PosCorpo[0][0] += deltaX;
            cobra.PosCorpo[0][1] += deltaY;
            if (cobra.PosCorpo[0][0] < 0){
                cobra.PosCorpo[0][0] = colunas - 1;
            }
            //Move corpo da cobra
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
            break;
        //MOVE COBRA PARA BAIXO
        case 4:
            deltaX = 0;
            deltaY = 1;
            cobra.direcaoAtual = 'v';
            aux1[0] = cobra.PosCorpo[0][0];
            aux1[1] = cobra.PosCorpo[0][1];
            //Move a cabeca da cobra
            cobra.PosCorpo[0][0] += deltaX;
            cobra.PosCorpo[0][1] += deltaY;
            if (cobra.PosCorpo[0][1] >= linhas){
                cobra.PosCorpo[0][1] = 0;
            }
            //Move corpo da cobra
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
            break;
    }

    return cobra;
}

int DirecaoDoMovimento(tCobra cobra, char mov){
    switch (cobra.direcaoAtual){
        case '>':
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
        case '<':
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
        case 'v':
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
        case '^':
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

int TamanhoCobra(tCobra cobra){
    return cobra.tamanho;
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

/* FUNCOES DO TIPO tMapa */

tMapa LeMapaEInicializa(FILE *pFileOut, FILE *pFile, tMapa mapa){
    int i, j;
    
    mapa.qtdMovimentos = 0;
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

    //Inicializa cobra e fecha arquivo "Inicializacao.txt"
    mapa.cobra = InicializaCobra(pFileOut, mapa.cobra, ObtemPosXCabecaInicial(mapa), ObtemPosYCabecaInicial(mapa));

    return mapa;
}

tMapa InicializaMapaDeCalor(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == '>'){
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
            if (mapa.mapa[i][j] == '>' || mapa.mapa[i][j] == '<' || mapa.mapa[i][j] == 'v' || mapa.mapa[i][j] == '^'){
                return j;
            }
        }
    }
}

int ObtemPosYCabecaInicial(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == '>' || mapa.mapa[i][j] == '<' || mapa.mapa[i][j] == 'v' || mapa.mapa[i][j] == '^'){
                return i;
            }
        }
    }
}

tMapa MoveCobraNoMapa(FILE *pFile, tMapa mapa, char mov){
    int xCabeca, yCabeca;
    mapa.qtdMovimentos++;
    xCabeca = ObtemPosXCabeca(mapa.cobra);
    yCabeca = ObtemPosYCabeca(mapa.cobra);
    //Verificação de onde a cabeca da cobra estara indo (#, o, *, $) 
    switch (DirecaoDoMovimento(mapa.cobra, mov)){
        case 1:
            //Verifica se colidiu com a parede
            if (mapa.mapa[yCabeca][xCabeca+1] == '#'){
                mapa.cobra = MorreCobra(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
            }
            //Verifica se colidiu com o corpo
            if (mapa.mapa[yCabeca][xCabeca+1] == 'o'){
                if (!EhFimDaCobra(mapa.cobra, xCabeca+1, yCabeca)){
                    mapa.cobra = MorreCobra(mapa.cobra);
                    fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
                }
            }
            //Verifica se comeu comida
            if (mapa.mapa[yCabeca][xCabeca+1] == '*'){
                mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", mapa.qtdMovimentos, mov, TamanhoCobra(mapa.cobra));
            }
            //Verifica se pegou dinheiro
            if (mapa.mapa[yCabeca][xCabeca+1] == '$'){
                mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) gerou dinheiro\n", mapa.qtdMovimentos, mov);
            }
            break;
        case 2:
            //Verifica se colidiu com a parede
            if (mapa.mapa[yCabeca-1][xCabeca] == '#'){
                mapa.cobra = MorreCobra(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
            }
            //Verifica se colidiu com o corpo
            if (mapa.mapa[yCabeca-1][xCabeca] == 'o'){
                if (!EhFimDaCobra(mapa.cobra, xCabeca, yCabeca-1)){
                    mapa.cobra = MorreCobra(mapa.cobra);
                    fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
                }
            }
            //Verifica se comeu comida
            if (mapa.mapa[yCabeca-1][xCabeca] == '*'){
                mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", mapa.qtdMovimentos, mov, TamanhoCobra(mapa.cobra));
            }
            //Verifica se pegou dinheiro
            if (mapa.mapa[yCabeca-1][xCabeca] == '$'){
                mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) gerou dinheiro\n", mapa.qtdMovimentos, mov);
            }
            break;
        case 3:
            //Verifica se colidiu com a parede
            if (mapa.mapa[yCabeca][xCabeca-1] == '#'){
                mapa.cobra = MorreCobra(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
            }
            //Verifica se colidiu com o corpo
            if (mapa.mapa[yCabeca][xCabeca-1] == 'o'){
                if (!EhFimDaCobra(mapa.cobra, xCabeca-1, yCabeca)){
                    mapa.cobra = MorreCobra(mapa.cobra);
                    fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
                }
            }
            //Verifica se comeu comida
            if (mapa.mapa[yCabeca][xCabeca-1] == '*'){
                mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", mapa.qtdMovimentos, mov, TamanhoCobra(mapa.cobra));
            }
            //Verifica se pegou dinheiro
            if (mapa.mapa[yCabeca][xCabeca-1] == '$'){
                mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) gerou dinheiro\n", mapa.qtdMovimentos, mov);
            }
            break;
        case 4:
            //Verifica se colidiu com a parede
            if (mapa.mapa[yCabeca+1][xCabeca] == '#'){
                mapa.cobra = MorreCobra(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
            }
            //Verifica se colidiu com o corpo
            if (mapa.mapa[yCabeca+1][xCabeca] == 'o'){
                if (!EhFimDaCobra(mapa.cobra, xCabeca, yCabeca+1)){
                    mapa.cobra = MorreCobra(mapa.cobra);
                    fprintf(pFile, "Movimento %d (%c) resultou no fim do jogo por conta de colisao\n", mapa.qtdMovimentos, mov);
                }
            }
            //Verifica se comeu comida
            if (mapa.mapa[yCabeca+1][xCabeca] == '*'){
                mapa.cobra = AumentaTamanhoCobra(mapa.cobra);
                mapa.cobra = AumentaPontuacaoComida(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", mapa.qtdMovimentos, mov, TamanhoCobra(mapa.cobra));
            }
            //Verifica se pegou dinheiro
            if (mapa.mapa[yCabeca+1][xCabeca] == '$'){
                mapa.cobra = AumentaPontuacaoDinheiro(mapa.cobra);
                fprintf(pFile, "Movimento %d (%c) gerou dinheiro\n", mapa.qtdMovimentos, mov);
            }
            break;
    }
    
    mapa.cobra = MoveCobra(mapa.cobra, mov, mapa.linhas, mapa.colunas);

    mapa = RefrescaMapa(mapa);
    mapa = PrintaCobraNoMapa(mapa);

    return mapa;
}

tMapa RefrescaMapa(tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == '>' || mapa.mapa[i][j] == '<' || mapa.mapa[i][j] == 'v' || mapa.mapa[i][j] == '^' ||
                mapa.mapa[i][j] == 'o'){
                    mapa.mapa[i][j] = ' ';
                }
        }
    }
    return mapa;
}

tMapa PrintaCobraNoMapa(tMapa mapa){
    int i;

    if (CobraEstaViva(mapa.cobra)){
        for (i = 0; i < TamanhoCobra(mapa.cobra); i++){
            if (!i){
                mapa.mapa[ObtemPosYParteDoCorpo(mapa.cobra, i)][ObtemPosXParteDoCorpo(mapa.cobra, i)] = ObtemDirecaoAtualCobra(mapa.cobra);
            }
            else {
                mapa.mapa[ObtemPosYParteDoCorpo(mapa.cobra, i)][ObtemPosXParteDoCorpo(mapa.cobra, i)] = 'o';
            }
        }
    }
    else {
        for (i = 0; i < TamanhoCobra(mapa.cobra); i++){
            mapa.mapa[ObtemPosYParteDoCorpo(mapa.cobra, i)][ObtemPosXParteDoCorpo(mapa.cobra, i)] = 'X';
        }
    }
    return mapa;
}

tMapa AtualizaMapaDeCalor(tMapa mapa){
    mapa.mapaDeCalor[ObtemPosYCabecaInicial(mapa)][ObtemPosXCabecaInicial(mapa)] += 1;
    return mapa;
}

void FilePrintaMapaNaSaida(FILE *pFile, tMapa mapa, char mov){
    int i, j;

    fprintf(pFile, "Estado do jogo apos o movimento '%c':\n", mov);
    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            fprintf(pFile, "%c", mapa.mapa[i][j]);
        }
        fprintf(pFile, "\n");
    }
    fprintf(pFile, "Pontuacao: %d\n", ObtemPontuacaoDaCobra(mapa.cobra));
    fprintf(pFile, "\n");
    if (!CobraEstaViva(mapa.cobra)){
        fprintf(pFile, "Game over!\nPontuacao final: %d", ObtemPontuacaoDaCobra(mapa.cobra));
    }
}

int EhFimDeJogoCobraMorta(tMapa mapa){
    if (CobraEstaViva(mapa.cobra)){
        return 0;
    }
    return 1;
}

tCobra ObtemCobra(tMapa mapa){
    return mapa.cobra;
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
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            if (mapa.mapa[i][j] == '*'){
                return 1;
            }
        }
    }
    return 0;
}

void FilePrintaHeatMap(FILE *pFile, tMapa mapa){
    int i, j;

    for (i = 0; i < mapa.linhas; i++){
        for (j = 0; j < mapa.colunas; j++){
            fprintf(pFile, "%d ", mapa.mapaDeCalor[i][j]);
        }
        fprintf(pFile, "\n");
    }
}

/* FUNCOES DO TIPO tEstatistica */