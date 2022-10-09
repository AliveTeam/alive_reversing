#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "SaveGame.hpp"

namespace AO {

struct PlaybackData final
{
    SaveData saveData;
    u8 randomSeed;
    //TODO probably more variables after
};

class DemoPlayback final : public ::BaseGameObject
{
public:
    DemoPlayback(u8**, s32 a3);
    ~DemoPlayback();
    
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    enum class States : s32
    {
        eState_0_Init = 0,
        eState_1_Playing = 1,
        eState_2_Done = 2,
    };
    States field_10_state = States::eState_0_Init;
    u8** field_14_ppDemoRes = nullptr;
    SaveData* field_18_ppRes = nullptr;
    s16 field_1C_bFromHandStone = 0;
    s16 field_1E = 0;
    s32 field_20 = 0;
    s32 field_24 = 0;
    s32 field_28 = 0;
    s32 field_2C = 0;
    s32 field_30 = 0;
    s32 field_34 = 0;
    s32 field_38 = 0;
    s32 field_3C = 0;
    s32 field_40 = 0;
    s32 field_44 = 0;
    s32 field_48 = 0;
    s32 field_4C = 0;
    s32 field_50 = 0;
    s32 field_54 = 0;
    s32 field_58 = 0;
    s32 field_5C = 0;
    s32 field_60 = 0;
    s32 field_64 = 0;
    s32 field_68 = 0;
    s32 field_6C = 0;
    s32 field_70 = 0;
    s32 field_74 = 0;
    s32 field_78 = 0;
    s32 field_7C = 0;
    s32 field_80 = 0;
};
ALIVE_ASSERT_SIZEOF(DemoPlayback, 0x84);

} // namespace AO
