#pragma once

#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"

struct Prim_Sprt;

struct DirtyBits
{
    WORD mData[20]; // 20 Columns

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

namespace Oddlib
{
    struct BitsLogic;
}

struct SprtTPage
{
    Prim_Sprt mSprt;
    Prim_SetTPage mTPage;
};

class ScreenManager : public BaseGameObject
{
public:
    // TODO
    EXPORT void sub_40EE10();

    EXPORT void MoveImage_40EB70();

    EXPORT void InvalidateRect_40EC90(s32 x, s32 y, s32 width, s32 height, s32 idx);
    EXPORT void InvalidateRect_40EC10(s32 x, s32 y, s32 width, s32 height);
    EXPORT void InvalidateRect_Layer3_40EDB0(s32 x, s32 y, s32 width, s32 height);
    EXPORT void InvalidateRect_40EC50(s32 x, s32 y, s32 width, s32 height, s32 idx);

    EXPORT s16 IsDirty_40EBC0(s32 idx, s32 x, s32 y);
    EXPORT void UnsetDirtyBits_40EDE0(s32 idx);
    EXPORT void UnsetDirtyBits_FG1_40ED70();

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override {}

    void process_segment(WORD* aVlcBufferPtr, s32 xPos);
    void vlc_decode(WORD* aCamSeg, WORD* aDst);
    void vlc_decoder(s32 aR, s32 aG, s32 aB, s32 aWidth, s32 aVramX, s32 aVramY);
    void write_4_pixel_block(const Oddlib::BitsLogic& aR, const Oddlib::BitsLogic& aG, const Oddlib::BitsLogic& aB, s32 aVramX, s32 aVramY);

    EXPORT void DecompressCameraToVRam_40EF60(WORD** ppBits);

    EXPORT ScreenManager* ctor_40E3E0(BYTE** ppBits, FP_Point* pCameraOffset);

    EXPORT void Init_40E4B0(BYTE** ppBits);

    EXPORT void dtor_40E490();
    EXPORT BaseGameObject* vdtor_40E460(s32 flags);
    s32 next_bits();

    EXPORT static s32 CC GetTPage_40F040(TPageMode tp, TPageAbr abr, s32* xpos, s32* ypos);

    virtual void VRender(PrimHeader** ppOt) override;
    EXPORT void Render_Helper_40E9F0(s32 xpos, s32 ypos, Layer idx, s32 sprite_idx, PrimHeader** ppOt);
    EXPORT void sub_40EE50();
    EXPORT void VRender_40E6E0(PrimHeader** ppOt);

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

    s32 g_left7_array = 0;
    s32 g_right25_array = 0;
    u16* g_pointer_to_vlc_buffer = nullptr;
};
//ALIVE_ASSERT_SIZEOF(ScreenManager, 0x1A4u);

ALIVE_VAR_EXTERN(ScreenManager*, pScreenManager_5BB5F4);

namespace AETest::TestsScreenManager
{
    void ScreenManagerTests();
}
