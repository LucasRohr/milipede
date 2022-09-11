#include <stdio.h>
#include "raylib.h"
#include <time.h> // Biblioteca necessaria para random
#include <math.h>
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
    Image fazendeiro_imagem = LoadImage("imagens/fazendeiro.png");

    if (fazendeiro.status == paralisado) {
        ImageColorInvert(&fazendeiro_imagem);
    }

    Texture2D textura_fazendeiro = LoadTextureFromImage(fazendeiro_imagem);

    DrawTexture(textura_fazendeiro, fazendeiro.posicao.x, fazendeiro.posicao.y, WHITE);

    UnloadImage(fazendeiro_imagem);
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

    Image cogumelo_imagem = LoadImage("imagens/cogumelo.png");
    Texture2D textura_cogumelo = LoadTextureFromImage(cogumelo_imagem);

    UnloadImage(cogumelo_imagem);

    for (i = 0; i < NUM_COGUMELOS; i++){
        if (cogumelos[i].status) {
            DrawTexture(textura_cogumelo, cogumelos[i].posicao.x, cogumelos[i].posicao.y, WHITE);
        }
    }
}

int instanciar_nome(char input[], int *num_letras){
    int tecla = GetCharPressed();

    while (tecla > 0) {
        if ((tecla >= 32) && (tecla <= 125) && (*num_letras < TAMANHO_NOME)) {
            input[*num_letras] = (char)tecla;
            input[*num_letras + 1] = '\0'; // Adiciona \0 ao fim da string
            *num_letras = *num_letras + 1;
        }
        tecla = GetCharPressed();  // Verifica proximo caractere
    }
    if (IsKeyPressed(KEY_BACKSPACE)){ // Apagar ultimo caractere se pressionar backspace
        *num_letras = *num_letras - 1;

        if (*num_letras < 0) {
            *num_letras = 0;
        }
        input[*num_letras] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER)){
        return 1; // Se apertar enter, retorna 1
    } else {
        return 0;
    }
}

int verifica_numero_jogadores(JOGADOR jogadores[]){
    int i, num_jogadores_validos = 0;

    // Verifica quantos jogadores lidos tem pontuação diferente de 0, ou seja, são pontuacoes válidas
    for(i = 0; i < NUM_JOGADORES - 1; i++){
        if(jogadores[i].pontos){
            num_jogadores_validos++;
        }
    }

    return num_jogadores_validos;
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
    strcpy(fazendeiro->nome, "Jogador");
    fazendeiro->vidas = NUM_VIDAS;
    fazendeiro->tiros_restantes = NUM_TIROS;
    fazendeiro->cogumelos_colhidos = 0;
    fazendeiro->status = livre;
    fazendeiro->doente = 0;
}

ESTADO_JOGO cria_save(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[]){
    int i;
    ESTADO_JOGO save;

    save.fazendeiro = *fazendeiro;
    for (i = 0; i < NUM_ARANHAS; i++){
        save.aranhas[i] = aranhas[i];
    }
    for(i = 0; i < NUM_COGUMELOS; i++){
        save.cogumelos[i] = cogumelos[i];
    }
    return (save);
}

int salvar_jogo(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[]){

    ESTADO_JOGO save = cria_save(fazendeiro, aranhas, cogumelos);
    FILE *arq;
    char nome_arquivo[TAMANHO_STR] = "";

    // Gera o nome do arquivo, formato <nome_jogador>.bin
    strcat(nome_arquivo, fazendeiro->nome);
    strcat(nome_arquivo, ".bin");

    arq = fopen(nome_arquivo, "wb");

    if (arq){
        if (fwrite(&save, sizeof(ESTADO_JOGO), 1, arq) != 1){
            return 0;
        }
    } else {
        return 0;
    }

    fclose(arq);

    return 1;
}

