#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <stdlib.h>

#define ENTER 10
#define LARGURA_TELA 800
#define ALTURA_TELA 640

#define MOVIMENTO 10.0
#define ESC 27

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define MARGEM_JOGO_Y 50 // Margem da área do jogo em si (sem barras em cima ou em baixo)
#define MARGEM_JOGO_X 10

#define TAMANHO_JOGADOR 15

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


void movimenta_jogador(int *posicao_x, int *posicao_y) {

    switch(GetKeyPressed()) {
        case KEY_RIGHT:
            if (*posicao_x < LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_JOGADOR - DIMENSAO_RETANGULO_BORDA) {
                *posicao_x += MOVIMENTO;
            }

            break;
        case KEY_LEFT:
            if (*posicao_x > MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA ) {
                *posicao_x -= MOVIMENTO;
            }

            break;
        case KEY_UP:
            // Regra para o jogador se mover somente no quarto inferior da tela
            if (*posicao_y > ((ALTURA_TELA - MARGEM_JOGO_Y) * 0.75)) {
                *posicao_y -= MOVIMENTO;
            }

            break;
        case KEY_DOWN:
            if (*posicao_y > 0 && *posicao_y < ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_JOGADOR - DIMENSAO_RETANGULO_BORDA) {
                *posicao_y += MOVIMENTO;
            }

            break;

        case ESC:
            CloseWindow();
            break;

        default:
            break;
    }
    printf("\n%d   %d", *posicao_x, *posicao_y);
}


int gera_posicao_random(int valor_maximo) {
    return (int) (rand() / (double) RAND_MAX * (valor_maximo + 1));
}


void gera_cogumelos() {
    // gerar N cogumelos chamando gera_posicao_random para X e Y
    // gera_posicao_random receberia largura e (altura * 0.75) da tela como maximos para X e Y
}

void desenha_jogador(int *posicao_x, int *posicao_y) {
    // estudar possibilidade de usar png
    // tambem pensei em fazer algo mais simples com retangulos pora agora
    DrawRectangle(*posicao_x, *posicao_y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
}

int main() {

    int posicao_x = LARGURA_TELA / 2, posicao_y = ALTURA_TELA * 0.80;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "milipede");
    SetTargetFPS(15);

    while (!WindowShouldClose()) {
        movimenta_jogador(&posicao_x, &posicao_y);

        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        gera_cogumelos();

        desenha_jogador(&posicao_x, &posicao_y);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
