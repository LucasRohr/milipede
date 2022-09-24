#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "definicoes.h"
#include "colisao.h"

int gera_posicao_cogumelos(int valor_minimo, int valor_maximo) {
    // Gera valores aleatorios, divisiveis pelo tamanho dos cogumelos + sua margem (para eles ficarem em seus proprios tiles)
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo) / (TAMANHO_COGUMELO + MARGEM_COGUMELO) * (TAMANHO_COGUMELO + MARGEM_COGUMELO);
}

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


void gera_cogumelos(COGUMELO cogumelos[], int num_cogumelos) {
    // Gera cogumelos em posicoes aleatorias.
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

int verifica_colisao_cogumelos(COORD posicao, int tamanho_objeto, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0, flag = 0;

    for(i = 0; i < num_cogumelos; i++){
        if (verifica_colisao(posicao, tamanho_objeto, cogumelos[i].posicao, TAMANHO_COGUMELO) && cogumelos[i].status){
            flag += verifica_colisao(posicao, tamanho_objeto, cogumelos[i].posicao, TAMANHO_COGUMELO);
        }
    }
    return flag;
}

void acertou_cogumelo(COORD posicao, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0;

    for(i = 0; i < num_cogumelos; i++){
        if (verifica_colisao(posicao, TAMANHO_TIRO, cogumelos[i].posicao, TAMANHO_COGUMELO) && cogumelos[i].status){
            cogumelos[i].status = 0;
        }
    }
}

int conta_cogumelos_restantes(COGUMELO cogumelos[], int num_cogumelos) {
    int i, contador = 0;

    for(i = 0; i < num_cogumelos; i++) {
        if (cogumelos[i].status == 1) {
            contador++;
        }
    }

    return contador;
}
