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
#include "milipede.h"
#include "saves.h"

void game_loop(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], JOGADOR jogadores[], MILIPEDE *milipede, Texture2D texturas[]){

    STATUS_JOGO status_jogo = normal;
    int sair = 0;

    char input[TAMANHO_NOME + 1] = "\0";
    int num_letras = 0;
    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};
    char itens_contador_doente[2][TAMANHO_STR] = {};

    while (!WindowShouldClose() && !sair) {
        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, *fazendeiro, NUM_COGUMELOS);

        desenha_tiros(fazendeiro->tiros, NUM_TIROS);
        desenha_aranhas(aranhas, NUM_ARANHAS, texturas[I_TEXTURA_ARANHA]);
        desenha_cogumelos(cogumelos, NUM_COGUMELOS, texturas[I_TEXTURA_COGUMELO]);
        desenha_milipede(*milipede, texturas[I_TEXTURA_MILIPEDE_CABECA], texturas[I_TEXTURA_MILIPEDE_CORPO]);
        desenha_jogador(*fazendeiro, texturas[I_TEXTURA_FAZENDEIRO]);

        desenha_contador_doente(itens_contador_doente, *fazendeiro);

        comandos_jogador(fazendeiro, aranhas, cogumelos, NUM_COGUMELOS, jogadores, &status_jogo);

        if (status_jogo == pausado){ // Comandos se o jogo esta pausado
            menu_pausa(fazendeiro, aranhas, cogumelos, input, &num_letras, &status_jogo);
        } else if(status_jogo == carregando){ // Comandos se o jogo esta carregando
            menu_carregar(fazendeiro, aranhas, cogumelos, input, &num_letras, &status_jogo);
        } else if(status_jogo == mostrando_ranking){ // Comandos se o ranking está sendo mostrado
            desenha_ranking(jogadores);
        } else if(status_jogo == saindo){ // Comandos se o jogador quer sair
            menu_sair(fazendeiro, jogadores, input, &num_letras, &status_jogo, &sair);
        } else if (status_jogo == game_over){
            menu_game_over(fazendeiro, jogadores, input, &num_letras, &status_jogo, &sair);
        } else {
            // Comandos se staus_jogo é normal
            atualiza_status_fazendeiro(fazendeiro, &status_jogo);
            movimenta_jogador(fazendeiro, cogumelos);
            move_aranhas(fazendeiro, aranhas, cogumelos, NUM_ARANHAS, &status_jogo);
            movimenta_tiros(fazendeiro->tiros);
            verifica_tiros(fazendeiro, cogumelos);
            verifica_tiros_milipede(fazendeiro, milipede);
            movimenta_milipede(milipede, cogumelos, NUM_COGUMELOS, fazendeiro, &status_jogo);
        }

        EndDrawing();
    }

}

int main() {
    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS] = {};
    ARANHA aranhas[NUM_ARANHAS] = {};
    MILIPEDE milipede = {};
    JOGADOR jogadores[NUM_JOGADORES] = {};
    Texture2D texturas[NUM_IMAGENS] = {};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(cogumelos, NUM_COGUMELOS);
    gera_todas_aranhas(aranhas, NUM_ARANHAS);
    gera_milipede(&milipede);
    carregar_ranking(jogadores);
    ordenar_ranking(jogadores);


    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);
    carrega_texturas(texturas);
    SetExitKey(KEY_F1); // Para liberar o ESC, default da raylib

    game_loop(&fazendeiro, cogumelos, aranhas, jogadores, &milipede, texturas);
    descarrega_texturas(texturas);

    CloseWindow();

    return 0;
}
