#pragma once

#include <cstdint>

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
public:
    const bn::regular_bg_tiles_item &tilesLower, &tilesUpper, &tilesUpper2;
    const bn::bg_palette_item &palLower, &palUpper, &palUpper2;

public:
    virtual ~MTilemapBase() = default;

    constexpr MTilemapBase(const bn::regular_bg_tiles_item& tilesLower_, const bn::regular_bg_tiles_item& tilesUpper_,
                           const bn::regular_bg_tiles_item& tilesUpper2_, const bn::bg_palette_item& palLower_,
                           const bn::bg_palette_item& palUpper_, const bn::bg_palette_item& palUpper2_)
        : tilesLower(tilesLower_), tilesUpper(tilesUpper_), tilesUpper2(tilesUpper2_), palLower(palLower_),
          palUpper(palUpper_), palUpper2(palUpper2_)
    {
    }

public:
    constexpr virtual int getWidth() const = 0;
    constexpr virtual int getHeight() const = 0;

    constexpr virtual auto getMTileLower(int mCellX, int mCellY) const -> const MTile& = 0;
    constexpr virtual auto getMTileUpper(int mCellX, int mCellY) const -> const MTile& = 0;
    constexpr virtual auto getMTileUpper2(int mCellX, int mCellY) const -> const MTile& = 0;
};

/**
 * @brief Meta-tilemap struct to be hard-coded in ROM.
 * The instance of this struct will be generated with `tilemap_converter.py`.
 *
 * @tparam `MWidth` Meta-cell width
 * @tparam `MHeight` Meta-cell height
 */
template <int MWidth, int MHeight>
struct MTilemap : MTilemapBase
{
    using BoardType = bn::array<MTile, MWidth * MHeight>;

private:
    const BoardType _lower, _upper, _upper2;

public:
    constexpr MTilemap(const bn::regular_bg_tiles_item& tilesLower_, const bn::regular_bg_tiles_item& tilesUpper_,
                       const bn::regular_bg_tiles_item& tilesUpper2_, const bn::bg_palette_item& palLower_,
                       const bn::bg_palette_item& palUpper_, const bn::bg_palette_item& palUpper2_,
                       const BoardType& lower, const BoardType& upper, const BoardType& upper2)
        : MTilemapBase(tilesLower_, tilesUpper_, tilesUpper2_, palLower_, palUpper_, palUpper2_), _lower(lower),
          _upper(upper), _upper2(upper2)
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
};

} // namespace ut::mtile
