#include "game/sys/FadeManager.hpp"

namespace ut::game::sys
{

FadeManager::FadeManager()
{
}

void FadeManager::render()
{
    if (_fade.has_value() && !_fade->done())
        _fade->update();
}

bool FadeManager::isFadeOut() const
{
    return _isFadeOut;
}

bool FadeManager::isDone() const
{
    return !_fade.has_value() || _fade->done();
}

void FadeManager::startFadeIn(int durationUpdates)
{
    _isFadeOut = false;
    _fade.emplace(durationUpdates, 0);
}

void FadeManager::startFadeOut(int durationUpdates)
{
    _isFadeOut = true;
    _fade.emplace(durationUpdates, 1);
}

} // namespace ut::game::sys
