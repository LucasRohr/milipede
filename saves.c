#include "definicoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ESTADO_JOGO cria_save(FAZENDEIRO fazendeiro, ARANHA aranhas[], MILIPEDE milipede, COGUMELO cogumelos[], CONFIG_FASE config_fase){
    int i;
    ESTADO_JOGO save;

    save.fazendeiro = fazendeiro;
    save.milipede = milipede;
    save.config_fase = config_fase;

    for (i = 0; i < NUM_ARANHAS_MAX; i++){
        save.aranhas[i] = aranhas[i];
    }
    for(i = 0; i < NUM_COGUMELOS_MAX; i++){
        save.cogumelos[i] = cogumelos[i];
    }
    return (save);
}

int salvar_jogo(FAZENDEIRO fazendeiro, ARANHA aranhas[], MILIPEDE milipede, COGUMELO cogumelos[], CONFIG_FASE config_fase){
    ESTADO_JOGO save = cria_save(fazendeiro, aranhas, milipede, cogumelos, config_fase);
    FILE *arq;
    char nome_arquivo[TAMANHO_STR] = "";

    // Gera o nome do arquivo, formato <nome_jogador>.bin
    strcat(nome_arquivo, fazendeiro.nome);
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

void instanciar_jogo(ESTADO_JOGO save, FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase){
    int i;

    *fazendeiro = save.fazendeiro;
    *milipede = save.milipede;
    *config_fase = save.config_fase;

    for (i = 0; i < NUM_ARANHAS_MAX; i++){
        aranhas[i] = save.aranhas[i];
    }
    for(i = 0; i < NUM_COGUMELOS_MAX; i++){
        cogumelos[i] = save.cogumelos[i];
    }
}

int carregar_jogo(FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase, char input[]){
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
        instanciar_jogo(save, fazendeiro, aranhas, milipede, cogumelos, config_fase);
    } else {
        printf("\n\nErro na abertura de arquivo\n\n");
    }

    fclose(arq);

    return 0;
}

