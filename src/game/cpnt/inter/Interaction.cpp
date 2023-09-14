#include "game/cpnt/inter/Interaction.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/sys/EntityManager.hpp"
#include "gen/EntityId.hpp"

namespace ut::game::cpnt::inter
{

Interaction::~Interaction() = default;

Interaction::Interaction(ent::Entity& entity, bn::type_id_t interactionType, bool isEnabled,
                         InteractionTriggers triggers)
    : Component(entity, bn::type_id<Interaction>(), isEnabled), _triggers(triggers), _interactionType(interactionType)
{
}

auto Interaction::getInteractionType() const -> bn::type_id_t
{
    return _interactionType;
}

auto Interaction::onInteract(GameContext&) -> task::Task
{
    if (!!(_triggers & InteractionTriggers::COLLIDE))
        _isColliding = true;

    co_return;
}

void Interaction::update(GameContext& ctx)
{
    if (!_isColliding)
        return;

    const auto* collPack = _entity.getComponent<cpnt::ColliderPack>();

    if (collPack == nullptr || !collPack->isEnabled())
    {
        _isColliding = false;
        return;
    }

    bool isStillColliding = false;

    if (!!(_triggers & InteractionTriggers::COLLIDE_FRISK))
    {
        const auto* frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
        if (frisk != nullptr)
        {
            const auto* friskColl = frisk->getComponent<cpnt::ColliderPack>();
            if (friskColl != nullptr && friskColl->isEnabled())
            {
                if (friskColl->isCollideWith(*collPack))
                    isStillColliding = true;
            }
        }
    }

    if (!!(_triggers & InteractionTriggers::COLLIDE_OTHERS))
    {
        const auto other = ctx.entMngr.findIf(
            [this, collPack](const ent::Entity& entity) -> bool {
                if (&_entity == &entity)
                    return false;

                const auto* otherColl = entity.getComponent<cpnt::ColliderPack>();
                if (otherColl == nullptr || !otherColl->isEnabled())
                    return false;
                if (otherColl->isTrigger() || !otherColl->isCollideWith(*collPack))
                    return false;

                return true;
            },
            ctx.entMngr.cBeforeBeginIter());

        if (other != ctx.entMngr.cEndIter())
            isStillColliding = true;
    }

    _isColliding = isStillColliding;

    if (!isStillColliding)
        onInteractionCollisionExit(ctx);
}

auto Interaction::getTriggers() const -> InteractionTriggers
{
    return _triggers;
}

void Interaction::setTriggers(InteractionTriggers triggers)
{
    _triggers = triggers;
}

} // namespace ut::game::cpnt::inter
