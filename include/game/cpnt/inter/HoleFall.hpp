#pragma once

#include "game/cpnt/inter/Interaction.hpp"

#include <bn_deque.h>
#include <bn_optional.h>
#include <bn_sprite_ptr.h>

#include "game/ent/EntityActions.hpp"

namespace ut::core
{
enum class Directions : uint8_t;
}

namespace ut::game::cpnt::inter
{

class HoleFall : public Interaction
{
public:
    HoleFall(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    void onInteract(GameContext&) override;

    void update(GameContext&) override;
    void render(GameContext&) override;

private:
    bool isWrongSwitchFall(GameContext&) const;

    void resetFriskFall();

    void initFriskMoveAction();

private:
    ent::Entity* _frisk;

    int _redrawCountdown = -1;
    core::Directions _friskDir;
    bn::optional<ent::EntityMoveToAction> _friskMoveAction;

    bn::deque<bn::sprite_ptr, 16> _holeSprs;
};

} // namespace ut::game::cpnt::inter
