#pragma once

namespace ut::scene
{
class IngameMenu;
}

namespace ut::game::menu
{

enum class MenuStateType;

class MenuState
{
public:
    virtual ~MenuState() = default;
    MenuState(scene::IngameMenu& scene) : _scene(scene){};

    /// @return next menu state
    [[nodiscard]] virtual auto handleInput() -> MenuStateType = 0;

    /// @return next menu state
    [[nodiscard]] virtual auto update() -> MenuStateType = 0;

protected:
    scene::IngameMenu& _scene;
};

} // namespace ut::game::menu
