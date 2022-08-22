#include "raylib.h"

#define ENTER 10
#define LARGURA_TELA 810
#define ALTURA_TELA 640

#define MOVIMENTO 4
#define ESC 27

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define MARGEM_JOGO_Y 50 // Margem da �rea do jogo em si (sem barras em cima ou em baixo)
#define MARGEM_JOGO_X 10

#define TAMANHO_FONTE 30
#define TAMANHO_STR 20

#define TAMANHO_JOGADOR 15
#define TAMANHO_COGUMELO 15

#define MARGEM_COGUMELO 5

#define NUM_ITEMS_MENU 4

#define NUM_COGUMELOS 60
#define NUM_VIDAS 3
#define NUM_TIROS 200

#define NUM_ARANHAS 3
#define TAMANHO_ARANHA 20

#define TAMNOME 100

typedef enum {
    esq = 6,
    dir = 2,
    cima = 0,
    baixo = 4,
    dir_cima = 1,
    dir_baixo = 3,
    esq_cima = 7,
    esq_baixo = 5,
    null = -1,
} DIRECAO;

// Estruturas

typedef struct {
    Vector2 posicao; // Da RayLib, posicao.x e posicao.y
    int tamanho; // numero de segmentos
    DIRECAO dir; //usa valores da enumera��o ESQ ou DIR ou BAIXO
} MILIPEDE; // n�o utilizada nessa aula

typedef struct {
    Vector2 posicao; // Da RayLib, posicao.x e posicao.y
    DIRECAO dir; // usa valores da enumera��o
    int status; // no cen�rio, oculta - 1, 0
} ARANHA;

typedef struct {
    char nome_jogador[TAMNOME];
    Vector2 posicao;
    int vidas;
    DIRECAO direcao;
    int cogumelos_colhidos; //inicializa em zero
    int tiros; //inicializa em 200
    DIRECAO dirTiro;
    int doente; //qtos cogumelos para curar, zero se estiver sao
    int status; // Livre, Paralisado, ou Morto. Pode definir enumeracao
} FAZENDEIRO;