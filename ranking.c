#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

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

int carregar_ranking(JOGADOR jogadores[]){
    FILE *arq;
    int i = 0, erro = 0;
    char buffer_str[TAMANHO_STR];

    arq = fopen("ranking.txt" , "r");

    if(!arq){
        erro+= 1;
    } else {
        while(i < NUM_JOGADORES - 1 && !feof(arq)){
            if(fgets(buffer_str, TAMANHO_STR, arq) == NULL){
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

