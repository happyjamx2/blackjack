#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/blackjack.h"

//  初期値を代入する
void setValue( game *blackjack, users *player, users *dealer ) {

    for (int i = 0; i < CARD; i++) {
        blackjack->card[i] = 0;
    }

    player->cardRank = 0;
    player->column = 0;
    player->score = 0;
    player->hand = HARD;

    dealer->cardRank = 0;
    dealer->column = 0;
    dealer->score = 0;
    dealer->hand = HARD;
}

//  1~52の番号をランダムに生成する
int generateNumber( game *blackjack ) {

    int N;

    do {
        N = rand() % 52 + 1;    
    } while (blackjack->card[N] == deck);
    blackjack->card[N]++;

    return N;
}

//  手札を保存する
void saveHand( int cardId, users *user ) {

    int mark = (cardId - 1) % 4;
    int number = (cardId + 3) / 4;

    if (number == 10) {
        user->string[user->column] = suit[mark];
        user->column++;
        user->string[user->column] = rank[number];
        user->column++;
        user->string[user->column] = rank[0];
        user->column++;
        user->string[user->column] = ' ';
        user->column++;
    } else {
        user->string[user->column] = suit[mark];
        user->column++;
        user->string[user->column] = rank[number];
        user->column++;
        user->string[user->column] = ' ';
        user->column++;
    }
    user->cardRank = number;
}

//  スコアを計算する
void calculateScore( users *user ) {

    if (user->cardRank == 1) {
        if (user->score <= 10) {
            user->score += 11;
            user->hand = SOFT;
        } else {
            user->score += 1;
        }
    } else if (user->cardRank >= 2 && user->cardRank <= 9) {
        user->score += user->cardRank;
    } else if (user->cardRank >= 10 && user->cardRank <= 13) {
        user->score += 10;
    }
    if (user->score > 21) {
        if (user->hand == SOFT) {
            user->score -= 10;
            user->hand = HARD;
        }
    }
}

//  手札を表示する
void printHand( users *user ) {

    for (int i = 0; i < user->column; i++) {
        printf("%c", user->string[i]);
    }
}

//  スコアを表示する
void printScore( users *user ) {

    printf("\nカードの合計: ");

    switch (user->hand) {
        case HARD:
            printf("%d", user->score);
            break;
        case SOFT:
            printf("%d/%d", (user->score - 10), user->score);    
            break;
        default:
            break;
    }
}

//  カードを配る
void dealCard(game *blackjack, users *player, users *dealer ) {

    int cardId = 0;

    printf("ディーラーの手札: ");
    cardId = generateNumber(blackjack);
    saveHand(cardId, dealer);            
    calculateScore(dealer);
    printHand(dealer);

    printf("\nプレイヤーの手札: ");
    for (int i = 0; i < 2; i++) {
        cardId = generateNumber(blackjack);
        saveHand(cardId, player);            
        calculateScore(player);
    }
    printHand(player);
}

//  入力を数字に変換する
void convertInput( char *input, action *command ) {

    int column = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        column++;
    }
    if (column == 1) {
        if (0) {
        } else if (input[0] == 'h') {
            *command = HIT;
        } else if (input[0] == 's') {
            *command = STAND;
        } else {
            *command = INVALID;
        }
    }
    
    if (column == 2) {
        if (0) {
        } else if (input[0] == 's' && input[1] == 'u') {
            *command = SURRENDER;
        } else if (input[0] == 'd' && input[1] == 'd') {
            *command = DOUBLEDOWN;
        } else {
            *command = INVALID;
        }
    }
    
    if (column == 3) {
        *command = INVALID;
    }
}

//  カードを引く
void drawCard( game *blackjack, users *user ) {

    int cardId;

    cardId = generateNumber(blackjack);
    saveHand(cardId, user);            
    calculateScore(user);
    printHand(user);
    printScore(user);
}

//  手札の状態を表示する
void printStatus( users *user ) {

    if (user->score < 21) {
        printf("\n<<< これで勝負します。>>>\n");
    } else if (user->score == 21) {
        printf("\n\n<<< 勝負します。>>>\n");
    } else if (user->score > 21) {
        printf("\n\n<<< バーストしました。>>>\n");
    }
}

