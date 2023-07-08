#include "game/coll/Collider.hpp"

namespace ut::game::coll
{

Collider::Collider(const CollInfo& info) : _info(info)
{
}

auto Collider::getInfo() const -> const CollInfo&
{
    return _info;
}

} // namespace ut::game::coll
