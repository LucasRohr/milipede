#include "raylib.h"

#define ESC 27
#define ENTER 10

#define LARGURA_TELA 810
#define ALTURA_TELA 640
#define FRAMERATE 60

#define MOVIMENTO 3
#define MOVIMENTO_TIRO 6

#define DIMENSAO_RETANGULO_BORDA 5
#define MARGEM_RETANGULO_BORDA 10

#define MARGEM_JOGO_Y 50 // Margem da �rea do jogo em si (sem barras em cima ou em baixo)
#define MARGEM_JOGO_X 10

#define TAMANHO_FONTE 30
#define TAMANHO_STR 30

#define TAMANHO_JOGADOR 38
#define TAMANHO_COGUMELO 25
#define TAMANHO_ARANHA 35

#define MARGEM_COGUMELO 5

#define NUM_ITEMS_MENU 4

#define NUM_COGUMELOS 60
#define NUM_VIDAS 3
#define NUM_TIROS 200
#define NUM_ARANHAS 3

#define TAMNOME 100

#define PASSO_ARANHA 1

#define TAMANHO_TIRO 6

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

typedef enum {
    morto = 0,
    livre = 1,
    paralisado = 2,
} STATUS_FAZENDEIRO;

// Estruturas


typedef struct {
    int x;
    int y;
} COORD;

typedef struct {
    COORD posicao; // Da RayLib, posicao.x e posicao.y
    int tamanho; // numero de segmentos
    DIRECAO dir; //usa valores da enumera��o ESQ ou DIR ou BAIXO
} MILIPEDE; // n�o utilizada nessa aula

typedef struct {
    COORD posicao; // Da RayLib, posicao.x e posicao.y
    DIRECAO dir; // usa valores da enumera��o
    int status; // no cen�rio, oculta - 1, 0
} ARANHA;

typedef struct {
    COORD posicao;
    DIRECAO direcao;
    int status;
} TIRO;

typedef struct {
    COORD posicao;
    DIRECAO direcao;
    char nome[TAMANHO_STR];
    int vidas;
    TIRO tiros[NUM_TIROS];
    int tiros_restantes;
    int cogumelos_colhidos; //inicializa em zero
    int status; // Livre, Paralisado, ou Morto. Pode definir enumeracao
    int doente; //qtos cogumelos para curar, zero se estiver sao
    int pausado;
} FAZENDEIRO;

typedef struct {
    COORD posicao;
    int status;
} COGUMELO;

typedef struct {
    FAZENDEIRO fazendeiro;
    ARANHA aranhas[NUM_ARANHAS];
    COGUMELO cogumelos[NUM_COGUMELOS];
} ESTADO_JOGO;
