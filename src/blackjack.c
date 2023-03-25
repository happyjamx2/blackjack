#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/blackjack.h"

int main(void) {

    game blackjack;
    users player;
    users dealer;

    int bet;
    int round = 1;

    char string[SIZE];

    srand((unsigned int)time(NULL));

    blackjack.gold = 10000;
    while (round <= finish) {

        setValue(&blackjack, &player, &dealer);

        printf("\n%d回戦を開始します。\n\n", round);
        printf("プレーヤーの所持金は%dGです。\n", blackjack.gold);
        printf("掛け金を入力してください。");

        while (1) {
            scanf("%d", &bet);
            if (bet > 0 && bet < 10000000) {
                break;
            }
            printf("有効な値ではありません。");
        }

        printf("\n手札を配ります。\n\n");
        dealCard(&blackjack, &player, &dealer);

        /* プレーヤーのターン */
        playerTurn(&blackjack, &player);

        /* ディーラーのターン */
        dealerTurn(&blackjack, &player, &dealer);

        /* 勝敗を判定 */
        judgeResult(&blackjack, &player, &dealer);

        calculateBet(&blackjack, &player , &bet);

        if (round < finish) {
            printf("\n\n次の試合に進みます。");
            printf("\n何かキーを押してください。");
            scanf("%1s%*[^\n]", string); 
        }
        round++;
    }
    printf("\n\n現在の所持金は%dGです。\n", blackjack.gold);

    return 0;
}