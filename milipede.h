void gera_milipede(MILIPEDE *milipede, int tam_min_milipede, int tam_max_milipede);
void inverte_movimento_milipede(MILIPEDE *milipede);
void verifica_colisao_milipede_cogumelos(MILIPEDE *milipede, int tam_max_milipede, COGUMELO cogumelos[], int num_cogumelos);
int verifica_movimento_milipede(int segmentos_milipede, COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro);
void testa_colisao_milipede_base(MILIPEDE *milipede, int tam_min_milipede, int tam_max_milipede);
void verifica_colisao_milipede_fazendeiro(FAZENDEIRO *fazendeiro, MILIPEDE *milipede, CONFIG_FASE config_fase, STATUS_JOGO *status_jogo);
void movimenta_milipede(MILIPEDE *milipede, COGUMELO cogumelos[], FAZENDEIRO *fazendeiro, CONFIG_FASE config_fase, STATUS_JOGO *status_jogo);
