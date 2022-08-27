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

void desenha_menu_inferior(char itens_menu[][TAMANHO_STR], FAZENDEIRO *fazendeiro, int num_cogumelos){
    int i;
    float pos = 0;

    // Convers�o de int para str
    sprintf(itens_menu[1], "%d", fazendeiro->cogumelos_colhidos);
    sprintf(itens_menu[3], "%d", num_cogumelos - fazendeiro->cogumelos_colhidos);
    sprintf(itens_menu[5], "%d", fazendeiro->vidas);
    sprintf(itens_menu[7], "%d", fazendeiro->tiros);

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
    Image fazendeiro_imagem = LoadImage("imagens/fazendeiro.png");

    if (fazendeiro.status == paralisado) {
        ImageColorInvert(&fazendeiro_imagem);
    }

    Texture2D textura_fazendeiro = LoadTextureFromImage(fazendeiro_imagem); 

    DrawTexture(textura_fazendeiro, fazendeiro.posicao.x, fazendeiro.posicao.y, WHITE);

    UnloadImage(fazendeiro_imagem);
}

void desenha_cogumelos(COGUMELO cogumelos[], int num_cogumelos){
    int i;

    Image cogumelo_imagem = LoadImage("imagens/cogumelo.png");
    Texture2D textura_cogumelo = LoadTextureFromImage(cogumelo_imagem); 

    UnloadImage(cogumelo_imagem);

    for (i = 0; i < NUM_COGUMELOS; i++){
        if (cogumelos[i].status == 1) {
            DrawTexture(textura_cogumelo, cogumelos[i].posicao.x, cogumelos[i].posicao.y, WHITE);
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
    strcpy(fazendeiro->nome, "Nome");
    fazendeiro->vidas = NUM_VIDAS;
    fazendeiro->tiros = NUM_TIROS;
    fazendeiro->cogumelos_colhidos = 0;
    fazendeiro->status = livre;
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

// Fun��o geral para verificar colisao. Recebe as coordernadas de dois objetos e seu tamanho.
int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam){
    if((pos_a.x < pos_b.x + b_tam) && (pos_a.x + a_tam > pos_b.x) && (pos_a.y < pos_b.y + b_tam) && (pos_a.y + a_tam > pos_b.y)){
        return 1;
    }

    return 0;
}

int verifica_colisao_cogumelos(COORD posicao, COGUMELO cogumelos[], int num_cogumelos){
    // Verifica a colisao para cada cogumelo.
    int i = 0, flag = 0;

    for(i = 0; i < num_cogumelos; i++){
        if(cogumelos[i].status == 1) {
            flag += verifica_colisao(posicao, TAMANHO_JOGADOR, cogumelos[i].posicao, TAMANHO_COGUMELO);
        }
    }

    return flag;
}

int verifica_movimento(COORD posicao, COGUMELO cogumelos[]){
    // Verifica se é possível o jogador fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se o movimento é possível, 0 se não
    int flag = 1;

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

    return flag;
}

void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]) {
    // Fun��o para ler inputs do jogador.
    COORD nova_posicao = {};

    if (fazendeiro->status == livre) {
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
}

// Funcoes para Aranhas

void gera_aranha(ARANHA *aranha) {
    float x_random = (float) (rand() / (double) RAND_MAX * (LARGURA_TELA - (2 * MARGEM_JOGO_X)));

    aranha->posicao.x = x_random;
    aranha->posicao.y = MARGEM_JOGO_Y + TAMANHO_ARANHA;
    aranha->status = 1;
    aranha->dir =  (int) (rand() % (esq - dir + 1)) + dir_baixo;
}

void inverte_movimento(ARANHA *aranha) {
    DIRECAO direcao_aranha = aranha->dir;

    switch(direcao_aranha) {
        case(cima):
            aranha->dir = (int) (rand() % (esq_baixo - dir_baixo + 1)) + dir_baixo;
            break;
        case(dir_cima):
            aranha->dir = (int) (rand() % (esq_cima - dir_baixo + 1)) + dir_baixo;
            break;
        case(dir):
            aranha->dir = (int) (rand() % (esq_cima - esq_baixo + 1)) + esq_baixo;
            break;
        case(dir_baixo):
            aranha->dir = (int) (rand() % (esq_cima - esq_baixo + 1)) + esq_baixo;
            break;
        case(baixo):
            aranha->dir = esq_cima;
            break;
        case(esq_baixo):
            aranha->dir = (int) (rand() % (dir_baixo - cima + 1)) + cima;
            break;
        case(esq):
            aranha->dir = (int) (rand() % (dir_baixo - dir_cima + 1)) + dir_cima;
            break;
        case(esq_cima):
            aranha->dir = (int) (rand() % (esq_baixo - dir_cima + 1)) + dir_cima;
            break;
        default:
            break;
    }
}

// Verifica a colisao para cada cogumelo.
void verifica_colisao_aranha_cogumelos(ARANHA aranha, COGUMELO cogumelos[], int num_cogumelos){
    int i = 0;

    for(i = 0; i < num_cogumelos; i++) {
        if(aranha.status == 1 && verifica_colisao(aranha.posicao, TAMANHO_ARANHA, cogumelos[i].posicao, TAMANHO_COGUMELO) ) {
            cogumelos[i].status = 0;
        }
    }
}

int verifica_movimento_aranha(COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro) {
    // Verifica se é possível a aranha fazer algum movimento. Recebe a nova posicao, nao a atual.
    // Retorna 1 se colide com um obstaculo, 0 se não
    int flag = 0;

    if(posicao.x >= LARGURA_TELA - MARGEM_JOGO_X - TAMANHO_ARANHA) {
        flag = 1; // Verifica se a aranha colide com a parede da direita
    } else if(posicao.x <= MARGEM_JOGO_X + DIMENSAO_RETANGULO_BORDA) {
        flag = 1; // Verifica se a aranha colide com a parede da esquerda
    } else if(posicao.y <= MARGEM_JOGO_Y) {
        flag = 1; // Verifica se a aranha vai acima do topo da tela
    } else if(posicao.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_ARANHA)) {
        flag = 1; // Verifica se a aranha colide a parede de baixo
    } else if(verifica_colisao(posicao, TAMANHO_ARANHA, fazendeiro.posicao, TAMANHO_ARANHA)) {
        flag = 1; // Verifica se a aranha colide com o fazendeiro
    }

    return flag;
}

