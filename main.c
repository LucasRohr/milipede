#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define ENTER 10
#define LARGURA_TELA 810
#define ALTURA_TELA 640
#define FRAMERATE 60

#define MOVIMENTO 3
#define MOVIMENTO_TIRO 6
#define ESC 27

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define MARGEM_JOGO_Y 50 // Margem da �rea do jogo em si (sem barras em cima ou em baixo)
#define MARGEM_JOGO_X 10

#define TAMANHO_FONTE 30
#define TAMANHO_STR 30

#define TAMANHO_JOGADOR 15
#define TAMANHO_TIRO 5
#define TAMANHO_COGUMELO 15

#define MARGEM_COGUMELO 5

#define NUM_ITEMS_MENU 4

#define NUM_COGUMELOS 60
#define NUM_VIDAS 3
#define NUM_TIROS 200

// Defini��o estruturas

typedef struct {
    int x;
    int y;
} COORD;

typedef enum {
    esq = 6,
    dir = 2,
    cima = 0,
    baixo = 4,
    dir_cima = 1,
    dir_baixo = 3,
    esq_cima = 7,
    esq_baixo = 5,
    null = -1,
} DIRECAO;

typedef struct {
    COORD posicao;
    DIRECAO direcao;
    int status;
} TIRO;



typedef struct {
    COORD posicao;
    int status;
} COGUMELO;

typedef struct {
    COORD posicao;
    DIRECAO direcao;
    char nome[TAMANHO_STR];
    int vidas;
    TIRO tiros[NUM_TIROS];
    int tiros_restantes;
    int cogumelos_colhidos;
    int status;
    int doente;
} FAZENDEIRO;

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

    // Convers�o de int para str
    sprintf(itens_menu[1], "%d", fazendeiro.cogumelos_colhidos);
    sprintf(itens_menu[3], "%d", num_cogumelos - fazendeiro.cogumelos_colhidos);
    sprintf(itens_menu[5], "%d", fazendeiro.vidas);
    sprintf(itens_menu[7], "%d", fazendeiro.tiros_restantes);

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

void desenha_jogador(FAZENDEIRO fazendeiro) {
    // estudar possibilidade de usar png
    // tambem pensei em fazer algo mais simples com retangulos pora agora
    DrawRectangle(fazendeiro.posicao.x, fazendeiro.posicao.y, TAMANHO_JOGADOR, TAMANHO_JOGADOR, RED);
}

void desenha_tiros(TIRO tiros[], int num_tiros){
    int i;

    for (i = 0; i < num_tiros; i++){
        if(tiros[i].status){
            DrawRectangle(tiros[i].posicao.x, tiros[i].posicao.y, TAMANHO_TIRO, TAMANHO_TIRO, YELLOW);
        }
    }
}

void desenha_cogumelos(COGUMELO cogumelos[], int num_cogumelos){
    int i;

    for (i = 0; i < num_cogumelos; i++){
        if (cogumelos[i].status){
            DrawRectangle(cogumelos[i].posicao.x, cogumelos[i].posicao.y, TAMANHO_COGUMELO, TAMANHO_COGUMELO, GREEN);
        }
    }
}

int gera_posicao_random(int valor_minimo, int valor_maximo) {
    // Gera valores aleat�rios entre o valor minimo e maximo
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo);
}

int gera_posicao_cogumelos(int valor_minimo, int valor_maximo) {
    // Gera valores aleat�rios, divisiveis pelo tamanho dos cogumelos + sua margem (para eles ficarem em seus pr�prios tiles)
    return (int) (rand() % (valor_maximo - valor_minimo + 1) + valor_minimo) / (TAMANHO_COGUMELO + MARGEM_COGUMELO) * (TAMANHO_COGUMELO + MARGEM_COGUMELO);
}

int verifica_posicao_cogumelos(COGUMELO cogumelos[], int num_cogumelos, int x, int y){
    // Se as coordenadas recebidas j� est�o ocupadas por um cogumelo, retorna 1, sen�o 0. Para de percorrer o array quando chega ao final, ou encontra o valor 0.
    int i = 0, flag = 0;

    while(i < num_cogumelos && cogumelos[i].posicao.x != 0){
        if (cogumelos[i].posicao.x == x && cogumelos[i].posicao.y == y){
            flag = 1;
        }
        i++;
    }
    return flag;
}

