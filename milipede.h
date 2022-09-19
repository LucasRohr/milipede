void gera_milipede(MILIPEDE *milipede);
void inverte_movimento_milipede(MILIPEDE *milipede);
void verifica_colisao_milipede_cogumelos(MILIPEDE *milipede, COGUMELO cogumelos[], int num_cogumelos);
int verifica_movimento_milipede(int segmentos_milipede, COORD posicao, COGUMELO cogumelos[], FAZENDEIRO fazendeiro);
void testa_colisao_milipede_base(MILIPEDE *milipede);
void movimenta_milipede(MILIPEDE *milipede, COGUMELO cogumelos[], int num_cogumelos, FAZENDEIRO *fazendeiro);
void acertou_milipede(MILIPEDE *milipede);
void verifica_impacto_tiro_milipede(TIRO *tiro, MILIPEDE *milipede);
void verifica_tiros_milipede(FAZENDEIRO *fazendeiro, MILIPEDE *milipede);
