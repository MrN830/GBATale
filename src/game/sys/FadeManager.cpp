#include "game/sys/FadeManager.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/sys/WorldBg.hpp"

namespace ut::game::sys
{

FadeManager::FadeManager(GameContext& ctx) : _context(ctx)
{
}

void FadeManager::render()
{
    if (_fade.has_value() && !_fade->done())
    {
        _fade->update();
        if (_fade->done() && bn::blending::fade_alpha() <= 0)
            setAllFadeBlend(false);
    }
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

    setAllFadeBlend(true);
}

void FadeManager::startFadeOut(int durationUpdates)
{
    _isFadeOut = true;
    _fade.emplace(durationUpdates, 1);

    setAllFadeBlend(true);
}

void FadeManager::setAllFadeBlend(bool isEnabled)
{
    // sys::WorldBg
    _context.worldBg.setBlendingEnabled(isEnabled);

    // cpnt::Sprite
    auto& entMngr = _context.entMngr;
    auto it = entMngr.beforeBeginIter();
    do
    {
        it = entMngr.findIf([](const ent::Entity& entity) { return entity.getComponent<cpnt::Sprite>() != nullptr; },
                            it);

        if (it != entMngr.endIter())
        {
            auto* spr = it->getComponent<cpnt::Sprite>();

            const bool newIsEnabled = (isEnabled || spr->isBlendingEnabled());
            spr->getSprPtr().set_blending_enabled(newIsEnabled);
        }
    } while (it != entMngr.endIter());
}

} // namespace ut::game::sys
