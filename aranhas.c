#include "definicoes.h"
#include "colisao.h"
#include "fazendeiro.h"
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
    COORD nova_posicao = {};

    verifica_colisao_aranha_cogumelos(*aranha, cogumelos, NUM_COGUMELOS);

    switch(direcao_aranha) {
        case cima:
            nova_posicao.y = aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case dir_cima:
            nova_posicao.x = aranha->posicao.x += PASSO_ARANHA;
            nova_posicao.y = aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case dir:
            nova_posicao.x = aranha->posicao.x += PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case dir_baixo:
            nova_posicao.x = aranha->posicao.x += PASSO_ARANHA;
            nova_posicao.y = aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case baixo:
            nova_posicao.y = aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case esq_baixo:
            nova_posicao.x = aranha->posicao.x -= PASSO_ARANHA;
            nova_posicao.y = aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case esq:
            nova_posicao.x = aranha->posicao.x -= PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
            }

            break;
        case esq_cima:
            nova_posicao.x = aranha->posicao.x -= PASSO_ARANHA;
            nova_posicao.y = aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(nova_posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            } else {
                aranha->posicao = nova_posicao;
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

void move_aranhas(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int total_aranhas, STATUS_JOGO *status_jogo) {
    int i, colidiu_com_fazendeiro;

    for(i = 0; i < total_aranhas; i++) {
        colidiu_com_fazendeiro = 0;
        move_aranha(&(aranhas[i]), cogumelos, *fazendeiro);

        colidiu_com_fazendeiro += verifica_colisao(aranhas[i].posicao, TAMANHO_ARANHA, fazendeiro->posicao, TAMANHO_JOGADOR);

        if (colidiu_com_fazendeiro && fazendeiro->contador_invulneravel == 0 && aranhas[i].status) { // Se uma aranha viva colidir com o fazendeiro, e ele estiver vulneravel
            if (fazendeiro->doente){ // Se ja estiver doente, morre
                fazendeiro_morre(fazendeiro, status_jogo);
            } else { // Se nao, paralisa o fazendeiro, deixa ele invulneravel por algum tempo e doente
                fazendeiro->doente = NUM_COGUMELOS_CURAR_ARANHA;
                fazendeiro->contador_paralisado = TEMPO_PARALISIA * FRAMERATE;
                fazendeiro->contador_invulneravel = TEMPO_INVULNERAVEL * FRAMERATE;
                fazendeiro->contador_doente = TEMPO_DOENTE * FRAMERATE;
                fazendeiro->status = paralisado;
            }
        }

        testa_colisao_aranha_base(&(aranhas[i]));

    }
}

void gera_todas_aranhas(ARANHA aranhas[], int total_aranhas) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        gera_aranha(&(aranhas[i]));
    }
}
