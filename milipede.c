#include "definicoes.h"
#include "colisao.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void gera_milipede(MILIPEDE *milipede) {
    int tamanho_milipede = (int) (rand() % (TAMANHO_MAX_MILIPEDE - TAMANHO_MIN_MILIPEDE + 1));

    float x_random = (float) (rand() / (double) RAND_MAX * (LARGURA_TELA - (2 * MARGEM_JOGO_X) - (tamanho_milipede * TAMANHO_SEGMENTO_MILIPEDE)));

    milipede->tamanho = tamanho_milipede;
    milipede->posicao_cabeca.x = x_random;
    milipede->posicao_cabeca.y = MARGEM_JOGO_Y + TAMANHO_SEGMENTO_MILIPEDE;
    milipede->status = 1;
    milipede->dir =  (int) (rand() % (dir_mili - esq_mili + 1));
}

void inverte_movimento_milipede(MILIPEDE *milipede) {
    DIRECAO direcao_milipede = milipede->dir;

    if (direcao_milipede == esq_mili) {
        milipede->dir = dir_mili;
        milipede->posicao_cabeca.x = TAMANHO_SEGMENTO_MILIPEDE + (milipede->tamanho * TAMANHO_SEGMENTO_MILIPEDE);
    } else {
        milipede->dir = esq_mili;
        milipede->posicao_cabeca.x = LARGURA_TELA - ((milipede->tamanho * TAMANHO_SEGMENTO_MILIPEDE) + TAMANHO_SEGMENTO_MILIPEDE);
    }

    milipede->posicao_cabeca.y += PASSO_VERTICAL_MILIPEDE;
}

// Verifica a colisao para cada cogumelo.
void verifica_colisao_milipede_cogumelos(MILIPEDE *milipede, COGUMELO cogumelos[], int num_cogumelos) {
    int i = 0;

    for(i = 0; i < num_cogumelos; i++) {
        if(milipede->status == 1 && cogumelos[i].status == 1 && verifica_colisao(milipede->posicao_cabeca, TAMANHO_SEGMENTO_MILIPEDE, cogumelos[i].posicao, TAMANHO_COGUMELO) ) {
            cogumelos[i].status = 0;

            if (milipede->tamanho < TAMANHO_MAX_MILIPEDE) {
                milipede->tamanho += 1;
            }

            milipede->posicao_cabeca.y += PASSO_VERTICAL_MILIPEDE;
        }
    }
}

int verifica_movimento_milipede(int segmentos_milipede, COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro) {
    // Verifica se é possível a milipede fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se colide com um obstaculo, 0 se não
    int flag = 0;
    // int tamanho_milipede = TAMANHO_SEGMENTO_MILIPEDE * segmentos_milipede;

    if(posicao.x >= LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_SEGMENTO_MILIPEDE) {
        flag = 1; // Verifica se a milipede colide com a parede da direita
    } else if(posicao.x <= MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 1; // Verifica se a milipede colide com a parede da esquerda
    } else if(posicao.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_SEGMENTO_MILIPEDE)) {
        flag = 1; // Verifica se a milipede colide a parede de baixo
    } else if(verifica_colisao(posicao, TAMANHO_SEGMENTO_MILIPEDE, fazendeiro.posicao, TAMANHO_SEGMENTO_MILIPEDE)) {
        flag = 1; // Verifica se a milipede colide com o fazendeiro
    }

    return flag;
}

void testa_colisao_milipede_base(MILIPEDE *milipede) {
    if (milipede->posicao_cabeca.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_SEGMENTO_MILIPEDE)) {
        milipede->status = 0;
        gera_milipede(milipede);
    }
}

void movimenta_milipede(MILIPEDE *milipede, COGUMELO cogumelos[], int num_cogumelos, FAZENDEIRO *fazendeiro) {
    int colidiu_com_fazendeiro;
    DIRECAO direcao_milipede = milipede->dir;

    verifica_colisao_milipede_cogumelos(milipede, cogumelos, num_cogumelos);

    switch(direcao_milipede) {
        case esq_mili:
            milipede->posicao_cabeca.x -= PASSO_MILIPEDE;

            if(verifica_movimento_milipede(milipede->tamanho, milipede->posicao_cabeca, cogumelos, *fazendeiro)) {
                inverte_movimento_milipede(milipede);
            }

            break;
        case dir_mili:
            milipede->posicao_cabeca.x += PASSO_MILIPEDE;

            if(verifica_movimento_milipede(milipede->tamanho, milipede->posicao_cabeca, cogumelos, *fazendeiro)) {
                inverte_movimento_milipede(milipede);
            }

            break;
        default:
            break;
    }

    colidiu_com_fazendeiro = verifica_colisao(milipede->posicao_cabeca, TAMANHO_SEGMENTO_MILIPEDE, fazendeiro->posicao, TAMANHO_JOGADOR);
    fazendeiro->doente += colidiu_com_fazendeiro;

    if (colidiu_com_fazendeiro) {
        fazendeiro->status = paralisado;

        WaitTime(3);

        // fazendeiro->status = livre;

        milipede->status = 0;
        gera_milipede(milipede);
    }

    testa_colisao_milipede_base(milipede);
}

void acertou_milipede(MILIPEDE *milipede) {
    milipede->tamanho -= 1;

    if (milipede->tamanho == 0) {
        milipede->status = 0;
    }
}

void verifica_impacto_tiro_milipede(TIRO *tiro, MILIPEDE *milipede) {
    if(tiro->posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(tiro->posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(tiro->posicao.y < MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA){
        tiro->status = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(tiro->posicao.y > ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao(tiro->posicao, TAMANHO_TIRO, milipede->posicao_cabeca, TAMANHO_SEGMENTO_MILIPEDE)){
        tiro->status = 0; // Verifica se o tiro colide com a cabeca da milipede.

        acertou_milipede(milipede);
    }
}

void verifica_tiros_milipede(FAZENDEIRO *fazendeiro, MILIPEDE *milipede) {
    int i;

    for (i = 0; i < NUM_TIROS; i++){
        if (fazendeiro->tiros[i].status){
            verifica_impacto_tiro_milipede(&fazendeiro->tiros[i], milipede);
        }
    }
}
