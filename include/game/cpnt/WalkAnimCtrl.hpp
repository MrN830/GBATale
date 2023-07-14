#pragma once

#include "game/cpnt/Component.hpp"

namespace ut::asset
{
enum class SpriteAnimKind : int16_t;
}
namespace ut::core
{
enum class Directions : uint8_t;
}
namespace ut::game::cmd
{
struct InputCmd;
}

namespace ut::game::cpnt
{

class SpriteAnim;

class WalkAnimCtrl : public Component
{
    friend class SpriteAnim;

public:
    WalkAnimCtrl(ent::Entity&, SpriteAnim&);

    auto getType() const -> bn::type_id_t override;

    void renderOnce();

public:
    void receiveInputCmd(const cmd::InputCmd&);

    bool hasDirectionAnims() const;
    void registerDirectionAnimKinds(asset::SpriteAnimKind up, asset::SpriteAnimKind down, asset::SpriteAnimKind left,
                                    asset::SpriteAnimKind right);

    auto getLastAnimDir() const -> core::Directions;
    void setStandStillDir(core::Directions);

private:
    void setCurAnimKind(asset::SpriteAnimKind, bool preserveRenderCount);

private:
    core::Directions _lastAnimDir;
    uint16_t _curRenderCount = 0;

    SpriteAnim& _sprAnim;

    asset::SpriteAnimKind _up;
    asset::SpriteAnimKind _down;
    asset::SpriteAnimKind _left;
    asset::SpriteAnimKind _right;
};

} // namespace ut::game::cpnt
