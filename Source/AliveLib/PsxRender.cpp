#include "stdafx.h"
#include "PsxRender.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Midi.hpp"
#include "Primitives.hpp"
#include <gmock/gmock.h>

using TPsxDraw = std::add_pointer<void(__cdecl)(short, short, int, int, int, int, int, short, short, int, int)>::type;

ALIVE_VAR(1, 0xC2D04C, TPsxDraw, dword_C2D04C, nullptr);

struct OtUnknown
{
    int** field_0_pOtStart;
    int** field_4;
    int** field_8_pOt_End;
};

ALIVE_ARY(1, 0xBD0D88, OtUnknown, 32, sOt_Stack_BD0D88, {});
ALIVE_VAR(1, 0xBD0C08, int, sOtIdxRollOver_BD0C08, 0);

ALIVE_VAR(1, 0xC2D03C, int, dword_C2D03C, 0);

EXPORT int CC PSX_EMU_SetDispType_4F9960(int dispType)
{
    NOT_IMPLEMENTED();

    // HACK / enough impl to alllow standalone to boot
    sVGA_DisplayType_BD1468 = dispType;

    dword_C2D04C = PSX_EMU_Render_51EF90;
}

EXPORT void CC PSX_ClearOTag_4F6290(int** otBuffer, int otBufferSize)
{
    PSX_OrderingTable_4F62C0(otBuffer, otBufferSize);

    // Set each element to point to the next
    int i = 0;
    for (i = 0; i < otBufferSize - 1; i++)
    {
        otBuffer[i] = reinterpret_cast<int*>(&otBuffer[i + 1]);
    }

    // Terminate the list
    otBuffer[i] = reinterpret_cast<int*>(0xFFFFFFFF);
}

EXPORT void CC PSX_OrderingTable_4F62C0(int** otBuffer, int otBufferSize)
{
    int otIdx = 0;
    for (otIdx = 0; otIdx < 32; otIdx++)
    {
        if (otBuffer == sOt_Stack_BD0D88[otIdx].field_0_pOtStart)
        {
            break;
        }
    }

    if (otIdx == 32)
    {
        sOtIdxRollOver_BD0C08 = (sOtIdxRollOver_BD0C08 & 31);
        otIdx = sOtIdxRollOver_BD0C08;
    }

    sOt_Stack_BD0D88[otIdx].field_0_pOtStart = otBuffer;
    sOt_Stack_BD0D88[otIdx].field_4 = otBuffer;
    sOt_Stack_BD0D88[otIdx].field_8_pOt_End = &otBuffer[otBufferSize];
}

EXPORT signed int __cdecl PSX_OT_Idx_From_Ptr_4F6A40(unsigned int /*ot*/)
{
    NOT_IMPLEMENTED();
    return 0;
}


EXPORT void __cdecl PSX_4F6A70(void* /*a1*/, WORD* /*a2*/, unsigned __int8* /*a3*/)
{
    NOT_IMPLEMENTED();
}

EXPORT unsigned __int8 *__cdecl PSX_4F7110(int /*a1*/, int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void __cdecl PSX_4F7960(int /*a1*/, int /*a2*/, int /*a3*/)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0xbd30e4, int, sScreenXOffSet_BD30E4, 0);
ALIVE_VAR(1, 0xbd30a4, int, sScreenYOffset_BD30A4, 0);

