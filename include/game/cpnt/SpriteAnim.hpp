#pragma once

#include "game/cpnt/Component.hpp"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>
#include <bn_vector.h>

namespace ut::asset
{
enum class SpriteAnimKind : int16_t;
}
namespace ut::game::cmd
{
struct InputCmd;
}

namespace ut::game::cpnt
{

class Sprite;

class SpriteAnim : public Component
{
public:
    SpriteAnim(ent::Entity&, cpnt::Sprite&);

    auto getType() const -> bn::type_id_t override;

    void render(GameContext&) override;
    void renderOnce();

    bool isManualRender() const;

public:
    void receiveInputCmd(const cmd::InputCmd&);

    bool hasDirectionAnims() const;
    void registerDirectionAnimKinds(asset::SpriteAnimKind up, asset::SpriteAnimKind down, asset::SpriteAnimKind left,
                                    asset::SpriteAnimKind right);

    auto getCurAnimKind() const -> asset::SpriteAnimKind;
    void setCurAnimKind(asset::SpriteAnimKind);

private:
    void setCurAnimKind(asset::SpriteAnimKind, bool preserveRenderCount);

private:
    Sprite& _sprCpnt;

    bool _isManualRender = false;

    asset::SpriteAnimKind _up;
    asset::SpriteAnimKind _down;
    asset::SpriteAnimKind _left;
    asset::SpriteAnimKind _right;

    asset::SpriteAnimKind _curAnimKind;
    bn::optional<bn::sprite_animate_action<4>> _action;

    uint16_t _curRenderCount = 0;
};

} // namespace ut::game::cpnt
