#include "game/StatInfo.hpp"

#include <cmath>

#include <bn_array.h>

namespace ut::game
{

static constexpr bn::array<StatInfo, StatInfo::MAX_LV> STAT_INFOS = []() {
    bn::array<StatInfo, StatInfo::MAX_LV> statInfos;

    for (int lv = 1; lv <= StatInfo::MAX_LV; ++lv)
    {
        auto& stat = statInfos[lv - 1];
        stat.lv = lv;
        stat.maxHp = 16 + lv * 4;
        stat.atk = 8 + lv * 2;
        stat.def = 9 + std::ceil(double(lv) / 4);
    }

    statInfos[1 - 1].exp = 0;
    statInfos[2 - 1].exp = 10;
    statInfos[3 - 1].exp = 30;
    statInfos[4 - 1].exp = 70;
    statInfos[5 - 1].exp = 120;
    statInfos[6 - 1].exp = 200;
    statInfos[7 - 1].exp = 300;
    statInfos[8 - 1].exp = 500;
    statInfos[9 - 1].exp = 800;
    statInfos[10 - 1].exp = 1200;
    statInfos[11 - 1].exp = 1700;
    statInfos[12 - 1].exp = 2500;
    statInfos[13 - 1].exp = 3500;
    statInfos[14 - 1].exp = 5000;
    statInfos[15 - 1].exp = 7000;
    statInfos[16 - 1].exp = 10000;
    statInfos[17 - 1].exp = 15000;
    statInfos[18 - 1].exp = 25000;
    statInfos[19 - 1].exp = 50000;
    statInfos[20 - 1].exp = 99999;

    statInfos[StatInfo::MAX_LV - 1].maxHp = 99;
    // statInfos[StatInfo::MAX_LV - 1].atk = 99;
    // statInfos[StatInfo::MAX_LV - 1].def = 99;

    return statInfos;
}();

auto StatInfo::getInfo(int lv) -> const StatInfo&
{
    BN_ASSERT(1 <= lv && lv <= MAX_LV);

    return STAT_INFOS[lv - 1];
}

int StatInfo::getLv(int exp)
{
    int lv = MAX_LV;
    while (lv >= 2 && exp < getInfo(lv).exp)
        --lv;

    return lv;
}

int StatInfo::getNextLvUpExp(int exp)
{
    const int curLv = getLv(exp);
    if (curLv == MAX_LV)
        return 0;

    return STAT_INFOS[(curLv + 1) - 1].exp - exp;
}

} // namespace ut::game