// TODO: For reference only, will be implemented when all prim types are recovered
static bool DrawOTagImpl(int** pOT, __int16 drawEnv_of0, __int16 drawEnv_of1)
{

    int** pOtItem = pOT;
    __int16 v2 = drawEnv_of0;
    __int16 v16 = drawEnv_of1;
    sScreenXOffSet_BD30E4 = 0;
    sScreenYOffset_BD30A4 = 0;
    //LOWORD(dword_578318) = -1;

    int v3 = PSX_OT_Idx_From_Ptr_4F6A40((unsigned int)pOT);
    if (v3 < 0)
    {
        return false;
    }

    int v4 = v3;
    int** v21 = sOt_Stack_BD0D88[v4].field_4;
    int** pOtEnd = sOt_Stack_BD0D88[v4].field_8_pOt_End;
    if (pOT)
    {
        do
        {
            if (pOtItem == (int **)-1)
            {
                break;
            }

            MIDI_UpdatePlayer_4FDC80();

            PrimAny any;
            any.mVoid = pOtItem;

            if (pOtItem < (int **)v21 || pOtItem >= pOtEnd)
            {
                char v5 = *((BYTE *)pOtItem + 11);
                int itemToDrawType = any.mPrimHeader->field_B_code;
                switch (itemToDrawType)
                {
                case 2: // ??
                    PSX_4F6A70(0, (WORD *)pOtItem + 6, (unsigned __int8 *)pOtItem);
                    break;

                case PrimTypeCodes::eSetTPage:
                    PSX_TPage_Change_4F6430(any.mSetTPage->field_C_tpage);
                    break;

                case PrimTypeCodes::ePrimClipper:
                    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x = any.mPrimClipper->field_C_x;
                    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.y = any.mPrimClipper->field_E_y;
                    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w = any.mPrimHeader->field_4.mRect.w;
                    sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h = any.mPrimHeader->field_4.mRect.h;

                    PSX_SetDrawEnv_Impl_4FE420(
                        16 * any.mPrimClipper->field_C_x,
                        16 * any.mPrimClipper->field_E_y,
                        (16 * (any.mPrimClipper->field_C_x + any.mPrimHeader->field_4.mRect.w)) - 16,
                        (16 * (any.mPrimClipper->field_E_y + any.mPrimHeader->field_4.mRect.h)) - 16,
                        1000 / 2,
                        nullptr);
                    break;
                case PrimTypeCodes::eScreenOffset:
                    // NOTE: Conditional on dword_55EF94 removed as it is constant 1
                    sScreenXOffSet_BD30E4 = any.mScreenOffset->field_C_xoff * 2;
                    sScreenYOffset_BD30A4 = any.mScreenOffset->field_E_yoff;
                    break;

                case 131: // 0x83 ?? move image ?? 
                    LOG_WARNING("131");
                    BMP_unlock_4F2100(&sPsxVram_C1D160);
                    PSX_MoveImage_4F5D50((PSX_RECT *)(pOtItem + 5), (int)pOtItem[3], (int)pOtItem[4]);
                    if (BMP_Lock_4F1FF0(&sPsxVram_C1D160))
                    {
                        break;
                    }

                    if (sPsxEmu_EndFrameFnPtr_C1D17C)
                    {
                        sPsxEmu_EndFrameFnPtr_C1D17C(1);
                    }
                    return true;

                case 132: // 0x84 ??
                    LOG_WARNING("132"); // Appears for gas..
                    //PSX_4F7B80((int)pOtItem[3], (int)pOtItem[4], (int)pOtItem[5], (int)pOtItem[6], (int)pOtItem[7]);
                    break;
                default:
                    int v12 = 0;
                    int v13 = 0;
                    int v25 = 0;
                    int v26 = 0;

                   // int v9 = dword_C2D03C++ + 1;
                    if ((v5 & 0x60) == 96)
                    {
                        //LOBYTE(itemToDrawType) = itemToDrawType & 0xFC;
                        switch (itemToDrawType & 0xFC)
                        {
                        case 96: // 0x60 Tile
                            LOG_INFO("96");
                            /*
                            v10 = *((WORD *)pOtItem + 9);
                            v25 = *((WORD *)pOtItem + 8);
                            v26 = v10;*/
                            goto LABEL_31;
                        case PrimTypeCodes::eSprt: // 0x64 Sprt
                            v12 = *((WORD *)pOtItem + 10);
                            v13 = *((WORD *)pOtItem + 11);
                            v25 = *((WORD *)pOtItem + 10);
                            v26 = v13;
                            goto LABEL_36;
                        case 104: // 0x68 Tile1
                            LOG_INFO("104");
                            //v11 = 1;
                            goto LABEL_30;
                        case 112: // 0x70 Tile8
                            LOG_INFO("112");
                            //v11 = 8;
                            goto LABEL_30;
                        case 116: // 0x74 Sprt8
                            LOG_INFO("116");
                            //v13 = 8;
                            //v26 = 8;
                            //v12 = 8;
                            goto LABEL_35;
                        case 120: // 0x78 Tile16
                            LOG_INFO("120");
                            //v11 = 16;
                        LABEL_30:
                            //v26 = v11;
                            //v25 = v11;
                        LABEL_31:
                            //LOWORD(v9) = v16 + *((WORD *)pOtItem + 7);
                            //v23 = v2 + *((WORD *)pOtItem + 6);
                            //v24 = v9;
                            //PSX_4F6A70(v9, &v23, (unsigned __int8 *)pOtItem);
                            break;
                        case 124: // 0x7C setSprt16
                        {
                            LOG_INFO("124");
                            //v13 = 16;
                            //v26 = 16;
                            //v12 = 16;
                        LABEL_35:
                            //v25 = v12;
                        LABEL_36: // e

                            itemToDrawType = *((BYTE *)pOtItem + 11); // TODO: LOBYTE() = 
                            BYTE r = 0;
                            BYTE g = 0;
                            BYTE b = 0;
                            
                            if (itemToDrawType & 1)
                            {
                                r = 128;
                                g = 128;
                                b = 128;
                            }
                            else
                            {
                                b = *((BYTE *)pOtItem + 8);
                                g = *((BYTE *)pOtItem + 9);
                                r = *((BYTE *)pOtItem + 10);
                            }

                            //LOWORD(itemToDrawType) = *((WORD *)pOtItem + 9);

                            short p1 = v2 + *((signed __int16 *)pOtItem + 6); // offset + ?
                            short p2 = v16 + *((signed __int16 *)pOtItem + 7); // offset + ?;
                            short p3 = *((unsigned __int8 *)pOtItem + 16);
                            short p4 = *((unsigned __int8 *)pOtItem + 17);
                            
                            // Textured rect rendering ?
                            dword_C2D04C(
                            p1,
                            p2,
                            p3,
                            p4,
                            b,
                            g,
                            r,
                            v12, // redraw width?
                            v13, // redraw height ?
                            itemToDrawType,
                            itemToDrawType & 2);
                        }
                        break;
                        default:
                            goto LABEL_45;
                        }
                    }
                    else if ((v5 & 0x40) == 64) // LineF2 and anything else that falls in 0x40 bit pattern  ?
                    {
                        //LOG_WARNING("64");
                        //PSX_4F7D90(pOtItem, v2, v16); // Line rendering ?
                    }
                    else if ((v5 & PrimTypeCodes::ePolyF3) == PrimTypeCodes::ePolyF3) // and anything else that falls in 0x20 bit pattern?
                    {
                        // Flat/G/Tri/Quad rendering?
                        unsigned __int8 * v15 = PSX_4F7110((int)pOtItem, v2, v16);
                        if (v15)
                        {
                            PSX_4F7960((int)v15, v2, v16);
                        }
                    }
                    break;
                }
            }
        LABEL_45:
            pOtItem = (int **)*pOtItem;
        } while (pOtItem);
    }

    return false;
}