void gera_cogumelos(COGUMELO cogumelos[], int num_cogumelos) {
    // Gera cogumelos em posi��es aleat�rias.
    int i = 0, x, y;
    srand(time(0));

    while (i < num_cogumelos){
        x = gera_posicao_cogumelos(MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA + TAMANHO_COGUMELO, LARGURA_TELA - MARGEM_JOGO_X - DIMENSAO_RETANGULO_BORDA);
        y = gera_posicao_cogumelos(MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA + TAMANHO_JOGADOR * 2, ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR * 2);
        // Verifica se a posi��o j� est� ocupada. Se n�o, gera uma nova posi��o para o cogumelo.

        if (!verifica_posicao_cogumelos(cogumelos, num_cogumelos, x, y)){
            cogumelos[i].posicao.x = x;
            cogumelos[i].posicao.y = y;
            cogumelos[i].status = 1;
            i++;
        }
    }
}

void gera_fazendeiro(FAZENDEIRO *fazendeiro){
    fazendeiro->posicao.x = LARGURA_TELA / 2;
    fazendeiro->posicao.y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    fazendeiro->direcao = cima;
    strcpy(fazendeiro->nome, "Nome");
    fazendeiro->vidas = NUM_VIDAS;
    fazendeiro->tiros_restantes = NUM_TIROS;
    fazendeiro->cogumelos_colhidos = 0;
    fazendeiro->status = 0;
    fazendeiro->doente = 0;
}

void pausar_jogo(){
    printf("\nPausar");
}

void carregar_jogo(){
    printf("\nCarregar");
}

void mostrar_ranking(){
    printf("\nRanking");

}

void atirar(FAZENDEIRO *fazendeiro){
    int tiro_atual = NUM_TIROS - fazendeiro->tiros_restantes;

    fazendeiro->tiros[tiro_atual].posicao.x = fazendeiro->posicao.x + TAMANHO_JOGADOR / 2;
    fazendeiro->tiros[tiro_atual].posicao.y = fazendeiro->posicao.y + TAMANHO_JOGADOR / 2;
    fazendeiro->tiros[tiro_atual].status = 1;
    fazendeiro->tiros[tiro_atual].direcao = fazendeiro->direcao;
    fazendeiro->tiros_restantes -= 1;
}

int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam){
    // Fun��o geral para verificar colisao. Recebe as coordernadas de dois objetos e seu tamanho.
    int flag = 0;

    if((pos_a.x < pos_b.x + b_tam) && (pos_a.x + a_tam > pos_b.x) && pos_a.y < pos_b.y + b_tam && pos_a.y + a_tam > pos_b.y){
        flag = 1;
    }
    return flag;
}

int verifica_colisao_cogumelos(COORD posicao, int tamanho_objeto, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao de algum objeto com qualquer cogumelo.
    int i = 0, flag = 0;

    for(i = 0; i < num_cogumelos; i++){
        if (verifica_colisao(posicao, tamanho_objeto, cogumelos[i].posicao, TAMANHO_COGUMELO) && cogumelos[i].status){
            flag += verifica_colisao(posicao, tamanho_objeto, cogumelos[i].posicao, TAMANHO_COGUMELO);
        }
    }
    return flag;
}

void acertou_cogumelo(COORD posicao, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0;

    for(i = 0; i < num_cogumelos; i++){
        if (verifica_colisao(posicao, TAMANHO_TIRO, cogumelos[i].posicao, TAMANHO_COGUMELO) && cogumelos[i].status){
            cogumelos[i].status = 0;
        }
    }
}

