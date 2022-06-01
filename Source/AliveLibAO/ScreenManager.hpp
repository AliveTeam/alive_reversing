#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

enum class LevelIds : s16;

class Camera final
{
public:
    Camera();
    ~Camera();

    static void On_Loaded_4447A0(Camera* pThis);

    DynamicArrayT<u8*> field_0_array;
    u8** field_C_ppBits;
    s32 field_10_resId;
    s16 field_14_cam_x;
    s16 field_16_cam_y;
    s16 field_18_path;
    LevelIds field_1A_level;
    s16 field_1C;
    char_type field_1E_fileName[18];
    s16 field_30_flags;
    s16 field_32;
};
ALIVE_ASSERT_SIZEOF(Camera, 0x34);


struct DirtyBits final
{
    u16 mData[20]; // 20 Columns

    bool GetTile(s32 x, s32 y)
    {
        return mData[x] & (1 << y) ? true : false;
    }

    void SetTile(s32 x, s32 y, bool b)
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

struct SprtTPage final
{
    Prim_Sprt mSprt;
    Prim_SetTPage mTPage;
};

class ScreenManager final : public BaseGameObject
{
public:
    ScreenManager(u8** ppBits, FP_Point* pCameraOffset);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void Init(u8** ppBits);
    void MoveImage();
    void DecompressCameraToVRam(u16** ppBits);
    void UnsetDirtyBits_FG1();

    void InvalidateRect(s32 x, s32 y, s32 width, s32 height, s32 idx);
    void InvalidateRectCurrentIdx(s32 x, s32 y, s32 width, s32 height);
    void InvalidateRect_Idx3(s32 x, s32 y, s32 width, s32 height);
    void InvalidateRect_IdxPlus4(s32 x, s32 y, s32 width, s32 height, s32 idx);

    s32 GetTPage(TPageMode tp, TPageAbr abr, s32* xpos, s32* ypos);
    void sub_406FF0();

    FP_Point* field_10_pCamPos;
    s16 field_14_xpos;
    u16 field_16_ypos;
    SprtTPage* field_18_screen_sprites;
    s32 field_1C;
    s16 field_20_upos;
    s16 field_22_vpos;
    s16 field_24_cam_width;
    s16 field_26_cam_height;
    s32 field_28;
    s16 field_2C;
    u16 field_2E_idx;
    u16 field_30_y_idx;
    u16 field_32_x_idx;
    s16 field_34;
    s16 field_36_flags;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    DirtyBits field_58_20x16_dirty_bits[6];
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x148);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager_4FF7C8);

} // namespace AO