// TODO: Impl main ordering table parser
EXPORT void CC PSX_DrawOTag_4F6540(int** pOT)
{
    NOT_IMPLEMENTED();

    if (!sPsxEmu_EndFrameFnPtr_C1D17C || !sPsxEmu_EndFrameFnPtr_C1D17C(0))
    {
        if (byte_BD0F20 || !BMP_Lock_4F1FF0(&sPsxVram_C1D160))
        {
            if (sPsxEmu_EndFrameFnPtr_C1D17C)
            {
                sPsxEmu_EndFrameFnPtr_C1D17C(1);
            }
            return;
        }

        __int16 drawEnv_of0 = 0;
        __int16 drawEnv_of1 = 0;

        if (bDontUseXYOffsetInRender_BD1464)
        {
            if (!BMP_Lock_4F1FF0(&stru_C1D1A0))
            {
                BMP_unlock_4F2100(&sPsxVram_C1D160);
                if (sPsxEmu_EndFrameFnPtr_C1D17C)
                {
                    sPsxEmu_EndFrameFnPtr_C1D17C(1);
                }
                return;
            }
            spBitmap_C2D038 = &stru_C1D1A0;
            drawEnv_of1 = 0;
            drawEnv_of0 = 0;
        }
        else
        {
            spBitmap_C2D038 = &sPsxVram_C1D160;
            drawEnv_of0 = sPSX_EMU_DrawEnvState_C3D080.field_8_ofs[0];
            drawEnv_of1 = sPSX_EMU_DrawEnvState_C3D080.field_8_ofs[1];
        }

        if (DrawOTagImpl(pOT, drawEnv_of0, drawEnv_of1))
        {
            return;
        }

        if (bDontUseXYOffsetInRender_BD1464)
        {
            BMP_unlock_4F2100(&stru_C1D1A0);
        }

        BMP_unlock_4F2100(&sPsxVram_C1D160);
        if (sPsxEmu_EndFrameFnPtr_C1D17C)
        {
            sPsxEmu_EndFrameFnPtr_C1D17C(1);
        }
        return;
    }
}

