void gera_fazendeiro(FAZENDEIRO *fazendeiro);
void atirar(FAZENDEIRO *fazendeiro);
void verifica_impacto_tiro(TIRO *tiro, int *cogumelos_colhidos, int *doente, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE config_fase);
void verifica_tiros(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE config_fase);
void revive_fazendeiro(FAZENDEIRO *fazendeiro);
void fazendeiro_morre(FAZENDEIRO *fazendeiro, STATUS_JOGO *status_jogo);
void atualiza_status_fazendeiro(FAZENDEIRO *fazendeiro, STATUS_JOGO *status_jogo);
void movimenta_tiros(TIRO tiros[]);
int verifica_movimento_jogador(COORD posicao, COGUMELO cogumelos[], int num_cogumelos);
void muda_direcao_jogador(DIRECAO *direcao);
void movimenta_jogador(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], int num_cogumelos);
void comandos_jogador(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], int num_cogumelos, JOGADOR jogadores[], STATUS_JOGO *status_jogo);
