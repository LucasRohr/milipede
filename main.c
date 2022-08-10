#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessária para random

#define ENTER 10
#define LARGURA_TELA 800
#define ALTURA_TELA 450

#define MOVIMENTO 10.0
#define ESC 27

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define ALTURA_BARRA_TOPO 40
#define MARGEM_BARRA_TOPO 15

void desenha_moldura() {
    int i;
    // Constante usada para começar a desenhar a borda depois da altura que a barra do topo deve ocupar
    const int ALTURA_TOPO_COM_MARGEM = ALTURA_BARRA_TOPO + MARGEM_BARRA_TOPO

    for(i = 0; i < LARGURA_TELA; i++) {
        DrawRectangle(i + MARGEM_RETANGULO_BORDA, ALTURA_TOPO_COM_MARGEM, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, YELLOW);
    }

    for(i = ALTURA_TOPO_COM_MARGEM; i < ALTURA_TELA; i++) {
        DrawRectangle(0, i + MARGEM_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, YELLOW);
    }

    for(i = 0; i < LARGURA_TELA; i++) {
        DrawRectangle(i + MARGEM_RETANGULO_BORDA, ALTURA_TELA, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, YELLOW);
    }

    for(i = ALTURA_TOPO_COM_MARGEM; i < LARGURA_TELA; i++) {
        DrawRectangle(LARGURA_TELA, i + MARGEM_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, DIMENSAO_RETANGULO_BORDA, YELLOW);
    }
}

void movimenta_jogador(int *posicao_x, int *posicao_y) {
    switch(GetKeyPressed()) {
        case KEY_RIGHT:
            if (*posicao_x < LARGURA_TELA) {
                *posicao_x += MOVIMENTO;
            }

            break;
        case KEY_LEFT:
            if (*posicao_x > 0) {
                *posicao_x -= MOVIMENTO;
            }

            break;
        case KEY_UP:
            // Regra para o jogador se mover somente no quarto inferior da tela
            if (*posicao_y < (ALTURA_TELA / 4)) {
                *posicao_y += MOVIMENTO;
            }

            break;
        case KEY_DOWN:
            if (*posicao_y > 0) {
                *posicao_y -= MOVIMENTO;
            }

            break;
        case ESC:
            CloseWindow();
            break;
        default:
            break;
    }
}

int gera_posicao_random(int valor_maximo) {
    return (int) (rand() / (double) RAND_MAX * (valor_maximo + 1));
}


void gera_cogumelos() {
    // gerar N cogumelos chamando gera_posicao_random para X e Y
    // gera_posicao_random receberia largura e (altura * 0.75) da tela como maximos para X e Y
}

void desenha_jogador() {
    // estudar possibilidade de usar png
    // também pensei em fazer algo mais simples com retangulos pora agora
    DrawRectangle(posicao_x, posicao_y, 15, 15, RED);
}

int main(void) {
    int posicao_x = LARGURA_TELA / 2, posicao_y = ALTURA_TELA / 5;

    InitWindow(LARGURA_TELA, ALTURA_TELA, "raylib [core] example - basic window");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        movimenta_jogador(&posicao_x, &posicao_y);

        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        gera_cogumelos();

        desenha_jogador();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
