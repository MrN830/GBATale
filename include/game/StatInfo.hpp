#pragma once

namespace ut::game
{

struct StatInfo
{
public:
    static constexpr int MAX_LV = 20;

public:
    static auto getInfo(int lv) -> const StatInfo&;
    static int getLv(int exp);

public:
    int lv;
    int exp;
    int maxHp;
    int atk;
    int def;
};

} // namespace ut::game
