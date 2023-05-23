#pragma once

#include <bn_optional.h>
#include <bn_unique_ptr.h>

#include "scene/SceneId.hpp"

namespace ut::core
{
class TextGens;
class Random;
} // namespace ut::core

namespace ut::scene
{

class SceneStack;

class Scene
{
public:
    using UPtr = bn::unique_ptr<Scene>;

    struct Context
    {
        core::TextGens& textGens;
        core::Random& rng;
    };

public:
    virtual ~Scene() = default;

    Scene(SceneStack&, Context&);

    /**
     * @brief Handle input on this scene.
     *
     * @return Whether the other `Scene` below the `SceneStack` must handle the input.
     */
    [[nodiscard]] virtual bool handleInput() = 0;

    /**
     * @brief Update the scene.
     *
     * @return Whether the other `Scene` below the `SceneStack` must be updated.
     */
    [[nodiscard]] virtual bool update() = 0;

protected:
    void reqStackPush(SceneId);
    void reqStackPop();
    void reqStackClear();

    auto getContext() -> Context&;
    auto getContext() const -> const Context&;

private:
    SceneStack& _sceneStack;
    Context& _context;
};

} // namespace ut::scene
