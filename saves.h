ESTADO_JOGO cria_save(FAZENDEIRO fazendeiro, ARANHA aranhas[], MILIPEDE milipede, COGUMELO cogumelos[], CONFIG_FASE config_fase);
int salvar_jogo(FAZENDEIRO fazendeiro, ARANHA aranhas[], MILIPEDE milipede, COGUMELO cogumelos[], CONFIG_FASE config_fase);
void instanciar_jogo(ESTADO_JOGO save, FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase);
int carregar_jogo(FAZENDEIRO *fazendeiro, ARANHA aranhas[], MILIPEDE *milipede, COGUMELO cogumelos[], CONFIG_FASE *config_fase, char input[]);