void menu_pausa(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], char input[], int *num_letras, STATUS_JOGO *status_jogo){

    DrawText("Pausado", LARGURA_TELA / 3, ALTURA_TELA / 3, TAMANHO_FONTE * 2, WHITE);
    DrawRectangle(0, ALTURA_TELA * 0.7, LARGURA_TELA, ALTURA_TELA * 0.7, WHITE); // Desenha margens brancas no menu de pausa
    DrawRectangle(MARGEM_JOGO_X, ALTURA_TELA * 0.7 + MARGEM_JOGO_X, LARGURA_TELA - MARGEM_JOGO_X * 2, ALTURA_TELA * 0.3 - MARGEM_JOGO_X * 2, BLACK); // Desenha retangulo preto atrás do texto
    DrawText("Digite o nome para salvar e voltar ao jogo", MARGEM_JOGO_X * 2, ALTURA_TELA* 0.8, TAMANHO_FONTE, WHITE);
    DrawText(input, MARGEM_JOGO_X * 2, ALTURA_TELA * 0.9, TAMANHO_FONTE, WHITE);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome para salvar o jogo e instanciar o nome do fazendeiro
        strcpy(fazendeiro->nome, input);
        if(!salvar_jogo(fazendeiro, aranhas, cogumelos)){
            printf("\n\nErro ao salvar jogo.\n\n");
        }
        *status_jogo = normal;
    }

}

void instanciar_jogo(ESTADO_JOGO save, FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[]){
    int i;

    *fazendeiro = save.fazendeiro;

    for (i = 0; i < NUM_ARANHAS; i++){
        aranhas[i] = save.aranhas[i];
    }
    for(i = 0; i < NUM_COGUMELOS; i++){
        cogumelos[i] = save.cogumelos[i];
    }
}

int carregar_jogo(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], char input[]){
    ESTADO_JOGO save;
    FILE *arq;

    char nome_arquivo[TAMANHO_STR] = "";

    // Gera o nome do arquivo, formato <nome_jogador>.bin
    strcat(nome_arquivo, input);
    strcat(nome_arquivo, ".bin");

    arq = fopen(nome_arquivo, "rb");
    if (arq){ // Se o arquivo existe, instancia o jogo.
        if (fread(&save, sizeof(ESTADO_JOGO), 1, arq) != 1){
            printf("\n\nErro na leitura de arquivo\n\n");
        }
        instanciar_jogo(save, fazendeiro, aranhas, cogumelos);
    } else {
        printf("\n\nErro na abertura de arquivo\n\n");
    }

    fclose(arq);

    return 0;
}

void menu_carregar(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], char input[], int *num_letras, STATUS_JOGO *status_jogo){
    DrawText("Pausado", LARGURA_TELA / 3, ALTURA_TELA / 3, TAMANHO_FONTE * 2, WHITE);
    DrawRectangle(0, ALTURA_TELA * 0.7, LARGURA_TELA, ALTURA_TELA * 0.7, WHITE); // Desenha margens brancas no menu de pausa
    DrawRectangle(MARGEM_JOGO_X, ALTURA_TELA * 0.7 + MARGEM_JOGO_X, LARGURA_TELA - MARGEM_JOGO_X * 2, ALTURA_TELA * 0.3 - MARGEM_JOGO_X * 2, BLACK); // Desenha retangulo preto atrás do texto
    DrawText("Digite o nome do jogo para carregar.", MARGEM_JOGO_X * 2, ALTURA_TELA* 0.8, TAMANHO_FONTE, WHITE);
    DrawText(input, MARGEM_JOGO_X * 2, ALTURA_TELA * 0.9, TAMANHO_FONTE, WHITE);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome para carregar o jogo
        carregar_jogo(fazendeiro, aranhas, cogumelos, input);
        *status_jogo = normal;
    }
}

void inserir_jogador_atual(FAZENDEIRO fazendeiro, JOGADOR jogadores[]){
    int i = 0, encontrado = 0;
    JOGADOR jogador_atual;

    strcpy(jogador_atual.nome, fazendeiro.nome);
    jogador_atual.pontos = fazendeiro.cogumelos_colhidos;

    while(jogadores[i].pontos != 0){
        if (!strcmp(jogadores[i].nome, fazendeiro.nome)){ // Se o jogador atual já está na lista de jogadores, flag encontrado para 1
            if (fazendeiro.cogumelos_colhidos > jogadores[i].pontos){ // Se a pontuacao atual é maior que a que já esta no ranking, atualiza ela.
                jogadores[i].pontos = fazendeiro.cogumelos_colhidos;
            }
            encontrado = 1;
        }
        i++;
    }

    if(!encontrado){ // Se o jogador não está na lista de jogadores, insere ele na ultima posicao
        jogadores[NUM_JOGADORES - 1] = jogador_atual;
    }
}

