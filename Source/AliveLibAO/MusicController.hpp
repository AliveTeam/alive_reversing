#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

class BaseGameObject;

class MusicController : public BaseGameObject
{
public:
    EXPORT static void CC sub_443810(signed __int16 a1, BaseGameObject* a2, __int16 a3, __int16 a4);

    static EXPORT void Shutdown_4437E0();

    static EXPORT void CC EnableMusic_443900(__int16 bEnable);

    __int16 field_10;
    __int16 field_12;
    __int16 field_14;
    __int16 field_16;
    __int16 field_18;
    __int16 field_1A;
    int field_1C;
    __int16 field_20;
    __int16 field_22;
    __int16 field_24;
    __int16 field_26;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    __int16 field_38;
    __int16 field_3A;
    int field_3C;
    int field_40;
    __int16 field_44;
    __int16 field_46;
    __int16 field_48;
    __int16 field_4A;
    __int16 field_4C;
    __int16 field_4E;
    int field_50;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x54);

END_NS_AO

