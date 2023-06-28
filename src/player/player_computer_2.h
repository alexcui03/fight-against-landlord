/**
 * player_computer_2.h
 */

#ifndef _PLAYER_COMPUTER_2_H_
#define _PLAYER_COMPUTER_2_H_

#include "player_base.h"

namespace fal {

enum class CheckStatus {
    UNAVAILABLE = 0,
    STRICT = 1,
    OPTIONAL = 2
};

class PlayerComputer2 : public PlayerBase {
public:
    PlayerComputer2();
    ~PlayerComputer2();
    CardSet play(const CardSet &last_cardset) override;
private:
    void get_cards(int card_id, int count, std::vector<Card *> &result);
    CardSet check_available_cards(CardSet cardset, std::vector<Card *> &result, bool greedy = false);
};

}

#endif
