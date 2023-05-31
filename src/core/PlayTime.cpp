#include "core/PlayTime.hpp"

#include <bn_format.h>

namespace ut::core
{

PlayTime::PlayTime(uint32_t ticks) : _ticks(ticks)
{
}

PlayTime::PlayTime(uint32_t minutes, uint32_t seconds, uint32_t framesUnder30)
{
    BN_ASSERT(seconds < 60);
    BN_ASSERT(framesUnder30 < 30);

    _ticks = framesUnder30;
    _ticks += seconds * 30;
    _ticks += minutes * 60 * 30;
}

void PlayTime::addTicks(uint32_t ticks)
{
    _ticks += ticks;
}

uint32_t PlayTime::getTicks() const
{
    return _ticks;
}

uint32_t PlayTime::getMinutes() const
{
    return _ticks / 30 / 60;
}

uint32_t PlayTime::getSeconds() const
{
    return _ticks / 30 % 60;
}

uint32_t PlayTime::getFramesUnder30() const
{
    return _ticks % 30;
}

auto PlayTime::getTimestamp() const -> bn::string<TIMESTAMP_MAX_LEN>
{
    return bn::format<TIMESTAMP_MAX_LEN>((getSeconds() < 10 ? "{}:0{}" : "{}:{}"),
                                         bn::min(getMinutes(), (uint32_t)9999), getSeconds());
}

auto PlayTime::operator<=>(const PlayTime& other) const -> bn::strong_ordering
{
    return _ticks <=> other._ticks;
}

auto PlayTime::operator+(const PlayTime& other) const -> PlayTime
{
    return PlayTime(_ticks + other._ticks);
}

auto PlayTime::operator+=(const PlayTime& other) -> PlayTime&
{
    _ticks += other._ticks;
    return *this;
}

auto PlayTime::operator-(const PlayTime& other) const -> PlayTime
{
    BN_ASSERT(_ticks >= other._ticks);

    return PlayTime(_ticks - other._ticks);
}

auto PlayTime::operator-=(const PlayTime& other) -> PlayTime&
{
    BN_ASSERT(_ticks >= other._ticks);

    _ticks -= other._ticks;
    return *this;
}

} // namespace ut::core
