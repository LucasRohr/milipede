#include "definicoes.h"
#include "colisao.h"
#include "fazendeiro.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Gerar aranha com atributos padrao via ponteiro
void gera_aranha(ARANHA *aranha) {
    float x_random = (float) (rand() / (double) RAND_MAX * (LARGURA_TELA - (2 * MARGEM_JOGO_X)));

    aranha->posicao.x = x_random;
    aranha->posicao.y = MARGEM_JOGO_Y + TAMANHO_ARANHA;
    aranha->status = 1;
    aranha->dir =  (int) (rand() % (esq - dir + 1)) + dir_baixo;
}

// Funcao para mudar a direcao de movimento da aranha caso a mesma colida
// com algum extremo da tela que não seja o fim dela
void inverte_movimento(ARANHA *aranha) {
    DIRECAO direcao_aranha = aranha->dir;

    switch(direcao_aranha) {
        case(cima):
            aranha->dir = GetRandomValue(esq_baixo, dir_baixo);
            break;
        case(dir_cima):
            aranha->dir = GetRandomValue(esq_cima, dir_baixo);
            break;
        case(dir):
            aranha->dir = GetRandomValue(esq_cima, esq_baixo);
            break;
        case(dir_baixo):
            aranha->dir = GetRandomValue(esq_cima, esq_baixo);
            break;
        case(baixo):
            aranha->dir = esq_cima;
            break;
        case(esq_baixo):
            aranha->dir = GetRandomValue(dir_cima, dir_baixo);
            break;
        case(esq):
            aranha->dir = GetRandomValue(dir_baixo, dir_cima);
            break;
        case(esq_cima):
            aranha->dir = GetRandomValue(dir_cima, esq_baixo);
            break;
        default:
            break;
    }
}

// Verifica a colisao para cada aranha
void verifica_colisao_aranha_cogumelos(ARANHA aranha, COGUMELO cogumelos[], int num_cogumelos){
    int i = 0;

    for(i = 0; i < num_cogumelos; i++) {
        if(aranha.status == 1 && verifica_colisao(aranha.posicao, TAMANHO_ARANHA, cogumelos[i].posicao, TAMANHO_COGUMELO) ) {
            cogumelos[i].status = 0;
        }
    }
}

// Verifica se eh possivel a aranha fazer algum movimento. Recebe a nova posicao, nao a atual.
// Retorna 1 se colide com um obstaculo, 0 se nao
int verifica_movimento_aranha(COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro) {
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

// Movimento individualmente uma aranha, testando colisao com cogumelos para come-los
// Tambem verifica colisoes de movimento para mudar trajetoria da aranha
void move_aranha(ARANHA *aranha, COGUMELO cogumelos[], int num_cogumelos, FAZENDEIRO fazendeiro) {
    DIRECAO direcao_aranha = aranha->dir;
    COORD nova_posicao = {};

    verifica_colisao_aranha_cogumelos(*aranha, cogumelos, num_cogumelos);

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

// Elimina a aranha se a mesma bate na base da tela
void testa_colisao_aranha_base(ARANHA *aranha) {
    if (aranha->posicao.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_ARANHA)) {
        aranha->status = 0;
    }
}

// Verifica se a aranha colidiu com o fazendeiro, matando ele caso ja esteja paralisado
// ou paralisando o mesmo e demandando comer cogumelos caso nao esteja
void verifica_colisao_aranha_fazendeiro(FAZENDEIRO *fazendeiro, ARANHA aranha, STATUS_JOGO *status_jogo) {
    int colidiu_com_fazendeiro = 0;

    colidiu_com_fazendeiro += verifica_colisao(aranha.posicao, TAMANHO_ARANHA, fazendeiro->posicao, TAMANHO_JOGADOR);

    if (colidiu_com_fazendeiro && fazendeiro->contador_invulneravel == 0 && aranha.status) { // Se uma aranha viva colidir com o fazendeiro, e ele estiver vulneravel
        if (fazendeiro->doente){ // Se ja estiver doente, morre
            fazendeiro_morre(fazendeiro, status_jogo);
        // Se nao, paralisa o fazendeiro, deixa ele invulneravel por algum tempo e doente
        } else {
            fazendeiro->doente = NUM_COGUMELOS_CURAR_ARANHA;
            fazendeiro->contador_paralisado = TEMPO_PARALISIA * FRAMERATE;
            fazendeiro->contador_invulneravel = TEMPO_INVULNERAVEL * FRAMERATE;
            fazendeiro->contador_doente = TEMPO_DOENTE * FRAMERATE;
            fazendeiro->status = paralisado;
        }
    }
}

// Movimenta todas as aranhas no vetor de aranhas
// Verifica a colisao com o fazendeiro e base a cada volta do laco
void move_aranhas(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], CONFIG_FASE config_fase, STATUS_JOGO *status_jogo) {
    int i;

    for(i = 0; i < config_fase.num_aranhas; i++) {
        move_aranha(&(aranhas[i]), cogumelos, config_fase.num_cogumelos, *fazendeiro);
        verifica_colisao_aranha_fazendeiro(fazendeiro, aranhas[i], status_jogo);
        testa_colisao_aranha_base(&(aranhas[i]));
    }
}

// Preenche lista de aranhas
void gera_todas_aranhas(ARANHA aranhas[], int total_aranhas) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        gera_aranha(&(aranhas[i]));
    }
}
