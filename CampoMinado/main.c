#include <stdio.h>
#include <stdlib.h>

#define LINHAS 9
#define COLUNAS 9

struct matriz {
        char tabChar[LINHAS][COLUNAS];
        char tabCharJogadas[LINHAS][COLUNAS];
        int  tabInt[LINHAS][COLUNAS];
    };
    typedef struct matriz tabuleiro;

int main()
{
    time_t t;
    srand((unsigned)time(&t));

    jogarCampoMinado();

    //testeConverterLinhaColunha();
    return 0;
}

void jogarCampoMinado()
{
    char resposta;
    printf("Deseja jogar? y/n\n");
    resposta = getchar();
    while(resposta == 'y')
    {
        printf("Numero de minas no tabuleiro: ");
        int numDeMinas;
        scanf("%d", &numDeMinas);
        printf("\n");

        tabuleiro tab;
        tabuleiro *tabPointer = &tab;

        inicializarTabuleiros(tabPointer);
        criarTabuleiro(numDeMinas, tabPointer);
        printf("\n\n");
        mostrarTabuleiros(tabPointer);
        printf("\n\n\n");
        printf("\n\n");

        printf("-----------NOVO JOGO-----------\n\n");
        mostrarTabuleiroDasJogadas(tabPointer);

        int perdeu = 0;
        while(perdeu == 0)
        {
            printf("Escolha um indice: ");
            int jogada;
            scanf("%d", &jogada);
            int linha = converterEmLinha(jogada);
            int coluna = converterEmColuna(jogada);
            if(tabPointer->tabChar[linha][coluna] == '*')
            {
                printf("\nVoce explodiu!\n");
                perdeu = 1;
            }
            else
            {
                if( (jogada < 0) || (jogada > LINHAS*COLUNAS) )
                    printf("\nJogada invalida.\n");
                else if(tabPointer->tabCharJogadas[linha][coluna] != '.')
                    printf("\nVoce ja jogou nesta posicao.\n");
                else
                {
                    abreVizinhos(linha, coluna, tabPointer);
                    if(indicesDisponiveis(tabPointer) == 0)
                    {
                        printf("\nVoce venceu!!!\n\n");
                        break;
                    }
                    else
                    {
                        mostrarTabuleiroDasJogadas(tabPointer);
                    }
                }
            }
            printf("-----\n");
        }
        printf("\nJogar novamente? y/n\n");
        getchar();
        resposta = getchar();
    }
}

int sortearSemRep(int arrayLinear[LINHAS*COLUNAS], int tamanhoTabuleiro)
{
    int indiceSorteado = rand()%tamanhoTabuleiro;
    // Esse é o valor que será passado
    int valorNoIndice = arrayLinear[indiceSorteado];
    // Faço a troca para não repetir valores
    int aux = arrayLinear[indiceSorteado];
    arrayLinear[indiceSorteado] = arrayLinear[tamanhoTabuleiro-1];
    arrayLinear[tamanhoTabuleiro-1] = aux;
    return valorNoIndice;
}

void criarTabuleiro(int numMinas, tabuleiro *tab)
{
    int tamanhoDoTabuleiro = LINHAS*COLUNAS;
    int tabuleiroLinear[LINHAS*COLUNAS];
    inicializarArrayLinear(tabuleiroLinear);

    printf("Posicoes das minas sorteadas\n");
    int i;
    for(i = 0; i < numMinas; i++)
    {
        int valorDoSorteio = sortearSemRep(tabuleiroLinear, tamanhoDoTabuleiro);
        int linha = converterEmLinha(valorDoSorteio);
        int coluna = converterEmColuna(valorDoSorteio);
        tab->tabChar[linha][coluna] = '*';
        somarEmVolta(linha, coluna, tab);
        printf("Indice %d | [%d][%d] | Valor no array linear: %d | Tamanho do tab: %d\n", valorDoSorteio, linha, coluna, tabuleiroLinear[valorDoSorteio], tamanhoDoTabuleiro);
        tamanhoDoTabuleiro--;
    }
}

void abreVizinhos(int linha, int coluna, tabuleiro *tab)
{
    int i,j;
    for(i = linha-1; i <= linha+1; i++)
    {
        for(j = coluna-1; j <= coluna+1; j++)
        {
            if(tab->tabChar[i][j] == '.')
            {
                if( (i >= 0 && i < LINHAS) && (j >= 0 && j < COLUNAS) )
                    {
                        tab->tabCharJogadas[i][j] = tab->tabInt[i][j] + '0';
                        tab->tabChar[i][j] = ',';
                    }
            }
        }
    }
}

