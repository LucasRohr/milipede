#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "definicoes.h"
#include "ranking.h"

void carrega_texturas(Texture2D texturas[]){
    texturas[I_TEXTURA_FAZENDEIRO] = LoadTexture("imagens/fazendeiro.png");
    texturas[I_TEXTURA_COGUMELO] = LoadTexture("imagens/cogumelo.png");
    texturas[I_TEXTURA_ARANHA] = LoadTexture("imagens/aranha.png");
    texturas[I_TEXTURA_MILIPEDE_CABECA] = LoadTexture("imagens/milipede-cabeca.png");
    texturas[I_TEXTURA_MILIPEDE_CORPO] = LoadTexture("imagens/milipede-corpo.png");
}

void descarrega_texturas(Texture2D texturas[]){
    UnloadTexture(texturas[I_TEXTURA_FAZENDEIRO]);
    UnloadTexture(texturas[I_TEXTURA_COGUMELO]);
    UnloadTexture(texturas[I_TEXTURA_ARANHA]);
    UnloadTexture(texturas[I_TEXTURA_MILIPEDE_CABECA]);
    UnloadTexture(texturas[I_TEXTURA_MILIPEDE_CORPO]);
}

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

void desenha_menu_inferior(char itens_menu[][TAMANHO_STR], FAZENDEIRO fazendeiro, int num_cogumelos){
    int i;
    float pos = 0;

    // Conversao de int para str
    sprintf(itens_menu[1], "%d", fazendeiro.cogumelos_colhidos);
    sprintf(itens_menu[3], "%d", num_cogumelos - fazendeiro.cogumelos_colhidos);
    sprintf(itens_menu[5], "%d", fazendeiro.vidas);
    sprintf(itens_menu[7], "%d", fazendeiro.tiros_restantes);

    // Loop para desenho do menu. Texto em cinza, n�meros em branco.
    for(i = 0; i < NUM_ITEMS_MENU * 2; i++){
        if(i % 2 == 0){
            DrawText(itens_menu[i], MARGEM_JOGO_X + (LARGURA_TELA) * pos, ALTURA_TELA - MARGEM_JOGO_Y + MARGEM_RETANGULO_BORDA, TAMANHO_FONTE, GRAY);
        } else {
            DrawText(itens_menu[i], MARGEM_JOGO_X + (LARGURA_TELA) * pos, ALTURA_TELA - MARGEM_JOGO_Y + MARGEM_RETANGULO_BORDA, TAMANHO_FONTE, WHITE);
        }
        pos = pos + 0.125;
    }
}

void desenha_contador_doente(char itens[][TAMANHO_STR], FAZENDEIRO fazendeiro){
    // Conversao de int para str
    sprintf(itens[0], "%d", fazendeiro.doente);
    sprintf(itens[1], "%d", fazendeiro.contador_doente);

    if (fazendeiro.doente){
        DrawText(itens[0], MARGEM_JOGO_X + MARGEM_RETANGULO_BORDA + DIMENSAO_RETANGULO_BORDA, MARGEM_JOGO_Y + MARGEM_RETANGULO_BORDA + DIMENSAO_RETANGULO_BORDA, TAMANHO_FONTE, GREEN);
        DrawText(itens[1], MARGEM_JOGO_X + MARGEM_RETANGULO_BORDA + DIMENSAO_RETANGULO_BORDA, MARGEM_JOGO_Y + MARGEM_RETANGULO_BORDA + DIMENSAO_RETANGULO_BORDA + TAMANHO_FONTE, TAMANHO_FONTE, WHITE);
    }
}

void desenha_jogador(FAZENDEIRO fazendeiro, Texture2D textura) {
    if (fazendeiro.status == paralisado) {
        DrawTexture(textura, fazendeiro.posicao.x, fazendeiro.posicao.y, RED);
    } else if (fazendeiro.doente) {
        DrawTexture(textura, fazendeiro.posicao.x, fazendeiro.posicao.y, GREEN);
    } else {
        DrawTexture(textura, fazendeiro.posicao.x, fazendeiro.posicao.y, WHITE);
    }


}

