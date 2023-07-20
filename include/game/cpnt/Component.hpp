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
    virtual ~Component() = 0;
    Component(ent::Entity&, bn::type_id_t);

    auto getType() const -> bn::type_id_t;

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
    const bn::type_id_t _type;

    bool _isEnabled = true;
};

} // namespace ut::game::cpnt
