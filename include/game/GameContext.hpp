#pragma once

#include "game/InteractState.hpp"
#include "mtile/WarpId.hpp"

namespace bn
{
class camera_ptr;
}
namespace ut::scene
{
struct SceneContext;
class Game;
} // namespace ut::scene
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
    scene::Game& game;
    GameState& state;

    game::sys::CameraManager& camMngr;
    game::sys::WorldBg& worldBg;
    game::sys::EntityManager& entMngr;
    game::sys::FadeManager& fadeMngr;
    game::sys::RoomChanger& roomChanger;

    mtile::WarpId warpId = mtile::WarpId::INIT;

    bool isDialogUpper = false;
    InteractState interactState = InteractState::FREE;
};

} // namespace ut::game