void ordenar_ranking(JOGADOR jogadores[]){
    JOGADOR buffer;
    int i, j;

    // Bubble sort para o ranking de jogadores
    for(i = 0; i < NUM_JOGADORES; i++){
        for (j = 0; j < NUM_JOGADORES - i - 1; j++){

            if (jogadores[j].pontos < jogadores[j+1].pontos) {
                buffer = jogadores[j];
                jogadores[j] = jogadores[j + 1];
                jogadores[j + 1] = buffer;
            }
        }
    }
}

void mostrar_ranking(FAZENDEIRO *fazendeiro, JOGADOR jogadores[], STATUS_JOGO *status_jogo){
    // Se ja esta mostrando o ranking, volta ao jogo. Senao, insere o jogador atual, ordena e mostra o ranking.
    if(*status_jogo == mostrando_ranking){
        *status_jogo = normal;
    } else {
        *status_jogo = mostrando_ranking;
    }

}

int carregar_ranking(JOGADOR jogadores[]){
    FILE *arq;
    int i = 0 , erro = 0;
    char buffer_str[TAMANHO_STR];

    arq = fopen("ranking.txt" , "r");

    if(!arq){
        printf("\n\nErro na abertura de arquivo.\n\n");
        erro+= 1;
    } else {
        while(i < NUM_JOGADORES - 1 && !feof(arq)){
            if(fgets(buffer_str, TAMANHO_STR, arq) == NULL){
                printf("\n\nErro na leitura de arquivo.\n\n");
                erro += 1;
            } else {
                buffer_str[TAMANHO_STR - 1] = '\0'; // Insere \0 no final da string
                strcpy(jogadores[i].nome, strtok(buffer_str, ";")); // Le o nome do jogador (até encontrar um ;)
                jogadores[i].pontos = atoi(strtok(NULL, ";")); // Le a pontuacao do jogador e converte para int.
                i++;
            }
        }
    }

    fclose(arq);
    return erro;
}

int salvar_ranking(JOGADOR jogadores[]){
    FILE *arq;
    int i = 0, erro = 0;


    arq = fopen("ranking.txt", "w");

    if (arq == NULL){
        erro += 1;
    } else{
        while (i < verifica_numero_jogadores(jogadores)){
            if(fprintf(arq, "%s;%d\n", jogadores[i].nome, jogadores[i].pontos) < 0){ // Printa jogador no arquivo, formato <nome_jogador>;<pontuacao>
                erro += 1;
            }
            i++;
        }
    }

    fclose(arq);
    return erro;
}

void menu_sair(FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair) {
    DrawText("Saindo", LARGURA_TELA / 3, ALTURA_TELA / 3, TAMANHO_FONTE * 2, WHITE);
    DrawRectangle(0, ALTURA_TELA * 0.7, LARGURA_TELA, ALTURA_TELA * 0.7, WHITE); // Desenha margens brancas no menu de pausa
    DrawRectangle(MARGEM_JOGO_X, ALTURA_TELA * 0.7 + MARGEM_JOGO_X, LARGURA_TELA - MARGEM_JOGO_X * 2, ALTURA_TELA * 0.3 - MARGEM_JOGO_X * 2, BLACK); // Desenha retangulo preto atrás do texto
    DrawText("Digite o nome antes de sair (cancelar com ESC)", MARGEM_JOGO_X * 2, ALTURA_TELA* 0.8, TAMANHO_FONTE, WHITE);
    DrawText(input, MARGEM_JOGO_X * 2, ALTURA_TELA * 0.9, TAMANHO_FONTE, WHITE);

    if(instanciar_nome(input, num_letras)){ // Verifica se o jogador acabou de digitar o nome, para sair o jogo e atualizar o ranking
        strcpy(fazendeiro->nome, input);
        inserir_jogador_atual(*fazendeiro, jogadores);
        ordenar_ranking(jogadores);
        if (salvar_ranking(jogadores)){
            printf("\n\nErro ao salvar ranking.\n\n");
        }
        *sair = 1;
    }
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
    // Verifica a colisao para cada cogumelo.
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

void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]) {
    // Funcao para movimentar o jogador.
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
}

