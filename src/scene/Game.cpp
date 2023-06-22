#include "scene/Game.hpp"

#include <bn_display.h>
#include <bn_dmg_music.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_music.h>

#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"

namespace ut::scene
{

static constexpr bn::fixed_point INIT_CAM_POS = {bn::display::width() / 2, bn::display::height() / 2};

Game::Game(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _camera(bn::camera_ptr::create(INIT_CAM_POS)), _worldBg(_camera)
{
    context.menuCursorIdx = 0;

    if (bn::music::playing())
        bn::music::stop();
    if (bn::dmg_music::playing())
        bn::dmg_music::stop();

    const auto room = context.gameState.getRoom();
    const auto mTilemap = game::getRoomMTilemap(room);

    BN_ASSERT(mTilemap != nullptr, "Invalid room=", (int)room);

    _worldBg.setMTilemap(*mTilemap);
    _worldBg.allocateGraphics();
}

bool Game::handleInput()
{
    if (bn::keypad::start_pressed() || bn::keypad::l_pressed() || bn::keypad::r_pressed())
        reqStackPush(SceneId::INGAME_MENU);

    return true;
}

bool Game::update()
{
    return true;
}

} // namespace ut::scene
