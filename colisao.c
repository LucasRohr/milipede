#include <stdio.h>
#include "definicoes.h"

int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam){
    // Fun��o geral para verificar colisao. Recebe as coordernadas de dois objetos e seu tamanho.
    int flag = 0;

    if((pos_a.x < pos_b.x + b_tam) && (pos_a.x + a_tam > pos_b.x) && pos_a.y < pos_b.y + b_tam && pos_a.y + a_tam > pos_b.y){
        flag = 1;
    }
    return flag;
}

