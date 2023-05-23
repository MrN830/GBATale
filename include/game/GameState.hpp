#pragma once

#include <cstdint>

#include <bn_array.h>
#include <bn_fixed.h>
#include <bn_string.h>
#include <bn_utility.h>

namespace ut::game
{

enum class ItemKind : uint8_t;
enum class RoomKind : int16_t;

class GameState final
{
private:
    struct RegularSave;
    struct PersistSave;

public:
    enum class LoadResult
    {
        FAILED,
        LOADED_1,
        LOADED_2
    };

public:
    GameState();

    /**
     * @brief Loads from SRAM.
     * It automatically chooses valid, recent slot.
     *
     * @param `checkOnly` Don't actually load, only use the returned `LoadResult`
     * @return `bn::pair<LoadResult, LoadResult>` `{Regular, Persist}` load results
     */
    [[nodiscard]] auto loadFromSave(bool checkOnly = false) -> bn::pair<LoadResult, LoadResult>;

    void saveRegular();

public:
    auto getCharName() const -> const bn::string_view;
    int getLv() const;
    int getExp() const;
    int getCurHp() const;
    int getMaxHp() const;
    int getBaseAtk() const;
    int getBaseDef() const;
    int getGold() const;
    int getKills() const;

    uint32_t getTime() const;
    uint32_t getRSavedCount() const;
    uint32_t getPSavedCount() const;

    void setTime(uint32_t time);

    // TODO: Add more getters & setters

private:
    void loadFromRSave(const RegularSave&);
    void loadFromPSave(const PersistSave&);

private:
    bn::string<8> _charName = "";
    int _lv = 1;
    int _exp = 0;
    int _curHp = 20;
    int _maxHp = 20;
    int _baseAtk = 0;
    int _baseDef = 0;
    int _gold = 0;
    int _kills = 0;
    bn::array<ItemKind, 8> _items = {};
    bn::array<ItemKind, 12> _dimensionalBoxA = {};
    bn::array<ItemKind, 12> _dimensionalBoxB = {};
    bn::array<int32_t, 8> _phone = {};
    ItemKind _weapon;
    ItemKind _armor;
    bn::fixed _plot = 0;
    RoomKind _room;
    uint32_t _time = 0;

    uint32_t _rSavedCount = 0;
    uint32_t _pSavedCount = 0;

private:
    static constexpr auto SAVE_VER = "ut00000";

    static constexpr int SRAM_REGU_SAVE_1 = 0;
    static constexpr int SRAM_PERSI_SAVE_1 = 8 * 1024;
    static constexpr int SRAM_REGU_SAVE_2 = 16 * 1024;
    static constexpr int SRAM_PERSI_SAVE_2 = 24 * 1024;

    struct RegularSave
    {
        bn::array<char, 8> charName;
        int32_t xp;
        int32_t gold;
        int8_t kills;
        bn::array<ItemKind, 8> item;
        bn::array<ItemKind, 12> dimensionalBoxA;
        bn::array<ItemKind, 12> dimensionalBoxB;
        bn::array<int32_t, 8> phone;
        ItemKind weapon;
        ItemKind armor;
        bn::fixed plot;
        RoomKind room;
        uint32_t time;

        bn::array<char, 8> saveVer;
        uint32_t savedCount;
        uint32_t checksum;

        bool isValid() const;
    };

    struct PersistSave
    {
        bn::array<char, 8> saveVer;
        uint32_t savedCount;
        uint32_t checksum;

        bool isValid() const;
    };
};

} // namespace ut::game
