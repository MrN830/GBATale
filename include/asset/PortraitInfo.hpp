#pragma once

#include <bn_array.h>
#include <bn_fixed_point.h>
#include <bn_span.h>

namespace bn
{
class sprite_item;
}

namespace ut::asset
{

enum class PortraitFaceKind : uint8_t;

struct IPortraitInfo
{
public:
    static auto get(PortraitFaceKind, uint8_t emotion) -> const IPortraitInfo&;

public:
    virtual ~IPortraitInfo() = default;

    constexpr IPortraitInfo(PortraitFaceKind faceKind_, uint8_t emotionKind_, uint8_t waitUpdate_,
                            uint16_t faceIdleGfxIdx_, int16_t paddingX_, const bn::sprite_item* faceTalk_,
                            const bn::sprite_item* faceBlink_, const bn::sprite_item* torso_, bn::fixed_point faceDiff_,
                            bn::fixed_point torsoDiff_)
        : faceKind(faceKind_), emotionKind(emotionKind_), waitUpdate(waitUpdate_), faceIdleGfxIdx(faceIdleGfxIdx_),
          paddingX(paddingX_), faceTalk(faceTalk_), faceBlink(faceBlink_), torso(torso_), faceDiff(faceDiff_),
          torsoDiff(torsoDiff_)
    {
    }

public:
    virtual auto getTalkGfxIdxes() const -> bn::span<const uint16_t> = 0;
    virtual auto getBlinkGfxIdxes() const -> bn::span<const uint16_t> = 0;

public:
    PortraitFaceKind faceKind;
    uint8_t emotionKind;

public:
    uint8_t waitUpdate;
    uint16_t faceIdleGfxIdx;
    int16_t paddingX; // dialog sprite will start from `DialogSettings.pos + paddingX`
    const bn::sprite_item* faceTalk;
    const bn::sprite_item* faceBlink;
    const bn::sprite_item* torso;
    bn::fixed_point faceDiff;  // diff from `DialogSettings.pos`, before moved by `paddingX`
    bn::fixed_point torsoDiff; // diff from `DialogSettings.pos`, before moved by `paddingX`
};

template <int TalkGfxIdxCnt, int BlinkGfxIdxCnt>
struct PortraitInfo : public IPortraitInfo
{
public:
    constexpr PortraitInfo(PortraitFaceKind faceKind_, uint8_t emotionKind_, uint8_t waitUpdate_,
                           uint16_t faceIdleGfxIdx_, int16_t paddingX_, const bn::sprite_item* faceTalk_,
                           const bn::sprite_item* faceBlink_, const bn::sprite_item* torso_, bn::fixed_point faceDiff_,
                           bn::fixed_point torsoDiff_, const bn::array<uint16_t, TalkGfxIdxCnt> talkGfxIdxes_,
                           const bn::array<uint16_t, BlinkGfxIdxCnt> blinkGfxIdxes_)
        : IPortraitInfo(faceKind_, emotionKind_, waitUpdate_, faceIdleGfxIdx_, paddingX_, faceTalk_, faceBlink_, torso_,
                        faceDiff_, torsoDiff_),
          talkGfxIdxes(talkGfxIdxes_), blinkGfxIdxes(blinkGfxIdxes_)
    {
    }

public:
    auto getTalkGfxIdxes() const -> bn::span<const uint16_t> override
    {
        return talkGfxIdxes;
    }

    auto getBlinkGfxIdxes() const -> bn::span<const uint16_t> override
    {
        return blinkGfxIdxes;
    }

public:
    bn::array<uint16_t, TalkGfxIdxCnt> talkGfxIdxes;
    bn::array<uint16_t, BlinkGfxIdxCnt> blinkGfxIdxes;
};

} // namespace ut::asset
