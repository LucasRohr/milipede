#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

#define ENTER 10
#define LARGURA_TELA 810
#define ALTURA_TELA 640
#define FRAMERATE 60

#define MOVIMENTO 3
#define ESC 27

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define MARGEM_JOGO_Y 50 // Margem da �rea do jogo em si (sem barras em cima ou em baixo)
#define MARGEM_JOGO_X 10

#define TAMANHO_FONTE 30
#define TAMANHO_STR 30

#define TAMANHO_JOGADOR 15
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

typedef struct {
    COORD posicao;
    int direcao;
    char nome[TAMANHO_STR];
    int vidas;
    int tiros;
    int cogumelos_colhidos;
    int status;
    int doente;
} FAZENDEIRO;

typedef struct {
    COORD posicao;
    int status;
} COGUMELO;

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

<<<<<<< HEAD
    // Convers�o de int para str
    sprintf(itens_menu[1], "%d", fazendeiro.cogumelos_colhidos);
    sprintf(itens_menu[3], "%d", num_cogumelos - fazendeiro.cogumelos_colhidos);
    sprintf(itens_menu[5], "%d", fazendeiro.vidas);
    sprintf(itens_menu[7], "%d", fazendeiro.tiros);

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

void desenha_cogumelos(COGUMELO cogumelos[], int num_cogumelos){
    int i;

    for (i = 0; i < NUM_COGUMELOS; i++){
        DrawRectangle(cogumelos[i].posicao.x, cogumelos[i].posicao.y, TAMANHO_COGUMELO, TAMANHO_COGUMELO, GREEN);
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
            i++;
        }
    }
}

void gera_fazendeiro(FAZENDEIRO *fazendeiro){
    fazendeiro->posicao.x = LARGURA_TELA / 2;
    fazendeiro->posicao.y = ALTURA_TELA - MARGEM_JOGO_Y - DIMENSAO_RETANGULO_BORDA - TAMANHO_JOGADOR;
    strcpy(fazendeiro->nome, "Nome");
    fazendeiro->vidas = NUM_VIDAS;
    fazendeiro->tiros = NUM_TIROS;
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

int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam){
    // Fun��o geral para verificar colisao. Recebe as coordernadas de dois objetos e seu tamanho.
    int flag = 0;

    if((pos_a.x < pos_b.x + b_tam) && (pos_a.x + a_tam > pos_b.x) && pos_a.y < pos_b.y + b_tam && pos_a.y + a_tam > pos_b.y){
        flag = 1;
    }
    return flag;
}

int verifica_colisao_cogumelos(COORD posicao, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0, flag = 0;

    for(i = 0; i < num_cogumelos; i++){
        flag += verifica_colisao(posicao, TAMANHO_JOGADOR, cogumelos[i].posicao, TAMANHO_COGUMELO);
    }
    return flag;
}

int verifica_movimento(COORD posicao, COGUMELO cogumelos[]){
    // Verifica se � poss�vel o jogador fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se o movimento � poss�vel, 0 se n�o
    int flag = 1;
=======
void desenha_cogumelos(COGUMELO cogumelos[], int num_cogumelos){
    int i;
>>>>>>> main

    if(posicao.x > LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_JOGADOR){
        flag = 0; // Verifica se o jogador ultrapassa a parede da direita
    } else if(posicao.x < MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 0; // Verifica se o jogador ultrapassa a parede da esquerda
    } else if(posicao.y < ((ALTURA_TELA - MARGEM_JOGO_Y) * 0.75)){
        flag = 0; // Verifica se o jogador vai acima do 1/4 inferior da tela
    } else if(posicao.y > (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_JOGADOR)){
        flag = 0; // Verifica se o jogador ultrapassa a parede de baixo
    } else if(verifica_colisao_cogumelos(posicao, cogumelos, NUM_COGUMELOS)){
        flag = 0; // Verifica se o jogador colide com algum cogumelo
    }
<<<<<<< HEAD

    return flag;
}

void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]) {
    // Fun��o para ler inputs do jogador.
    COORD nova_posicao = {};

    switch(GetKeyPressed()) {
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

    if(IsKeyDown(KEY_RIGHT)){
        nova_posicao.x = fazendeiro->posicao.x + MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if (IsKeyDown(KEY_LEFT)){
        nova_posicao.x = fazendeiro->posicao.x - MOVIMENTO;
        nova_posicao.y = fazendeiro->posicao.y;
        if (verifica_movimento(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if (IsKeyDown(KEY_UP)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y - MOVIMENTO;
        if (verifica_movimento(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
    }
    if (IsKeyDown(KEY_DOWN)){
        nova_posicao.x = fazendeiro->posicao.x;
        nova_posicao.y = fazendeiro->posicao.y + MOVIMENTO;
        if (verifica_movimento(nova_posicao, cogumelos)) {
            fazendeiro->posicao = nova_posicao;
        }
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

    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS] = {};
    ARANHA aranhas[NUM_ARANHAS] = {};

    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(cogumelos, NUM_COGUMELOS);
    printf("%d", cogumelos[1].posicao.x);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);

    while (!WindowShouldClose()) {
        movimenta_jogador(&fazendeiro, cogumelos);
        move_aranhas(&fazendeiro, aranhas, NUM_ARANHAS);

        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, fazendeiro, NUM_COGUMELOS);

        desenha_jogador(fazendeiro);
        desenha_cogumelos(cogumelos, NUM_COGUMELOS);

        desenha_aranhas(aranhas, NUM_ARANHAS);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
