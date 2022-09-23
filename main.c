#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"
#include "ranking.h"
#include "jogo.h"
#include "desenho.h"

int main() {
    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS_MAX] = {};
    ARANHA aranhas[NUM_ARANHAS_MAX] = {};
    MILIPEDE milipede = {};
    JOGADOR jogadores[NUM_JOGADORES] = {};
    Texture2D texturas[NUM_IMAGENS] = {};

    carregar_ranking(jogadores);
    ordenar_ranking(jogadores);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);
    carrega_texturas(texturas);
    SetExitKey(KEY_F1); // Para liberar o ESC, default da raylib

    game_loop(&fazendeiro, cogumelos, aranhas, jogadores, &milipede, texturas);
    descarrega_texturas(texturas);

    CloseWindow();

    return 0;
}
