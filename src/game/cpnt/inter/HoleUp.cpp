#include "game/cpnt/inter/HoleUp.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"

#include "gen/EntityId.hpp"

namespace ut::game::cpnt::inter
{

namespace
{

constexpr int START_GO_UP_DELAY = 10;

enum FriskGfxIdx
{
    DOWN = 1
};

} // namespace

HoleUp::HoleUp(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::HoleUp, isEnabled, triggers), _frisk(nullptr)
{
}

void HoleUp::awake(GameContext& ctx)
{
    _frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(_frisk != nullptr);
}

auto HoleUp::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    if (ctx.interactStack.top() != InteractState::FREE)
        co_return;

    ctx.interactStack.push(InteractState::FALLING);

    _delayCountdown = START_GO_UP_DELAY + 1;

    co_return;
}

void HoleUp::update(GameContext& ctx)
{
    Interaction::update(ctx);

    if (_delayCountdown > 0)
    {
        // start going up
        if (--_delayCountdown == 0)
        {
            auto* friskSpr = _frisk->getComponent<cpnt::Sprite>();
            BN_ASSERT(friskSpr != nullptr);

            friskSpr->setEnabled(false);

            initFriskMoveAction();
        }
    }
    else if (_friskMoveAction.has_value() && !_friskMoveAction->done())
    {
        _friskMoveAction->update();

        // end going up
        if (_friskMoveAction->done())
        {
            auto* friskSpr = _frisk->getComponent<cpnt::Sprite>();
            BN_ASSERT(friskSpr != nullptr);
            auto& friskSprPtr = friskSpr->getSprPtr();

            friskSpr->setGfxIdx(FriskGfxIdx::DOWN);
            friskSprPtr.set_horizontal_flip(false);
            friskSpr->setEnabled(true);

            BN_ASSERT(ctx.interactStack.top() == InteractState::FALLING);
            ctx.interactStack.pop();
        }
    }
}

void HoleUp::initFriskMoveAction()
{
    constexpr int FRISK_UP_SPEED = 8;
    constexpr int M_TILE_HEIGHT = 16;

    const int upMTileCount = (_entity.getId() == ent::gen::EntityId::down ? 26 : 20);

    const auto finalPos = _frisk->getPosition() - bn::fixed_point(0, upMTileCount * M_TILE_HEIGHT);

    _friskMoveAction.emplace(*_frisk, upMTileCount * M_TILE_HEIGHT / FRISK_UP_SPEED, finalPos);
}

} // namespace ut::game::cpnt::inter
