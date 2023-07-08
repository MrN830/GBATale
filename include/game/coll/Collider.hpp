#pragma once

#include <bn_intrusive_forward_list.h>

#include "game/coll/CollInfo.hpp"

namespace ut::game::coll
{

class Collider final : public bn::intrusive_forward_list_node_type
{
public:
    Collider(const CollInfo& info);

public:
    auto getInfo() const -> const CollInfo&;

private:
    CollInfo _info;
};

} // namespace ut::game::coll
