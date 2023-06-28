/**
 * player_base.h
 */

#ifndef _PLAYER_BASE_H_
#define _PLAYER_BASE_H_

#include <cstdlib>
#include <vector>

#include "../cardset.h"
#include "../card.h"

namespace fal {

class PlayerBase {
public:
    PlayerBase();
    virtual ~PlayerBase();
    /**
     * Set whether this player is landlord or not.
     * @param is_landload true if is lord.
     */
    void set_landlord(bool is_landlord = true);

    inline bool get_is_landlord() {
        return this->is_landlord;
    }
    /**
     * Play this player's turn.
     * @param last_cardset the last cardset, nullptr if no last cardset.
     */
    virtual CardSet play(const CardSet &last_cardset) = 0;

    void print_cards(bool visible);

    inline std::size_t get_remaining_card_count() {
        return this->remaining_card_count;
    }

    virtual void add_cards(Card cards[], std::size_t size);

    void sort_cards();
protected:
    /**
     * True if this player is landlord.
     */
    bool is_landlord;
    /**
     * The pointer to the array of cards.
     */
    std::vector<Card> cards;

    std::size_t remaining_card_count;
};

}

#endif // _PLAYER_BASE_H_
