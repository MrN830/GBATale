#pragma once

#include "game/cpnt/Input.hpp"

namespace ut::game::cpnt
{

class PlayerInput final : public Input
{
public:
    PlayerInput(ent::Entity&);

    auto getType() const -> bn::type_id_t override;

    void handleInput(GameContext&) override;
};

} // namespace ut::game::cpnt
