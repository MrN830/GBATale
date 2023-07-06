#pragma once

#include <type_traits>

#include <bn_intrusive_forward_list.h>

#include "game/coll/CollInfo.hpp"

namespace ut::game::coll
{

class Collider final : public bn::intrusive_forward_list_node_type
{
public:
    template <typename CInfo>
        requires(std::is_base_of_v<CollInfo, CInfo> && !std::is_same_v<CollInfo, CInfo>)
    Collider(const CInfo& info)
    {
        new (_collInfo) CInfo(info);
    }

    ~Collider();

public:
    auto getInfo() const -> const CollInfo&;

private:
    alignas(INFO_ALIGN_SIZE) uint8_t _collInfo[INFO_MAX_SIZE];
};

} // namespace ut::game::coll
