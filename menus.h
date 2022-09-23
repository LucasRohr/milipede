void menu_pausa(char texto[], FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase, STATUS_JOGO *status_jogo, char input[], int *num_letras);
void menu_sair(char texto[], FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair);
void menu_carregar(char texto[], FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase, STATUS_JOGO *status_jogo, char input[], int *num_letras);
void menu_game_over(char texto[], FAZENDEIRO *fazendeiro, JOGADOR jogadores[], char input[], int *num_letras, STATUS_JOGO *status_jogo, int *sair);
void menu_inicio_fase(STATUS_JOGO *status_jogo, int fase, int *contador);
int instanciar_nome(char input[], int *num_letras);
