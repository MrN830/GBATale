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
    friend bn::ostringstream& operator<<(bn::ostringstream& oss, const GameState&);

private:
    struct RegularSave;
    struct PersistSave;

public:
    enum class LoadResult
    {
        FAILED,
        LOADED_SLOT_1,
        LOADED_SLOT_2
    };

public:
    GameState();

    bool isNewRegularSave() const;

    // This doesn't reset the charName (not a `True Reset`)
    void resetToNewRegularSave();

    /**
     * @brief Loads both regular & persist saves from SRAM.
     * It automatically chooses valid, recent slot.
     *
     * @param `checkOnly` Only check the recent save slot (returned result), not actually load it.
     * @return `bn::pair<LoadResult, LoadResult>` `{Regular, Persist}` save slots chosen for load
     */
    auto loadFromAllSave(bool checkOnly = false) -> bn::pair<LoadResult, LoadResult>;

    /**
     * @brief Loads regular save from SRAM.
     * It automatically chooses valid, recent slot.
     *
     * @param `checkOnly` Only check the recent save slot (returned result), not actually load it.
     * @return `LoadResult` regular save slot chosen for load
     */
    auto loadFromRegularSave(bool checkOnly = false) -> LoadResult;

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

    void setCharName(const bn::string_view);

    void setTime(uint32_t time);

    // TODO: Add more getters & setters

private:
    void loadFromRSave(const RegularSave&);
    void loadFromPSave(const PersistSave&);

private:
    bn::string<8> _charName;
    int _lv;
    int _exp;
    int _curHp;
    int _maxHp;
    int _baseAtk;
    int _baseDef;
    int _gold;
    int _kills;
    bn::array<ItemKind, 8> _items;
    bn::array<ItemKind, 12> _dimensionalBoxA;
    bn::array<ItemKind, 12> _dimensionalBoxB;
    bn::array<int32_t, 8> _phone;
    ItemKind _weapon;
    ItemKind _armor;
    bn::fixed _plot;
    RoomKind _room;
    uint32_t _time;

    uint32_t _rSavedCount;
    uint32_t _pSavedCount = 0;

private:
    static constexpr auto SAVE_VER = "ut00000";

    static constexpr int SRAM_REGU_SAVE_1 = 0;
    static constexpr int SRAM_PERSI_SAVE_1 = 8 * 1024;
    static constexpr int SRAM_REGU_SAVE_2 = 16 * 1024;
    static constexpr int SRAM_PERSI_SAVE_2 = 24 * 1024;

    struct RegularSave
    {
        uint32_t checksum;
        uint32_t savedCount;
        bn::array<char, 8> saveVer;

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

        bool isValid() const;
    };

    struct PersistSave
    {
        uint32_t checksum;
        uint32_t savedCount;
        bn::array<char, 8> saveVer;

        bool isValid() const;
    };
};

bn::ostringstream& operator<<(bn::ostringstream& oss, const GameState&);

} // namespace ut::game
