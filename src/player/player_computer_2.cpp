/**
 * player_computer_2.cpp
 */

#include "player_computer_2.h"

#include <algorithm>
#include <vector>
#include <iostream>

#include "../card.h"
#include "../cardset.h"

namespace fal {

constexpr double CARD_THRESHOLD = 50.0;
constexpr int NO_CARD = 5;

constexpr CardSetType cardset_order[] = {
    CardSetType::THREE_WITH_DOUBLE,
    CardSetType::THREE_WITH_SINGLE,
    CardSetType::THREE,
    CardSetType::THREE_WITH_DOUBLE,
    CardSetType::THREE_WITH_SINGLE,
    CardSetType::THREE,
    CardSetType::DOUBLE,
    CardSetType::SINGLE,
    CardSetType::DOUBLE,
    CardSetType::SINGLE,
    CardSetType::BOMB,
    CardSetType::KING_BOMB
};

constexpr int cardset_order_scale[][2] = {
    { 3, 6 },
    { 3, 6 },
    { 3, 6 },
    { 1, 1 },
    { 1, 1 },
    { 1, 1 },
    { 3, 12 },
    { 5, 12 },
    { 1, 1 },
    { 1, 1 },
    { 1, 1 },
    { 1, 1 },
};

static inline int lowbit(int x) {
    return x & (-x);
}

static inline int get_cardcnt(CardSetType type) {
    switch (type) {
        case CardSetType::SINGLE: return 1;
        case CardSetType::DOUBLE: return 2;
        case CardSetType::THREE: return 3;
        case CardSetType::BOMB: return 4;
        default: return NO_CARD;
    }
}

PlayerComputer2::PlayerComputer2() {
    
}

PlayerComputer2::~PlayerComputer2() {

}

CardSet PlayerComputer2::play(const CardSet &last_cardset) {
    std::vector<Card *> candidate_cards;
    if (last_cardset.type == CardSetType::NONE) {
        // todo
        for (int i = 0; i < sizeof(cardset_order) / sizeof(CardSetType); ++i) {
            for (int j = cardset_order_scale[i][1]; j >= cardset_order_scale[i][0]; --j) {
                CardSet res = this->check_available_cards({
                    cardset_order[i],
                    -1,
                    j
                }, candidate_cards);
                if (res.type != CardSetType::NONE) {
                    std::cout << "出牌：";
                    for (const auto &card : candidate_cards) {
                        std::cout << "[";
                        std::wcout << card_type_str[card->type];
                        std::cout << card_display_str[card->value] << "] ";
                        card->type = CardType::NONE_TYPE;
                    }
                    std::cout << std::endl;

                    this->remaining_card_count -= candidate_cards.size();
                    return res;
                }
            }
        }
        std::cout << "不要" << std::endl;
        return CardSet { CardSetType::NONE, -1, 0 };
    }
    else {
        CardSet res = this->check_available_cards(last_cardset, candidate_cards);
        
        // check if there is an bomb or king bomb
        if (last_cardset.type != CardSetType::KING_BOMB && res.type == CardSetType::NONE) {
            const int idx = this->cards.size() - 1;
            if (
                this->cards[idx - 1].type != CardType::NONE_TYPE &&
                this->cards[idx - 2].type != CardType::NONE_TYPE &&
                this->cards[idx - 1].value + this->cards[idx - 2].value == 13 + 14
            ) {
                this->cards[idx - 1].type = CardType::NONE_TYPE;
                this->cards[idx - 2].type = CardType::NONE_TYPE;
                return { CardSetType::KING_BOMB, 13, 1 };
            }
        }
        else if (last_cardset.type != CardSetType::BOMB && res.type == CardSetType::NONE) {
            res = this->check_available_cards({ CardSetType::BOMB, -1, 1 }, candidate_cards);
        }

        if (res.type == CardSetType::NONE) {
            std::cout << "不要" << std::endl;
        }
        else {
            std::cout << "出牌：";
            for (const auto &card : candidate_cards) {
                std::cout << "[";
                std::wcout << card_type_str[card->type];
                std::cout << card_display_str[card->value] << "] ";
                card->type = CardType::NONE_TYPE;
            }
            std::cout << std::endl;

            this->remaining_card_count -= candidate_cards.size();
        }
        return res;
    }
}

void PlayerComputer2::get_cards(int card_id, int count, std::vector<Card *> &result) {
    if (count <= 0) return;
    for (auto &card : this->cards) {
        if (card.type != CardType::NONE_TYPE && card.value == card_id) {
            result.push_back(&card);
            if (--count <= 0) return;
        }
    }
}

CardSet PlayerComputer2::check_available_cards(CardSet last_cardset, std::vector<Card *> &result, bool greedy) {
    result.clear();
    
    CardSet cardset;
    cardset.type = CardSetType::NONE;
    cardset.count = last_cardset.count;

    if (last_cardset.type == CardSetType::KING_BOMB) {
        return cardset;
    }

    int card_count[CARD_COUNT] = { 0 };
    for (const auto &card : this->cards) {
        if (card.type != CardType::NONE_TYPE) {
            ++card_count[card.value];
        }
    }

    if (last_cardset.type == CardSetType::BOMB) {
        for (int cur = last_cardset.major_card + 1; cur < JOKER_ID; ++cur) {
            if (card_count[cur] == 4) {
                for (Card &card : this->cards) {
                    if (card.value == cur && card.type != CardType::NONE_TYPE) {
                        result.push_back(&card);
                    }
                }
                cardset.type = CardSetType::BOMB;
                cardset.major_card = cur;
                return cardset;
            }
        }
        return cardset;
    }

    int card_type = get_cardcnt(static_cast<CardSetType>(lowbit(last_cardset.type)));
    int extra_card_type = get_cardcnt(static_cast<CardSetType>(
        static_cast<int>(last_cardset.type) ^ lowbit(last_cardset.type)
    ));

    int card_id[CARD_COUNT];
    for (int i = 0; i < CARD_COUNT; ++i) card_id[i] = i;
    std::sort(
        card_id, card_id + CARD_COUNT,
        [card_count, card_type](int a, int b) -> bool {
            int cnt_a = std::min(card_count[a], card_type);
            int cnt_b = std::min(card_count[b], card_type);
            if (cnt_a == cnt_b) return a < b;
            return cnt_a > cnt_b;
        }
    );

    double card_select_weight[CARD_COUNT] = { 0 };
    double extra_card_select_weight[CARD_COUNT] = { 0 };

    int card_select_weight_id[CARD_COUNT];
    int extra_card_select_weight_id[CARD_COUNT];
    for (int i = 0; i < CARD_COUNT; ++i) {
        card_select_weight_id[i] = extra_card_select_weight_id[i] = i;
    }
    
    int cont_cnt = 0, next_id = card_id[0];
    int last_possible_major_card = 0;
    // find the continuous card (cnt >= card_type)
    for (int i = 0; i < CARD_COUNT; ++i) {
        if (card_count[card_id[i]] >= card_type) {
            if (card_id[i] <= last_cardset.major_card) {
                continue;
            }
            if (card_id[i] == next_id) {
                ++cont_cnt;
                ++next_id;
            }
            else {
                // isn't continuous card
                cont_cnt = 1;
                next_id = card_id[i] + 1;
            }
            if (cont_cnt >= last_cardset.count) {
                if (greedy) last_cardset.count = cont_cnt;
                if (last_cardset.count > 1 && card_id[i] > MAX_CONT_CARD_ID) {
                    continue;
                }
                // it is an valid cardset
                card_select_weight[card_id[i] - last_cardset.count + 1] = 100.0;
            }
        }
        else {
            last_possible_major_card = i - 1;
            break;
        }
    }
    // find, cnt >= extra_card_type
    const int k = 4 - extra_card_type + 1;
    for (int i = 1; i < CARD_COUNT; ++i) {
        if (card_count[card_id[i]] >= extra_card_type) {
            // it is an valid cardset
            extra_card_select_weight[card_id[i]] = 100.0 * (k - (card_count[card_id[i]] - extra_card_type)) / k;
        }
        else {
            break;
        }
    }

    std::sort(
        card_select_weight_id, card_select_weight_id + CARD_COUNT,
        [weight = card_select_weight](const int a, const int b) -> bool {
            if (weight[a] == weight[b]) return a < b;
            return weight[a] > weight[b];
        }
    );

    std::sort(
        extra_card_select_weight_id, extra_card_select_weight_id + CARD_COUNT,
        [weight = extra_card_select_weight](const int a, const int b) -> bool {
            if (weight[a] == weight[b]) return a < b;
            return weight[a] > weight[b];
        }
    );

    const int major_card = card_select_weight_id[0];
    if (card_select_weight[major_card] >= CARD_THRESHOLD) {
        for (int i = 0; i < last_cardset.count; ++i) {
            this->get_cards(major_card + i, card_type, result);
        }
        int extra_card_cnt = 0;
        // if extra card is needed
        if (extra_card_type != NO_CARD) {
            for (int i = 0; i < CARD_COUNT; ++i) {
                if (extra_card_cnt == last_cardset.count) {
                    break;
                }
                // extra card cannot be the same with major card
                if (extra_card_select_weight_id[i] == major_card) {
                    continue;
                }
                if (extra_card_select_weight[extra_card_select_weight_id[i]] < CARD_THRESHOLD) {
                    break;
                }
                this->get_cards(extra_card_select_weight_id[i], extra_card_type, result);
                ++extra_card_cnt;
            }
            if (extra_card_cnt < last_cardset.count) {
                // there is not enough extra card
                return cardset; // NONE
            }
        }
        cardset.type = last_cardset.type;
        cardset.major_card = major_card;
        return cardset;
    }
    return cardset;
}

}
