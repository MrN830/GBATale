#pragma once

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
class EntityManager;
class CameraManager;
} // namespace ut::game::sys

namespace ut::game
{

class GameState;

struct GameContext
{
    scene::SceneContext& sceneContext;
    GameState& state;
    game::sys::CameraManager& camMngr;
    game::sys::EntityManager& entMngr;

    bool isDialogUpper = false;
    bool isShowingUI = false;
};

} // namespace ut::game
