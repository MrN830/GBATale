#pragma once

#include "game/cpnt/Input.hpp"

#include "game/coll/CollInfo.hpp"

namespace ut::game::cpnt
{

class WalkAnimCtrl;

class PlayerInput final : public Input
{
public:
    PlayerInput(ent::Entity&, bool isEnabled, const WalkAnimCtrl&);

    void handleInput(GameContext&) override;

private:
    auto handleWarpCollision(const cmd::MoveCmd&, GameContext&) -> cmd::MoveCmd;

    auto getInteractCollInfo() const -> coll::RectCollInfo;

private:
    const WalkAnimCtrl& _walk;
};

} // namespace ut::game::cpnt