void verifica_impacto_tiro(TIRO *tiro, int *cogumelos_colhidos, COGUMELO cogumelos[]){
    if(tiro->posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(tiro->posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(tiro->posicao.y < MARGEM_JOGO_Y + DIMENSAO_RETANGULO_BORDA){
        tiro->status = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(tiro->posicao.y > ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_TIRO){
        tiro->status = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(tiro->posicao, TAMANHO_TIRO, cogumelos, NUM_COGUMELOS)){
        tiro->status = 0; // Verifica se o tiro colide com algum cogumelo.
        *cogumelos_colhidos += 1;
        acertou_cogumelo(tiro->posicao, cogumelos, NUM_COGUMELOS);
    }
}

void verifica_tiros(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]){
    int i;

    for (i = 0; i < NUM_TIROS; i++){
        if (fazendeiro->tiros[i].status){
            verifica_impacto_tiro(&fazendeiro->tiros[i], &fazendeiro->cogumelos_colhidos, cogumelos);
        }
    }
}

void movimenta_tiros(TIRO tiros[]){
    int i;
    int mov_diagonal = MOVIMENTO_TIRO * 0.707;

    for (i=0; i < NUM_TIROS; i++){
        if(tiros[i].status){
            switch(tiros[i].direcao){
                case cima:
                    tiros[i].posicao.y -= MOVIMENTO_TIRO;
                    break;
                case dir_cima:
                    tiros[i].posicao.x += mov_diagonal;
                    tiros[i].posicao.y -= mov_diagonal;
                    break;
                case dir:
                    tiros[i].posicao.x += MOVIMENTO_TIRO;
                    break;
                case dir_baixo:
                    tiros[i].posicao.x += mov_diagonal;
                    tiros[i].posicao.y += mov_diagonal;
                    break;
                case baixo:
                    tiros[i].posicao.y += MOVIMENTO_TIRO;
                    break;
                case esq_baixo:
                    tiros[i].posicao.x -= mov_diagonal;
                    tiros[i].posicao.y += mov_diagonal;
                    break;
                case esq:
                    tiros[i].posicao.x -= MOVIMENTO_TIRO;
                    break;
                case esq_cima:
                    tiros[i].posicao.x -= mov_diagonal;
                    tiros[i].posicao.y -= mov_diagonal;
                    break;
                default:
                    break;
            }
        }
    }
}

int verifica_movimento_jogador(COORD posicao, COGUMELO cogumelos[]){
    // Verifica se � poss�vel o jogador fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se o movimento � poss�vel, 0 se n�o
    int flag = 1;

    if(posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_JOGADOR){
        flag = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(posicao.y < ((ALTURA_TELA - MARGEM_JOGO_Y) * 0.75)){
        flag = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(posicao.y > (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_JOGADOR)){
        flag = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(posicao, TAMANHO_JOGADOR, cogumelos, NUM_COGUMELOS)){
        flag = 0; // Verifica se o jogador colide com algum cogumelo
    }
    return flag;
}

void muda_direcao_jogador(DIRECAO *direcao){
    if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_UP)){
        *direcao = dir_cima;
    } else if(IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_DOWN)){
        *direcao = dir_baixo;
    } else if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_DOWN)){
        *direcao = esq_baixo;
    } else if(IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_UP)){
        *direcao = esq_cima;
    } else if(IsKeyDown(KEY_UP)){
        *direcao = cima;
    } else if(IsKeyDown(KEY_RIGHT)){
        *direcao = dir;
    } else if(IsKeyDown(KEY_DOWN)){
        *direcao = baixo;
    } else if(IsKeyDown(KEY_LEFT)){
        *direcao = esq;
    }
}

void comandos_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]) {
    // Fun��o para ler inputs do jogador.
    COORD nova_posicao = {};

    if(IsKeyDown(KEY_UP)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y - MOVIMENTO;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_RIGHT)){
        nova_posicao.x = fazendeiro->posicao.x + MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_DOWN)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y + MOVIMENTO;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if(IsKeyDown(KEY_LEFT)){
        nova_posicao.x = fazendeiro->posicao.x - MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento_jogador(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }

    muda_direcao_jogador(&fazendeiro->direcao);


    switch(GetKeyPressed()) {
        case KEY_SPACE:
            if (fazendeiro->tiros_restantes > 0){
                atirar(fazendeiro);
            }
            break;
        case KEY_P:
            pausar_jogo();
            break;
        case KEY_C:
            carregar_jogo();
            break;
        case KEY_R:
            mostrar_ranking();
            break;
        case ESC:
            CloseWindow();
            break;
        default:
            break;
    }
}

int main() {

    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS] = {};

    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(cogumelos, NUM_COGUMELOS);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);

        comandos_jogador(&fazendeiro, cogumelos);
        movimenta_tiros(fazendeiro.tiros);
        verifica_tiros(&fazendeiro, cogumelos);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, fazendeiro, NUM_COGUMELOS);
        desenha_tiros(fazendeiro.tiros, NUM_TIROS);
        desenha_jogador(fazendeiro);
        desenha_cogumelos(cogumelos, NUM_COGUMELOS);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
