#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

enum class LevelIds : __int16;

class Camera
{
public:
    EXPORT Camera* ctor_4446E0();

    EXPORT void dtor_444700();

    EXPORT static void CC On_Loaded_4447A0(Camera* pThis);

    DynamicArrayT<BYTE*> field_0_array;
    BYTE** field_C_ppBits;
    int field_10_resId;
    __int16 field_14_cam_x;
    __int16 field_16_cam_y;
    __int16 field_18_path;
    LevelIds field_1A_level;
    __int16 field_1C;
    char field_1E_fileName[18];
    __int16 field_30_flags;
    __int16 field_32;
};
ALIVE_ASSERT_SIZEOF(Camera, 0x34);


struct DirtyBits
{
    WORD mData[20]; // 20 Columns

    bool GetTile(int x, int y)
    {
        return mData[x] & (1 << y) ? true : false;
    }

    void SetTile(int x, int y, bool b)
    {
        if (b)
        {
            mData[x] |= 1 << y;
        }
        else
        {
            mData[x] &= ~(1 << y);
        }
    }
};
ALIVE_ASSERT_SIZEOF(DirtyBits, 0x28);

struct SprtTPage
{
    Prim_Sprt mSprt;
    Prim_SetTPage mTPage;
};

class ScreenManager : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ScreenManager* vdtor_407290(signed int flags);

    EXPORT ScreenManager* ctor_406830(BYTE** ppBits, FP_Point* pCameraOffset);

    EXPORT void Init_4068A0(BYTE** ppBits);

    EXPORT void MoveImage_406C40();

    EXPORT void InvalidateRect_406CC0(int x, int y, signed int width, signed int height);

    EXPORT void DecompressCameraToVRam_407110(unsigned __int16** ppBits);

    EXPORT void UnsetDirtyBits_FG1_406EF0();

    EXPORT void InvalidateRect_406E40(int x, int y, signed int width, signed int height, int idx);

    EXPORT void InvalidateRect_Layer3_406F20(int x, int y, int width, int height);

    EXPORT void InvalidateRect_406D80(int x, int y, signed int width, signed int height, int idx);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    int GetTPage(TPageMode tp, char abr, int* xpos, int* ypos);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_406A60(PrimHeader** ppOt);

    EXPORT void sub_406FF0();

    FP_Point* field_10_pCamPos;
    __int16 field_14_xpos;
    unsigned __int16 field_16_ypos;
    SprtTPage* field_18_screen_sprites;
    int field_1C;
    __int16 field_20_upos;
    __int16 field_22_vpos;
    short field_24_cam_width;
    short field_26_cam_height;
    int field_28;
    __int16 field_2C;
    unsigned __int16 field_2E_idx;
    unsigned __int16 field_30_y_idx;
    unsigned __int16 field_32_x_idx;
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
    DirtyBits field_58_20x16_dirty_bits[6];
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x148);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager_4FF7C8);

}
