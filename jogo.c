#include <stdio.h>
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


void gera_elementos_jogo(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE *config_fase){
    // Gera elementos do jogo, baseado nos parametros em config_fase
    gera_fazendeiro(fazendeiro);
    gera_cogumelos(cogumelos, config_fase->num_cogumelos);
    gera_todas_aranhas(aranhas, config_fase->num_aranhas);
    gera_milipede(milipede, config_fase->tam_min_milipede, config_fase->tam_max_milipede);
}

void verifica_fim_fase(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE *config_fase, STATUS_JOGO *status_jogo, int *contador_menu){
    if(!conta_cogumelos_restantes(cogumelos, config_fase->num_cogumelos)){ // Se todos cogumelos foram colhidos, muda a fase e incrementa as variaveis que variam com fase
        config_fase->fase += 1;
        *status_jogo = inicio_fase;
        config_fase->num_cogumelos += INCREMENTO_COGUMELOS;
        config_fase->num_aranhas += INCREMENTO_ARANHA;
        config_fase->tam_min_milipede += INCREMENTO_MILIPEDE;
        config_fase->tam_max_milipede += INCREMENTO_MILIPEDE;
        *contador_menu = TEMPO_INICIO_FASE * FRAMERATE;
        gera_elementos_jogo(fazendeiro, cogumelos, aranhas, milipede, config_fase); // Gera os elementos da nova fase
    }
}

void game_loop(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], JOGADOR jogadores[], MILIPEDE *milipede, Texture2D texturas[]){
    char input[TAMANHO_NOME + 1] = "\0"; // Variaveis para instanciamento do nome do jogador
    int num_letras = 0;
    int contador_menu = TEMPO_INICIO_FASE * FRAMERATE;

    // Textos dos menus (para não serem gerados novamente a cada loop da funcao)
    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};
    char texto_menu_pausa[TAMANHO_STR] = "Digite o nome para salvar e voltar ao jogo";
    char texto_menu_carregar[TAMANHO_STR] = "Digite o seu nome para carregar.";
    char texto_menu_sair[TAMANHO_STR] = "Digite o seu nome antes de sair (voltar com ESC)";
    char texto_menu_game_over[TAMANHO_STR] = "GAME OVER! Digite seu nome para salvar ranking.";
    char itens_contador_doente[2][TAMANHO_STR] = {};

    // Variaveis para o andamento do jogo
    STATUS_JOGO status_jogo = inicio_fase;
    int sair = 0;
    CONFIG_FASE config_fase = {1, NUM_COGUMELOS_INICIAL, NUM_ARANHAS_INICIAL, TAMANHO_MIN_MILIPEDE_INICIAL, TAMANHO_MAX_MILIPEDE_INICIAL};

    gera_elementos_jogo(fazendeiro, cogumelos, aranhas, milipede, &config_fase);
    fazendeiro->vidas = 3;

    while (!WindowShouldClose() && !sair) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Funcoes de desenho
        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, *fazendeiro, conta_cogumelos_restantes(cogumelos, config_fase.num_cogumelos));

        desenha_tiros(fazendeiro->tiros, NUM_TIROS);
        desenha_aranhas(aranhas, config_fase.num_aranhas, texturas[I_TEXTURA_ARANHA]);
        desenha_cogumelos(cogumelos, config_fase.num_cogumelos, texturas[I_TEXTURA_COGUMELO]);
        desenha_milipede(*milipede, texturas[I_TEXTURA_MILIPEDE_CABECA], texturas[I_TEXTURA_MILIPEDE_CORPO]);
        desenha_jogador(*fazendeiro, texturas[I_TEXTURA_FAZENDEIRO]);
        desenha_contador_doente(itens_contador_doente, *fazendeiro);


        // Funcao para ler os comandos do jogador
        comandos_jogador(fazendeiro, aranhas, cogumelos, config_fase.num_cogumelos, jogadores, &status_jogo);

        if (status_jogo == pausado){ // Comandos se o jogo esta pausado
            menu_pausa(texto_menu_pausa, fazendeiro, aranhas, milipede, cogumelos, &config_fase, &status_jogo, input, &num_letras);
        } else if(status_jogo == carregando){ // Comandos se o jogo esta carregando
            menu_carregar(texto_menu_carregar, fazendeiro, aranhas, milipede, cogumelos, &config_fase, &status_jogo, input, &num_letras);
        } else if(status_jogo == mostrando_ranking){ // Comandos se o ranking está sendo mostrado
            desenha_ranking(jogadores);
        } else if(status_jogo == saindo){ // Comandos se o jogador quer sair
            menu_sair(texto_menu_sair, fazendeiro, jogadores, input, &num_letras, &status_jogo, &sair);
        } else if (status_jogo == game_over){
            menu_game_over(texto_menu_game_over, fazendeiro, jogadores, input, &num_letras, &status_jogo, &sair);
        } else if (status_jogo == inicio_fase){
            menu_inicio_fase(&status_jogo, config_fase.fase, &contador_menu);
        } else {
            // Comandos se status do jogo é normal
            atualiza_status_fazendeiro(fazendeiro, &status_jogo);
            movimenta_jogador(fazendeiro, cogumelos, config_fase.num_cogumelos);
            move_aranhas(fazendeiro, aranhas, cogumelos, config_fase, &status_jogo);
            movimenta_tiros(fazendeiro->tiros);
            verifica_tiros(fazendeiro, cogumelos, config_fase.num_cogumelos);
            verifica_tiros_milipede(fazendeiro, milipede);
            movimenta_milipede(milipede, cogumelos, fazendeiro, config_fase, &status_jogo);
            verifica_fim_fase(fazendeiro, cogumelos, aranhas, milipede, &config_fase, &status_jogo, &contador_menu);
        }

        EndDrawing();
    }

}
