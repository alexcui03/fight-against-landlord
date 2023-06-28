/**
 * cardset.cpp
 */

#include "cardset.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>

namespace fal {

constexpr int BUCKET_SIZE = 15;
constexpr int JOKER_INDEX = 13;

bool generate_cardset_from_cards(const std::vector<Card> &cards, CardSet &cardset) {
    const int len = cards.size();
    if (len == 0) return false;
    
    int bucket[BUCKET_SIZE];
    int bucket_id[BUCKET_SIZE];
    memset(bucket, 0, sizeof(bucket));
    for (int i = 0; i < BUCKET_SIZE; ++i) bucket_id[i] = i;
    for (int i = 0; i < len; ++i) ++bucket[cards[i].value];

    std::sort(
        bucket_id, bucket_id + BUCKET_SIZE,
        [&bucket](const int a, const int b) -> bool {
            if (bucket[a] == bucket[b]) return a < b;
            return bucket[a] > bucket[b];
        }
    );

    int cont_cnt = 1;
    int major_card = bucket_id[0];
    int major_card_type = bucket[major_card];
    for (int i = 1; i < BUCKET_SIZE; ++i) {
        if (
            bucket[bucket_id[i]] == major_card_type &&
            bucket_id[i] == bucket_id[i - 1] + 1 &&
            bucket_id[i] < JOKER_INDEX - 1 // exclude 2, joker, JOKER
        ) {
            ++cont_cnt;
        }
        else {
            break;
        }
    }

    cardset.major_card = major_card;
    cardset.count = cont_cnt;

    switch (major_card_type) {
        case 1: {
            if (len == 1) {
                // e.g. 3
                cardset.type = CardSetType::SINGLE;
                return true;
            }
            if (cont_cnt == len && cont_cnt >= 5) {
                // e.g. 34567
                cardset.type = CardSetType::SINGLE;
                return true;
            }
            if (len == 2 && cards[0].value + cards[1].value == 13 + 14) {
                // e.g. joker JOKER
                cardset.type = CardSetType::KING_BOMB;
                return true;
            }
            return false;
        }
        case 2: {
            if (len == 2) {
                // e.g. 33
                cardset.type = CardSetType::DOUBLE;
                return true;
            }
            if (cont_cnt * 2 == len && cont_cnt >= 3) {
                // e.g. 334455
                cardset.type = CardSetType::DOUBLE;
                return true;
            }
            return false;
        }
        case 3: {
            if (cont_cnt * 3 == len) {
                // e.g. 333, 333444
                cardset.type = CardSetType::THREE;
                return true;
            }
            int next_type = bucket[bucket_id[cont_cnt]];
            if (cont_cnt * 4 == len && next_type == 1) {
                // e.g. 3339, 33344489
                cardset.type = CardSetType::THREE_WITH_SINGLE;
                return true;
            }
            int last_type = 0, cur = cont_cnt;
            while (bucket[bucket_id[cur]]) {
                last_type = bucket[bucket_id[cur]];
                ++cur;
            }
            if (cont_cnt * 5 == len && next_type == 2 && last_type == 2) {
                // e.g. 33399, 3334448899
                cardset.type = CardSetType::THREE_WITH_DOUBLE;
                return true;
            }
            return false;
        }
        case 4: {
            if (len == 4) {
                // e.g. 3333
                cardset.type = CardSetType::BOMB;
                return true;
            }
            int next_type = bucket[bucket_id[cont_cnt]];
            if (len == 6 && next_type == 1) {
                // e.g. 333389
                cardset.type = CardSetType::FOUR_WITH_SINGLE;
                return true;
            }
            int last_type = bucket[bucket_id[cont_cnt + 1]];
            if (len == 8 && next_type == 2 && last_type == 2) {
                // e.g. 33338899
                cardset.type = CardSetType::FOUR_WITH_DOUBLE;
                return true;
            }
            return false;
        }
        default: {
            return false;
        }
    }
}

bool compare_cardset(const CardSet &last_cardset, const CardSet &new_cardset) {
    if (last_cardset.type == CardSetType::NONE) return true;
    if (new_cardset.type == CardSetType::KING_BOMB) return true;
    if (new_cardset.type == CardSetType::BOMB && last_cardset.type != CardSetType::BOMB) return true;
    if (last_cardset.type != new_cardset.type) return false;
    if (last_cardset.count != new_cardset.count) return false;
    return last_cardset.major_card < new_cardset.major_card;
}

}
