#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <stdlib.h>
#include <string.h>

#define ENTER 10
#define LARGURA_TELA 810
#define ALTURA_TELA 640

#define MOVIMENTO 4
#define ESC 27

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define MARGEM_JOGO_Y 50 // Margem da área do jogo em si (sem barras em cima ou em baixo)
#define MARGEM_JOGO_X 10

#define TAMANHO_FONTE 30
#define TAMANHO_STR 20

#define TAMANHO_JOGADOR 15
#define TAMANHO_COGUMELO 15

#define MARGEM_COGUMELO 5

#define NUM_ITEMS_MENU 4

#define NUM_COGUMELOS 60
#define NUM_VIDAS 3
#define NUM_TIROS 200

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

    // Conversão de int para str
    sprintf(itens_menu[1], "%d", pontos);
    sprintf(itens_menu[3], "%d", cogumelos_restantes);
    sprintf(itens_menu[5], "%d", vidas);
    sprintf(itens_menu[7], "%d", tiros);

    // Loop para desenho do menu. Texto em cinza, números em branco.
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
    // Verifica se é possível o jogador fazer algum movimento. !!!Recebe a nova posicao, nao a atual!!!
    // Retorna 1 se o movimento é possível, 0 se não
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

    // Se as coordenadas recebidas já estão ocupadas por um cogumelo, retorna 1, senão 0. Para de percorrer o array quando chega ao final, ou encontra o valor 0.
    while(i < tamanho_array && pos_cogumelos[i][0] != 0){
        if (pos_cogumelos[i][0] == x && pos_cogumelos[i][1] == y){
            flag = 1;
        }
        i++;
    }
    return flag;
}

int gera_posicao_random(int valor_minimo, int valor_maximo) {
    // Gera valores aleatórios entre o valor minimo e maximo
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo);
}

int gera_posicao_cogumelos(int valor_minimo, int valor_maximo) {
    // Gera valores aleatórios, divisiveis pelo tamanho dos cogumelos + sua margem (para eles ficarem em seus próprios tiles)
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo) / (TAMANHO_COGUMELO + MARGEM_COGUMELO) * (TAMANHO_COGUMELO + MARGEM_COGUMELO);
}

void gera_cogumelos(int pos_cogumelos[][2]) {
    int i = 0, x, y;
    srand(time(0));

    while (i < NUM_COGUMELOS){
        x = gera_posicao_cogumelos(MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA + TAMANHO_COGUMELO, LARGURA_TELA - MARGEM_JOGO_X - DIMENSAO_RETANGULO_BORDA);
        y = gera_posicao_cogumelos(MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA + TAMANHO_COGUMELO * 2, ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR - TAMANHO_COGUMELO);
        // Verifica se a posição já está ocupada. Se não, gera uma nova posição para o cogumelo.
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

int main() {

    int posicao_x = LARGURA_TELA / 2, posicao_y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    int pontos = 0, cogumelos_restantes = NUM_COGUMELOS, vidas = NUM_VIDAS, tiros = NUM_TIROS;

    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    int pos_cogumelos[NUM_COGUMELOS][2] = {};
    gera_cogumelos(pos_cogumelos);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        movimenta_jogador(&posicao_x, &posicao_y, pos_cogumelos);

        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, pontos, cogumelos_restantes, vidas, tiros);

        desenha_jogador(&posicao_x, &posicao_y);
        desenha_cogumelos(pos_cogumelos);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
