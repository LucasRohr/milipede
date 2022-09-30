#include <stdio.h>
#include "definicoes.h"

// Dado o as coordenadas e tamanho de dois objetos, verifica se houve colisao entre eles. Devolve 0 se nao.
int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam){
    // Fun��o geral para verificar colisao. Recebe as coordernadas de dois objetos e seu tamanho.
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

