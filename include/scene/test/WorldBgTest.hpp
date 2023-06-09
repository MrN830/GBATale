#pragma once

#include "scene/Scene.hpp"

#include <bn_camera_ptr.h>
#include <bn_sprite_text_generator.h>

#include "game/WorldBg.hpp"

#include "common_info.h"

namespace ut::game
{
enum class RoomKind : int16_t;
}

namespace ut::scene::test
{

class WorldBgTest : public Scene
{
public:
    WorldBgTest(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

private:
    void snapCamToRoom();

private:
    bn::camera_ptr _camera;

    game::WorldBg _worldBg;
    game::RoomKind _curRoom;

    bool _fastMove = false;

    bn::sprite_text_generator _infoTextGen;
    common::info _info;
};

} // namespace ut::scene::test