void move_aranha(ARANHA *aranha, COGUMELO cogumelos[], FAZENDEIRO fazendeiro) {
    DIRECAO direcao_aranha = aranha->dir;

    verifica_colisao_aranha_cogumelos(*aranha, cogumelos, NUM_COGUMELOS);

    switch(direcao_aranha) {
        case cima:
            aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case dir_cima:
            aranha->posicao.x += PASSO_ARANHA;
            aranha->posicao.y += PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case dir:
            aranha->posicao.x += PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case dir_baixo:
            aranha->posicao.x += PASSO_ARANHA;
            aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case baixo:
            aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case esq_baixo:
            aranha->posicao.x -= PASSO_ARANHA;
            aranha->posicao.y -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case esq:
            aranha->posicao.x -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        case esq_cima:
            aranha->posicao.y += PASSO_ARANHA;
            aranha->posicao.x -= PASSO_ARANHA;

            if(verifica_movimento_aranha(aranha->posicao, cogumelos, fazendeiro)) {
                inverte_movimento(aranha);
            }

            break;
        default:
            break;
    }
}

void testa_colisao_aranha_base(ARANHA *aranha) {
    if (aranha->posicao.y >= (ALTURA_TELA - MARGEM_JOGO_Y - TAMANHO_ARANHA)) {
        aranha->status = 0;
    }
}

void move_aranhas(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int total_aranhas) {
    int i, colidiu_com_fazendeiro;

    for(i = 0; i < total_aranhas; i++) {
        move_aranha(&(aranhas[i]), cogumelos, *fazendeiro);

        colidiu_com_fazendeiro = verifica_colisao(aranhas[i].posicao, TAMANHO_ARANHA, fazendeiro->posicao, TAMANHO_JOGADOR);
        fazendeiro->doente += colidiu_com_fazendeiro;

        if (colidiu_com_fazendeiro) {
            fazendeiro->status = paralisado;
            fazendeiro->vidas -= 1;

            WaitTime(3);

            // fazendeiro->status = livre;
        }

        testa_colisao_aranha_base(&(aranhas[i]));

        WaitTime(1);
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

    Image aranha = LoadImage("imagens/aranha.png");
    Texture2D textura_aranha = LoadTextureFromImage(aranha); 

    UnloadImage(aranha);

    for(i = 0; i < total_aranhas; i++) {
        if (aranhas[i].status == 1) {
            DrawTexture(textura_aranha, aranhas[i].posicao.x, aranhas[i].posicao.y, WHITE);
        }
    }
}

// === Fim das Funcoes para Aranhas ===

int conta_cogumelos_restantes(COGUMELO cogumelos[], int total_cogumelos) {
    int i, contador = 0;

    for(i = 0; i < total_cogumelos; i++) {
        if (cogumelos[i].status == 1) {
            contador++;
        }
    }

    return contador;
}

int main() {
    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS] = {};
    ARANHA aranhas[NUM_ARANHAS] = {};

    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(cogumelos, NUM_COGUMELOS);
    gera_todas_aranhas(aranhas, NUM_ARANHAS);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);

    // EnableEventWaiting();

    while (!WindowShouldClose()) {
        movimenta_jogador(&fazendeiro, cogumelos);
        move_aranhas(&fazendeiro, aranhas, cogumelos, NUM_ARANHAS);

        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, &fazendeiro, conta_cogumelos_restantes(cogumelos, NUM_COGUMELOS));

        desenha_jogador(fazendeiro);
        desenha_cogumelos(cogumelos, NUM_COGUMELOS);
        desenha_aranhas(aranhas, NUM_ARANHAS);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
