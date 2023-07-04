#pragma once

#include "game/cpnt/IInput.hpp"

namespace ut::game::cpnt
{

class PlayerInput : public IInput
{
public:
    PlayerInput(ent::Entity&);

    auto getType() const -> bn::type_id_t override;

    void handleInput(GameContext&) override;
};

} // namespace ut::game::cpnt
