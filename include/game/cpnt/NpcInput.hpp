#pragma once

#include "game/cpnt/Input.hpp"

#include <bn_fixed.h>

#include "game/cmd/MoveCmd.hpp"

namespace ut::asset
{
struct IPath;
};

namespace ut::game::cpnt
{

class NpcInput final : public Input
{
public:
    NpcInput(ent::Entity&, bool isEnabled);

    void handleInput(GameContext&) override;

public:
    void startOneWay(const cmd::MoveCmd&, int lastTicks);
    void startPath(const asset::IPath& path, bn::fixed speed);

    void setPathSpeed(bn::fixed speed);

    void pause();
    void resume();
    void stop(GameContext&);

    bool isPaused() const;
    bool isDone() const;

private:
    void handleOneWay(GameContext&);
    void handlePath(GameContext&);

private:
    enum class Mode : uint8_t
    {
        NONE,
        ONE_WAY,
        PATH,
    };

private:
    Mode _mode = Mode::NONE;
    bool _isPaused = false;

    union {
        struct
        {
            cmd::MoveCmd moveCmd;
            int remainTicks;
        } _oneWay;
        struct
        {
            const asset::IPath* ptr;
            int pathPointIdx;
            bn::fixed speed;
        } _path;
    };
};

} // namespace ut::game::cpnt
