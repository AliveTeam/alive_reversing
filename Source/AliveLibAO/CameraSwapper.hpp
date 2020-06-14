#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

enum class CameraSwapEffects : __int16;

class CameraSwapper : public BaseGameObject
{
public:
    EXPORT CameraSwapper* ctor_48C540(BYTE** ppBits, int movieId, int movieFlag, __int16 movieFlags, int a6, __int16 a7);

    EXPORT CameraSwapper* ctor_48C5E0(BYTE** ppBits, int moviePos1, int movieId1, int moviePos2, int movieFlag1, __int16 movieFlags1, int movieVol1, __int16 movieFlag2, __int16 movieFlag2_1, __int16 movieFlags2_1, __int16 movieVol2);

    EXPORT CameraSwapper* ctor_48C6B0(BYTE** ppBits, int moviePos1, int movieIds1, int moviePos2, int movieId2, int moviePos3, int movieId3, __int16 movieFlag1, int movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlags2, __int16 movieVol2, __int16 movieFlag3, __int16 movieFlags3, __int16 movieVol3);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT CameraSwapper* Vdtor_48D380(signed int flags);

    EXPORT CameraSwapper* ctor_48C7A0(BYTE** ppBits, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos);

    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    __int16 field_28;
    __int16 field_2A;
    int field_2C;
    __int16 field_30;
    __int16 field_32;
    __int16 field_34;
    __int16 field_36;
    __int16 field_38;
    __int16 field_3A;
    __int16 field_3C;
    __int16 field_3E;
    __int16 field_40;
    __int16 field_42;
    int field_44;
};
ALIVE_ASSERT_SIZEOF(CameraSwapper, 0x48);

END_NS_AO
