#pragma once

namespace ut::core
{
class TextGens;
class Random;
} // namespace ut::core

namespace ut::game
{
class GameState;
struct GameContext;
} // namespace ut::game

namespace ut::game::bt
{
struct BattleContext;
}

namespace ut::scene
{

struct SceneContext
{
    core::TextGens& textGens;
    core::Random& rng;
    game::GameState& gameState;

    game::bt::BattleContext& battleContext;
    game::GameContext* gameContext = nullptr;

    int menuCursorIdx = 0; // which menu item is chosen on `scene::Title` or `game::menu::MainMenu`?
};

} // namespace ut::scene