ALIVE_VAR(1, 0x578318, WORD, sActiveTPage_578318, 0xFFFF);
ALIVE_VAR(1, 0xbd0f0c, DWORD, sTexture_page_x_BD0F0C, 0);
ALIVE_VAR(1, 0xbd0f10, DWORD, sTexture_page_y_BD0F10, 0);
ALIVE_VAR(1, 0xbd0f14, DWORD, sTexture_page_idx_BD0F14, 0);
ALIVE_VAR(1, 0x57831c, DWORD, dword_57831C, 10);
ALIVE_VAR(1, 0xBD0F18, DWORD, sTexture_page_abr_BD0F18, 0);
ALIVE_VAR(1, 0xbd0f1c, WORD *, sTPage_src_ptr_BD0F1C, nullptr);


EXPORT void CC PSX_TPage_Change_4F6430(unsigned __int16 tPage)
{
    if (sActiveTPage_578318 != tPage)
    {
        sActiveTPage_578318 = tPage;

        // NOTE: Branch guarded by PSX_Ret_0_4F60D0 removed

        // Extract parts of the tpage
        sTexture_page_x_BD0F0C = (tPage & 0xF) << 6;
        sTexture_page_y_BD0F10 = 16 * (tPage & 0x10) + (((unsigned int)tPage >> 2) & 0x200);

        sTexture_page_idx_BD0F14 = ((unsigned int)tPage >> 7) & 3;
        sTexture_page_abr_BD0F18 = ((unsigned int)tPage >> 5) & 3;

        // TODO: Figure out why page 3 is forced to 2
        if (sTexture_page_idx_BD0F14 == 3)
        {
            sTexture_page_idx_BD0F14 = 2;
        }

        // NOTE: Branch guarded by dword_BD2250[tPage & 31] removed as it is never written

        dword_57831C = 10;
        sTPage_src_ptr_BD0F1C = reinterpret_cast<WORD*>(sPsxVram_C1D160.field_4_pLockedPixels) + (sTexture_page_x_BD0F0C + (sTexture_page_y_BD0F10 * 1024));
    }
}

EXPORT bool CC PSX_Rects_intersect_point_4FA100(const PSX_RECT* pRect1, const PSX_RECT* pRect2, PSX_RECT* pOverlapRect, int* pOverlapX, int* pOverlapY)
{
    NOT_IMPLEMENTED();

    __int16 rect1_x; // dx
    __int16 rect2_x; // si
    int rect1_right; // ecx
    int rect2_right; // eax
    __int16 v10; // ax
    __int16 v11; // di
    __int16 v12; // dx
    int v13; // eax
    __int16 v14; // ax

    if (!PSX_Rects_overlap_4FA0B0(pRect1, pRect2))
    {
        return false;
    }

    rect1_x = pRect1->x;
    rect2_x = pRect2->x;
    rect1_right = pRect1->x + pRect1->w;
    rect2_right = pRect2->x + pRect2->w;

    short t1 = rect2_x;
    if (rect2_right > rect1_right)
    {
        t1 = rect1_right;
    }
    v10 = t1 - rect2_x;
    pOverlapRect->w = v10;
    if (rect2_x >= rect1_x)
    {
        pOverlapRect->x = rect2_x;
    }
    else
    {
        pOverlapRect->x = rect1_x;
        pOverlapRect->w = rect2_x + v10 - rect1_x;
        *pOverlapX += rect1_x - rect2_x;
    }
    v11 = pRect2->y;
    v12 = pRect1->y;
    v13 = v11 + pRect2->h;

    int t2 = v13;
    if (v13 > v12 + pRect1->h)
    {
        t2 = v12 + pRect1->h;
    }
    v14 = t2 - v11;
    pOverlapRect->h = v14;
    if (v11 >= v12)
    {
        pOverlapRect->y = v11;
    }
    else
    {
        pOverlapRect->y = v12;
        pOverlapRect->h = v11 + v14 - v12;
        *pOverlapY += v12 - v11;
    }

    return true;
}

