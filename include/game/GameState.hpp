#pragma once

#include <cstdint>

#include <bn_array.h>
#include <bn_fixed.h>
#include <bn_string.h>
#include <bn_utility.h>
#include <bn_vector.h>

#include "core/PlayTime.hpp"
#include "core/Random.hpp"
#include "game/GameFlags.hpp"

namespace ut::scene::test
{
class SaveTest;
}

namespace ut::game
{

enum class ItemKind : uint8_t;
enum class RoomKind : int16_t;

class GameState final
{
    friend bn::ostringstream& operator<<(bn::ostringstream& oss, const GameState&);
    friend class scene::test::SaveTest;

public:
    static constexpr int CHAR_NAME_MAX_LEN = 15;

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
    GameState(core::Random& rng);

    bool isNewRegularSave() const;

    // This doesn't reset the charName (not a `True Reset`)
    void resetToNewRegularSave(core::Random& rng);

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

private: // not SRAM saved fields
    bool _isInBattle = false;
    bool _isSeriousBattle = false;

private: // SRAM saved fields
    bn::string<CHAR_NAME_MAX_LEN> _charName;
    int _lv;
    int _exp;
    int _curHp;
    int _maxHp;
    int _baseAtk;
    int _baseDef;
    int _gold;
    int _kills;
    bn::vector<ItemKind, 8> _items;
    bn::vector<ItemKind, 12> _dimensionalBoxA;
    bn::vector<ItemKind, 12> _dimensionalBoxB;
    bn::array<int32_t, 8> _phone;
    ItemKind _weapon;
    ItemKind _armor;
    GameFlags _flags;
    bn::fixed _plot;
    bool _hasPhone;
    RoomKind _room;
    core::PlayTime _time;

    uint32_t _rSavedCount;
    uint32_t _pSavedCount = 0;

public: // not SRAM saved fields
    bool isInBattle() const;
    bool isSeriousBattle() const;

public: // SRAM saved fields
    auto getCharName() const -> const bn::string_view;
    int getLv() const;
    int getExp() const;
    int getCurHp() const;
    int getMaxHp() const;
    int getBaseAtk() const;
    int getBaseDef() const;
    int getGold() const;
    int getKills() const;

    auto getItems() const -> decltype((_items));
    auto getItems() -> decltype((_items));
    auto getDimensionalBoxA() const -> decltype((_dimensionalBoxA));
    auto getDimensionalBoxA() -> decltype((_dimensionalBoxA));
    auto getDimensionalBoxB() const -> decltype((_dimensionalBoxB));
    auto getDimensionalBoxB() -> decltype((_dimensionalBoxB));

    auto getWeapon() const -> ItemKind;
    auto getArmor() const -> ItemKind;

    auto getFlags() const -> const GameFlags&;
    auto getFlags() -> GameFlags&;

    bool getHasPhone() const;
    auto getRoom() const -> RoomKind;

    auto getTime() const -> const core::PlayTime&;
    auto getTime() -> core::PlayTime&;
    uint32_t getRSavedCount() const;
    uint32_t getPSavedCount() const;

public:
    void setCharName(const bn::string_view);
    void changeHp(int diff);

    void setWeapon(ItemKind);
    void setArmor(ItemKind);

    void setHasPhone(bool hasPhone);
    void setRoom(RoomKind);

    void setTime(const core::PlayTime& time);

    // TODO: Add more getters & setters

private:
    void loadFromRSave(const RegularSave&);
    void loadFromPSave(const PersistSave&);

private:
    static constexpr auto SAVE_VER = "ut00001";

    static constexpr int SRAM_REGU_SAVE_1 = 0;
    static constexpr int SRAM_PERSI_SAVE_1 = 8 * 1024;
    static constexpr int SRAM_REGU_SAVE_2 = 16 * 1024;
    static constexpr int SRAM_PERSI_SAVE_2 = 24 * 1024;

    struct RegularSave
    {
        uint32_t checksum;
        uint32_t savedCount;
        bn::array<char, 8> saveVer;

        bn::array<char, CHAR_NAME_MAX_LEN + 1> charName;
        int32_t xp;
        int32_t gold;
        int32_t kills;
        bn::array<ItemKind, 8> item;
        bn::array<ItemKind, 12> dimensionalBoxA;
        bn::array<ItemKind, 12> dimensionalBoxB;
        bn::array<int32_t, 8> phone;
        ItemKind weapon;
        ItemKind armor;
        GameFlags flag;
        bn::fixed plot;
        bool menuChoice2;
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
