/**
 * player_normal.cpp
 */

#include "player_normal.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "../ui.h"

namespace fal {

PlayerNormal::PlayerNormal() {
    
}

PlayerNormal::~PlayerNormal() {

}

CardSet PlayerNormal::play(const CardSet &last_cardset) {
    if (last_cardset.type == CardSetType::NONE) {
        std::cout << "请输入牌的名称（数字、字母、joker、或JOKER），用空格分隔" << std::endl;
    }
    else {
        std::cout << "请输入牌的名称（数字、字母、joker、或JOKER），用空格分隔，输入 0 表示不要" << std::endl;
    }

    std::vector<Card> cards;
    std::vector<Card *> cards_ptr;
    CardSet cardset;
    cardset.type = CardSetType::NONE;

    std::string str;
    bool err;
    do {
        err = false;
        cardset.type = CardSetType::NONE;
        // reset type
        for (int i = 0; i < cards.size(); ++i) {
            cards_ptr[i]->type = cards[i].type;
        }
        cards.clear();
        cards_ptr.clear();
        bool need_clear = true;
        bool is_first = true;
        while ((need_clear = input_until_endl(str))) {
            // check 0
            if (str == "0" && is_first) {
                clear_until_endl();
                if (last_cardset.type != CardSetType::NONE) {
                    std::cout << "不要" << std::endl;
                    return cardset;
                }
                else {
                    err = true;
                    break;
                }
            }
            is_first = false;

            // check if the card valid
            int val = fal::get_card_value_from_str(str.c_str());
            if (val == -1) {
                err = true;
                break;
            }
            // check if the card exists
            bool exists = false;
            for (Card &card : this->cards) {
                if (card.type != CardType::NONE_TYPE && card.value == val) {
                    exists = true;
                    cards.push_back(card);
                    cards_ptr.push_back(&card);
                    card.type = CardType::NONE_TYPE;
                    break;
                }
            }
            if (!exists) {
                err = true;
                break;
            }
        }
        if (err) {
            std::cout << "输入错误！请重新输入！" << std::endl;
        }
        else {
            // check if the cardset valid
            if (!generate_cardset_from_cards(cards, cardset) ||
                !compare_cardset(last_cardset, cardset)
            ) {
                err = true;
                std::cout << "不合法的牌型！请重新输入！" << std::endl;
            }
        }
        if (need_clear) clear_until_endl();
    } while (err);

    std::sort(cards_ptr.begin(), cards_ptr.end(), [](const Card *a, const Card *b) -> bool {
        return a->value < b->value;
    });
    std::cout << "出牌：";
    for (Card *&card : cards_ptr) {
        std::cout << "[";
        std::wcout << card_type_str[card->type];
        std::cout << card_display_str[card->value] << "] ";
        card->type = CardType::NONE_TYPE;
    }
    std::cout << std::endl;

    this->remaining_card_count -= cards.size();

    return cardset;
}

}
