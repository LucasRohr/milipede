#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "definicoes.h"
#include "colisao.h"

// Gera uma posica aleatoria para os cogumelos. E divisivel pelo tamanho dos cogumelos, para ocuparem posicoes como em uma grade
int gera_posicao_cogumelos(int valor_minimo, int valor_maximo) {
    // Gera valores aleatorios, divisiveis pelo tamanho dos cogumelos + sua margem (para eles ficarem em seus proprios tiles)
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo) / (TAMANHO_COGUMELO + MARGEM_COGUMELO) * (TAMANHO_COGUMELO + MARGEM_COGUMELO);
}

// Verifica se a posicao gerada para um cogumelo ja esta ocupada. Devolve 0 se nao.
int verifica_posicao_cogumelos(COGUMELO cogumelos[], int num_cogumelos, int x, int y){
    // Se as coordenadas recebidas j� est�o ocupadas por um cogumelo, retorna 1, sen�o 0. Para de percorrer o array quando chega ao final, ou encontra o valor 0.
    int i = 0, flag = 0;

    while(i < num_cogumelos && cogumelos[i].posicao.x != 0){
        if (cogumelos[i].posicao.x == x && cogumelos[i].posicao.y == y){
            flag = 1;
        }
        i++;
    }
    return flag;
}

// Gera cogumelos em posiçoes aleatórias
void gera_cogumelos(COGUMELO cogumelos[], int num_cogumelos) {
    int i = 0, x, y;
    srand(time(0));

    while (i < num_cogumelos){
        x = gera_posicao_cogumelos(MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA + TAMANHO_COGUMELO, LARGURA_TELA - MARGEM_JOGO_X - DIMENSAO_RETANGULO_BORDA);
        y = gera_posicao_cogumelos(MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA + TAMANHO_JOGADOR * 2, ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR * 2);
        // Verifica se a posicao ja esta ocupada. Se nao, gera uma nova posicao para o cogumelo.

        if (!verifica_posicao_cogumelos(cogumelos, num_cogumelos, x, y)){
            cogumelos[i].posicao.x = x;
            cogumelos[i].posicao.y = y;
            cogumelos[i].status = 1;
            i++;
        }
    }
}

// Devolve o numero de cogumelos ainda no mapa.
int conta_cogumelos_restantes(COGUMELO cogumelos[], int num_cogumelos) {
    int i, contador = 0;

    for(i = 0; i < num_cogumelos; i++) {
        if (cogumelos[i].status == 1) {
            contador++;
        }
    }

    return contador;
}
