#pragma once

#include <bn_string_view.h>

#include "game/InteractStateStack.hpp"
#include "mtile/WarpId.hpp"

#include "consts.hpp"

namespace ut::scene
{
struct SceneContext;
class Game;
} // namespace ut::scene
namespace ut::game::sys
{
class WorldBg;
class TaskManager;
class EntityManager;
class CameraManager;
class FadeManager;
class RoomChanger;
} // namespace ut::game::sys
namespace ut::core
{
struct DialogSettingsOverride;
}

namespace ut::game
{

class GameState;

struct GameContext
{
    scene::SceneContext& sceneContext;
    scene::Game& game;
    GameState& state;

    game::sys::TaskManager& taskMngr;
    game::sys::CameraManager& camMngr;
    game::sys::WorldBg& worldBg;
    game::sys::EntityManager& entMngr;
    game::sys::FadeManager& fadeMngr;
    game::sys::RoomChanger& roomChanger;

    InteractStateStack& interactStack;

    bn::vector<bn::string_view, consts::DIALOG_MAX_LINES>& msg;
    core::DialogSettingsOverride& msgSettings;

    mtile::WarpId warpId = mtile::WarpId::INIT;

    bool isDialogUpper : 1 = false;
    bool isDialogGold : 1 = false;
};

} // namespace ut::game
