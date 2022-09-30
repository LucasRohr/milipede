void gera_aranha(ARANHA *aranha);
void inverte_movimento(ARANHA *aranha);
void verifica_colisao_aranha_cogumelos(ARANHA aranha, COGUMELO cogumelos[], int num_cogumelos);
int verifica_movimento_aranha(COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro);
void move_aranha(ARANHA *aranha, COGUMELO cogumelos[], int num_cogumelos, FAZENDEIRO fazendeiro);
void testa_colisao_aranha_base(ARANHA *aranha);
void verifica_colisao_aranha_fazendeiro(FAZENDEIRO *fazendeiro, ARANHA aranha, STATUS_JOGO *status_jogo);
void move_aranhas(FAZENDEIRO *fazendeiro, ARANHA aranhas[], COGUMELO cogumelos[], CONFIG_FASE config_fase, STATUS_JOGO *status_jogo);
void gera_todas_aranhas(ARANHA aranhas[], int total_aranhas);
