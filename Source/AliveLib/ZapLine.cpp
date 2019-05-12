#include "stdafx.h"
#include "ZapLine.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"

EXPORT ZapLine* ZapLine::ctor_4CC690(FP x1, FP y1, FP x2, FP y2, __int16 aliveTime, signed __int16 type, __int16 layer)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    field_12A_type = type;
    SetVTable(this, 0x5478A0);
    field_4_typeId = Types::eZapLine_135;

    int frameTable = 0;
    if (type == 1)
    {
        field_130_count_per_part = 20;
        field_12E_part_count = 12;
        frameTable = 276;
        field_12C = 3;
    }
    else if (type == 0)
    {
        field_130_count_per_part = 10;
        field_12E_part_count = 28;
        frameTable = 288;
        field_12C = 1;
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 355);
    Animation_Init_424E10(frameTable, 14, 9, ppRes, 1, 1u);

    field_132_pSprts_count = field_12E_part_count * field_130_count_per_part;
    field_20_animation.field_4_flags.Set(AnimFlags::eBit25_bDecompressDone); // HIBYTE |= 1
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
    field_20_animation.field_C_render_layer = layer;
    
    field_F8_ppRes = ResourceManager::Allocate_New_Locked_Resource_49BF40('nlpS', 0, sizeof(ZapLineSprites) * field_132_pSprts_count);// Spln (spline?)
    
    field_134_pSprts = reinterpret_cast<ZapLineSprites*>(*field_F8_ppRes);
    field_138_buf1 = malloc_non_zero_4954F0(4 * field_132_pSprts_count);
    field_13C_buf2 = malloc_non_zero_4954F0(12 * field_130_count_per_part);
    field_140_buf3 = malloc_non_zero_4954F0(8 * field_12E_part_count);
    
    field_128_max_alive_time = aliveTime;
    
    field_B8_xpos = x1;
    field_BC_ypos = y1;

    field_F4_state = 0;
    field_126_alive_timer = 0;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
    {
        field_124_tPageMode = 1;
    }
    else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
    {
        field_124_tPageMode = 2;
    }
    else
    {
        field_124_tPageMode = 0; // 4 bit
    }

    BYTE u0 = field_20_animation.field_84_vram_rect.x & 0x3F;
    if (field_124_tPageMode == 1)
    {
        u0 = 2 * u0;
    }
    else if (!field_124_tPageMode)
    {
        u0 = 4 * u0;
    }

    auto pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset]);

    const BYTE frameW = pFrameHeader->field_4_width;
    const BYTE frameH = pFrameHeader->field_5_height;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < field_12E_part_count; j++)
        {
            for (int k = 0; k < field_130_count_per_part; k++)
            {
                Prim_Sprt* pSprt = &field_134_pSprts[(j * field_130_count_per_part) + k].field_0_sprts[i];
                Sprt_Init_4F8910(pSprt);

                Poly_Set_SemiTrans_4F8A60(&pSprt->mBase.header, 1);
                Poly_Set_Blending_4F8A20(&pSprt->mBase.header, 1);

                SetClut(pSprt, static_cast<short>(PSX_getClut_4F6350(field_20_animation.field_8C_pal_vram_xy.field_0_x, field_20_animation.field_8C_pal_vram_xy.field_2_y)));

                SetUV0(pSprt, u0, field_20_animation.field_84_vram_rect.y & 0xFF);
                pSprt->field_14_w = frameW - 1;
                pSprt->field_16_h = frameH - 1;
            }
        }
    }

    sub_4CCAD0(x1, y1, x2, y2);

    return this;
}

void ZapLine::sub_4CCAD0(FP x1, FP y1, FP x2, FP y2)
{
    field_11C_x1 = FP_GetExponent(x1 - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_11E_y1 = FP_GetExponent(y1 - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
    field_120_x2 = FP_GetExponent(x2 - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
    field_122_y2 = FP_GetExponent(y2 - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

    field_11C_x1 = PsxToPCX(field_11C_x1, 11);
    field_120_x2 = PsxToPCX(field_120_x2, 11); 

    __int16 xOff = 0;
    __int16 yOff = 0;
    field_20_animation.Get_Frame_Offset_40C480(&xOff, &yOff);

    field_11C_x1 = FP_GetExponent(FP_FromInteger(xOff) + FP_FromInteger(field_11C_x1));
    field_11E_y1 = FP_GetExponent(FP_FromInteger(yOff) + FP_FromInteger(field_11E_y1));
    field_120_x2 = FP_GetExponent(FP_FromInteger(xOff) + FP_FromInteger(field_120_x2));
    field_122_y2 = FP_GetExponent(FP_FromInteger(yOff) + FP_FromInteger(field_122_y2));
}

void ZapLine::dtor_4CCCB0()
{
    SetVTable(this, 0x5478A0);
    ResourceManager::FreeResource_49C330(field_F8_ppRes);
    Mem_Free_495560(field_138_buf1);
    Mem_Free_495560(field_13C_buf2);
    Mem_Free_495560(field_140_buf3);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ZapLine::vScreenChanged_4CDBE0()
{
    if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}
