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

    /// @return next menu state
    [[nodiscard]] virtual auto handleInput(scene::IngameMenu&) -> MenuStateType = 0;

    /// @return next menu state
    [[nodiscard]] virtual auto update(scene::IngameMenu&) -> MenuStateType = 0;
};

} // namespace ut::game::menu
