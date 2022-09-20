#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"
#include "desenho.h"
#include "menus.h"
#include "saves.h"
#include "ranking.h"


void menu_pausa(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], char input[], int *num_letras, STATUS_JOGO *status_jogo){

    desenha_menu_pausa("Digite o nome para salvar e voltar ao jogo", input);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome para salvar o jogo e instanciar o nome do fazendeiro
        strcpy(fazendeiro->nome, input);
        if(!salvar_jogo(fazendeiro, aranhas, cogumelos)){
            printf("\n\nErro ao salvar jogo.\n\n");
        }
        *status_jogo = normal;
    }

}

void menu_sair(FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair) {

    desenha_menu_pausa("Digite o seu nome antes de sair (cancelar com ESC)", input);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome, para sair o jogo e atualizar o ranking
        strcpy(fazendeiro->nome, input);
        inserir_jogador_atual(*fazendeiro, jogadores);
        ordenar_ranking(jogadores);
        if (salvar_ranking(jogadores)){
            printf("\n\nErro ao salvar ranking.\n\n");
        }
        *sair = 1;
    }
}

void menu_carregar(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], char input[], int *num_letras, STATUS_JOGO *status_jogo){

    desenha_menu_pausa("Digite o seu nome para carregar.", input);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome para carregar o jogo
        carregar_jogo(fazendeiro, aranhas, cogumelos, input);
        *status_jogo = normal;
    }
}

void menu_game_over(FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair){

    desenha_menu_pausa("GAME OVER! Digite seu nome para salvar ranking.", input);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome, para sair o jogo e atualizar o ranking
        strcpy(fazendeiro->nome, input);
        inserir_jogador_atual(*fazendeiro, jogadores);
        ordenar_ranking(jogadores);
        if (salvar_ranking(jogadores)){
            printf("\n\nErro ao salvar ranking.\n\n");
        }
        *sair = 1;
    }
}

int instanciar_nome(char input[], int *num_letras){
    int tecla = GetCharPressed();

    while (tecla > 0) {
        if ((tecla >= ESPACO) && (tecla <= CURLY_BRACKET) && (*num_letras < TAMANHO_NOME)) {
            input[*num_letras] = (char)tecla;
            input[*num_letras + 1] = '\0'; // Adiciona \0 ao fim da string
            *num_letras = *num_letras + 1;
        }
        tecla = GetCharPressed();  // Verifica proximo caractere
    }
    if (IsKeyPressed(KEY_BACKSPACE)){ // Apagar ultimo caractere se pressionar backspace
        *num_letras = *num_letras - 1;

        if (*num_letras < 0) {
            *num_letras = 0;
        }
        input[*num_letras] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER)){
        return 1; // Se apertar enter, retorna 1
    } else {
        return 0;
    }
}