void somarEmVolta(int linha, int coluna, tabuleiro *tab)
{
    int i,j;
    for(i = linha-1; i <= linha+1; i++)
    {
        for(j = coluna-1; j <= coluna+1; j++)
        {
            if( !((i == linha) && (j == coluna)) )
            {
                if( (i >= 0 && i < LINHAS) && (j >= 0 && j < COLUNAS) )
                    {
                        tab->tabInt[i][j]++;
                    }
            }
        }
    }
}

void mostrarTabuleiroDasJogadas(tabuleiro *tab)
{
    int i,j;

    printf("\nIndices disponiveis: ");
    for(i = 0; i < LINHAS*COLUNAS; i++)
        if(tab->tabCharJogadas[converterEmLinha(i)][converterEmColuna(i)] == '.')
            printf("%d ", i);

    printf("\n\n");

    printf("Tabuleiro\n");
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            printf("%c", tab->tabCharJogadas[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int indicesDisponiveis(tabuleiro *tab)
{
    int i;
    int contador = 0;
    for(i = 0; i < LINHAS*COLUNAS; i++)
    {
        if(tab->tabChar[converterEmLinha(i)][converterEmColuna(i)] == '.')
        {
            contador++;
        }
    }
    return contador;
}

void mostrarTabuleiros(tabuleiro *tab)
{
    int i,j;
    printf("Tabuleiro char\n");
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            printf("%c", tab->tabChar[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");

    printf("Tabuleiro int\n");
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            printf("%d", tab->tabInt[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void inicializarTabuleiros(tabuleiro *tab)
{
    int i,j;
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            tab->tabChar[i][j] = '.';
            tab->tabCharJogadas[i][j] = '.';
            tab->tabInt[i][j] = 0;
        }
    }
}

void inicializarArrayLinear(int array[LINHAS*COLUNAS])
{
    int i;
    for(i = 0; i < LINHAS*COLUNAS; i++)
    {
        array[i] = i;
    }
}

int converterEmLinha(int indice)
{
    if(indice == 0)
    {
        return 0;
    }
    else if(indice < COLUNAS)
    {
        return 0;
    }
    else
    {
        return indice/COLUNAS;
    }
}

int converterEmColuna(int indice)
{
    if(indice == 0)
    {
        return 0;
    }
    else if(indice < COLUNAS)
    {
        return indice;
    }
    else
    {
        return (indice%COLUNAS);
    }
}

// metodo pra ver se esta convertendo corretamente
void testeConverterLinhaColunha()
{
    int arrayTeste[LINHAS][COLUNAS];
    int contador = 0;
    int i,j;

    // incializa
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            arrayTeste[i][j] = contador;
            contador++;
        }
    }

    // testa conversoes
    contador = 0;
    int numeroDeErros = 0;
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            if( !((i == converterEmLinha(contador)) && (j == converterEmColuna(contador))) )
            {
                printf("Conversao FALHOU no elemento %d\nPosicao que deveria estar no array [%d][%d] | Posicao que esta [%d][%d]\n", contador, i,j, converterEmLinha(contador),converterEmColuna(contador));
                numeroDeErros++;
            }
            else
            {
                printf("Conversao BEM SUCEDIDA do elemento %d\nPosicao que esta [%d][%d]\n", contador, converterEmLinha(contador),converterEmColuna(contador));
            }
            printf("\n");
            contador++;
        }
    }

    printf("\n\n");

    printf("Numero de FALHAS: %d", numeroDeErros);

    printf("\n\n");

    // mostra o array
    for(i = 0; i < LINHAS; i++)
    {
        for(j = 0; j < COLUNAS; j++)
        {
            printf("%d ", arrayTeste[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

    char resposta;
    do
    {
        printf("Indice: ");
        int indiceTest;
        scanf("%d", &indiceTest);

        printf("\nConvertido em linha: %d | Convertido em coluna: %d\n\n", converterEmLinha(indiceTest), converterEmColuna(indiceTest));

        printf("Testar mais indices? y/n\n");
        getchar();
        resposta = getchar();

        printf("\n\n");

    } while(resposta == 'y');
}
