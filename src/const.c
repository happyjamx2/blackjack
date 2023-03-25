#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/blackjack.h"

const char suit[SUIT] = "SHCD";
const char rank[RANK] = "0A234567891JQK";

const int deck = 6;    //  デッキ数
const int finish = 3;  //  最大ゲーム数