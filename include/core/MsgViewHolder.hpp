#pragma once

#include <bn_string_view.h>
#include <bn_vector.h>
#include <bn_span.h>

#include "asset/gen/TextData_fwd.hpp"
#include "core/DialogSettings.hpp"
#include "core/DialogVariableInfo.hpp"

#include "consts.hpp"

namespace ut::core
{

/**
 * @brief Non-owning `bn::string_view` holder to store `TextData`s.
 *
 * Added `bn::string_view` must alive until `Game::startDialog()` call.
 */
class MsgViewHolder
{
public:
    /**
     * @brief This Only clear `msg`, DOES NOT reset settings.
     */
    void clearMsg();

    auto getMsg() const -> bn::span<const bn::string_view>;

    /**
     * @brief Added `bn::string_view` must alive until `Game::startDialog()` call.
     */
    void add(bn::string_view);

    void add(asset::gen::TextData);

    auto getSettings() -> core::DialogSettingsOverride&;
    auto getSettings() const -> const core::DialogSettingsOverride&;

    auto getMsgArg1() const -> const bn::string_view;
    auto getMsgArg2() const -> const bn::string_view;

    void setMsgArg1(const bn::string_view);
    void setMsgArg2(const bn::string_view);

private:
    bn::vector<bn::string_view, consts::DIALOG_MAX_LINES> _msg;
    core::DialogSettingsOverride _msgSettings;

    bn::string<DialogVariableInfo::DIALOG_VAR_MAX_CHARS> _msgArg1, _msgArg2;
};

} // namespace ut::core
