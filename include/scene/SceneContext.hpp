#pragma once

namespace ut::core
{
class TextGens;
class Random;
} // namespace ut::core

namespace ut::game
{
class GameState;
}

namespace ut::scene
{

struct SceneContext
{
    core::TextGens& textGens;
    core::Random& rng;
    game::GameState& gameState;
    int menuCursorIdx = 0; // which menu item is chosen on `scene::Title` or `game::menu::MainMenu`?
};

} // namespace ut::scene
