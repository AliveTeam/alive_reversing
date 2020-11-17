#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "SaveGame.hpp"

namespace AO {

struct PlaybackData
{
    SaveData saveData;
    BYTE randomSeed;
    //TODO probably more variables after
};

class DemoPlayback : public BaseGameObject
{
public:
    EXPORT DemoPlayback* ctor_4517B0(BYTE**, __int16 a3);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* dtor_4518D0();

    EXPORT DemoPlayback* Vdtor_451F50(signed int flags);

    virtual void VScreenChanged() override;

    virtual void VRender(int** ppOt);

    virtual void VUpdate() override;

    EXPORT void VUpdate_451960();

    enum class States : int
    {
        eState_0_Init = 0,
        eState_1_Playing = 1,
        eState_2_Done = 2,
    };
    States field_10_state;
    BYTE** field_14_ppDemoRes;
    BYTE** field_18_ppRes;
    __int16 field_1C;
    __int16 field_1E;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
};
ALIVE_ASSERT_SIZEOF(DemoPlayback, 0x84);

}

