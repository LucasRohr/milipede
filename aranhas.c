#include "definicoes.h"
#include "colisao.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


void gera_aranha(ARANHA *aranha) {
    float x_random = (float) (rand() / (double) RAND_MAX * (LARGURA_TELA - (2 * MARGEM_JOGO_X)));

    aranha->posicao.x = x_random;
    aranha->posicao.y = MARGEM_JOGO_Y + TAMANHO_ARANHA;
    aranha->status = 1;
    aranha->dir =  (int) (rand() % (esq - dir + 1)) + dir_baixo;
}

void inverte_movimento(ARANHA *aranha) {
    DIRECAO direcao_aranha = aranha->dir;

    switch(direcao_aranha) {
        case(cima):
            aranha->dir = (int) (rand() % (esq_baixo - dir_baixo + 1)) + dir_baixo;
            break;
        case(dir_cima):
            aranha->dir = (int) (rand() % (esq_cima - dir_baixo + 1)) + dir_baixo;
            break;
        case(dir):
            aranha->dir = (int) (rand() % (esq_cima - esq_baixo + 1)) + esq_baixo;
            break;
        case(dir_baixo):
            aranha->dir = (int) (rand() % (esq_cima - esq_baixo + 1)) + esq_baixo;
            break;
        case(baixo):
            aranha->dir = esq_cima;
            break;
        case(esq_baixo):
            aranha->dir = (int) (rand() % (dir_baixo - cima + 1)) + cima;
            break;
        case(esq):
            aranha->dir = (int) (rand() % (dir_baixo - dir_cima + 1)) + dir_cima;
            break;
        case(esq_cima):
            aranha->dir = (int) (rand() % (esq_baixo - dir_cima + 1)) + dir_cima;
            break;
        default:
            break;
    }
}

// Verifica a colisao para cada cogumelo.
void verifica_colisao_aranha_cogumelos(ARANHA aranha, COGUMELO cogumelos[], int num_cogumelos){
    int i = 0;

    for(i = 0; i < num_cogumelos; i++) {
        if(aranha.status == 1 && verifica_colisao(aranha.posicao, TAMANHO_ARANHA, cogumelos[i].posicao, TAMANHO_COGUMELO) ) {
            cogumelos[i].status = 0;
        }
    }
}

int verifica_movimento_aranha(COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro) {
    // Verifica se é possível a aranha fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se colide com um obstaculo, 0 se não
    int flag = 0;

    if(posicao.x >= LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_ARANHA) {
        flag = 1; // Verifica se a aranha colide com a parede da direita
    } else if(posicao.x <= MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 1; // Verifica se a aranha colide com a parede da esquerda
    } else if(posicao.y <= MARGEM_JOGO_Y) {
        flag = 1; // Verifica se a aranha vai acima do topo da tela
    } else if(posicao.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_ARANHA)) {
        flag = 1; // Verifica se a aranha colide a parede de baixo
    } else if(verifica_colisao(posicao, TAMANHO_ARANHA, fazendeiro.posicao, TAMANHO_ARANHA)) {
        flag = 1; // Verifica se a aranha colide com o fazendeiro
    }

    return flag;
}

void move_aranha(ARANHA *aranha, COGUMELO cogumelos[], FAZENDEIRO fazendeiro) {
    DIRECAO direcao_aranha = aranha->dir;

    verifica_colisao_aranha_cogumelos(*aranha, cogumelos, NUM_COGUMELOS);

    switch(direcao_aranha) {
        case cima:
            aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case dir_cima:
            aranha->posicao.x += PASSO_ARANHA;
            aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case dir:
            aranha->posicao.x += PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case dir_baixo:
            aranha->posicao.x += PASSO_ARANHA;
            aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case baixo:
            aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case esq_baixo:
            aranha->posicao.x -= PASSO_ARANHA;
            aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case esq:
            aranha->posicao.x -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case esq_cima:
            aranha->posicao.y += PASSO_ARANHA;
            aranha->posicao.x -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        default:
            break;
    }
}

void testa_colisao_aranha_base(ARANHA *aranha) {
    if (aranha->posicao.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_ARANHA)) {
        aranha->status = 0;
    }
}

void move_aranhas(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int total_aranhas) {
    int i, colidiu_com_fazendeiro;

    for(i = 0; i < total_aranhas; i++) {
        move_aranha(&(aranhas[i]), cogumelos, *fazendeiro);

        colidiu_com_fazendeiro = verifica_colisao(aranhas[i].posicao, TAMANHO_ARANHA, fazendeiro->posicao, TAMANHO_JOGADOR);
        fazendeiro->doente += colidiu_com_fazendeiro;

        if (colidiu_com_fazendeiro) {
            fazendeiro->status = paralisado;
            fazendeiro->vidas -= 1;

            WaitTime(3);

            // fazendeiro->status = livre;
        }

        testa_colisao_aranha_base(&(aranhas[i]));

        WaitTime(1);
    }
}

void gera_todas_aranhas(ARANHA aranhas[], int total_aranhas) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        gera_aranha(&(aranhas[i]));
    }
}
