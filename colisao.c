#include <stdio.h>
#include "definicoes.h"

// Dado o as coordenadas e tamanho de dois objetos, verifica se houve colisao entre eles. Devolve 0 se nao.
int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam){
    // Funcao geral para verificar colisao. Recebe as coordernadas de dois objetos e seu tamanho.
    int flag = 0;

    if((pos_a.x < pos_b.x + b_tam) && (pos_a.x + a_tam > pos_b.x) && pos_a.y < pos_b.y + b_tam && pos_a.y + a_tam > pos_b.y){
        flag = 1;
    }
    return flag;
}

// Percorre o arranjo de cogumelos e verifica se algum objeto colidiu com algum deles. Devolve 0 se nao.
int verifica_colisao_cogumelos(COORD posicao, int tamanho_objeto, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0, flag = 0;

    for(i = 0; i < num_cogumelos; i++){
        if (verifica_colisao(posicao, tamanho_objeto, cogumelos[i].posicao, TAMANHO_COGUMELO) && cogumelos[i].status){
            flag += verifica_colisao(posicao, tamanho_objeto, cogumelos[i].posicao, TAMANHO_COGUMELO);
        }
    }

    return flag;
}

// Percorre o arranjo de aranhas e verifica se algum objeto colidiu com alguma delas.
int verifica_colisao_tiro_aranhas(COORD posicao, int tamanho_objeto, ARANHA aranhas[], int num_aranhas) {
    // Verifica a colisao de tiro para cada aranha
    int i = 0, flag = 0;

    for(i = 0; i < num_aranhas; i++){
        if (verifica_colisao(posicao, tamanho_objeto, aranhas[i].posicao, TAMANHO_ARANHA) && aranhas[i].status){
            flag += verifica_colisao(posicao, tamanho_objeto, aranhas[i].posicao, TAMANHO_ARANHA);
        }
    }

    return flag;
}

// Percorre lista de aranhas e elimina a que colidiu com tiro.
void acertou_aranha(COORD posicao, ARANHA aranhas[], int num_aranhas){
    // Verifica a colisao para cada aranha
    int i = 0;

    for(i = 0; i < num_aranhas; i++) {
        if (verifica_colisao(posicao, TAMANHO_TIRO, aranhas[i].posicao, TAMANHO_ARANHA) && aranhas[i].status) {
            aranhas[i].status = 0;
        }
    }
}

// Percorre lista de cogumelois e elimina o que colidiu com tiro.
void acertou_cogumelo(COORD posicao, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0;

    for(i = 0; i < num_cogumelos; i++){
        if (verifica_colisao(posicao, TAMANHO_TIRO, cogumelos[i].posicao, TAMANHO_COGUMELO) && cogumelos[i].status){
            cogumelos[i].status = 0;
        }
    }
}

// Diminui o tamanho da milipede, ou mata ela se seu tamanho for 0.
void acertou_milipede(MILIPEDE *milipede) {
    milipede->tamanho -= 1;

    if (milipede->tamanho < 1) {
        milipede->status = 0;
    }
}

// Funcao para verificar a colisao de um tiro com objetos. (paredes, cogumelos, aranhas, milipede)
void verifica_impacto_tiro(TIRO *tiro, int *cogumelos_colhidos, int *doente, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE config_fase){
    if(tiro->posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(tiro->posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(tiro->posicao.y < MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA){
        tiro->status = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(tiro->posicao.y > ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(tiro->posicao, TAMANHO_TIRO, cogumelos, config_fase.num_cogumelos)){
        tiro->status = 0; // Verifica se o tiro colide com algum cogumelo.
        acertou_cogumelo(tiro->posicao, cogumelos, config_fase.num_cogumelos);
        if(*doente) { // Se o jogador esta doente, diminui o numero de cogumelos restantes para e curar. Se nao, ganha um ponto
            *doente -= 1;
        } else {
            *cogumelos_colhidos += 1;
        }
    } else if(verifica_colisao_tiro_aranhas(tiro->posicao, TAMANHO_TIRO, aranhas, config_fase.num_aranhas)){
        tiro->status = 0; // Verifica se o tiro colide com uma aranha e o elimina da tela
        acertou_aranha(tiro->posicao, aranhas, config_fase.num_aranhas);
    } else if(verifica_colisao(tiro->posicao, TAMANHO_TIRO, milipede->posicao_cabeca, TAMANHO_SEGMENTO_MILIPEDE) && milipede->status){
        tiro->status = 0; // Verifica se o tiro colide com a cabeca da milipede.

        acertou_milipede(milipede);
    }
}

// Funcao que chama verifica_impacto_tiro para cada tiro do arranjo.
void verifica_tiros(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE config_fase){
    int i;

    for (i = 0; i < NUM_TIROS; i++){
        if (fazendeiro->tiros[i].status){
            verifica_impacto_tiro(&fazendeiro->tiros[i], &fazendeiro->cogumelos_colhidos, &fazendeiro->doente, cogumelos, aranhas, milipede, config_fase);
        }
    }
}

