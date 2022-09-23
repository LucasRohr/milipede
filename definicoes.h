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
#define TAMANHO_NOME 20
#define TAMANHO_STR 50
#define TAMANHO_DIGITOS 5

#define TAMANHO_JOGADOR 38
#define TAMANHO_COGUMELO 25
#define TAMANHO_ARANHA 35

#define MARGEM_COGUMELO 5

#define NUM_JOGADORES 6
#define NUM_ITEMS_MENU 4

#define NUM_COGUMELOS_INICIAL 60
#define INCREMENTO_COGUMELOS 20
#define NUM_COGUMELOS_MAX 200
#define NUM_VIDAS 3
#define NUM_TIROS 200
#define NUM_ARANHAS_INICIAL 3
#define NUM_ARANHAS_MAX 5
#define INCREMENTO_ARANHA 1

#define TAMNOME 100

#define PASSO_ARANHA 1

#define TAMANHO_TIRO 6

#define TAMANHO_SEGMENTO_MILIPEDE 40
#define TAMANHO_MIN_MILIPEDE_INICIAL 4
#define TAMANHO_MIN_MILIPEDE_MAX 10
#define TAMANHO_MAX_MILIPEDE_INICIAL 10
#define TAMANHO_MAX_MILIPEDE_MAX 16
#define INCREMENTO_MILIPEDE 2
#define PASSO_MILIPEDE 1
#define PASSO_VERTICAL_MILIPEDE 20

#define ESPACO 35
#define CURLY_BRACKET 125

#define NUM_COGUMELOS_CURAR_ARANHA 5
#define TEMPO_COOLDOWN_TIRO 0.2 // Em segundos
#define TEMPO_PARALISIA 0.5
#define TEMPO_INVULNERAVEL 1
#define TEMPO_DOENTE 5
#define TEMPO_INICIO_FASE 4

#define FRAMES_PISCAR_MENU 10

#define NUM_IMAGENS 5
#define I_TEXTURA_FAZENDEIRO 0
#define I_TEXTURA_COGUMELO 1
#define I_TEXTURA_ARANHA 2
#define I_TEXTURA_MILIPEDE_CABECA 3
#define I_TEXTURA_MILIPEDE_CORPO 4

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
    esq_mili = 0,
    dir_mili = 1,
} DIRECAO_MILIPEDE;

typedef enum {
    morto = 0,
    livre = 1,
    paralisado = 2,
} STATUS_FAZENDEIRO;

typedef enum {
    normal = 0,
    pausado = 1,
    carregando = 2,
    mostrando_ranking = 3,
    saindo = 4,
    game_over = 5,
    inicio_fase = 6,
} STATUS_JOGO;

// Estruturas

typedef struct {
    int x;
    int y;
} COORD;

typedef struct {
    COORD posicao_cabeca;
    int tamanho;
    DIRECAO_MILIPEDE dir;
    int status;
} MILIPEDE;

typedef struct {
    COORD posicao;
    DIRECAO dir;
    int status;
} ARANHA;

typedef struct {
    COORD posicao;
    DIRECAO direcao;
    int status;
} TIRO;

typedef struct {
    COORD posicao;
    DIRECAO direcao;
    char nome[TAMANHO_NOME];
    int vidas;
    TIRO tiros[NUM_TIROS];
    int tiros_restantes;
    int cooldown_tiro;
    int cogumelos_colhidos; //inicializa em zero
    int status; // Livre, Paralisado, ou Morto.
    int doente; //qtos cogumelos para curar, zero se estiver sao
    int contador_paralisado;
    int contador_invulneravel;
    int contador_doente;
} FAZENDEIRO;

typedef struct {
    COORD posicao;
    int status;
} COGUMELO;

typedef struct {
    int fase;
    int num_cogumelos;
    int num_aranhas;
    int tam_min_milipede;
    int tam_max_milipede;
} CONFIG_FASE; // Estrutura para guardar dados sobre a fase atual

typedef struct {
    FAZENDEIRO fazendeiro;
    ARANHA aranhas[NUM_ARANHAS_MAX];
    MILIPEDE milipede;
    COGUMELO cogumelos[NUM_COGUMELOS_MAX];
    CONFIG_FASE config_fase;
} ESTADO_JOGO; // Estrutura para salvar e carregar o jogo

typedef struct {
    char nome[TAMANHO_STR];
    int pontos;
} JOGADOR;
