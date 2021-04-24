#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

enum class LevelIds : s16;

class Camera
{
public:
    EXPORT Camera* ctor_4446E0();

    EXPORT void dtor_444700();

    EXPORT static void CC On_Loaded_4447A0(Camera* pThis);

    DynamicArrayT<BYTE*> field_0_array;
    BYTE** field_C_ppBits;
    int field_10_resId;
    s16 field_14_cam_x;
    s16 field_16_cam_y;
    s16 field_18_path;
    LevelIds field_1A_level;
    s16 field_1C;
    char field_1E_fileName[18];
    s16 field_30_flags;
    s16 field_32;
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
    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ScreenManager* vdtor_407290(s32 flags);

    EXPORT ScreenManager* ctor_406830(BYTE** ppBits, FP_Point* pCameraOffset);

    EXPORT void Init_4068A0(BYTE** ppBits);

    EXPORT void MoveImage_406C40();

    EXPORT void InvalidateRect_406CC0(int x, int y, s32 width, s32 height);

    EXPORT void DecompressCameraToVRam_407110(u16** ppBits);

    EXPORT void UnsetDirtyBits_FG1_406EF0();

    EXPORT void InvalidateRect_406E40(int x, int y, s32 width, s32 height, int idx);

    EXPORT void InvalidateRect_Layer3_406F20(int x, int y, int width, int height);

    EXPORT void InvalidateRect_406D80(int x, int y, s32 width, s32 height, int idx);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    int GetTPage(TPageMode tp, TPageAbr abr, int* xpos, int* ypos);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_406A60(PrimHeader** ppOt);

    EXPORT void sub_406FF0();

    FP_Point* field_10_pCamPos;
    s16 field_14_xpos;
    u16 field_16_ypos;
    SprtTPage* field_18_screen_sprites;
    int field_1C;
    s16 field_20_upos;
    s16 field_22_vpos;
    short field_24_cam_width;
    short field_26_cam_height;
    int field_28;
    s16 field_2C;
    u16 field_2E_idx;
    u16 field_30_y_idx;
    u16 field_32_x_idx;
    s16 field_34;
    s16 field_36_flags;
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
