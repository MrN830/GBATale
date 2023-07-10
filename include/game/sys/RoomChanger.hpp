#pragma once

#include <cstdint>

namespace ut::mtile
{
enum class WarpId : int8_t;
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

    void reqChange(RoomKind room, mtile::WarpId, GameContext&);
    void instantChange(RoomKind room, mtile::WarpId, GameContext&);

private:
    void changeRoom(GameContext&);

private:
    int _countdown = -1;

    RoomKind _room;
};

} // namespace ut::game::sys
