#include "definicoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

