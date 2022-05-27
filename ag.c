#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// funcoes
int PRIMO_PROXIMO(int x)
{
    while (1)
    {
        x++;
        for (int j = 2; j < x; j++)
        {
            if (x % j == 0)
            {
                break;
            }
            if (j == x - 1)
            {
                return x;
            }
        }
    }
}

int MMC(int a, int b)
{
    int x = 2, mmc = 1;
    while (a != 1 || b != 1)
    {
        if (a % x == 0 || b % x == 0)
        {
            mmc *= x;
        }
        if (a % x == 0)
        {
            a /= x;
        }
        if (b % x == 0)
        {
            b /= x;
        }
        if (a % x != 0 && b % x != 0)
        {
            x = PRIMO_PROXIMO(x);
        }
    }
    return mmc;
}

int main()
{
    // inputs e inicializacao
    int tamanho_mapa, tamanho_spawn, tamanho_individuo, tamanho_populacao = 120, geracoes = 8;
    int top_quantos = 10;
    long long int melhores_individuos[top_quantos][tamanho_individuo];
    int melhores_desempenhos[top_quantos];

    printf("Digite o tamanho do lado do mapa:\n>>> ");
    scanf("%d", &tamanho_mapa);

    // gerar mapa
    int mapa[tamanho_mapa][tamanho_mapa];

    srand(0);

    for (int i = 0; i < tamanho_mapa; i++)
    {
        for (int j = 0; j < tamanho_mapa; j++)
        {
            mapa[i][j] = rand() % 24; // 24 hora em hora, 48 meia em meia hora, 168 semanal

            if (mapa[i][j] < 10)
            {
                printf("0");
            }
            printf("%d ", mapa[i][j]);
        }
        printf("\n");
    }
    // partida e chegada
    int x_partida = tamanho_mapa / 3, y_partida = tamanho_mapa / 2, x_chegada = tamanho_mapa * 2 / 3, y_chegada = tamanho_mapa / 2;
    printf(" xi: %d\n yi: %d\n xf:%d\n yf: %d\n valor_partida: %d\n valor_chegada: %d\n", x_partida, y_partida, x_chegada, y_chegada, mapa[x_partida][y_partida], mapa[x_chegada][y_chegada]);

    // gerar populacao
    tamanho_individuo = tamanho_mapa / 16; // cada passo sao 2 bits, e o tamanho_spawn deve ser totalmente acessivel, portanto, a cada 8 de tamanho nos lados do tamanho_spawn, eh necessario +1 long long int no vetor individuo
    tamanho_individuo++;

    unsigned long long int populacao[tamanho_populacao][tamanho_individuo];

    for (int i = 0; i < tamanho_populacao; i++)
    {
        for (int j = 0; j < tamanho_individuo; j++)
        {
            populacao[i][j] = rand(); // geracao inicial de individuo
            // printf ("%lld ", populacao[i][j]);
        }
        // printf ("\n");
    }

    // treino
    for (int geracao = 0; geracao < geracoes; geracao++)
    {
        float vetor_desempenho[tamanho_populacao];
        // simulacao
        for (int i = 0; i < tamanho_populacao; i++)
        {

            int x_posicao = x_partida, y_posicao = y_partida;
            int duracao_viagem = 0, distancia_chegada = 0, ja_chegou = 0;
            float desempenho = 0;

            for (int j = 0; j < tamanho_individuo; j++)
            {
                if (ja_chegou == 1)
                {
                    break;
                }
                unsigned long long int mascara = 3;
                for (int k = 0; k < 32; k++)
                {
                    int passo, estacao_atual, estacao_seguinte;
                    passo = populacao[i][j] & mascara;
                    mascara = mascara << 2;
                    estacao_atual = mapa[x_posicao][y_posicao];
                    if (passo >> k * 2 == 0)
                    {
                        y_posicao++;
                    }
                    else if (passo >> k * 2 == 1)
                    {
                        x_posicao++;
                    }
                    else if (passo >> k * 2 == 2)
                    {
                        y_posicao--;
                    }
                    else if (passo >> k * 2 == 3)
                    {
                        x_posicao--;
                    }
                    else
                    {
                        printf("error");
                        break;
                    }
                    estacao_seguinte = mapa[x_posicao][y_posicao];
                    duracao_viagem += MMC(estacao_atual, estacao_seguinte); // condicao pre-estabelecida para medida de desempenho (no caso o tempo de viagem total)

                    if (x_posicao == x_chegada && y_posicao == y_chegada)
                    { // o individuo chegou ao destino
                        desempenho = (float)1 / duracao_viagem;
                        ja_chegou = 1;
                        break;
                    }
                    if (j == tamanho_individuo - 1 && k == 31 && (x_posicao != x_chegada || y_posicao != y_chegada))
                    { // caso em que o individuo deu todos os passos e nao chegou na chegada; nesse caso o desempenho eh dado pela distancia euclidiana
                        desempenho =
                            (float)1 / duracao_viagem - (sqrt(pow((x_chegada - x_posicao), 2) + pow((y_chegada - y_posicao), 2)));
                    }
                }
                vetor_desempenho[i] = desempenho;
            }
        }
        // bubble sort

        for (int i = tamanho_populacao - 1; i > 0; i--)
        {
            long long int individuo_1[tamanho_individuo],
                individuo_2[tamanho_individuo];
            for (int j = 0; j < tamanho_individuo; j++)
            { // copia duas linhas da matriz para dois vetores
                individuo_1[j] = populacao[i][j];
                individuo_2[j] = populacao[i + 1][j];
            }
            for (int j = 0; j < i; j++)
            {
                if (vetor_desempenho[j] < vetor_desempenho[j + 1])
                {
                    vetor_desempenho[j] = vetor_desempenho[j] + vetor_desempenho[j + 1];
                    vetor_desempenho[j + 1] = vetor_desempenho[j] - vetor_desempenho[j + 1];
                    vetor_desempenho[j] = vetor_desempenho[j] - vetor_desempenho[j + 1];
                    for (int k = 0; k < tamanho_individuo; k++)
                    { // copia dois vetores para duas linhas da matriz
                        populacao[i][k] = individuo_2[k];
                        populacao[i + 1][k] = individuo_1[k];
                    }
                }
            }
        }
        // for(int i=0; i<tamanho_populacao; i++)
        // printf("%d\n",v[i]);
        // }

        // armazenamento dos melhores individuo de todas as geracoes
        for (int i = top_quantos - 1; i > 0; i--)
        {
            if (vetor_desempenho[i] > melhores_desempenhos[i])
            {
                vetor_desempenho[i] = melhores_desempenhos[i];
                for (int j = 0; j < tamanho_individuo; j++)
                {
                    melhores_individuos[i][j] = populacao[i][j];
                }
            }
        }

        // cruzamento
        for (int i = tamanho_populacao / 3; i < tamanho_populacao; i++)
        { // o criterio para cruzamento entre individuos pode ser descrito como: a populacao total eh dividadida em 3 partes, sendo 1/3, os individuos com melhor desempenho, e os 2/3 restantes, os piores. O 2/3 cruzaram com o 1/3 e serao eliminados, dando espaco para a prole.
            for (int j = 0; j < tamanho_individuo; j++)
            {
                populacao[tamanho_populacao - i * 2 - 1][j] = populacao[tamanho_populacao - i * 2 - 1][j] & 4294967295 | populacao[i][j] & 18446744069414584320; // os numeros sao mascaras que pegam a primeira e segunda metades dos bits
                populacao[tamanho_populacao - i * 2 - 2][j] = populacao[tamanho_populacao - i * 2 - 2][j] & 4294967295 | populacao[i][j] & 18446744069414584320;
            }
        }
        // mutacao
        for (int i = 0; i < tamanho_populacao; i++)
        {
            for (int j = 0; j < tamanho_individuo; j++)
            {
                for (int k = 0; k < 32; k++)
                {
                    int chance_mutacao = 10;
                    if (rand() % chance_mutacao == 0)
                    {
                        populacao[i][j] = populacao[i][j] ^ 3 << 2 * k;
                    }
                }
            }
        }
    }

    // display
    for (int i = 0; i < top_quantos; i++)
    {
        for (int j = 0; j < tamanho_individuo; j++)
        {
            printf("\nDesempenho :%d\n Individuo: %lld", melhores_desempenhos[i], melhores_individuos[i][j]);
        }
    }

    return 0;
}