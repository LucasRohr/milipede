#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

void desenha_moldura() {
    int i;

    for(i = MARGEM_JOGO_X; i < LARGURA_TELA - MARGEM_JOGO_X; i++) {
        DrawRectangle(i, MARGEM_JOGO_Y, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, RED);
    }

    for(i = MARGEM_JOGO_Y; i < ALTURA_TELA - MARGEM_JOGO_Y; i++) {
        DrawRectangle(MARGEM_JOGO_X, i, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, BLUE);
    }

    for(i = MARGEM_JOGO_X; i < LARGURA_TELA - MARGEM_JOGO_X; i++) {
        DrawRectangle(i, ALTURA_TELA - MARGEM_JOGO_Y, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, GREEN);
    }

    for(i = MARGEM_JOGO_Y; i < ALTURA_TELA - MARGEM_JOGO_Y; i++) {
        DrawRectangle(LARGURA_TELA - MARGEM_JOGO_X, i, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, YELLOW);
    }
}

void desenha_menu_superior(char itens_menu[][TAMANHO_STR]){

    int i;
    float pos = 0;

    // Loop para desenho do menu.
    for(i = 0; i < NUM_ITEMS_MENU; i++){
        DrawText(itens_menu[i], MARGEM_JOGO_X + (LARGURA_TELA) * pos, 10, TAMANHO_FONTE, WHITE);
        pos = pos + 0.25;
    }
}

void desenha_menu_inferior(char itens_menu[][TAMANHO_STR], int pontos, int cogumelos_restantes, int vidas, int tiros){
    int i;
    float pos = 0;

    // Convers�o de int para str
    sprintf(itens_menu[1], "%d", pontos);
    sprintf(itens_menu[3], "%d", cogumelos_restantes);
    sprintf(itens_menu[5], "%d", vidas);
    sprintf(itens_menu[7], "%d", tiros);

    // Loop para desenho do menu. Texto em cinza, n�meros em branco.
    for(i = 0; i < NUM_ITEMS_MENU * 2; i++){
        if(i % 2 == 0){
            DrawText(itens_menu[i], MARGEM_JOGO_X + (LARGURA_TELA) * pos, ALTURA_TELA - MARGEM_JOGO_Y + 10, TAMANHO_FONTE, GRAY);
        } else {
            DrawText(itens_menu[i], MARGEM_JOGO_X + (LARGURA_TELA) * pos, ALTURA_TELA - MARGEM_JOGO_Y + 10, TAMANHO_FONTE, WHITE);
        }
        pos = pos + 0.125;
    }
}

int verifica_colisao_cogumelos(int posicao_x, int posicao_y, int tamanho_objeto, int pos_cogumelos[][2]){
    int i = 0, flag = 0;

    for(i = 0; i < NUM_COGUMELOS; i++){
        if((posicao_x < pos_cogumelos[i][0] + TAMANHO_COGUMELO) && (posicao_x + TAMANHO_JOGADOR > pos_cogumelos[i][0]) && posicao_y < pos_cogumelos[i][1] + TAMANHO_COGUMELO && posicao_y + TAMANHO_JOGADOR > pos_cogumelos[i][1]){
            flag = 1;
        }
    }
    return flag;
}

