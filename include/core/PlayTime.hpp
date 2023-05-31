#pragma once

#include <bn_string.h>

namespace ut::core
{

class PlayTime final
{
public:
    static constexpr int TIMESTAMP_MAX_LEN = 7;

public:
    explicit PlayTime(uint32_t ticks);
    PlayTime(uint32_t minutes, uint32_t seconds, uint32_t framesUnder30 = 0);

public:
    void addTicks(uint32_t ticks);

    uint32_t getTicks() const;

    uint32_t getMinutes() const;
    uint32_t getSeconds() const;
    uint32_t getFramesUnder30() const;

    auto getTimestamp() const -> bn::string<TIMESTAMP_MAX_LEN>;

public:
    auto operator<=>(const PlayTime&) const -> bn::strong_ordering;

    auto operator+(const PlayTime&) const -> PlayTime;
    auto operator+=(const PlayTime& other) -> PlayTime&;
    auto operator-(const PlayTime&) const -> PlayTime;
    auto operator-=(const PlayTime& other) -> PlayTime&;

private:
    uint32_t _ticks;
};

} // namespace ut::core
