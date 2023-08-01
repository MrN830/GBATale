#pragma once

#include <cstdint>

namespace ut::mtile
{
struct Warp;
}

namespace ut::game
{
enum class RoomKind : int16_t;
struct GameContext;
} // namespace ut::game

namespace ut::game::sys
{

class RoomChanger final
{
public:
    RoomChanger();

    void update(GameContext&);

    bool isChanging() const;

    void reqChange(const mtile::Warp&, GameContext&);
    void instantChange(const mtile::Warp&, GameContext&);

private:
    void changeRoom(GameContext&);

    void bgmFadeOut(const mtile::Warp&, GameContext&);

private:
    int _countdown = -1;

    RoomKind _room;
};

} // namespace ut::game::sys
