/**
 * cardset.h
 */

#ifndef _CARDSET_H_
#define _CARDSET_H_

#include <cstdlib>
#include <vector>

#include "card.h"

namespace fal {

enum CardSetType {
    NONE = 0x00,
    SINGLE = 0x10, // A
    DOUBLE = 0x08, // AA
    THREE = 0x04, // AAA
    THREE_WITH_SINGLE = THREE | SINGLE, // AAAB
    THREE_WITH_DOUBLE = THREE | DOUBLE, // AAABB
    BOMB = 0x02, // AAAA
    FOUR_WITH_SINGLE = BOMB | SINGLE, // AAAABC
    FOUR_WITH_DOUBLE = BOMB | DOUBLE, // AAAABBCC
    KING_BOMB = 0x01, // joker + JOKER
};

class CardSet {
public:
    /**
     * The type of the cardset.
     */
    CardSetType type;
    /**
     * The major card, usually it is the minimum card of the card group.
     * e.g. "334455" will be 3 (type = DOUBLE)
     */
    int major_card;
    /**
     * The count of card unit.
     * e.g. "456789" will be 6 (type = SINGLE)
     */
    int count;
};

/**
 * Generate a cardset from the array of cards.
 * @param cards all cards.
 * @param len the length of card array.
 * @param cardset the reference to result.
 * @return false if the cardset is not valid.
 */
bool generate_cardset_from_cards(const std::vector<Card> &cards, CardSet &cardset);

/**
 * Compare the two cardset and decide whether the new cardset is valid.
 * @param last_cardset last cardset.
 * @param new_cardset new cardset.
 * @return true if the new cardset is greater than the last cardset.
 */
bool compare_cardset(const CardSet &last_cardset, const CardSet &new_cardset);

}

#endif // _CARDSET_H_
