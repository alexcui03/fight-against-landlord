/**
 * card.cpp
 */

#include "card.h"

#include <cstring>
#include <string>

namespace fal {

Card basic_card_group[BASIC_CARD_GROUP_SIZE];

const wchar_t *card_type_str[] = {
    L"", L"♥", L"♦", L"♥", L"♣"
};

const char *card_display_str[] = {
    "3", "4", "5", "6", "7", "8", "9", "10",
    "J", "Q", "K", "A", "2", "joker", "JOKER"
};

void init_basic_card_group() {
    int cur = 0;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 0; j < 13; ++j) {
            basic_card_group[cur].type = static_cast<CardType>(i);
            basic_card_group[cur].value = j;
            ++cur;
        }
    }

    // black joker
    basic_card_group[cur].type = CardType::SPADE;
    basic_card_group[cur].value = 13;
    ++cur;

    // red joker
    basic_card_group[cur].type = CardType::HEART;
    basic_card_group[cur].value = 14;
}

int get_card_value_from_str(const std::string &str) {
    for (int i = 0; i < sizeof(card_display_str) / sizeof(char *); ++i) {
        if (str == card_display_str[i]) {
            return i;
        }
    }
    return -1;
}

}
