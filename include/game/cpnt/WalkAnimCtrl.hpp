#pragma once

#include "game/cpnt/Component.hpp"

namespace ut::asset
{
enum class SpriteAnimKind : int16_t;
enum class WalkAnimKind : int8_t;
} // namespace ut::asset
namespace ut::core
{
enum class Directions : uint8_t;
}
namespace ut::game::cmd
{
struct MoveCmd;
}

namespace ut::game::cpnt
{

class SpriteAnim;

class WalkAnimCtrl : public Component
{
    friend class SpriteAnim;

public:
    WalkAnimCtrl(ent::Entity&, bool isEnabled, SpriteAnim&);
    ~WalkAnimCtrl();

    void renderOnce();

public:
    void receiveMoveCmd(const cmd::MoveCmd&);

    bool hasWalkAnim() const;
    void registerWalkAnimKind(asset::WalkAnimKind);

    auto getLastAnimDir() const -> core::Directions;
    void setStandStillDir(core::Directions);

private:
    void setCurAnimKind(asset::SpriteAnimKind, bool preserveRenderCount);

private:
    core::Directions _lastAnimDir;
    uint16_t _curRenderCount = 0;

    SpriteAnim& _sprAnim;

    asset::WalkAnimKind _walkAnimKind;
};

} // namespace ut::game::cpnt
