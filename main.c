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

    game_loop(&fazendeiro, cogumelos, aranhas, jogadores, &milipede);

    CloseWindow();

    return 0;
}
