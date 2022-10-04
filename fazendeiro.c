#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"
#include "aranhas.h"
#include "colisao.h"
#include "cogumelos.h"
#include "milipede.h"

// Instancia o fazendeiro
void gera_fazendeiro(FAZENDEIRO *fazendeiro) {
    fazendeiro->posicao.x = LARGURA_TELA / 2;
    fazendeiro->posicao.y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    fazendeiro->direcao = cima;
    fazendeiro->tiros_restantes = NUM_TIROS;
    fazendeiro->cooldown_tiro = 0;
    fazendeiro->status = livre;
    fazendeiro->doente = 0;
    fazendeiro->contador_paralisado = 0;
    fazendeiro->contador_invulneravel = 0;
    fazendeiro->contador_doente = 0;
}

// Atira um tiro da posicao atual do jogador, diminui os tiros restantes do jogador e ativa o cooldown entre tiros.
void atirar(FAZENDEIRO *fazendeiro){
    int tiro_atual = NUM_TIROS - fazendeiro->tiros_restantes;

    fazendeiro->tiros[tiro_atual].posicao.x = fazendeiro->posicao.x + TAMANHO_JOGADOR / 2;
    fazendeiro->tiros[tiro_atual].posicao.y = fazendeiro->posicao.y + TAMANHO_JOGADOR / 2;
    fazendeiro->tiros[tiro_atual].status = 1;
    fazendeiro->tiros[tiro_atual].direcao = fazendeiro->direcao;
    fazendeiro->tiros_restantes -= 1;
    fazendeiro->cooldown_tiro = TEMPO_COOLDOWN_TIRO * FRAMERATE;
}

// Funcao para reinstanciar o jogador apos uma morte. Revive com uma vida a menos, e mesma quantidade de tiros que tinha antes da morte.
void revive_fazendeiro(FAZENDEIRO *fazendeiro){
    fazendeiro->posicao.x = LARGURA_TELA / 2;
    fazendeiro->posicao.y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    fazendeiro->direcao = cima;
    fazendeiro->vidas -= 1;
    fazendeiro->cooldown_tiro = 0;
    fazendeiro->status = livre;
    fazendeiro->doente = 0;
    fazendeiro->contador_paralisado = 0;
    fazendeiro->contador_invulneravel = 0;
    fazendeiro->contador_doente = 0;
}

// Se o fazendeiro ainda tem vidas extras, revive o fazendeiro. Senao, morre e o status do jogo vai para game over.
void fazendeiro_morre(FAZENDEIRO *fazendeiro, STATUS_JOGO *status_jogo){
    if(fazendeiro->vidas > 1){
        revive_fazendeiro(fazendeiro);
    } else {
        fazendeiro->status = morto;
        *status_jogo = game_over;
    }
}

// Atualiza os contadores do fazendeiro frame a frame. (cooldown de tiro, paralisia, invulnerabilidade, etc.)
void atualiza_status_fazendeiro(FAZENDEIRO *fazendeiro, STATUS_JOGO *status_jogo){
    if (fazendeiro->cooldown_tiro){
        fazendeiro->cooldown_tiro--;
    }
    if (fazendeiro->contador_paralisado){
        fazendeiro->status = paralisado;
        fazendeiro->contador_paralisado--;
    } else {
        fazendeiro->status = livre;
    }
    if (fazendeiro->contador_invulneravel){
        fazendeiro->contador_invulneravel--;
    }
    if (fazendeiro->contador_doente){
        fazendeiro->contador_doente--;
    } else {
        if(fazendeiro->doente){ // Se o contador de doente chegar a 0 e o fazendeiro ainda não tiver se curado, morre
            fazendeiro_morre(fazendeiro, status_jogo);
        }
    }

}

// Movimenta todos os tiros ativos na sua direcao.
void movimenta_tiros(TIRO tiros[]){
    int i;
    int mov_diagonal = MOVIMENTO_TIRO * MULT_TIRO_DIAGONAL;

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

// Verifica se um movimento do jogador e possivel. Recebe a nova posicao, nao a atual. Devolve 0 se nao, 1 se sim.
int verifica_movimento_jogador(COORD posicao, COGUMELO cogumelos[], int num_cogumelos){
    int flag = 1;

    if(posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_JOGADOR){
        flag = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(posicao.y < ((ALTURA_TELA - MARGEM_JOGO_Y) * 0.75)){
        flag = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(posicao.y > (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_JOGADOR)){
        flag = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(posicao, TAMANHO_JOGADOR, cogumelos, num_cogumelos)){
        flag = 0; // Verifica se o jogador colide com algum cogumelo
    }

    return flag;
}

// Muda a direcao do jogador, dependendo das teclas apertadas no momento.
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

// Movimenta o jogador, lendo inputs do teclado
void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], int num_cogumelos) {
    // Funcao para movimentar o jogador.
    COORD nova_posicao = {};

    if(IsKeyDown(KEY_UP)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y - MOVIMENTO;
        if (verifica_movimento_jogador(nova_posicao, cogumelos, num_cogumelos) && fazendeiro->status == livre) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_RIGHT)){
        nova_posicao.x = fazendeiro->posicao.x + MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento_jogador(nova_posicao, cogumelos, num_cogumelos) && fazendeiro->status == livre) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_DOWN)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y + MOVIMENTO;
        if (verifica_movimento_jogador(nova_posicao, cogumelos, num_cogumelos) && fazendeiro->status == livre) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_LEFT)){
        nova_posicao.x = fazendeiro->posicao.x - MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento_jogador(nova_posicao, cogumelos, num_cogumelos) && fazendeiro->status == livre) {
            fazendeiro->posicao = nova_posicao;
        }
    }

    muda_direcao_jogador(&fazendeiro->direcao);
}

// Le comandos do jogador, que nao sao as teclas de movimento
void comandos_jogador(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int num_cogumelos, JOGADOR jogadores[], STATUS_JOGO *status_jogo){

    switch(GetKeyPressed()) {

        case KEY_SPACE:
            // Se tem tiros, está livre e sem cooldown de tiro anterior, e o jogo não está pausado, atira
            if (fazendeiro->tiros_restantes && fazendeiro->status == livre && *status_jogo == normal && !fazendeiro->cooldown_tiro){
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
