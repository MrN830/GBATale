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
        gameState.resetToNewRegularSave();

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
        // ruins items save
        if (bn::keypad::a_held())
        {
            gameState.setCharName("RuinItem");
            gameState._weapon = game::ItemKind::STICK;
            gameState._armor = game::ItemKind::BANDAGE;
            gameState._gold = 42;
            gameState._time.addTicks(72 * 60 * 30); // 72 minutes

            auto& items = gameState.getItems();
            items.clear();
            items.push_back(game::ItemKind::MONSTER_CANDY);
            items.push_back(game::ItemKind::SPIDER_DONUT);
            items.push_back(game::ItemKind::SPIDER_CIDER);
            items.push_back(game::ItemKind::FADED_RIBBON);
            items.push_back(game::ItemKind::TOY_KNIFE);
            items.push_back(game::ItemKind::BUTTERSCOTCH_PIE);
            items.push_back(game::ItemKind::ABANDONED_QUICHE);
            items.push_back(game::ItemKind::PUPPYDOUGH_ICECREAM);
        }
        else // normal save
        {
            gameState.setCharName("SaveTs");
        }

        gameState.saveRegular();

        BN_LOG("charName: \"", gameState.getCharName(), "\" saved!");
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
