#include "game/cpnt/IInput.hpp"

#include "game/cmd/InputCmd.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

IInput::IInput(ent::Entity& entity) : Component(entity)
{
}

void IInput::sendInput(const cmd::InputCmd& cmd)
{
    // translate entity position
    _entity.setPosition(_entity.getPosition() + cmd.movePos);

    // send `cmd` to `cpnt::SpriteAnim`
    auto* sprAnim = _entity.getComponent<cpnt::SpriteAnim>();
    if (sprAnim != nullptr)
        sprAnim->receiveInputCmd(cmd);
}

auto IInput::fixMoveCmdCollision(const cmd::InputCmd& cmd) -> cmd::InputCmd
{
    // TODO

    return cmd;
}

} // namespace ut::game::cpnt
