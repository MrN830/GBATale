#include "scene/test/SaveTest.hpp"

#include <bn_keypad.h>
#include <bn_log.h>

#include "game/GameState.hpp"

namespace ut::scene::test
{

SaveTest::SaveTest(SceneStack& sceneStack, Context& context) : Scene(sceneStack, context)
{
}

bool SaveTest::handleInput()
{
    if (bn::keypad::start_pressed())
    {
        BN_LOG("==== Load  ===");

        auto& gameState = getContext().gameState;
        const auto [rLoadResult, pLoadResult] = gameState.loadFromAllSave();

        BN_LOG("rLoadResult: ", (int)rLoadResult);
        BN_LOG("charName   : ", gameState.getCharName());
        BN_LOG("rSavedCount: ", gameState.getRSavedCount());
        BN_LOG("time saved : ", gameState.getTime());
    }
    else if (bn::keypad::select_pressed())
    {
        BN_LOG("=== Save ===");

        auto& gameState = getContext().gameState;

        gameState.setCharName("SaveTs");
        gameState.saveRegular();

        BN_LOG("saved!");
    }

    return true;
}

bool SaveTest::update()
{
    auto& gameState = getContext().gameState;

    gameState.getTime().addTicks(1);

    return true;
}

} // namespace ut::scene::test
