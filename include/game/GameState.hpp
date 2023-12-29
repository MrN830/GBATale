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
#include "game/PersistData.hpp"

#include "consts.hpp"

namespace ut::scene::test
{
class SaveTest;
}

namespace ut::asset
{
enum class BgmKind : uint8_t;
}

namespace ut::game
{

enum class ItemKind : uint8_t;
enum class RoomKind : int16_t;
enum class GamePlot : uint16_t;

class GameState final
{
    friend bn::ostringstream& operator<<(bn::ostringstream& oss, const GameState&);
    friend class scene::test::SaveTest;

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

    /**
     * @brief Loads persist save from SRAM.
     * It automatically chooses valid, recent slot.
     *
     * @param `checkOnly` Only check the recent save slot (returned result), not actually load it.
     * @return `LoadResult` persist save slot chosen for load
     */
    auto loadFromPersistSave(bool checkOnly = false) -> LoadResult;

    void saveRegular();
    void savePersist();

private: // not SRAM saved fields
    bool _isInBattle = false;
    bool _isSeriousBattle = false;

private: // SRAM saved fields
    bn::string<consts::CHARA_NAME_MAX_LEN> _charName;
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
    GamePlot _plot;
    bool _hasPhone;
    asset::BgmKind _worldBgm;
    RoomKind _room;
    core::PlayTime _time;

    PersistData _persistData;

    uint32_t _rSavedCount;
    uint32_t _pSavedCount = 0;

public: // not SRAM saved fields
    bool isInBattle() const;
    bool isSeriousBattle() const;

    int getMurderLv() const;

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

    auto getPersistData() const -> const PersistData&;
    auto getPersistData() -> PersistData&;

    auto getPlot() const -> GamePlot;

    bool getHasPhone() const;
    auto getWorldBgm() const -> asset::BgmKind;
    auto getRoom() const -> RoomKind;

    auto getTime() const -> const core::PlayTime&;
    auto getTime() -> core::PlayTime&;
    uint32_t getRSavedCount() const;
    uint32_t getPSavedCount() const;

public:
    void setCharName(const bn::string_view);
    void changeHp(int diff);
    void setGold(int gold);
    void setKills(int kills);

    void setWeapon(ItemKind);
    void setArmor(ItemKind);

    void setPlot(GamePlot plot);

    void setHasPhone(bool hasPhone);
    void setWorldBgm(asset::BgmKind);
    void setRoom(RoomKind);

    void setTime(const core::PlayTime& time);

    // TODO: Add more getters & setters

private:
    void loadFromRSave(const RegularSave&);
    void loadFromPSave(const PersistSave&);

private:
    static constexpr auto SAVE_VER = "ut00003";
    static constexpr int SRAM_REGU_SAVE_SIZE = 1 * 1024;
    static constexpr int SRAM_PERSI_SAVE_SIZE = 1 * 1024;

    static constexpr int SRAM_REGU_SAVE_1 = 0;
    static constexpr int SRAM_PERSI_SAVE_1 = SRAM_REGU_SAVE_SIZE;
    static constexpr int SRAM_REGU_SAVE_2 = SRAM_REGU_SAVE_SIZE + SRAM_PERSI_SAVE_SIZE;
    static constexpr int SRAM_PERSI_SAVE_2 = 2 * SRAM_REGU_SAVE_SIZE + SRAM_PERSI_SAVE_SIZE;

    struct RegularSave
    {
        uint32_t checksum;
        uint32_t savedCount;
        bn::array<char, 8> saveVer;

        bn::array<char, consts::CHARA_NAME_MAX_LEN + 1> charName;
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
        GamePlot plot;
        bool menuChoice2;
        asset::BgmKind song;
        RoomKind room;
        uint32_t time;

        bool isValid() const;
    };

    struct PersistSave
    {
        uint32_t checksum;
        uint32_t savedCount;
        bn::array<char, 8> saveVer;

        PersistData data;

        bool isValid() const;
    };

    static_assert(sizeof(RegularSave) <= SRAM_REGU_SAVE_SIZE);
    static_assert(sizeof(PersistSave) <= SRAM_PERSI_SAVE_SIZE);
};

bn::ostringstream& operator<<(bn::ostringstream& oss, const GameState&);

} // namespace ut::game
