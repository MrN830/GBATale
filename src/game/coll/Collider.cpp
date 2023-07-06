#include "game/coll/Collider.hpp"

namespace ut::game::coll
{

Collider::~Collider()
{
    getInfo().~CollInfo();
}

auto Collider::getInfo() const -> const CollInfo&
{
    return *reinterpret_cast<const CollInfo*>(_collInfo);
}

} // namespace ut::game::coll
