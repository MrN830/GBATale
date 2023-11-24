#pragma once

#include "game/cpnt/Component.hpp"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>
#include <bn_vector.h>

namespace ut::asset
{
enum class SpriteAnimKind : int16_t;
}

namespace ut::game::cpnt
{

class Sprite;
class WalkAnimCtrl;

class SpriteAnim : public Component
{
    friend class WalkAnimCtrl;

public:
    SpriteAnim(ent::Entity&, bool isEnabled, cpnt::Sprite&);

    void render(GameContext&) override;
    void renderOnce(GameContext&);

public:
    bool isDone() const;
    bool isManualRender() const;

    auto getCurAnimKind() const -> asset::SpriteAnimKind;
    void setCurAnimKind(asset::SpriteAnimKind);

private:
    bool _isManualRender = false;

    Sprite& _sprCpnt;
    WalkAnimCtrl* _walkAnimCtrl = nullptr;

    asset::SpriteAnimKind _curAnimKind;
    bn::optional<bn::sprite_animate_action<8>> _action;

    int _lastAnimFrameWait = -1;
};

} // namespace ut::game::cpnt
