#include "scene/test/SaveTest.hpp"

#include <bn_keypad.h>
#include <bn_log.h>

#include "game/GameState.hpp"
#include "game/ItemInfo.hpp"

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
        BN_LOG("time saved : ", gameState.getTime().getTicks());
    }
    else if (bn::keypad::select_pressed())
    {
        BN_LOG("=== Save ===");

        auto& gameState = getContext().gameState;

        // tweaked save
        if (bn::keypad::b_held())
        {
            gameState.setCharName("TweakedLongName");
            gameState._exp = 99999;
            gameState._curHp = -1234567890;
            gameState._weapon = game::ItemKind::REAL_KNIFE;
            gameState._armor = game::ItemKind::THE_LOCKET;
            gameState._gold = -1234567890;
            gameState._kills = 1234567890;
            constexpr uint32_t time = ~(0u);
            gameState._time.addTicks(time - gameState._time.getTicks());
        }
        else // normal save
        {
            gameState.setCharName("SaveTs");
        }

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
