#pragma once

#include "game/cpnt/Component.hpp"

namespace ut::game::cmd
{
struct InputCmd;
}

namespace ut::game::cpnt
{

class Input : public Component
{
public:
    Input(ent::Entity&);

protected:
    /// @brief Sends `InputCmd` to the `cpnt::SpriteAnim`, and translates entity position
    void sendInput(const cmd::InputCmd&);

    /// @brief If the input command moves the entity
    /// to the wall collider (or other non-enterables),
    /// this function reverts it
    auto fixMoveCmdCollision(const cmd::InputCmd&) -> cmd::InputCmd;
};

} // namespace ut::game::cpnt
