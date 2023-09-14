#include "scene/test/TextChoiceTest.hpp"

#include <bn_keypad.h>
#include <bn_utility.h>

#include "core/TextGens.hpp"

#include "gen/TextData.hpp"

#include "bn_regular_bg_items_bg_battle_dialog.h"

namespace ut::scene::test
{

namespace
{

constexpr bn::fixed_point TEXT_DATA_ID_POS = {-115, 13};

constexpr bn::pair<asset::gen::TextData, bn::string_view> CHOICE_DIALOGS[] = {
    {asset::gen::TextData::SCR_TEXT_1603, "SCR_TEXT_1603"},
    {asset::gen::TextData::SCR_TEXT_1778, "SCR_TEXT_1778"},
    {asset::gen::TextData::SCR_TEXT_4479, "SCR_TEXT_4479"},
    {asset::gen::TextData::SCR_TEXT_4493, "SCR_TEXT_4493"},
    {asset::gen::TextData::SCR_TEXT_4512, "SCR_TEXT_4512"},
    {asset::gen::TextData::SCR_TEXT_1824, "SCR_TEXT_1824"},
    {asset::gen::TextData::SCR_TEXT_1882, "SCR_TEXT_1882"},
    {asset::gen::TextData::SCR_TEXT_1900, "SCR_TEXT_1900"},
    {asset::gen::TextData::SCR_TEXT_1915, "SCR_TEXT_1915"},

    {asset::gen::TextData::SCR_TEXT_4021, "SCR_TEXT_4021"},
    {asset::gen::TextData::SCR_TEXT_1946, "SCR_TEXT_1946"},
    {asset::gen::TextData::SCR_TEXT_2192, "SCR_TEXT_2192"},
    {asset::gen::TextData::SCR_TEXT_2887, "SCR_TEXT_2887"},
    {asset::gen::TextData::SCR_TEXT_4329, "SCR_TEXT_4329"},
    {asset::gen::TextData::SCR_TEXT_dogshrine_5914, "SCR_TEXT_dogshrine_5914"},
    {asset::gen::TextData::SCR_TEXT_3886, "SCR_TEXT_3886"},
    {asset::gen::TextData::SCR_TEXT_5014, "SCR_TEXT_5014"},
    {asset::gen::TextData::SCR_TEXT_4684, "SCR_TEXT_4684"},
    {asset::gen::TextData::obj_wizard_610, "obj_wizard_610"},
    {asset::gen::TextData::SCR_TEXT_3925, "SCR_TEXT_3925"},
    {asset::gen::TextData::SCR_TEXT_4286, "SCR_TEXT_4286"},
    {asset::gen::TextData::SCR_TEXT_1647, "SCR_TEXT_1647"},
    {asset::gen::TextData::SCR_TEXT_4106, "SCR_TEXT_4106"},
    {asset::gen::TextData::SCR_TEXT_4744, "SCR_TEXT_4744"},
    {asset::gen::TextData::SCR_TEXT_2968, "SCR_TEXT_2968"},
    {asset::gen::TextData::SCR_TEXT_3840, "SCR_TEXT_3840"},
    {asset::gen::TextData::obj_papyrusboss_1804, "obj_papyrusboss_1804"},
    {asset::gen::TextData::SCR_TEXT_2843, "SCR_TEXT_2843"},
    {asset::gen::TextData::SCR_TEXT_4615, "SCR_TEXT_4615"},
    {asset::gen::TextData::SCR_TEXT_3177, "SCR_TEXT_3177"},
    {asset::gen::TextData::SCR_TEXT_4194, "SCR_TEXT_4194"},
    {asset::gen::TextData::SCR_TEXT_2402, "SCR_TEXT_2402"},
    {asset::gen::TextData::SCR_TEXT_2925, "SCR_TEXT_2925"},
    {asset::gen::TextData::SCR_TEXT_3446, "SCR_TEXT_3446"},
    {asset::gen::TextData::SCR_TEXT_4678, "SCR_TEXT_4678"},
    {asset::gen::TextData::obj_tembattle_550, "obj_tembattle_550"},
    {asset::gen::TextData::SCR_TEXT_4172, "SCR_TEXT_4172"},
    {asset::gen::TextData::SCR_TEXT_2675, "SCR_TEXT_2675"},
    {asset::gen::TextData::SCR_TEXT_1101, "SCR_TEXT_1101"},
    {asset::gen::TextData::SCR_TEXT_3117, "SCR_TEXT_3117"},
    {asset::gen::TextData::SCR_TEXT_5294, "SCR_TEXT_5294"},
    {asset::gen::TextData::SCR_TEXT_5542, "SCR_TEXT_5542"},
    {asset::gen::TextData::SCR_TEXT_2125, "SCR_TEXT_2125"},
    {asset::gen::TextData::SCR_TEXT_1173, "SCR_TEXT_1173"},
    {asset::gen::TextData::SCR_TEXT_1238, "SCR_TEXT_1238"},
    {asset::gen::TextData::SCR_TEXT_2698, "SCR_TEXT_2698"},
    {asset::gen::TextData::SCR_TEXT_1501, "SCR_TEXT_1501"},
    {asset::gen::TextData::SCR_TEXT_5295, "SCR_TEXT_5295"},
    {asset::gen::TextData::SCR_TEXT_2523, "SCR_TEXT_2523"},
    {asset::gen::TextData::SCR_TEXT_2637, "SCR_TEXT_2637"},
    {asset::gen::TextData::SCR_TEXT_3393, "SCR_TEXT_3393"},
    {asset::gen::TextData::SCR_TEXT_723, "SCR_TEXT_723"},
    {asset::gen::TextData::SCR_TEXT_730, "SCR_TEXT_730"},
    {asset::gen::TextData::SCR_TEXT_dogshrine_6076, "SCR_TEXT_dogshrine_6076"},
    {asset::gen::TextData::SCR_TEXT_4415, "SCR_TEXT_4415"},
    {asset::gen::TextData::SCR_TEXT_4572, "SCR_TEXT_4572"},
    {asset::gen::TextData::SCR_TEXT_985, "SCR_TEXT_985"},
    {asset::gen::TextData::SCR_TEXT_2472, "SCR_TEXT_2472"},
    {asset::gen::TextData::SCR_TEXT_3036, "SCR_TEXT_3036"},
    {asset::gen::TextData::SCR_TEXT_3817, "SCR_TEXT_3817"},
    {asset::gen::TextData::SCR_TEXT_823, "SCR_TEXT_823"},
    {asset::gen::TextData::SCR_TEXT_3998, "SCR_TEXT_3998"},
    {asset::gen::TextData::SCR_TEXT_1494, "SCR_TEXT_1494"},
    {asset::gen::TextData::SCR_TEXT_4944, "SCR_TEXT_4944"},
    {asset::gen::TextData::SCR_TEXT_4305, "SCR_TEXT_4305"},
    {asset::gen::TextData::SCR_TEXT_5835, "SCR_TEXT_5835"},
    {asset::gen::TextData::SCR_TEXT_5250, "SCR_TEXT_5250"},
    {asset::gen::TextData::SCR_TEXT_246, "SCR_TEXT_246"},
    {asset::gen::TextData::SCR_TEXT_663, "SCR_TEXT_663"},
    {asset::gen::TextData::SCR_TEXT_1198, "SCR_TEXT_1198"},
    {asset::gen::TextData::SCR_TEXT_dogshrine_6048, "SCR_TEXT_dogshrine_6048"},
    {asset::gen::TextData::SCR_TEXT_5778, "SCR_TEXT_5778"},
    {asset::gen::TextData::SCR_TEXT_738, "SCR_TEXT_738"},
    {asset::gen::TextData::SCR_TEXT_2374, "SCR_TEXT_2374"},
    {asset::gen::TextData::SCR_TEXT_4721, "SCR_TEXT_4721"},
    {asset::gen::TextData::SCR_TEXT_5462, "SCR_TEXT_5462"},
    {asset::gen::TextData::SCR_TEXT_1705, "SCR_TEXT_1705"},
    {asset::gen::TextData::SCR_TEXT_2360, "SCR_TEXT_2360"},
    {asset::gen::TextData::SCR_TEXT_849, "SCR_TEXT_849"},
    {asset::gen::TextData::SCR_TEXT_2332, "SCR_TEXT_2332"},
    {asset::gen::TextData::SCR_TEXT_5518, "SCR_TEXT_5518"},
    {asset::gen::TextData::SCR_TEXT_4349, "SCR_TEXT_4349"},
    {asset::gen::TextData::SCR_TEXT_4374, "SCR_TEXT_4374"},
    {asset::gen::TextData::SCR_TEXT_5296, "SCR_TEXT_5296"},
    {asset::gen::TextData::SCR_TEXT_5763, "SCR_TEXT_5763"},
    {asset::gen::TextData::SCR_TEXT_5890, "SCR_TEXT_5890"},
    {asset::gen::TextData::SCR_TEXT_2838, "SCR_TEXT_2838"},
    {asset::gen::TextData::SCR_TEXT_2427, "SCR_TEXT_2427"},
    {asset::gen::TextData::SCR_TEXT_3413, "SCR_TEXT_3413"},
    {asset::gen::TextData::SCR_TEXT_2781, "SCR_TEXT_2781"},
    {asset::gen::TextData::SCR_TEXT_1675, "SCR_TEXT_1675"},
    {asset::gen::TextData::SCR_TEXT_4978, "SCR_TEXT_4978"},
    {asset::gen::TextData::SCR_TEXT_4589, "SCR_TEXT_4589"},
    {asset::gen::TextData::SCR_TEXT_1274, "SCR_TEXT_1274"},
    {asset::gen::TextData::SCR_TEXT_5127, "SCR_TEXT_5127"},
    {asset::gen::TextData::SCR_TEXT_1379, "SCR_TEXT_1379"},
    {asset::gen::TextData::SCR_TEXT_945, "SCR_TEXT_945"},
    {asset::gen::TextData::SCR_TEXT_5812, "SCR_TEXT_5812"},
    {asset::gen::TextData::SCR_TEXT_706, "SCR_TEXT_706"},
    {asset::gen::TextData::SCR_TEXT_4215, "SCR_TEXT_4215"},
    {asset::gen::TextData::SCR_TEXT_4552, "SCR_TEXT_4552"},
    {asset::gen::TextData::SCR_TEXT_1861, "SCR_TEXT_1861"},
    {asset::gen::TextData::SCR_TEXT_2930, "SCR_TEXT_2930"},
    {asset::gen::TextData::SCR_TEXT_3631, "SCR_TEXT_3631"},
    {asset::gen::TextData::SCR_TEXT_4078, "SCR_TEXT_4078"},
    {asset::gen::TextData::SCR_TEXT_3564, "SCR_TEXT_3564"},
    {asset::gen::TextData::SCR_TEXT_5076, "SCR_TEXT_5076"},
    {asset::gen::TextData::SCR_TEXT_4680, "SCR_TEXT_4680"},
    {asset::gen::TextData::SCR_TEXT_5156, "SCR_TEXT_5156"},
    {asset::gen::TextData::SCR_TEXT_1061, "SCR_TEXT_1061"},
    {asset::gen::TextData::SCR_TEXT_1447, "SCR_TEXT_1447"},
    {asset::gen::TextData::SCR_TEXT_2059, "SCR_TEXT_2059"},
    {asset::gen::TextData::SCR_TEXT_3747, "SCR_TEXT_3747"},
    {asset::gen::TextData::SCR_TEXT_4773, "SCR_TEXT_4773"},
    {asset::gen::TextData::SCR_TEXT_5570, "SCR_TEXT_5570"},
    {asset::gen::TextData::SCR_TEXT_2497, "SCR_TEXT_2497"},
    {asset::gen::TextData::SCR_TEXT_4665, "SCR_TEXT_4665"},
    {asset::gen::TextData::SCR_TEXT_2031, "SCR_TEXT_2031"},
    {asset::gen::TextData::SCR_TEXT_2882, "SCR_TEXT_2882"},
    {asset::gen::TextData::SCR_TEXT_4899, "SCR_TEXT_4899"},
    {asset::gen::TextData::SCR_TEXT_4262, "SCR_TEXT_4262"},
    {asset::gen::TextData::SCR_TEXT_4151, "SCR_TEXT_4151"},
    {asset::gen::TextData::SCR_TEXT_2585, "SCR_TEXT_2585"},
    {asset::gen::TextData::SCR_TEXT_713, "SCR_TEXT_713"},
    {asset::gen::TextData::SCR_TEXT_5371, "SCR_TEXT_5371"},
    {asset::gen::TextData::SCR_TEXT_3792, "SCR_TEXT_3792"},
    {asset::gen::TextData::SCR_TEXT_1025, "SCR_TEXT_1025"},
    {asset::gen::TextData::SCR_TEXT_2063, "SCR_TEXT_2063"},
    {asset::gen::TextData::SCR_TEXT_498, "SCR_TEXT_498"},
    {asset::gen::TextData::SCR_TEXT_3955, "SCR_TEXT_3955"},
    {asset::gen::TextData::SCR_TEXT_4241, "SCR_TEXT_4241"},
    {asset::gen::TextData::SCR_TEXT_5411, "SCR_TEXT_5411"},
    {asset::gen::TextData::SCR_TEXT_2706, "SCR_TEXT_2706"},
    {asset::gen::TextData::SCR_TEXT_622, "SCR_TEXT_622"},
    {asset::gen::TextData::SCR_TEXT_3863, "SCR_TEXT_3863"},
    {asset::gen::TextData::SCR_TEXT_1409, "SCR_TEXT_1409"},
    {asset::gen::TextData::SCR_TEXT_471, "SCR_TEXT_471"},
    {asset::gen::TextData::SCR_TEXT_5494, "SCR_TEXT_5494"},
    {asset::gen::TextData::SCR_TEXT_1140, "SCR_TEXT_1140"},
    {asset::gen::TextData::SCR_TEXT_4682, "SCR_TEXT_4682"},
    {asset::gen::TextData::SCR_TEXT_5213, "SCR_TEXT_5213"},
    {asset::gen::TextData::SCR_TEXT_4795, "SCR_TEXT_4795"},
    {asset::gen::TextData::SCR_TEXT_836, "SCR_TEXT_836"},
    {asset::gen::TextData::SCR_TEXT_5183, "SCR_TEXT_5183"},
    {asset::gen::TextData::SCR_TEXT_1927, "SCR_TEXT_1927"},
    {asset::gen::TextData::SCR_TEXT_5664, "SCR_TEXT_5664"},
    {asset::gen::TextData::SCR_TEXT_3482, "SCR_TEXT_3482"},
    {asset::gen::TextData::SCR_TEXT_4641, "SCR_TEXT_4641"},
    {asset::gen::TextData::SCR_TEXT_2271, "SCR_TEXT_2271"},
    {asset::gen::TextData::SCR_TEXT_4042, "SCR_TEXT_4042"},
    {asset::gen::TextData::SCR_TEXT_3148, "SCR_TEXT_3148"},
    {asset::gen::TextData::SCR_TEXT_1344, "SCR_TEXT_1344"},
    {asset::gen::TextData::SCR_TEXT_2994, "SCR_TEXT_2994"},
    {asset::gen::TextData::SCR_TEXT_656, "SCR_TEXT_656"},
    {asset::gen::TextData::SCR_TEXT_5768, "SCR_TEXT_5768"},
    {asset::gen::TextData::SCR_TEXT_1736, "SCR_TEXT_1736"},
    {asset::gen::TextData::SCR_TEXT_2305, "SCR_TEXT_2305"},
    {asset::gen::TextData::SCR_TEXT_2241, "SCR_TEXT_2241"},
    {asset::gen::TextData::SCR_TEXT_2580, "SCR_TEXT_2580"},
    {asset::gen::TextData::SCR_TEXT_3205, "SCR_TEXT_3205"},
    {asset::gen::TextData::SCR_TEXT_4131, "SCR_TEXT_4131"},
    {asset::gen::TextData::SCR_TEXT_4394, "SCR_TEXT_4394"},
    {asset::gen::TextData::SCR_TEXT_884, "SCR_TEXT_884"},
    {asset::gen::TextData::SCR_TEXT_5486, "SCR_TEXT_5486"},
    {asset::gen::TextData::SCR_TEXT_5618, "SCR_TEXT_5618"},
    {asset::gen::TextData::SCR_TEXT_2144, "SCR_TEXT_2144"},
    {asset::gen::TextData::SCR_TEXT_2338, "SCR_TEXT_2338"},
    {asset::gen::TextData::SCR_TEXT_5864, "SCR_TEXT_5864"},
    {asset::gen::TextData::SCR_TEXT_2300, "SCR_TEXT_2300"},
    {asset::gen::TextData::SCR_TEXT_3429, "SCR_TEXT_3429"},
    {asset::gen::TextData::SCR_TEXT_dogshrine_5919, "SCR_TEXT_dogshrine_5919"},
    {asset::gen::TextData::SCR_TEXT_816, "SCR_TEXT_816"},
    {asset::gen::TextData::SCR_TEXT_1796, "SCR_TEXT_1796"},
    {asset::gen::TextData::SCR_TEXT_4447, "SCR_TEXT_4447"},
    {asset::gen::TextData::SCR_TEXT_5338, "SCR_TEXT_5338"},
    {asset::gen::TextData::SCR_TEXT_579, "SCR_TEXT_579"},
    {asset::gen::TextData::SCR_TEXT_dogshrine_6024, "SCR_TEXT_dogshrine_6024"},
    {asset::gen::TextData::SCR_TEXT_4541, "SCR_TEXT_4541"},
    {asset::gen::TextData::SCR_TEXT_3548, "SCR_TEXT_3548"},
    {asset::gen::TextData::SCR_TEXT_3358, "SCR_TEXT_3358"},
    {asset::gen::TextData::SCR_TEXT_2513, "SCR_TEXT_2513"},
    {asset::gen::TextData::SCR_TEXT_1604, "SCR_TEXT_1604"},
    {asset::gen::TextData::SCR_TEXT_2454, "SCR_TEXT_2454"},
    {asset::gen::TextData::SCR_TEXT_3463, "SCR_TEXT_3463"},
    {asset::gen::TextData::SCR_TEXT_5856, "SCR_TEXT_5856"},
    {asset::gen::TextData::SCR_TEXT_1309, "SCR_TEXT_1309"},
    {asset::gen::TextData::SCR_TEXT_4855, "SCR_TEXT_4855"},
    {asset::gen::TextData::SCR_TEXT_2740, "SCR_TEXT_2740"},
    {asset::gen::TextData::SCR_TEXT_4670, "SCR_TEXT_4670"},
    {asset::gen::TextData::SCR_TEXT_3016, "SCR_TEXT_3016"},
};

} // namespace

TextChoiceTest::TextChoiceTest(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::TEXT_CHOICE_TEST),
      _bg(bn::regular_bg_items::bg_battle_dialog.create_bg(0, 0)), _dialogWriter(context)
{
    using namespace ut::asset::gen;

    for (const auto& p : CHOICE_DIALOGS)
        _dialogs.emplace_back(getTextEn(p.first));

    using DS = core::DialogSettings;
    using DSPKind = DS::PresetKind;
    const auto& dialogSettings = DS::getPreset(DSPKind::WORLD_LOWER);
    _dialogWriter.start(bn::span(_dialogs.cbegin(), _dialogs.cend()), dialogSettings, _text);

    redrawTextDataId();
}

bool TextChoiceTest::handleInput()
{
    if (bn::keypad::a_pressed())
        _dialogWriter.confirmKeyInput();
    if (bn::keypad::b_pressed())
        _dialogWriter.instantWrite();

    return true;
}

bool TextChoiceTest::update()
{
    _dialogWriter.update();

    if (!_dialogWriter.isStarted())
        _bg.set_visible(false);

    if (_curDialogIdx != _dialogWriter.getCurDialogIdx())
    {
        _curDialogIdx = _dialogWriter.getCurDialogIdx();
        redrawTextDataId();
    }

    return true;
}

void TextChoiceTest::redrawTextDataId()
{
    _textDataIdSpr.clear();

    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);
    textGen.generate(TEXT_DATA_ID_POS, CHOICE_DIALOGS[_curDialogIdx].second, _textDataIdSpr);
}

} // namespace ut::scene::test
