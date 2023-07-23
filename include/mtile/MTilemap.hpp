#pragma once

#include <cstdint>

#include <bn_array.h>
#include <bn_span.h>

#include "game/coll/CollInfo.hpp"
#include "game/ent/EntityInfo.hpp"
#include "mtile/SpawnPoints.hpp"

namespace bn
{
class regular_bg_tiles_item;
class bg_palette_item;
} // namespace bn

namespace ut::mtile
{

using MTileIdx = uint8_t;

struct MTile
{
    MTileIdx mTileIdx : 8;
    bool hFlip : 1;
    bool vFlip : 1;
};

inline constexpr MTile EMPTY_M_TILE = {0, false, false};

/**
 * @brief Parent type of all `MTilemap<MWidth, MHeight>`.
 */
struct MTilemapBase
{
    using WarpPoints = bn::array<bn::fixed_point, (int)WarpId::TOTAL_COUNT>;

public:
    const bn::regular_bg_tiles_item &tilesLower, &tilesUpper, &tilesUpper2;
    const bn::bg_palette_item &palLower, &palUpper, &palUpper2;

private:
    const WarpPoints _warpPoints;

public:
    virtual ~MTilemapBase() = default;

    constexpr MTilemapBase(const bn::regular_bg_tiles_item& tilesLower_, const bn::regular_bg_tiles_item& tilesUpper_,
                           const bn::regular_bg_tiles_item& tilesUpper2_, const bn::bg_palette_item& palLower_,
                           const bn::bg_palette_item& palUpper_, const bn::bg_palette_item& palUpper2_,
                           const WarpPoints& warpPoints)
        : tilesLower(tilesLower_), tilesUpper(tilesUpper_), tilesUpper2(tilesUpper2_), palLower(palLower_),
          palUpper(palUpper_), palUpper2(palUpper2_), _warpPoints(warpPoints)
    {
    }

public:
    constexpr const bn::fixed_point* getWarpPoint(WarpId warpId) const
    {
        BN_ASSERT(0 <= (int)warpId && (int)warpId < (int)WarpId::TOTAL_COUNT);

        const auto& warpPoint = _warpPoints[(int)warpId];
        if (warpPoint.x() < 0 || warpPoint.y() < 0)
            return nullptr;

        return &warpPoint;
    };

public:
    constexpr virtual int getWidth() const = 0;
    constexpr virtual int getHeight() const = 0;

    constexpr virtual auto getMTileLower(int mCellX, int mCellY) const -> const MTile& = 0;
    constexpr virtual auto getMTileUpper(int mCellX, int mCellY) const -> const MTile& = 0;
    constexpr virtual auto getMTileUpper2(int mCellX, int mCellY) const -> const MTile& = 0;

    constexpr virtual auto getEntities() const -> bn::span<const game::ent::EntityInfo> = 0;
    constexpr virtual auto getRectWalls() const -> bn::span<const game::coll::RectCollInfo> = 0;
    constexpr virtual auto getTriWalls() const -> bn::span<const game::coll::AAIRTriCollInfo> = 0;

    constexpr virtual auto getWarps() const -> bn::span<const Warp> = 0;
};

/**
 * @brief Meta-tilemap struct to be hard-coded in ROM.
 * The instance of this struct will be generated with `tilemap_converter.py`.
 *
 * @tparam `MWidth` Meta-cell width
 * @tparam `MHeight` Meta-cell height
 */
template <int MWidth, int MHeight, int EntityCount, int RectWallCount, int TriWallCount, int WarpCount>
struct MTilemap : MTilemapBase
{
    using BoardType = bn::array<MTile, MWidth * MHeight>;

    using Entities = bn::array<game::ent::EntityInfo, EntityCount>;
    using RectWalls = bn::array<game::coll::RectCollInfo, RectWallCount>;
    using TriWalls = bn::array<game::coll::AAIRTriCollInfo, TriWallCount>;
    using Warps = bn::array<Warp, WarpCount>;

private:
    const Entities _entities;
    const RectWalls _rectWalls;
    const TriWalls _triWalls;
    const Warps _warps;
    const BoardType _lower, _upper, _upper2;

public:
    constexpr MTilemap(const bn::regular_bg_tiles_item& tilesLower_, const bn::regular_bg_tiles_item& tilesUpper_,
                       const bn::regular_bg_tiles_item& tilesUpper2_, const bn::bg_palette_item& palLower_,
                       const bn::bg_palette_item& palUpper_, const bn::bg_palette_item& palUpper2_,
                       const Entities& entities, const RectWalls& rectWalls, const TriWalls& triWalls,
                       const Warps& warps, const WarpPoints& warpPoints, const BoardType& lower, const BoardType& upper,
                       const BoardType& upper2)
        : MTilemapBase(tilesLower_, tilesUpper_, tilesUpper2_, palLower_, palUpper_, palUpper2_, warpPoints),
          _entities(entities), _rectWalls(rectWalls), _triWalls(triWalls), _warps(warps), _lower(lower), _upper(upper),
          _upper2(upper2)
    {
    }

public:
    constexpr int getWidth() const override
    {
        return MWidth;
    }

    constexpr int getHeight() const override
    {
        return MHeight;
    }

    constexpr const MTile& getMTileLower(int mCellX, int mCellY) const override
    {
        if (mCellX < 0 || mCellX >= MWidth || mCellY < 0 || mCellY >= MHeight)
            return EMPTY_M_TILE;

        return _lower[mCellY * MWidth + mCellX];
    }

    constexpr const MTile& getMTileUpper(int mCellX, int mCellY) const override
    {
        if (mCellX < 0 || mCellX >= MWidth || mCellY < 0 || mCellY >= MHeight)
            return EMPTY_M_TILE;

        return _upper[mCellY * MWidth + mCellX];
    }

    constexpr const MTile& getMTileUpper2(int mCellX, int mCellY) const override
    {
        if (mCellX < 0 || mCellX >= MWidth || mCellY < 0 || mCellY >= MHeight)
            return EMPTY_M_TILE;

        return _upper2[mCellY * MWidth + mCellX];
    }

    constexpr bn::span<const game::ent::EntityInfo> getEntities() const
    {
        return _entities;
    }

    constexpr bn::span<const game::coll::RectCollInfo> getRectWalls() const override
    {
        return _rectWalls;
    }

    constexpr bn::span<const game::coll::AAIRTriCollInfo> getTriWalls() const override
    {
        return _triWalls;
    }

    constexpr bn::span<const Warp> getWarps() const override
    {
        return _warps;
    }
};

} // namespace ut::mtile
