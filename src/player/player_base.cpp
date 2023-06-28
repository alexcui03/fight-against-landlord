/**
 * player_base.cpp
 */

#include "player_base.h"

#include <iostream>
#include <vector>
#include <algorithm>

namespace fal {

PlayerBase::PlayerBase() {
    this->is_landlord = false;
    this->remaining_card_count = 0;
}

PlayerBase::~PlayerBase() {

}

void PlayerBase::set_landlord(bool is_landlord) {
    this->is_landlord = is_landlord;
}

void PlayerBase::print_cards(bool visible) {
    for (int i = 0; i < this->cards.size(); ++i) {
        if (this->cards[i].type != CardType::NONE_TYPE) {
            if (visible) {
                std::cout << "[";
                std::wcout << card_type_str[this->cards[i].type];
                std::cout << card_display_str[this->cards[i].value] << "] ";
            }
            else {
                std::cout << "[##] ";
            }
        }
    }
}

void PlayerBase::add_cards(Card cards[], std::size_t size) {
    for (int i = 0; i < size; ++i) {
        this->cards.push_back(cards[i]);
    }
    this->remaining_card_count += size;
}

void PlayerBase::sort_cards() {
    std::sort(this->cards.begin(), this->cards.end(), [](const Card &a, const Card &b) -> bool {
        if (a.value == b.value) return a.type < b.type;
        return a.value < b.value;
    });
}

}
