#pragma once

#include <bn_array.h>
#include <bn_fixed_point.h>
#include <bn_span.h>

namespace bn
{
class sprite_item;
}
namespace ut::core
{
enum class Directions : uint8_t;
}

namespace ut::asset
{

enum class SpriteAnimKind : int16_t;

struct ISpriteAnimInfo
{
public:
    static auto get(SpriteAnimKind) -> const ISpriteAnimInfo&;

public:
    virtual ~ISpriteAnimInfo() = default;

    constexpr ISpriteAnimInfo(const bn::sprite_item& sprItem_, core::Directions dirs, const bn::fixed_point& diff_,
                              uint16_t waitUpdate_, bool forever_, bool manualRender_, bool hFlip_, bool vFlip_)
        : sprItem(sprItem_), diff(diff_), waitUpdate(waitUpdate_), directions(dirs), forever(forever_),
          manualRender(manualRender_), hFlip(hFlip_), vFlip(vFlip_)
    {
    }

public:
    virtual auto getGfxIdxes() const -> bn::span<const uint16_t> = 0;

public:
    const bn::sprite_item& sprItem;
    bn::fixed_point diff;
    uint16_t waitUpdate;
    core::Directions directions;
    bool forever : 1;
    bool manualRender : 1;
    bool hFlip : 1;
    bool vFlip : 1;
};

template <int GfxIdxCnt>
struct SpriteAnimInfo : public ISpriteAnimInfo
{
public:
    constexpr SpriteAnimInfo(const bn::sprite_item& sprItem_, core::Directions dirs, const bn::fixed_point& diff_,
                             uint16_t waitUpdate_, bool forever_, bool manualRender_, bool hFlip_, bool vFlip_,
                             const bn::array<uint16_t, GfxIdxCnt>& gfxIdxes_)
        : ISpriteAnimInfo(sprItem_, dirs, diff_, waitUpdate_, forever_, manualRender_, hFlip_, vFlip_),
          gfxIdxes(gfxIdxes_)
    {
    }

public:
    auto getGfxIdxes() const -> bn::span<const uint16_t> override
    {
        return gfxIdxes;
    }

public:
    bn::array<uint16_t, GfxIdxCnt> gfxIdxes;
};

} // namespace ut::asset