void desenha_tiros(TIRO tiros[], int num_tiros){
    int i;

    for (i = 0; i < num_tiros; i++){
        if(tiros[i].status){
            DrawRectangle(tiros[i].posicao.x, tiros[i].posicao.y, TAMANHO_TIRO, TAMANHO_TIRO, YELLOW);
        }
    }
}

void desenha_cogumelos(COGUMELO cogumelos[], int num_cogumelos, Texture2D textura){
    int i;

    for (i = 0; i < NUM_COGUMELOS; i++){
        if (cogumelos[i].status) {
            DrawTexture(textura, cogumelos[i].posicao.x, cogumelos[i].posicao.y, WHITE);
        }
    }
}

void desenha_ranking(JOGADOR jogadores[]){
    int i, num_jogadores_validos;
    Color cor;
    float pos = 0.2;

    char scores[NUM_JOGADORES - 1][TAMANHO_STR] = {"1º ", "2º ", "3º ", "4º ", "5º "};
    char buffer_pontos[TAMANHO_STR] = {};

    num_jogadores_validos = verifica_numero_jogadores(jogadores);

    for (i = 0; i < num_jogadores_validos; i++){
        sprintf(buffer_pontos, "%d", jogadores[i].pontos); // Converte pontos para str
        strcat(scores[i], jogadores[i].nome); // Concatena o nome na str
        strcat(scores[i], " : "); // Concatena o nome na str
        strcat(scores[i], buffer_pontos); // Concatena os nomes na str
    }

    DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, BLACK);
    DrawText("HALL OF FAME", LARGURA_TELA / 3, MARGEM_JOGO_Y, TAMANHO_FONTE, GOLD);

    for(i = 0; i < num_jogadores_validos; i++){
        switch(i){ // Seleciona cor dos jogadores baseado em sua classificacao
            case 0:
                cor = GOLD;
                break;
            case 1:
                cor = GRAY;
                break;
            default:
                cor = BROWN;
                break;
        }
        DrawText(scores[i], MARGEM_JOGO_X, ALTURA_TELA * pos, TAMANHO_FONTE, cor); // Escreve as pontuacoes
        pos += 0.1; // Pula uma linha
    }

}

void desenha_menu_pausa(char texto[], char input[]){
    DrawText("Pausado", LARGURA_TELA / 3, ALTURA_TELA / 3, TAMANHO_FONTE * 2, WHITE);
    DrawRectangle(0, ALTURA_TELA * 0.7, LARGURA_TELA, ALTURA_TELA * 0.7, WHITE); // Desenha margens brancas no menu de pausa
    DrawRectangle(MARGEM_JOGO_X, ALTURA_TELA * 0.7 + MARGEM_JOGO_X, LARGURA_TELA - MARGEM_JOGO_X * 2, ALTURA_TELA * 0.3 - MARGEM_JOGO_X * 2, BLACK); // Desenha retangulo preto atrás do texto
    DrawText(texto, MARGEM_JOGO_X * 2, ALTURA_TELA* 0.8, TAMANHO_FONTE, WHITE);
    DrawText(input, MARGEM_JOGO_X * 2, ALTURA_TELA * 0.9, TAMANHO_FONTE, WHITE);
}

void desenha_aranhas(ARANHA aranhas[], int total_aranhas, Texture2D textura) {
    int i;

    for(i = 0; i < total_aranhas; i++) {
        if (aranhas[i].status) {
            DrawTexture(textura, aranhas[i].posicao.x, aranhas[i].posicao.y, WHITE);
        }
    }
}

void desenha_milipede(MILIPEDE milipede, Texture2D textura_cabeca, Texture2D textura_corpo) {
    int i;

    DrawTexture(textura_cabeca, milipede.posicao_cabeca.x, milipede.posicao_cabeca.y, WHITE);
    if(milipede.status){
        for (i = 0; i < milipede.tamanho; i++) {
            if (milipede.dir == dir_mili) {
                DrawTexture(textura_corpo, milipede.posicao_cabeca.x - (TAMANHO_SEGMENTO_MILIPEDE * (i + 1)), milipede.posicao_cabeca.y, WHITE);
            } else {
                DrawTexture(textura_corpo, milipede.posicao_cabeca.x + (TAMANHO_SEGMENTO_MILIPEDE * (i + 1)), milipede.posicao_cabeca.y, WHITE);
            }
        }
    }

}
