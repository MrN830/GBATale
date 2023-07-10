#pragma once

#include <bn_optional.h>
#include <bn_unique_ptr.h>

#include "scene/SceneContext.hpp"
#include "scene/SceneId.hpp"

namespace ut::scene
{

class SceneStack;

class Scene
{
public:
    using UPtr = bn::unique_ptr<Scene>;

public:
    virtual ~Scene() = default;

    Scene(SceneStack&, SceneContext&);

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

    auto getContext() -> SceneContext&;
    auto getContext() const -> const SceneContext&;

private:
    SceneStack& _sceneStack;
    SceneContext& _context;
};

} // namespace ut::scene