int verifica_movimento(int posicao_x, int posicao_y, int pos_cogumelos[][2]){
    // Verifica se � poss�vel o jogador fazer algum movimento. !!!Recebe a nova posicao, nao a atual!!!
    // Retorna 1 se o movimento � poss�vel, 0 se n�o
    int flag = 1;

    if(posicao_x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_JOGADOR){
        flag = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(posicao_x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(posicao_y < ((ALTURA_TELA - MARGEM_JOGO_Y) * 0.75)){
        flag = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(posicao_y > (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_JOGADOR)){
        flag = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(posicao_x, posicao_y, TAMANHO_JOGADOR, pos_cogumelos)){
        flag = 0; // Verifica se o jogador colide com algum cogumelo
    }

    return flag;
}

void movimenta_jogador(int *posicao_x, int *posicao_y, int pos_cogumelos[][2]) {

    switch(GetKeyPressed()) {
        case ESC:
            CloseWindow();
            break;

        default:
            break;
    }

    if(IsKeyDown(KEY_RIGHT)){
        if (verifica_movimento(*posicao_x + MOVIMENTO, *posicao_y, pos_cogumelos)) {
            *posicao_x += MOVIMENTO;
        }
    }
    if (IsKeyDown(KEY_LEFT)){
        if (verifica_movimento(*posicao_x - MOVIMENTO, *posicao_y, pos_cogumelos)) {
            *posicao_x -= MOVIMENTO;
        }
    }
    if (IsKeyDown(KEY_UP)){
        if (verifica_movimento(*posicao_x, *posicao_y - MOVIMENTO, pos_cogumelos)) {
            *posicao_y -= MOVIMENTO;
        }
    }
    if (IsKeyDown(KEY_DOWN)){
        if (verifica_movimento(*posicao_x, *posicao_y + MOVIMENTO, pos_cogumelos)) {
            *posicao_y += MOVIMENTO;
        }
    }
}

int verifica_posicao_cogumelos(int pos_cogumelos[][2], int tamanho_array, int x, int y){
    int i = 0, flag = 0;

    // Se as coordenadas recebidas j� est�o ocupadas por um cogumelo, retorna 1, sen�o 0. Para de percorrer o array quando chega ao final, ou encontra o valor 0.
    while(i < tamanho_array && pos_cogumelos[i][0] != 0){
        if (pos_cogumelos[i][0] == x && pos_cogumelos[i][1] == y){
            flag = 1;
        }
        i++;
    }
    return flag;
}

int gera_posicao_random(int valor_minimo, int valor_maximo) {
    // Gera valores aleat�rios entre o valor minimo e maximo
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo);
}

int gera_posicao_cogumelos(int valor_minimo, int valor_maximo) {
    // Gera valores aleat�rios, divisiveis pelo tamanho dos cogumelos + sua margem (para eles ficarem em seus pr�prios tiles)
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo) / (TAMANHO_COGUMELO + MARGEM_COGUMELO) * (TAMANHO_COGUMELO + MARGEM_COGUMELO);
}

void gera_cogumelos(int pos_cogumelos[][2]) {
    int i = 0, x, y;
    srand(time(0));

    while (i < NUM_COGUMELOS){
        x = gera_posicao_cogumelos(MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA + TAMANHO_COGUMELO, LARGURA_TELA - MARGEM_JOGO_X - DIMENSAO_RETANGULO_BORDA);
        y = gera_posicao_cogumelos(MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA + TAMANHO_JOGADOR * 2, ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR * 2);
        
        // Verifica se a posi��o j� est� ocupada. Se n�o, gera uma nova posi��o para o cogumelo.
        if (!verifica_posicao_cogumelos(pos_cogumelos, NUM_COGUMELOS, x, y)){
            pos_cogumelos[i][0] = x;
            pos_cogumelos[i][1] = y;
            i++;
        }
    }
}

void desenha_jogador(int *posicao_x, int *posicao_y) {
    // estudar possibilidade de usar png
    // tambem pensei em fazer algo mais simples com retangulos pora agora
    DrawRectangle(*posicao_x, *posicao_y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
}

void desenha_cogumelos(int pos_cogumelos[][2]){
    int i;

    for (i = 0; i < NUM_COGUMELOS; i++){
        DrawRectangle(pos_cogumelos[i][0], pos_cogumelos[i][1], TAMANHO_COGUMELO, TAMANHO_COGUMELO, GREEN);
    }
}

// Funcoes para Aranhas

void gera_aranha(ARANHA *aranha) {
    float x_random = (float) (rand() / (double) RAND_MAX * LARGURA_TELA);
    float y_random = (float) (rand() / (double) RAND_MAX * (ALTURA_TELA * 0.75));

    (*aranha).posicao.x = x_random;
    (*aranha).posicao.y = y_random;
    (*aranha).status = 1;
    (*aranha).dir = cima;
}

void inverte_movimento(ARANHA *aranha) {
    DIRECAO direcao_aranha = (*aranha).dir;

    switch(direcao_aranha) {
        case(cima):
            (*aranha).dir = (int) (rand() % (esq_baixo - dir_baixo + 1)) + dir_baixo;
            break;
        case(dir_cima):
            (*aranha).dir = (int) (rand() % (esq_cima - dir_baixo + 1)) + dir_baixo;
            break;
        case(dir):
            (*aranha).dir = (int) (rand() % (esq_cima - esq_baixo + 1)) + esq_baixo;
            break;
        case(dir_baixo):
            (*aranha).dir = (int) (rand() % (esq_cima - esq_baixo + 1)) + esq_baixo;
            break;
        case(baixo):
            (*aranha).dir = esq_cima;
            break;
        case(esq_baixo):
            (*aranha).dir = (int) (rand() % (dir_baixo - cima + 1)) + cima;
            break;
        case(esq):
            (*aranha).dir = (int) (rand() % (dir_baixo - dir_cima + 1)) + dir_cima;
            break;
        case(esq_cima):
            (*aranha).dir = (int) (rand() % (esq_baixo - dir_cima + 1)) + dir_cima;
            break;
        default:
            break;
    }
}

int checa_colisao_aranha(int x_elemento, int y_elemento, int x_obstaculo, int y_obstaculo) {
    if (x_elemento == x_obstaculo || y_elemento == y_obstaculo) {
        return 1;
    } else {
        return 0;
    }
}

void move_aranha(ARANHA *aranha) {
    DIRECAO direcao_aranha = (*aranha).dir;

    switch(direcao_aranha) {
        case cima:
            (*aranha).posicao.y += 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, LARGURA_TELA, ALTURA_TELA)) {
                inverte_movimento(aranha);
            }

            break;
        case dir_cima:
            (*aranha).posicao.x += 1.0;
            (*aranha).posicao.y += 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, LARGURA_TELA, ALTURA_TELA)) {
                inverte_movimento(aranha);
            }

            break;
        case dir:
            (*aranha).posicao.x += 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, LARGURA_TELA, ALTURA_TELA)) {
                inverte_movimento(aranha);
            }

            break;
        case dir_baixo:
            (*aranha).posicao.x += 1.0;
            (*aranha).posicao.y -= 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, LARGURA_TELA, 0)) {
                inverte_movimento(aranha);
            }

            break;
        case baixo:
            (*aranha).posicao.y -= 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, 0, 0)) {
                inverte_movimento(aranha);
            }

            break;
        case esq_baixo:
            (*aranha).posicao.x -= 1.0;
            (*aranha).posicao.y -= 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, 0, 0)) {
                inverte_movimento(aranha);
            }

            break;
        case esq:
            (*aranha).posicao.x -= 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, 0, 0)) {
                inverte_movimento(aranha);
            }

            break;
        case esq_cima:
            (*aranha).posicao.y += 1.0;
            (*aranha).posicao.x -= 1.0;

            if(checa_colisao_aranha((*aranha).posicao.x, (*aranha).posicao.y, 0, ALTURA_TELA)) {
                inverte_movimento(aranha);
            }

            break;
        default:
            break;
    }
}

