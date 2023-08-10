#include "game/GameState.hpp"

#include <bn_log.h>
#include <bn_sram.h>

#include "crc32/Crc32.h"

#include "asset/Bgm.hpp"
#include "game/GamePlot.hpp"
#include "game/ItemInfo.hpp"
#include "game/RoomInfo.hpp"
#include "game/StatInfo.hpp"
#include "util/String.hpp"

namespace ut::game
{

GameState::GameState(core::Random& rng) : _time(0)
{
    resetToNewRegularSave(rng);
    _charName = "";
    _rSavedCount = 0;

    const auto [rRes, pRes] = loadFromAllSave();
    BN_LOG("`RegularSave` load ", (rRes == LoadResult::FAILED ? "FAILED" : "SUCCESS"));
    BN_LOG(*this);
}

bool GameState::isNewRegularSave() const
{
    return _rSavedCount <= 0;
}

void GameState::resetToNewRegularSave(core::Random& rng)
{
    _lv = 1;
    _exp = 0;
    const auto& stat = game::StatInfo::getInfo(_lv);
    _curHp = _maxHp = stat.maxHp;
    _baseAtk = stat.atk;
    _baseDef = stat.def;
    _gold = 0;
    _kills = 0;
    _items.clear();
    _dimensionalBoxA.clear();
    _dimensionalBoxA.push_back(ItemKind::TOUGH_GLOVE);
    _dimensionalBoxB.clear();
    _phone = {};
    _weapon = ItemKind::STICK;
    _armor = ItemKind::BANDAGE;

    _flags = GameFlags();
    _flags.fun = rng.get_int(1, 101);
    if ("frisk" == bn::string_view(util::toLowerAscii(bn::string<CHAR_NAME_MAX_LEN>(_charName))))
        _flags.hardmode = true;

    bn::string<CHAR_NAME_MAX_LEN> charName = _charName;
    auto lowerName = util::toLowerAscii(charName);
    const bn::string_view name = lowerName;
    if (name == "frisk")
        _flags.hardmode = true;

    _plot = GamePlot::NEW_GAME;
    _hasPhone = false;
    _worldBgm = asset::BgmKind::NONE;
    _room = RoomKind::ROOM_AREA1;
    _time = core::PlayTime(0);
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

    for (int i = 0; i < CHAR_NAME_MAX_LEN + 1; ++i)
        rSave.charName[i] = (i < _charName.size() ? _charName[i] : '\0');

    rSave.xp = _exp;
    rSave.gold = _gold;
    rSave.kills = _kills;
    for (int i = 0; i < rSave.item.size(); ++i)
        rSave.item[i] = (i < _items.size() ? _items[i] : ItemKind::NONE);
    for (int i = 0; i < rSave.dimensionalBoxA.size(); ++i)
        rSave.dimensionalBoxA[i] = (i < _dimensionalBoxA.size() ? _dimensionalBoxA[i] : ItemKind::NONE);
    for (int i = 0; i < rSave.dimensionalBoxB.size(); ++i)
        rSave.dimensionalBoxB[i] = (i < _dimensionalBoxB.size() ? _dimensionalBoxB[i] : ItemKind::NONE);
    rSave.phone = _phone;
    rSave.weapon = _weapon;
    rSave.armor = _armor;
    rSave.flag = _flags;
    rSave.plot = _plot;
    rSave.menuChoice2 = _hasPhone;
    rSave.song = _worldBgm;
    rSave.room = _room;
    rSave.time = _time.getTicks();

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

bool GameState::isInBattle() const
{
    return _isInBattle;
}

bool GameState::isSeriousBattle() const
{
    return _isSeriousBattle;
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

auto GameState::getItems() const -> decltype((_items))
{
    return _items;
}

auto GameState::getItems() -> decltype((_items))
{
    return _items;
}

auto GameState::getDimensionalBoxA() const -> decltype((_dimensionalBoxA))
{
    return _dimensionalBoxA;
}

auto GameState::getDimensionalBoxA() -> decltype((_dimensionalBoxA))
{
    return _dimensionalBoxA;
}

auto GameState::getDimensionalBoxB() const -> decltype((_dimensionalBoxB))
{
    return _dimensionalBoxB;
}

auto GameState::getDimensionalBoxB() -> decltype((_dimensionalBoxB))
{
    return _dimensionalBoxB;
}

auto GameState::getWeapon() const -> ItemKind
{
    return _weapon;
}

auto GameState::getArmor() const -> ItemKind
{
    return _armor;
}

auto GameState::getFlags() const -> const GameFlags&
{
    return _flags;
}

auto GameState::getFlags() -> GameFlags&
{
    return _flags;
}

auto GameState::getPlot() const -> GamePlot
{
    return _plot;
}

bool GameState::getHasPhone() const
{
    return _hasPhone;
}

auto GameState::getWorldBgm() const -> asset::BgmKind
{
    return _worldBgm;
}

auto GameState::getRoom() const -> RoomKind
{
    return _room;
}

auto GameState::getTime() const -> const core::PlayTime&
{
    return _time;
}

auto GameState::getTime() -> core::PlayTime&
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

void GameState::changeHp(int diff)
{
    _curHp = bn::clamp(_curHp + diff, 0, _maxHp);
}

void GameState::setWeapon(ItemKind weapon)
{
    _weapon = weapon;
}

void GameState::setArmor(ItemKind armor)
{
    _armor = armor;
}

void GameState::setPlot(GamePlot plot)
{
    _plot = plot;
}

void GameState::setHasPhone(bool hasPhone)
{
    _hasPhone = hasPhone;
}

void GameState::setWorldBgm(asset::BgmKind bgm)
{
    _worldBgm = bgm;
}

void GameState::setRoom(RoomKind room)
{
    _room = room;
}

void GameState::setTime(const core::PlayTime& time)
{
    _time = time;
}

void GameState::loadFromRSave(const RegularSave& rSave)
{
    _charName.clear();
    for (char ch : rSave.charName)
        if (ch != '\0' && !_charName.full())
            _charName.push_back(ch);

    // Load `exp` related values
    _exp = rSave.xp;
    _lv = game::StatInfo::getLv(_exp);
    _kills = rSave.kills;
    const auto& stat = game::StatInfo::getInfo(_lv);
    _curHp = _maxHp = stat.maxHp;
    _baseAtk = stat.atk;
    _baseDef = stat.def;

    _gold = rSave.gold;

    _items.clear();
    for (int i = 0; i < rSave.item.size(); ++i)
        if (rSave.item[i] != ItemKind::NONE)
            _items.push_back(rSave.item[i]);

    _dimensionalBoxA.clear();
    for (int i = 0; i < rSave.dimensionalBoxA.size(); ++i)
        if (rSave.dimensionalBoxA[i] != ItemKind::NONE)
            _dimensionalBoxA.push_back(rSave.dimensionalBoxA[i]);

    _dimensionalBoxB.clear();
    for (int i = 0; i < rSave.dimensionalBoxB.size(); ++i)
        if (rSave.dimensionalBoxB[i] != ItemKind::NONE)
            _dimensionalBoxB.push_back(rSave.dimensionalBoxB[i]);

    _phone = rSave.phone;
    _weapon = rSave.weapon;
    _armor = rSave.armor;
    _flags = rSave.flag;
    _plot = rSave.plot;
    _hasPhone = rSave.menuChoice2;
    _worldBgm = rSave.song;
    _room = rSave.room;
    _time = core::PlayTime(rSave.time);
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

bn::ostringstream& operator<<(bn::ostringstream& oss, const GameState& gs)
{
    oss << "[GameState]\n";
    oss << "time: " << gs._time.getTicks() << '\n';
    oss << "rSavCnt: " << gs._rSavedCount << '\n';
    oss << "pSavCnt: " << gs._pSavedCount << '\n';
    oss << "room: " << (int)gs._room << '\n';
    oss << "plot: " << (int)gs._plot << '\n';
    oss << "charName: " << gs._charName << '\n';
    oss << "lv: " << gs._lv << '\n';
    oss << "exp: " << gs._exp << '\n';
    oss << "curHp: " << gs._curHp << '\n';
    oss << "maxHp: " << gs._maxHp << '\n';
    oss << "baseAtk: " << gs._baseAtk << '\n';
    oss << "baseDef: " << gs._baseDef << '\n';
    oss << "weapon: " << (int)gs._weapon << '\n';
    oss << "armor: " << (int)gs._armor << '\n';
    oss << "gold: " << gs._gold << '\n';
    oss << "kills: " << gs._kills << '\n';

    return oss;
}

} // namespace ut::game
