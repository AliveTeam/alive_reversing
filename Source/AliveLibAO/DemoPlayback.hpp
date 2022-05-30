#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "SaveGame.hpp"

namespace AO {

struct PlaybackData final
{
    SaveData saveData;
    u8 randomSeed;
    //TODO probably more variables after
};

class DemoPlayback final : public BaseGameObject
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
    States field_10_state;
    u8** field_14_ppDemoRes;
    u8** field_18_ppRes;
    s16 field_1C_bFromHandStone;
    s16 field_1E;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 field_70;
    s32 field_74;
    s32 field_78;
    s32 field_7C;
    s32 field_80;
};
ALIVE_ASSERT_SIZEOF(DemoPlayback, 0x84);

} // namespace AO
