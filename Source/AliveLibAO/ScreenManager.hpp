#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint_common.hpp" // TODO: Replace with FixedPoint.hpp

START_NS_AO

class Camera
{
public:
    EXPORT void dtor_444700();

    DynamicArray field_0_array;
    BYTE** field_C_ppBits;
    int field_10_resId;
    __int16 field_14_cam_x;
    __int16 field_16_cam_y;
    __int16 field_18;
    __int16 field_1A;
    __int16 field_1C;
    char field_1E_fileName[6];
    int field_24;
    int field_28;
    int field_2C;
    __int16 field_30_flags;
    __int16 field_32;
};
ALIVE_ASSERT_SIZEOF(Camera, 0x34);


struct DirtyBits
{
    __int16 field_0[20];
};
ALIVE_ASSERT_SIZEOF(DirtyBits, 0x28);

class ScreenManager : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ScreenManager* vdtor_407290(signed int flags);

    EXPORT ScreenManager* ctor_406830(BYTE** ppBits, FP_Point* pCameraOffset);

    EXPORT void MoveImage_406C40();

    EXPORT void InvalidateRect_406CC0(int x, int y, signed int width, signed int height);

    EXPORT void DecompressToVRam_407110(unsigned __int16** ppBits);

    FP_Point* field_10_pCamPos;
    __int16 field_14;
    unsigned __int16 field_16;
    int field_18;
    int field_1C;
    __int16 field_20;
    __int16 field_22;
    int field_24;
    int field_28;
    __int16 field_2C;
    unsigned __int16 field_2E_idx;
    unsigned __int16 field_30;
    unsigned __int16 field_32;
    __int16 field_34;
    __int16 field_36_flags;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
    DirtyBits field_58[6];
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x148);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager_4FF7C8);

END_NS_AO
