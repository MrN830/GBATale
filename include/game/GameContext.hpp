#pragma once

#include "mtile/WarpId.hpp"

namespace bn
{
class camera_ptr;
}
namespace ut::scene
{
struct SceneContext;
}
namespace ut::game::sys
{
class WorldBg;
class EntityManager;
class CameraManager;
class FadeManager;
class RoomChanger;
} // namespace ut::game::sys

namespace ut::game
{

class GameState;

struct GameContext
{
    scene::SceneContext& sceneContext;
    GameState& state;
    game::sys::CameraManager& camMngr;
    game::sys::WorldBg& worldBg;
    game::sys::EntityManager& entMngr;
    game::sys::FadeManager& fadeMngr;
    game::sys::RoomChanger& roomChanger;

    mtile::WarpId warpId = mtile::WarpId::INIT;

    bool isDialogUpper = false;
    bool isShowingUI = false;
};

} // namespace ut::game
