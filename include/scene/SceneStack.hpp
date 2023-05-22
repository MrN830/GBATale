#pragma once

#include <functional>
#include <type_traits>

#include <bn_array.h>
#include <bn_vector.h>

#include "scene/Scene.hpp"
#include "scene/SceneId.hpp"

namespace ut::scene
{

/**
 * @brief State stack for `Scene`.
 * Copycat of https://github.com/SFML/SFML-Game-Development-Book/blob/master/05_States/Include/Book/StateStack.hpp
 */
class SceneStack
{
private:
    static constexpr int STACK_MAX_SIZE = 3;
    static constexpr int PENDING_CHANGE_MAX_SIZE = 2 * STACK_MAX_SIZE;

public:
    SceneStack(const SceneStack&) = delete;
    SceneStack& operator=(const SceneStack&) = delete;

public:
    SceneStack(Scene::Context&);

    template <typename TScene>
        requires std::is_base_of_v<Scene, TScene>
    void registerScene(SceneId);

public:
    void handleInput();
    void update();

    void pushScene(SceneId);
    void popScene();
    void clearScene();

    bool isEmpty() const;

private:
    auto createScene(SceneId) -> Scene::UPtr;
    void applyPendingChanges();

private:
    struct PendingChange
    {
        enum class Kind
        {
            PUSH,
            POP,
            CLEAR
        };

        explicit PendingChange(Kind, SceneId = SceneId::NONE);

        Kind kind;
        SceneId sceneId;
    };

private:
    bn::vector<Scene::UPtr, STACK_MAX_SIZE> _stack;
    bn::vector<PendingChange, PENDING_CHANGE_MAX_SIZE> _pendingChanges;

    Scene::Context& _context;
    bn::array<std::function<Scene::UPtr()>, (int)SceneId::TOTAL_COUNT> _sceneFactories;
};

template <typename TScene>
    requires std::is_base_of_v<Scene, TScene>
void SceneStack::registerScene(SceneId sceneId)
{
    _sceneFactories[(int)sceneId] = [this]() { return Scene::UPtr(new TScene(*this, _context)); };
}

} // namespace ut::scene
