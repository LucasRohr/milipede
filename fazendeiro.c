#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"
#include "colisao.h"
#include "cogumelos.h"

void gera_fazendeiro(FAZENDEIRO *fazendeiro) {
    fazendeiro->posicao.x = LARGURA_TELA / 2;
    fazendeiro->posicao.y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    fazendeiro->direcao = cima;
    strcpy(fazendeiro->nome, "Jogador");
    fazendeiro->vidas = NUM_VIDAS;
    fazendeiro->tiros_restantes = NUM_TIROS;
    fazendeiro->cogumelos_colhidos = 0;
    fazendeiro->status = livre;
    fazendeiro->doente = 0;
}

void atirar(FAZENDEIRO *fazendeiro){
    int tiro_atual = NUM_TIROS - fazendeiro->tiros_restantes;

    fazendeiro->tiros[tiro_atual].posicao.x = fazendeiro->posicao.x + TAMANHO_JOGADOR / 2;
    fazendeiro->tiros[tiro_atual].posicao.y = fazendeiro->posicao.y + TAMANHO_JOGADOR / 2;
    fazendeiro->tiros[tiro_atual].status = 1;
    fazendeiro->tiros[tiro_atual].direcao = fazendeiro->direcao;
    fazendeiro->tiros_restantes -= 1;
}

void verifica_impacto_tiro(TIRO *tiro, int *cogumelos_colhidos, COGUMELO cogumelos[]){
    if(tiro->posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(tiro->posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(tiro->posicao.y < MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA){
        tiro->status = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(tiro->posicao.y > ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(tiro->posicao, TAMANHO_TIRO, cogumelos, NUM_COGUMELOS)){
        tiro->status = 0; // Verifica se o tiro colide com algum cogumelo.
        *cogumelos_colhidos += 1;
        acertou_cogumelo(tiro->posicao, cogumelos, NUM_COGUMELOS);
    }
}

void verifica_tiros(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]){
    int i;

    for (i = 0; i < NUM_TIROS; i++){
        if (fazendeiro->tiros[i].status){
            verifica_impacto_tiro(&fazendeiro->tiros[i], &fazendeiro->cogumelos_colhidos, cogumelos);
        }
    }
}

void movimenta_tiros(TIRO tiros[]){
    int i;
    int mov_diagonal = MOVIMENTO_TIRO * 0.707;

    for (i=0; i < NUM_TIROS; i++){
        if(tiros[i].status){
            switch(tiros[i].direcao){
                case cima:
                    tiros[i].posicao.y -= MOVIMENTO_TIRO;
                    break;
                case dir_cima:
                    tiros[i].posicao.x += mov_diagonal;
                    tiros[i].posicao.y -= mov_diagonal;
                    break;
                case dir:
                    tiros[i].posicao.x += MOVIMENTO_TIRO;
                    break;
                case dir_baixo:
                    tiros[i].posicao.x += mov_diagonal;
                    tiros[i].posicao.y += mov_diagonal;
                    break;
                case baixo:
                    tiros[i].posicao.y += MOVIMENTO_TIRO;
                    break;
                case esq_baixo:
                    tiros[i].posicao.x -= mov_diagonal;
                    tiros[i].posicao.y += mov_diagonal;
                    break;
                case esq:
                    tiros[i].posicao.x -= MOVIMENTO_TIRO;
                    break;
                case esq_cima:
                    tiros[i].posicao.x -= mov_diagonal;
                    tiros[i].posicao.y -= mov_diagonal;
                    break;
                default:
                    break;
            }
        }
    }
}

int verifica_movimento_jogador(COORD posicao, COGUMELO cogumelos[]){
    // Verifica se � poss�vel o jogador fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se o movimento � poss�vel, 0 se n�o
    int flag = 1;

    if(posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_JOGADOR){
        flag = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(posicao.y < ((ALTURA_TELA - MARGEM_JOGO_Y) * 0.75)){
        flag = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(posicao.y > (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_JOGADOR)){
        flag = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(posicao, TAMANHO_JOGADOR, cogumelos, NUM_COGUMELOS)){
        flag = 0; // Verifica se o jogador colide com algum cogumelo
    }

    return flag;
}

void muda_direcao_jogador(DIRECAO *direcao){
    if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP)){
        *direcao = dir_cima;
    } else if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_DOWN)){
        *direcao = dir_baixo;
    } else if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_DOWN)){
        *direcao = esq_baixo;
    } else if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_UP)){
        *direcao = esq_cima;
    } else if(IsKeyDown(KEY_UP)){
        *direcao = cima;
    } else if(IsKeyDown(KEY_RIGHT)){
        *direcao = dir;
    } else if(IsKeyDown(KEY_DOWN)){
        *direcao = baixo;
    } else if(IsKeyDown(KEY_LEFT)){
        *direcao = esq;
    }
}

void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]) {
    // Funcao para movimentar o jogador.
    COORD nova_posicao = {};

    if(IsKeyDown(KEY_UP)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y - MOVIMENTO;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_RIGHT)){
        nova_posicao.x = fazendeiro->posicao.x + MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_DOWN)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y + MOVIMENTO;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_LEFT)){
        nova_posicao.x = fazendeiro->posicao.x - MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }

    muda_direcao_jogador(&fazendeiro->direcao);
}

void comandos_jogador(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int num_cogumelos, JOGADOR jogadores[], STATUS_JOGO *status_jogo){
    // Funcao para ler inputs do jogador que nao sao as teclas de movimento

    switch(GetKeyPressed()) {

        case KEY_SPACE:
            if (fazendeiro->tiros_restantes > 0 && fazendeiro->status == livre && *status_jogo == normal){
                atirar(fazendeiro);
            }
            break;

        case KEY_P:
            if (*status_jogo == normal){
                *status_jogo = pausado;
            }
            break;

        case KEY_C:
            if (*status_jogo == normal){
                *status_jogo = carregando;
            }
            break;

        case KEY_R:
            if (*status_jogo == normal){
                *status_jogo = mostrando_ranking;
            } else if (*status_jogo == mostrando_ranking){
                *status_jogo = normal;
            }
            break;

        case KEY_ESCAPE:
            if (*status_jogo == normal){
                *status_jogo = saindo;
            } else if (*status_jogo == saindo){
                *status_jogo = normal;
            }
            break;

        default:
            break;
    }
}
