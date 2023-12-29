#include "core/MsgViewHolder.hpp"

#include "gen/TextData.hpp"

namespace ut::core
{

void MsgViewHolder::clearMsg()
{
    _msg.clear();
}

auto MsgViewHolder::getMsg() const -> bn::span<const bn::string_view>
{
    return bn::span(_msg.begin(), _msg.end());
}

void MsgViewHolder::add(bn::string_view strView)
{
    _msg.push_back(strView);
}

void MsgViewHolder::add(asset::gen::TextData textData)
{
    // TODO: Add textdata considering language settings
    _msg.push_back(asset::gen::getTextEn(textData));
}

auto MsgViewHolder::getSettings() -> core::DialogSettingsOverride&
{
    return _msgSettings;
}

auto MsgViewHolder::getSettings() const -> const core::DialogSettingsOverride&
{
    return _msgSettings;
}

auto MsgViewHolder::getMsgArg1() const -> const bn::string_view
{
    return _msgArg1;
}

auto MsgViewHolder::getMsgArg2() const -> const bn::string_view
{
    return _msgArg2;
}

void MsgViewHolder::setMsgArg1(const bn::string_view arg)
{
    _msgArg1 = arg;
}

void MsgViewHolder::setMsgArg2(const bn::string_view arg)
{
    _msgArg2 = arg;
}

} // namespace ut::core
