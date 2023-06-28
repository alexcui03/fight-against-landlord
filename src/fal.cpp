/**
 * fal.cpp
 * 
 * The main part of the game.
 */

#include <iostream>
#include <algorithm>
#include <limits>
#include <random>

#include "fal.h"
#include "ui.h"
#include "player/player_base.h"
#include "player/player_normal.h"
//#include "player/player_computer.h" // deleted
#include "player/player_computer_2.h"

namespace fal {
    PlayerBase **players;
    Card *lord_cards;
    constexpr int player_count = 3;
    constexpr int lord_extra_count = 3;
    constexpr int normal_cards_count = 17;
    constexpr int base_score = 10;
}

int main() {
    // step 1: ui
    std::cout << "欢迎来到斗地主世界！" << std::endl;
    std::cout << "点击回车键开始游戏" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // step 2: setup players
    fal::players = new fal::PlayerBase*[fal::player_count];

    fal::players[0] = new fal::PlayerNormal();
    for (int i = 1; i < fal::player_count; ++i) {
        fal::players[i] = new fal::PlayerComputer2();
    }

    // step 3: deal cards
    fal::init_basic_card_group();
    
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(fal::basic_card_group, fal::basic_card_group + fal::BASIC_CARD_GROUP_SIZE, rd);
    std::shuffle(fal::basic_card_group, fal::basic_card_group + fal::BASIC_CARD_GROUP_SIZE, rd);

    fal::lord_cards = fal::basic_card_group;
    for (int i = 0; i < fal::player_count; ++i) {
        fal::players[i]->add_cards(
            fal::basic_card_group + fal::lord_extra_count + fal::normal_cards_count * i,
            fal::normal_cards_count
        );
    }

    std::sort(
        fal::lord_cards,
        fal::lord_cards + fal::lord_extra_count,
        [](const fal::Card &a, const fal::Card &b) -> bool {
            if (a.value == b.value) return a.type < b.type;
            return a.value < b.value;
        }
    );
    for (int i = 0; i < fal::player_count; ++i) {
        fal::players[i]->sort_cards();
    }

    // step 3.5
    fal::print_lord_cards(false);
    fal::print_game();

    std::cout << "(1) 叫地主 (2) 不叫（只需要输入数字）" << std::endl;
    int want_lord;
    while (!fal::input_with_check(want_lord) || (want_lord != 1 && want_lord != 2)) {
        std::cout << "输入错误！请重新输入！" << std::endl;
    }
    fal::clear_until_endl();

    int cur_player = 0;
    if (want_lord == 1) {
        fal::players[0]->set_landlord(true);
        fal::players[0]->add_cards(fal::lord_cards, fal::lord_extra_count);
        fal::players[0]->sort_cards();
    }
    else {
        std::uniform_int_distribution<int> uni_dist(1, fal::player_count - 1);
        cur_player = uni_dist(mt);
        const auto &player = fal::players[cur_player];
        player->set_landlord(true);
        player->add_cards(fal::lord_cards, fal::lord_extra_count);
        player->sort_cards();
    }

    // step 4: game
    int rate = 1;
    int cards_count = fal::lord_extra_count + fal::normal_cards_count;
    int last_player = cur_player;
    fal::CardSet last_cardset;
    while (cards_count) {
        if (cur_player == last_player) {
            last_cardset.type = fal::CardSetType::NONE;
        }

        std::cout << "========================================" << std::endl;
        fal::print_lord_cards(true);
        fal::print_game();
        std::cout << "玩家 #" << cur_player << " 的回合" << std::endl;

        fal::CardSet new_cardset = fal::players[cur_player]->play(last_cardset);
        if (new_cardset.type != fal::CardSetType::NONE) {
            last_player = cur_player;
            last_cardset = new_cardset;

            if (new_cardset.type == fal::CardSetType::BOMB) rate *= 2;
            else if (new_cardset.type == fal::CardSetType::KING_BOMB) rate *= 4;
        }

        cards_count = std::min<int>(cards_count, fal::players[cur_player]->get_remaining_card_count());
        cur_player = (cur_player + 1) % fal::player_count;
    }

    // step 5: calc score
    auto &winner = fal::players[last_player];
    std::cout << "游戏结束！" << (winner->get_is_landlord() ? "地主" : "农民") << "获胜！\n";
    for (int i = 0; i < fal::player_count; ++i) {
        std::cout << "玩家 #" << i << " 分数: "
            << (winner->get_is_landlord() == fal::players[i]->get_is_landlord() ? "+" : "-")
            << fal::base_score * rate * (winner->get_is_landlord() ? 2 : 1) << '\n';
    }

    // step 6: end
    for (int i = 0; i < fal::player_count; ++i) {
        delete fal::players[i];
    }
    delete[] fal::players;

    std::cout << "点击回车键退出游戏" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return 0;
}

namespace fal {

void print_lord_cards(bool visible) {
    std::cout << "地主牌: ";
    for (int i = 0; i < lord_extra_count; ++i) {
        if (visible) {
            std::cout << "[";
            std::wcout << card_type_str[lord_cards[i].type];
            std::cout << card_display_str[lord_cards[i].value] << "] ";
        }
        else {
            std::cout << "[##] ";
        }
    }
    std::cout.put('\n');
}

void print_game() {
    for (int i = 1; i < player_count; ++i) {
        std::cout << "玩家 #" << i << ": (" << players[i]->get_remaining_card_count() << ") ";
#ifdef SHOW_ALL_CARDS
        players[i]->print_cards(true);
#else
        players[i]->print_cards(false);
#endif
        std::cout.put('\n');
    }
    std::cout << "玩家 #0: (" << players[0]->get_remaining_card_count() << ") ";
    players[0]->print_cards(true);
    std::cout.put('\n');
    std::cout.flush();
}

}
