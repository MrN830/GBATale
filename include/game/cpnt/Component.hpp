#pragma once

#include <bn_intrusive_forward_list.h>
#include <bn_type_id.h>

namespace ut::game
{
struct GameContext;
}

namespace ut::game::ent
{
class Entity;
}

namespace ut::game::cpnt
{

class Component : public bn::intrusive_forward_list_node_type
{
public:
    virtual ~Component() = default;
    Component(ent::Entity&);

    virtual auto getType() const -> bn::type_id_t = 0;

    virtual void handleInput(GameContext&){};
    virtual void update(GameContext&){};
    virtual void render(GameContext&){};

    virtual void onEntityActiveChanged(bool isEntityActive);

public:
    bool isEnabled() const;
    virtual void setEnabled(bool isEnabled);

protected:
    ent::Entity& _entity;

private:
    bool _isEnabled = true;
};

} // namespace ut::game::cpnt
