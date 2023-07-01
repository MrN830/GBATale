#pragma once

namespace bn
{
class camera_ptr;
}
namespace ut::scene
{
struct SceneContext;
}
namespace ut::game::mngr
{
class EntityManager;
}

namespace ut::game
{

class GameState;

struct GameContext
{
    scene::SceneContext& sceneContext;
    GameState& state;
    bn::camera_ptr& camera;
    game::mngr::EntityManager& entities;
};

} // namespace ut::game
