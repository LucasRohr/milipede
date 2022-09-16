int gera_posicao_cogumelos(int valor_minimo, int valor_maximo);
int verifica_posicao_cogumelos(COGUMELO cogumelos[], int num_cogumelos, int x, int y);
void gera_cogumelos(COGUMELO cogumelos[], int num_cogumelos);
int verifica_colisao_cogumelos(COORD posicao, int tamanho_objeto, COGUMELO cogumelos[], int num_cogumelos);
void acertou_cogumelo(COORD posicao, COGUMELO cogumelos[], int num_cogumelos);
int conta_cogumelos_restantes(COGUMELO cogumelos[], int total_cogumelos);
