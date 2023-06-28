/**
 * player_normal.h
 */

#ifndef _PLAYER_NORMAL_H_
#define _PLAYER_NORMAL_H_

#include "player_base.h"

namespace fal {

class PlayerNormal : public PlayerBase {
public:
    PlayerNormal();
    ~PlayerNormal();
    CardSet play(const CardSet &last_cardset) override;
};

}

#endif // _PLAYER_NORMAL_H_
