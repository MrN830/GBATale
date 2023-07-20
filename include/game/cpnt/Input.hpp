#pragma once

#include "game/cpnt/Component.hpp"

namespace ut::game::cmd
{
struct MoveCmd;
}

namespace ut::game::cpnt
{

class Input : public Component
{
public:
    virtual ~Input() = 0;
    Input(ent::Entity&, bn::type_id_t, bool isEnabled);

protected:
    /// @brief Sends `InputCmd` to the `cpnt::SpriteAnim`, and translates entity position
    void sendMoveCmd(const cmd::MoveCmd&, GameContext&);

private:
    void collideWithInteraction(const cmd::MoveCmd&, GameContext&);

    /// @brief If the input command moves the entity
    /// to the wall collider (or other non-enterables),
    /// this function reverts it
    auto fixMoveCmdCollision(const cmd::MoveCmd&, GameContext&) -> cmd::MoveCmd;
};

} // namespace ut::game::cpnt
