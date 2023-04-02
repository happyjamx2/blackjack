#ifndef INCLUDE_BLACKJACK
#define INCLUDE_BLACKJACK

#define CARD 53
#define RANK 15
#define SUIT 5

#define GUEST 2
#define SIZE 32

typedef enum Fight {
    WIN, DRAW, LOSE
} fight;

typedef enum Type {
    HARD, SOFT
} type;

typedef enum Status {
    BURST, LE21, BJ
} status;

typedef enum Action {
    INVALID, HIT, STAND, SURRENDER, DOUBLEDOWN
} action;

typedef struct Game {
    int card[CARD];
    int gold;
    action command;
    fight result;
} game;

typedef struct Users {
    char string[SIZE];        //  手札保存用配列
    int cardRank;            //  配られた札の番号
    int column;             //  手札保存用配列の添え字
    int score;             //  スコアを格納する変数
    type hand;
    status end;
} users;

void setValue(game *blackjack, users *player, users *dealer);
int generateNumber(game *blackjack);
void saveHand(int cardId, users *user);
void calculateScore(users *user);
void printHand(users *user);
void printScore(users *user);
void dealCard(game *blackjack, users *player, users *dealer);
void convertInput(char *input, action *command);
void drawCard(game *blackjack, users *user);
void printStatus(users *user);
void playerCommand(game *blackjack, users *player);
void playerTurn(game *blackjack, users *player);
void dealerTurn(game *blackjack, users *player, users *dealer);
void judgeResult(game *blackjack, users *player, users *dealer);
void calculateBet(game *blackjack, users *player , int *bet);

extern const char suit[SUIT];
extern const char rank[RANK];
extern const int deck;
extern const int finish;

#endif//INCLUDE_BLACKJACK