//  各コマンドでの動き
void playerCommand( game *blackjack, users *player ) {

    action output;
    char input[SIZE];

    switch (player->command) {
        case HIT:
            do {
                printf("\nカードを追加します。");
                printf("\n\nプレーヤーの手札: ");
                drawCard(blackjack, player);
                if (player->score > 21) {
                    player->end = BURST;
                    break;
                } else if (player->score == 21) {
                    player->end = LE21;
                    break;
                }
                printf("\n\n行動を選択してください。(H/S) ");
                while (1) {
                    scanf("%2s%*[^\n]", input);
                    getchar();
                    convertInput(input, &output);
                    if (output == STAND) {
                        player->end = LE21;
                        break;
                    } else if (output == HIT) {
                        break;
                    }
                    printf("有効な値ではありません。");
                }
            } while (output == HIT);
            break;
        case STAND:
            player->end = LE21;
            break;
        case SURRENDER:
            blackjack->result = LOSE;
            player->end = LE21;
            break;        
        case DOUBLEDOWN:
            printf("\nカードを追加します。");
            printf("\n\nプレーヤーの手札: ");
            drawCard(blackjack, player);
            if (player->score > 21) {
                player->end = BURST;
            } else if (player->score == 21) {
                player->end = LE21;
            } else {
                player->end = LE21;
                printf("\n");
            }
            break;        
        default:
            break;
    }
}

//  プレーヤーのターン
void playerTurn( game *blackjack, users *player ) {

    action output;
    char input[SIZE];

    printf("\n\n---プレーヤーのターン---");
    printf("\n\nプレーヤーの手札: ");
    printHand(player);
    printScore(player);
    if (player->score < 21) {
        printf("\n\n行動を選択してください。(H/S/SU/DD) ");
        while (1) {
            scanf("%2s%*[^\n]", input);
            getchar();
            convertInput(input, &output);
            if (output == INVALID) {
                printf("有効な値ではありません。");
            } else {
                break;
            }
        }
        player->command = output;
        playerCommand(blackjack, player);
    } else if (player->score == 21) {
        player->end = BJ;
    }
    if (output == SURRENDER) {
    } else {
        printStatus(player);
    }
}

//  ディーラーのターン
void dealerTurn( game *blackjack, users *player, users *dealer ) {

    int count = 0;

    if (player->command == SURRENDER) {
    } else {

        printf("\n---ディーラーのターン---");
        printf("\n\nディーラーの手札: ");
        printHand(dealer);
        printScore(dealer);

        while (1) {
            printf("\n\nカードを追加します。");
            printf("\n\nディーラーの手札: ");
            drawCard(blackjack, dealer);
            count++;
            if (dealer->score > 21) {
                dealer->end = BURST;
                break;
            } else if (dealer->score == 21 && count == 1) {
                dealer->end = BJ;
                break;
            } else if (dealer->score == 21) {
                dealer->end = LE21;
                break;
            } else if (dealer->score > 17 && dealer->score <= 21) {
                dealer->end = LE21;
                printf("\n");
                break;
            } else if (dealer->score == 17 && dealer->hand == HARD) {
                dealer->end = LE21;
                printf("\n");
                break;
            }
        }
        printStatus(dealer);
    }
}

//  勝敗を判定する
void judgeResult( game *blackjack, users *player, users *dealer ) {

    if (player->command == SURRENDER) {
        printf("\n降参しました。");
    } else {

        printf("\n勝敗を判定します。");
        switch (player->end) {
            case BURST:
                blackjack->result = LOSE;
                printf("\nプレーヤーの負けです。"); 
                break;
            case LE21:
                if (0) {
                } else if (dealer->end == BURST) {
                    blackjack->result = WIN;
                    printf("\nプレーヤーの勝ちです。");
                } else if (dealer->end == LE21) {
                    if (player->score < dealer->score) {
                        blackjack->result = LOSE;
                        printf("\nプレーヤーの負けです。");
                    } else if (player->score == dealer->score) {
                        blackjack->result = DRAW;
                        printf("\n引き分けです。");
                    } else if (player->score > dealer->score) {
                        blackjack->result = WIN;
                        printf("\nプレーヤーの勝ちです。");
                    }
                } else if (dealer->end == BJ) {
                    blackjack->result = LOSE;
                    printf("\nプレーヤーの負けです。");
                }
                break;
            case BJ:
                if (dealer->end == BJ) {
                    blackjack->result = DRAW;
                    printf("\n引き分けです。");
                } else {
                    blackjack->result = WIN;
                    printf("\nプレーヤーの勝ちです。");
                }
                break;
            default:
                break;
        }
    }
}

//  掛け金を計算する
void calculateBet( game *blackjack, users *player , int *bet ) {

    if (player->command == DOUBLEDOWN) {
        *bet *= 2;
    }

    if (blackjack->result == WIN) {
        if (player->end == BJ) {
            *bet = *bet * 3 / 2;
        }
        printf("+%dG", *bet);     
    } else if (blackjack->result == DRAW) {
        *bet = 0;
        printf("±%dG", *bet);
    } else if (blackjack->result == LOSE) {
        if (player->command == SURRENDER) {
            *bet = *bet / 2 * (-1);
        } else {
            *bet = *bet * (-1);
        }
        printf("%dG", *bet);
    }

    blackjack->gold += *bet;
}