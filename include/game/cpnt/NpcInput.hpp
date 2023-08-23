#pragma once

#include "game/cpnt/Input.hpp"

#include "game/cmd/MoveCmd.hpp"

namespace ut::game::cpnt
{

class NpcInput final : public Input
{
public:
    NpcInput(ent::Entity&, bool isEnabled);

    void handleInput(GameContext&) override;

public:
    void startInput(const cmd::MoveCmd&, int lastTicks);
    void stopInput();

    bool isDone() const;

private:
    cmd::MoveCmd _moveCmd;
    int _remainTicks = 0;
};

} // namespace ut::game::cpnt
