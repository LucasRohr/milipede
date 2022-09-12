void gera_fazendeiro(FAZENDEIRO *fazendeiro);
void atirar(FAZENDEIRO *fazendeiro);
void verifica_impacto_tiro(TIRO *tiro, int *cogumelos_colhidos, COGUMELO cogumelos[]);
void verifica_tiros(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]);
void movimenta_tiros(TIRO tiros[]);
int verifica_movimento_jogador(COORD posicao, COGUMELO cogumelos[]);
void muda_direcao_jogador(DIRECAO *direcao);
void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[]);
void comandos_jogador(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int num_cogumelos, JOGADOR jogadores[], STATUS_JOGO *status_jogo);
