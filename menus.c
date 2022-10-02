#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"
#include "desenho.h"
#include "menus.h"
#include "saves.h"
#include "ranking.h"

// Funcao para o menu de pausa. Chama a funcao de desenho do menu, e de instanciamento de nome para fazer um save
void menu_pausa(char texto[], FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase, STATUS_JOGO *status_jogo, char input[], int *num_letras){
    desenha_menu_pausa(texto, input);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome para salvar o jogo e instanciar o nome do fazendeiro
        strcpy(fazendeiro->nome, input);
        if(salvar_jogo(*fazendeiro, aranhas, *milipede, cogumelos, *config_fase)){
            printf("\n\nErro ao salvar jogo.\n\n");
        }
        *status_jogo = normal;
    }

}

// Funcao para o menu de saida. Chama a funcao de desenho do menu, e de instanciamento de nome para salvar ranking
void menu_sair(char texto[], FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair) {
    desenha_menu_pausa(texto, input);

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

// Funcao para o menu de carregamento. Chama a funcao de desenho do menu, e de instanciamento de nome para carregar um save
void menu_carregar(char texto[], FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase, STATUS_JOGO *status_jogo, char input[], int *num_letras){
    desenha_menu_pausa(texto, input);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome para carregar o jogo
        if(carregar_jogo(fazendeiro, aranhas, milipede, cogumelos, config_fase, input)){
            printf("\n\nErro ao carregar jogo.\n\n");
        }
        *status_jogo = normal;
    }
}

// Funcao para o menu de game over. Chama a funcao de desenho do menu, e de instanciamento de nome para salvar ranking
void menu_game_over(char texto[], FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair){
    desenha_menu_pausa(texto, input);

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

// Funcao para o menu de inicio da fase. Chama a funcao de desenho do menu, e diminui o contador de duracao a cada frame
void menu_inicio_fase(STATUS_JOGO *status_jogo, int fase, int *contador){
    char texto[TAMANHO_STR] = "Fase ";
    char num_fase[2] = {fase + '0', '\0'};

    strcat(texto, num_fase);

    if (*contador % FRAMERATE >= FRAMES_PISCAR_MENU){ // Para ter efeito de piscar na tela
       desenha_menu_inicio_fase(texto);
    }

    if (*contador < 1) {
        *status_jogo = normal;
    } else {
        *contador -= 1;
    }
}

// Funcao para instancia o nome do jogador.
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
