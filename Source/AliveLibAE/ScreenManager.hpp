#pragma once

#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"

struct Prim_Sprt;

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

struct SprtTPage final
{
    Prim_Sprt mSprt;
    Prim_SetTPage mTPage;
};

class ScreenManager final : public BaseGameObject
{
public:
    // TODO
    void sub_40EE10();

    void MoveImage_40EB70();

    void InvalidateRect_40EC90(s32 x, s32 y, s32 width, s32 height, s32 idx);
    void InvalidateRect_40EC10(s32 x, s32 y, s32 width, s32 height);
    void InvalidateRect_Layer3_40EDB0(s32 x, s32 y, s32 width, s32 height);
    void InvalidateRect_40EC50(s32 x, s32 y, s32 width, s32 height, s32 idx);

    s16 IsDirty_40EBC0(s32 idx, s32 x, s32 y);
    void UnsetDirtyBits_40EDE0(s32 idx);
    void UnsetDirtyBits_FG1_40ED70();

    virtual void VUpdate() override
    { }


    void DecompressCameraToVRam_40EF60(u16** ppBits);

    ScreenManager(u8** ppBits, FP_Point* pCameraOffset);

    void Init_40E4B0(u8** ppBits);

    static s32 GetTPage_40F040(TPageMode tp, TPageAbr abr, s32* xpos, s32* ypos);

    virtual void VRender(PrimHeader** ppOt) override;
    void Render_Helper_40E9F0(s32 xpos, s32 ypos, Layer idx, s32 sprite_idx, PrimHeader** ppOt);
    void sub_40EE50();
    void VRender_40E6E0(PrimHeader** ppOt);

    virtual void VScreenChanged() override;

private:
    void AddCurrentSPRT_TPage(PrimHeader** ppOt);

public:
    FP_Point* field_20_pCamPos;
    SprtTPage* field_24_screen_sprites;
    s32 field_28_padding;
    u16 field_2C_upos;
    u16 field_2E_vpos;
    s16 field_30_cam_width;
    s16 field_32_cam_height;
    s32 field_34_padding;
    s16 field_38_padding;
    u16 field_3A_idx;
    u16 field_3C_y_idx;
    u16 field_3E_x_idx;
    s32 field_40_flags;
    u16 field_44_unused;
    u16 field_46_padding;
    s32 field_48_padding;
    s32 field_4C_padding;
    s32 field_50_padding;
    s32 field_54_padding;
    s32 field_58_padding;
    s32 field_5C_padding;
    s32 field_60_padding;
    DirtyBits field_64_20x16_dirty_bits[8];
};
ALIVE_ASSERT_SIZEOF(ScreenManager, 0x1A4u);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager_5BB5F4);
