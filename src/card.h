/**
 * card.h
 */

#ifndef _CARD_H_
#define _CARD_H_

#include <string>

namespace fal {

enum CardType {
    NONE_TYPE,
    HEART,
    DIMOND,
    SPADE,
    CLUB
};

class Card {
public:
    CardType type;
    int value;
};

constexpr int BASIC_CARD_GROUP_SIZE = 54;
constexpr int CARD_COUNT = 15;
constexpr int JOKER_ID = 13;
constexpr int MAX_CONT_CARD_ID = JOKER_ID - 2;

extern Card basic_card_group[BASIC_CARD_GROUP_SIZE];
extern const char *card_display_str[15];
extern const wchar_t *card_type_str[5];

/**
 * Initialize the basic card group.
 */
void init_basic_card_group();

int get_card_value_from_str(const std::string &str);

}

#endif // _CARD_H_
