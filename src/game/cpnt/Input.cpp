#include "game/cpnt/Input.hpp"

#include "game/cmd/InputCmd.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

Input::Input(ent::Entity& entity) : Component(entity)
{
}

void Input::sendInput(const cmd::InputCmd& cmd)
{
    // translate entity position
    _entity.setPosition(_entity.getPosition() + cmd.movePos);

    // send `cmd` to `cpnt::SpriteAnim`
    auto* sprAnim = _entity.getComponent<cpnt::SpriteAnim>();
    if (sprAnim != nullptr)
        sprAnim->receiveInputCmd(cmd);
}

auto Input::fixMoveCmdCollision(const cmd::InputCmd& cmd) -> cmd::InputCmd
{
    // TODO

    return cmd;
}

} // namespace ut::game::cpnt