EXPORT void CC PSX_EMU_Render_TPage_0_51F0E0(PSX_RECT* /*a1*/, int /*a2*/, int /*a3*/, unsigned __int8 /*a4*/, unsigned __int8 /*a5*/, unsigned __int8 /*a6*/, unsigned __int16 /*a7*/, __int16* /*a8*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_EMU_Render_TPage_1_51F660(PSX_RECT* /*pRect*/, int /*tpagex_off*/, int /*tpagey_off*/, unsigned __int8 /*r*/, unsigned __int8 /*g*/, unsigned __int8 /*b*/, unsigned __int16 /*a7*/, char /*a8*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_EMU_Background_Render_51C490(BYTE *pVram, BYTE *pSrc, unsigned int amount)
{
    NOT_IMPLEMENTED();

    int offset; // eax
    unsigned int counter; // ecx
    double v5; // st7

    offset = 0;
    counter = amount >> 4;                        // div 16 ?
    do
    {
        v5 = (double)*(signed __int64 *)&pSrc[offset + 8];
        *(__int64 *)&pVram[offset] = (signed __int64)(double)*(signed __int64 *)&pSrc[offset];
        *(__int64 *)&pVram[offset + 8] = (signed __int64)v5;
        offset += 16;
        --counter;
    } while (counter);
}

EXPORT void CC PSX_EMU_Render_TPage_2_51FA30(PSX_RECT* /*pRect*/, int /*tpageX*/, int /*tpageY*/, unsigned __int8 /*r*/, unsigned __int8 /*g*/, unsigned __int8 /*b*/, int /*a7*/, char /*a8*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_EMU_Render_51EF90(__int16 x, __int16 y, int minX, int minY, int r, int g, int b, __int16 w, __int16 h, int a10, int a11)
{
    // Get the screen rect
    PSX_RECT screenRect  = {};
    if (bDontUseXYOffsetInRender_BD1464)
    {
        screenRect.y = 0;
        screenRect.x = 0;
        screenRect.w = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w;
        screenRect.h = sPSX_EMU_DrawEnvState_C3D080.field_0_clip.h;
    }
    else
    {
        screenRect = sPSX_EMU_DrawEnvState_C3D080.field_0_clip;
    }

    // Check for overlap
    const PSX_RECT toRenderRect = { x , y, w, h };
    PSX_RECT overlapRect = {};
    if (PSX_Rects_intersect_point_4FA100(&screenRect, &toRenderRect, &overlapRect, &minX, &minY))
    {
        // Render
        if (sTexture_page_idx_BD0F14 == 0)
        {
            PSX_EMU_Render_TPage_0_51F0E0(&overlapRect, minX, minY, r, g, b, a10, (__int16 *)a11);
        }
        else if (sTexture_page_idx_BD0F14 == 1)
        {
            PSX_EMU_Render_TPage_1_51F660(&overlapRect, minX, minY, r, g, b, a10, a11);
        }
        else if (sTexture_page_idx_BD0F14 == 2)
        {
            PSX_EMU_Render_TPage_2_51FA30(&overlapRect, minX, minY, r, g, b, a10, a11);
        }
    }
}

namespace Test
{
    static void Test_PSX_Rects_intersect_point_4FA100()
    {
        PSX_RECT r1 = { 0,0, 300, 150 };
        PSX_RECT r2 = { 10, 10, 50, 200 };
        PSX_RECT ro = {};
        int px = 0;
        int py = 0;
        ASSERT_EQ(1, PSX_Rects_intersect_point_4FA100(&r1, &r2, &ro, &px, &py));

        ASSERT_EQ(px, 0);

    }

    static void Test_PSX_TPage_Change_4F6430()
    {
        sActiveTPage_578318 = 0;
        sTexture_page_x_BD0F0C = 0;
        sTexture_page_y_BD0F10 = 0;
        sTexture_page_idx_BD0F14 = 0;
        dword_57831C = 0;

        for (DWORD i = 0; i < 3; i++)
        {
            DWORD tpage = PSX_getTPage_4F60E0(static_cast<char>(i), static_cast<char>(3 - i), 64 * i, (i == 0u) ? 0u : 256u);
            PSX_TPage_Change_4F6430(static_cast<short>(tpage));

            ASSERT_EQ(sActiveTPage_578318, tpage);
            ASSERT_EQ(sTexture_page_x_BD0F0C, 64u * i);
            ASSERT_EQ(sTexture_page_y_BD0F10, (i == 0) ? 0u : 256u);
            ASSERT_EQ(sTexture_page_idx_BD0F14, i);
            ASSERT_EQ(sTexture_page_abr_BD0F18, 3 - i);
            ASSERT_EQ(dword_57831C, 10u);
        }
    }

    void PsxRenderTests()
    {
        Test_PSX_TPage_Change_4F6430();
        Test_PSX_Rects_intersect_point_4FA100();
    }
}
