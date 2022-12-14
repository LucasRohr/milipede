int verifica_colisao(COORD pos_a, int a_tam, COORD pos_b, int b_tam);
int verifica_colisao_cogumelos(COORD posicao, int tamanho_objeto, COGUMELO cogumelos[], int num_cogumelos);
int verifica_colisao_tiro_aranhas(COORD posicao, int tamanho_objeto, ARANHA aranhas[], int num_aranhas);
void acertou_cogumelo(COORD posicao, COGUMELO cogumelos[], int num_cogumelos);
void acertou_aranha(COORD posicao, ARANHA aranhas[], int num_aranhas);
void acertou_milipede(MILIPEDE *milipede);
void verifica_impacto_tiro(TIRO *tiro, int *cogumelos_colhidos, int *doente, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE config_fase);
void verifica_tiros(FAZENDEIRO *fazendeiro, COGUMELO cogumelos[], ARANHA aranhas[], MILIPEDE *milipede, CONFIG_FASE config_fase);
