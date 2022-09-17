#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"
#include "colisao.h"
#include "desenho.h"
#include "ranking.h"
#include "menus.h"
#include "cogumelos.h"
#include "fazendeiro.h"
#include "aranhas.h"
#include "saves.h"

void game_loop(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], JOGADOR jogadores[], MILIPEDE *milipede){

    STATUS_JOGO status_jogo = normal;
    int sair = 0;

    char input[TAMANHO_NOME + 1] = "\0";
    int num_letras = 0;
    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    while (!WindowShouldClose() && !sair) {
        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, *fazendeiro, NUM_COGUMELOS);

        desenha_tiros(fazendeiro->tiros, NUM_TIROS);
        desenha_jogador(*fazendeiro);
        desenha_cogumelos(cogumelos, NUM_COGUMELOS);
        desenha_aranhas(aranhas, NUM_ARANHAS);
        desenha_milipede(*milipede);

        comandos_jogador(fazendeiro, aranhas, cogumelos, NUM_COGUMELOS, jogadores, &status_jogo);

        if (status_jogo == pausado){ // Comandos se o jogo esta pausado
            menu_pausa(fazendeiro, aranhas, cogumelos, input, &num_letras, &status_jogo);
        } else if(status_jogo == carregando){ // Comandos se o jogo esta carregando
            menu_carregar(fazendeiro, aranhas, cogumelos, input, &num_letras, &status_jogo);
        } else if(status_jogo == mostrando_ranking){ // Comandos se o ranking está sendo mostrado
            desenha_ranking(jogadores);
        } else if(status_jogo == saindo){ // Comandos se o jogador quer sair
            menu_sair(fazendeiro, jogadores, input, &num_letras, &status_jogo, &sair);
        } else{
            // Comandos se nao
            movimenta_jogador(fazendeiro, cogumelos);
            move_aranhas(fazendeiro, aranhas, cogumelos, NUM_ARANHAS);
            movimenta_tiros(fazendeiro->tiros);
            verifica_tiros(fazendeiro, cogumelos);
            verifica_tiros_milipede(fazendeiro, milipede);
            movimenta_milipede(milipede, cogumelos, NUM_COGUMELOS, fazendeiro);
        }

        EndDrawing();
    }

}

// Funcoes para Milipede

void gera_milipede(MILIPEDE *milipede) {
    int tamanho_milipede = (int) (rand() % (TAMANHO_MAX_MILIPEDE - TAMANHO_MIN_MILIPEDE + 1));

    float x_random = (float) (rand() / (double) RAND_MAX * (LARGURA_TELA - (2 * MARGEM_JOGO_X) - (tamanho_milipede * TAMANHO_SEGMENTO_MILIPEDE)));

    milipede->tamanho = tamanho_milipede;
    milipede->posicao_cabeca.x = x_random;
    milipede->posicao_cabeca.y = MARGEM_JOGO_Y + TAMANHO_SEGMENTO_MILIPEDE;
    milipede->status = 1;
    milipede->dir =  (int) (rand() % (dir_mili - esq_mili + 1));
}

void desenha_milipede(MILIPEDE milipede) {
    int i;
    
    Image cabeca_imagem = LoadImage("imagens/milipede-cabeca.png");
    Texture2D textura_cabeca = LoadTextureFromImage(cabeca_imagem);

    Image corpo_imagem = LoadImage("imagens/milipede-corpo.png");
    Texture2D textura_corpo = LoadTextureFromImage(corpo_imagem); 

    DrawTexture(textura_cabeca, milipede.posicao_cabeca.x, milipede.posicao_cabeca.y, WHITE);

    for (i = 0; i < milipede.tamanho; i++) {
        if (milipede.dir == dir_mili) {
            DrawTexture(textura_corpo, milipede.posicao_cabeca.x - (TAMANHO_SEGMENTO_MILIPEDE * (i + 1)), milipede.posicao_cabeca.y, WHITE);
        } else {
            DrawTexture(textura_corpo, milipede.posicao_cabeca.x + (TAMANHO_SEGMENTO_MILIPEDE * (i + 1)), milipede.posicao_cabeca.y, WHITE);
        }
    }

    UnloadImage(cabeca_imagem);
    UnloadImage(corpo_imagem);
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
void verifica_colisao_milipede_cogumelos(MILIPEDE *milipede, COGUMELO cogumelos[], int num_cogumelos){
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

void verifica_tiros_milipede(FAZENDEIRO *fazendeiro, MILIPEDE *milipede){
    int i;

    for (i = 0; i < NUM_TIROS; i++){
        if (fazendeiro->tiros[i].status){
            verifica_impacto_tiro_milipede(&fazendeiro->tiros[i], milipede);
        }
    }
}

/// Fim das Funcoes para Milipede

int main() {
    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS] = {};
    ARANHA aranhas[NUM_ARANHAS] = {};
    JOGADOR jogadores[NUM_JOGADORES] = {};
    MILIPEDE milipede = {};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(cogumelos, NUM_COGUMELOS);
    gera_todas_aranhas(aranhas, NUM_ARANHAS);
    gera_milipede(&milipede);
    carregar_ranking(jogadores);
    ordenar_ranking(jogadores);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);
    SetExitKey(KEY_F1); // Para liberar o ESC, default da raylib
    // EnableEventWaiting();

    game_loop(&fazendeiro, cogumelos, aranhas, jogadores);

    CloseWindow();

    return 0;
}