void comandos_jogador(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int num_cogumelos, JOGADOR jogadores[], STATUS_JOGO *status_jogo){
    // Funcao para ler inputs do jogador que nao sao as teclas de movimento

    switch(GetKeyPressed()) {

        case KEY_SPACE:
            if (fazendeiro->tiros_restantes > 0 && fazendeiro->status == livre && *status_jogo == normal){
                atirar(fazendeiro);
            }
            break;

        case KEY_P:
            if (*status_jogo == normal){
                *status_jogo = pausado;
            }
            break;

        case KEY_C:
            if (*status_jogo == normal){
                *status_jogo = carregando;
            }
            break;

        case KEY_R:
            if (*status_jogo == normal){
                *status_jogo = mostrando_ranking;
            } else if (*status_jogo == mostrando_ranking){
                *status_jogo = normal;
            }
            break;

        case KEY_ESCAPE:
            if (*status_jogo == normal){
                *status_jogo = saindo;
            } else if (*status_jogo == saindo){
                *status_jogo = normal;
            }
            break;

        default:
            break;
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
        if (aranhas[i].status) {
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

void game_loop(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], JOGADOR jogadores[]){

    STATUS_JOGO status_jogo = normal;
    int sair = 0;

    char input[TAMANHO_NOME + 1] = "\0";
    int num_letras = 0;
    char itens_menu_superior[NUM_ITEMS_MENU][TAMANHO_STR] = {"ESC-Sair", "C-Carregar", "P-Pausar", "R-Ranking"};
    char itens_menu_inferior[NUM_ITEMS_MENU * 2][TAMANHO_STR] = {"PTS", "", "COG", "", "VDS", "", "TRS", ""};

    while (!WindowShouldClose() && !sair) {
        BeginDrawing();
        ClearBackground(BLACK);

        desenha_moldura();
        desenha_menu_superior(itens_menu_superior);
        desenha_menu_inferior(itens_menu_inferior, *fazendeiro, NUM_COGUMELOS);

        desenha_tiros(fazendeiro->tiros, NUM_TIROS);
        desenha_jogador(*fazendeiro);
        desenha_cogumelos(cogumelos, NUM_COGUMELOS);
        desenha_aranhas(aranhas, NUM_ARANHAS);

        comandos_jogador(fazendeiro, aranhas, cogumelos, NUM_COGUMELOS, jogadores, &status_jogo);

        if (status_jogo == pausado){ // Comandos se o jogo esta pausado
            menu_pausa(fazendeiro, aranhas, cogumelos, input, &num_letras, &status_jogo);
        } else if(status_jogo == carregando){ // Comandos se o jogo esta carregando
            menu_carregar(fazendeiro, aranhas, cogumelos, input, &num_letras, &status_jogo);
        } else if(status_jogo == mostrando_ranking){ // Comandos se o ranking está sendo mostrado
            desenha_ranking(jogadores);
        } else if(status_jogo == saindo){ // Comandos se o jogador quer sair
            menu_sair(fazendeiro, jogadores, input, &num_letras, &status_jogo, &sair);
        } else{
            // Comandos se nao
            movimenta_jogador(fazendeiro, cogumelos);
            move_aranhas(fazendeiro, aranhas, cogumelos, NUM_ARANHAS);
            movimenta_tiros(fazendeiro->tiros);
            verifica_tiros(fazendeiro, cogumelos);
        }

        EndDrawing();
    }

}

int main() {
    FAZENDEIRO fazendeiro = {};
    COGUMELO cogumelos[NUM_COGUMELOS] = {};
    ARANHA aranhas[NUM_ARANHAS] = {};
    JOGADOR jogadores[NUM_JOGADORES] = {};

    gera_fazendeiro(&fazendeiro);
    gera_cogumelos(cogumelos, NUM_COGUMELOS);
    gera_todas_aranhas(aranhas, NUM_ARANHAS);
    carregar_ranking(jogadores);

    InitWindow(LARGURA_TELA, ALTURA_TELA, "millipede");
    SetTargetFPS(FRAMERATE);
    SetExitKey(KEY_F1); // Para liberar o ESC, default da raylib
    // EnableEventWaiting();

    game_loop(&fazendeiro, cogumelos, aranhas, jogadores);

    CloseWindow();

    return 0;
}
