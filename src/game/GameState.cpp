#include "game/GameState.hpp"

#include <bn_sram.h>

#include "crc32/Crc32.h"

#include "game/ItemInfo.hpp"
#include "game/RoomInfo.hpp"

namespace ut::game
{

GameState::GameState()
{
    resetToNewRegularSave();

    loadFromAllSave();
}

bool GameState::isNewRegularSave() const
{
    return _rSavedCount <= 0;
}

void GameState::resetToNewRegularSave()
{
    _charName = "";
    _lv = 1;
    _exp = 0;
    _curHp = 20;
    _maxHp = 20;
    _baseAtk = 0;
    _baseDef = 0;
    _gold = 0;
    _kills = 0;
    _items = {};
    _dimensionalBoxA = {ItemKind::TOUGH_GLOVE};
    _dimensionalBoxB = {};
    _phone = {};
    _weapon = ItemKind::STICK;
    _armor = ItemKind::BANDAGE;
    _plot = 0;
    _room = RoomKind::ROOM_AREA1;
    _time = 0;

    _rSavedCount = 0;
}

auto GameState::loadFromAllSave(bool checkOnly) -> bn::pair<LoadResult, LoadResult>
{
    LoadResult rLoadResult = loadFromRegularSave(checkOnly);
    // TODO: Load from `PersistSave` too
    LoadResult pLoadResult = LoadResult::FAILED;

    return {rLoadResult, pLoadResult};
}

auto GameState::loadFromRegularSave(bool checkOnly) -> LoadResult
{
    LoadResult rLoadResult = LoadResult::FAILED;

    RegularSave rSave1, rSave2;

    bn::sram::read_offset(rSave1, SRAM_REGU_SAVE_1);
    bn::sram::read_offset(rSave2, SRAM_REGU_SAVE_2);

    const bool isValidRSave1 = rSave1.isValid();
    const bool isValidRSave2 = rSave2.isValid();

    if (isValidRSave1 && isValidRSave2)
    {
        if (rSave1.savedCount >= rSave2.savedCount)
        {
            if (!checkOnly)
                loadFromRSave(rSave1);
            rLoadResult = LoadResult::LOADED_SLOT_1;
        }
        else
        {
            if (!checkOnly)
                loadFromRSave(rSave2);
            rLoadResult = LoadResult::LOADED_SLOT_2;
        }
    }
    else if (isValidRSave1)
    {
        if (!checkOnly)
            loadFromRSave(rSave1);
        rLoadResult = LoadResult::LOADED_SLOT_1;
    }
    else if (isValidRSave2)
    {
        if (!checkOnly)
            loadFromRSave(rSave2);
        rLoadResult = LoadResult::LOADED_SLOT_2;
    }

    return rLoadResult;
}

void GameState::saveRegular()
{
    RegularSave rSave;

    for (int i = 0; i < 8; ++i)
        rSave.charName[i] = (i < _charName.size() ? _charName[i] : '\0');

    rSave.xp = _exp;
    rSave.gold = _gold;
    rSave.kills = _kills;
    rSave.item = _items;
    rSave.dimensionalBoxA = _dimensionalBoxA;
    rSave.dimensionalBoxB = _dimensionalBoxB;
    rSave.phone = _phone;
    rSave.weapon = _weapon;
    rSave.armor = _armor;
    rSave.plot = _plot;
    rSave.room = _room;
    rSave.time = _time;

    for (int i = 0; i < 8; ++i)
        rSave.saveVer[i] = SAVE_VER[i];

    rSave.savedCount = ++_rSavedCount;
    rSave.checksum = crc32_fast(((uint8_t*)&rSave) + sizeof(rSave.checksum), sizeof(rSave) - sizeof(rSave.checksum));

    // Get the recent save slot (so that we can overwrite the old slot)
    const auto rLoadResult = loadFromRegularSave(true);

    // slot 2 is recent -> save to slot 1
    if (rLoadResult == LoadResult::LOADED_SLOT_2)
        bn::sram::write_offset(rSave, SRAM_REGU_SAVE_1);
    else
        bn::sram::write_offset(rSave, SRAM_REGU_SAVE_2);
}

auto GameState::getCharName() const -> const bn::string_view
{
    return _charName;
}

int GameState::getLv() const
{
    return _lv;
}

int GameState::getExp() const
{
    return _exp;
}

int GameState::getCurHp() const
{
    return _curHp;
}

int GameState::getMaxHp() const
{
    return _maxHp;
}

int GameState::getBaseAtk() const
{
    return _baseAtk;
}

int GameState::getBaseDef() const
{
    return _baseDef;
}

int GameState::getGold() const
{
    return _gold;
}

int GameState::getKills() const
{
    return _kills;
}

uint32_t GameState::getTime() const
{
    return _time;
}

uint32_t GameState::getRSavedCount() const
{
    return _rSavedCount;
}

uint32_t GameState::getPSavedCount() const
{
    return _pSavedCount;
}

void GameState::setCharName(const bn::string_view charName)
{
    BN_ASSERT(charName.size() <= _charName.max_size());
    _charName = charName;
}

void GameState::setTime(uint32_t time)
{
    _time = time;
}

void GameState::loadFromRSave(const RegularSave& rSave)
{
    _charName.clear();
    for (char ch : rSave.charName)
        if (ch != '\0')
            _charName.push_back(ch);

    // TODO: Load exp related vals (lv, maxHp, atk, def)
    _exp = rSave.xp;
    _gold = rSave.gold;
    _time = rSave.time;
    _room = rSave.room;
    _weapon = rSave.weapon;
    _armor = rSave.armor;
    _items = rSave.item;
    _dimensionalBoxA = rSave.dimensionalBoxA;
    _dimensionalBoxB = rSave.dimensionalBoxB;
    _phone = rSave.phone;
    _weapon = rSave.weapon;
    _armor = rSave.armor;
    _plot = rSave.plot;
    _room = rSave.room;
    _time = rSave.time;
    _rSavedCount = rSave.savedCount;
}

bool GameState::RegularSave::isValid() const
{
    if (checksum != crc32_fast(((uint8_t*)this) + sizeof(checksum), sizeof(RegularSave) - sizeof(checksum)))
        return false;

    for (int i = 0; i < 8; ++i)
        if (saveVer[i] != SAVE_VER[i])
            return false;

    return true;
}

bool GameState::PersistSave::isValid() const
{
    if (checksum != crc32_fast(((uint8_t*)this) + sizeof(checksum), sizeof(PersistSave) - sizeof(checksum)))
        return false;

    for (int i = 0; i < 8; ++i)
        if (saveVer[i] != SAVE_VER[i])
            return false;

    return true;
}

} // namespace ut::game
