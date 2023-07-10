#pragma once

#include "game/RoomInfo.hpp"
#include "game/coll/CollInfo.hpp"
#include "mtile/WarpId.hpp"

namespace ut::mtile
{

struct Warp
{
    game::coll::RectCollInfo collInfo;
    game::RoomKind room;
    WarpId warpId;
};

} // namespace ut::mtile