void gera_fazendeiro(FAZENDEIRO *fazendeiro) {
    (*fazendeiro).tiros = NUM_TIROS;
    (*fazendeiro).vidas = NUM_VIDAS;
    (*fazendeiro).cogumelos_colhidos = 0;
    (*fazendeiro).doente = 0;

    (*fazendeiro).posicao.x = (float) (rand() / (double) RAND_MAX * LARGURA_TELA);
    (*fazendeiro).posicao.y = (float) (rand() / (double) RAND_MAX * ALTURA_TELA);
}

int testa_colisao_aranha_fazendeiro(ARANHA aranha, FAZENDEIRO fazendeiro) {
    float x_aranha = aranha.posicao.x;
    float y_aranha = aranha.posicao.y;

    float x_fazendeiro = fazendeiro.posicao.x;
    float y_fazendeiro = fazendeiro.posicao.y;

     if (x_aranha == x_fazendeiro && y_aranha == y_fazendeiro) {
        return 1;
    } else {
        return 0;
    }
}

void testa_colisao_aranha_base(ARANHA *aranha) {
    if ((*aranha).posicao.y == 0) {
        (*aranha).status = 0;
    }
}

void move_aranhas(FAZENDEIRO *fazendeiro, ARANHA aranhas[], int total_aranhas) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        move_aranha(&(aranhas[i]));

        (*fazendeiro).doente += testa_colisao_aranha_fazendeiro(aranhas[i], *fazendeiro);

        testa_colisao_aranha_base(&(aranhas[i]));

        WaitTime(1000);
    }
}

void gera_todas_aranhas(ARANHA aranhas[], int total_aranhas) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        gera_aranha(&(aranhas[i]));
    }
}

void desenha_aranhas(ARANHA aranhas[], int total_aranhas) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        DrawRectangle(aranhas[i].posicao.x, aranhas[i].posicao.y, TAMANHO_ARANHA, TAMANHO_ARANHA, BLUE);
    }
}

// === Fim das Funcoes para Aranhas ===

int main() {
    int posicao_x = LARGURA_TELA / 2, posicao_y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    int pontos = 0, cogumelos_restantes = NUM_COGUMELOS, vidas = NUM_VIDAS, tiros = NUM_TIROS;

    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    ARANHA aranhas[NUM_ARANHAS];
    FAZENDEIRO fazendeiro;

    int pos_cogumelos[NUM_COGUMELOS][2] = {};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(pos_cogumelos);
    gera_todas_aranhas(aranhas, NUM_ARANHAS);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        movimenta_jogador(&posicao_x, &posicao_y, pos_cogumelos);
        move_aranhas(&fazendeiro, aranhas, NUM_ARANHAS);

        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, pontos, cogumelos_restantes, vidas, tiros);

        desenha_jogador(&posicao_x, &posicao_y);
        desenha_cogumelos(pos_cogumelos);
        desenha_aranhas(aranhas, NUM_ARANHAS);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
