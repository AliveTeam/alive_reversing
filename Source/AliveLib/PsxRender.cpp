#include "stdafx.h"
#include "PsxRender.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "Midi.hpp"

using TPsxDraw = std::add_pointer<int(__cdecl)(DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD)>::type;

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


EXPORT void __cdecl PSX_4F6A70(void *a1, WORD *a2, unsigned __int8 *a3)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC PSX_4F6430(unsigned __int16 a1)
{
    NOT_IMPLEMENTED();
}

EXPORT unsigned __int8 *__cdecl PSX_4F7110(int a1, int a2, int a3)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

EXPORT void __cdecl PSX_4F7960(int a1, int a2, int a3)
{
    NOT_IMPLEMENTED();
}

// TODO: For reference only, will be implemented when all prim types are recovered
static bool DrawOTagImpl(int** pOT, __int16 drawEnv_of0, __int16 drawEnv_of1)
{

    int** v1 = pOT;
    __int16 v2 = drawEnv_of0;
    __int16 v16 = drawEnv_of1;
    //dword_BD30E4 = 0;
    //dword_BD30A4 = 0;
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
            if (v1 == (int **)-1)
            {
                break;
            }

            MIDI_UpdatePlayer_4FDC80();

            if (v1 < (int **)v21 || v1 >= pOtEnd)
            {
                char v5 = *((BYTE *)v1 + 11);
                int itemToDrawType = *((unsigned __int8 *)v1 + 11);
                switch (itemToDrawType)
                {
                case 2: // ??
                    PSX_4F6A70(0, (WORD *)v1 + 6, (unsigned __int8 *)v1);
                    break;
                case 128: // 0x80 = change tpage?
                    PSX_4F6430(*((WORD *)v1 + 6));
                    break;
                case 129: // 0x81 Init_PrimClipper_4F5B80
                    LOG_WARNING("129");
                    /*
                    v8 = (int)v1[1];
                    v27 = v1[3];
                    *(_DWORD *)&sPSX_EMU_DrawEnvState_C3D080.field_0_clip.x = v27;
                    v28 = v8;
                    *(_DWORD *)&sPSX_EMU_DrawEnvState_C3D080.field_0_clip.w = v8;
                    PSX_SetDrawEnv_Impl_4FE420(
                    16 * (signed __int16)v27,
                    16 * SHIWORD(v27),
                    16 * ((signed __int16)v27 + (signed __int16)v8) - 16,
                    16 * (SHIWORD(v27) + SHIWORD(v8)) - 16,
                    sConst_1000_578E88 / 2,
                    byte_989680);
                    */
                    break;
                case 130: // 0x82 Prim_ScreenOffset
                          //LOG_WARNING("130");
                          /*
                          if (dword_55EF94)
                          {
                          v7 = *((signed __int16 *)v1 + 7);
                          dword_BD30E4 = 2 * *((signed __int16 *)v1 + 6);
                          dword_BD30A4 = v7;
                          }
                          else
                          {
                          v2 = drawEnv_of0 + *((signed __int16 *)v1 + 6);
                          v16 = drawEnv_of1 + *((signed __int16 *)v1 + 7);
                          }*/
                    break;
                case 131: // 0x83 ?? move image ?? 
                    LOG_WARNING("131");
                    BMP_unlock_4F2100(&sPsxVram_C1D160);
                    PSX_MoveImage_4F5D50((PSX_RECT *)(v1 + 5), (int)v1[3], (int)v1[4]);
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
                    LOG_WARNING("132");
                    //PSX_4F7B80((int)v1[3], (int)v1[4], (int)v1[5], (int)v1[6], (int)v1[7]);
                    break;
                default:
                    int v12 = 0;
                    int v13 = 0;
                    int v25 = 0;
                    int v26 = 0;

                    int v9 = dword_C2D03C++ + 1;
                    if ((v5 & 0x60) == 96)
                    {
                        //LOBYTE(itemToDrawType) = itemToDrawType & 0xFC;
                        switch (itemToDrawType & 0xFC)
                        {
                        case 96: // 0x60
                            LOG_INFO("96");
                            /*
                            v10 = *((WORD *)v1 + 9);
                            v25 = *((WORD *)v1 + 8);
                            v26 = v10;*/
                            goto LABEL_31;
                        case 100: // 0x64
                            v12 = *((WORD *)v1 + 10);
                            v13 = *((WORD *)v1 + 11);
                            v25 = *((WORD *)v1 + 10);
                            v26 = v13;
                            goto LABEL_36;
                        case 104: // 0x68
                            LOG_INFO("104");
                            //v11 = 1;
                            goto LABEL_30;
                        case 112: // 0x70
                            LOG_INFO("112");
                            //v11 = 8;
                            goto LABEL_30;
                        case 116: // 0x74
                            LOG_INFO("116");
                            //v13 = 8;
                            //v26 = 8;
                            //v12 = 8;
                            goto LABEL_35;
                        case 120: // 0x78
                            LOG_INFO("120");
                            //v11 = 16;
                        LABEL_30:
                            //v26 = v11;
                            //v25 = v11;
                        LABEL_31:
                            //LOWORD(v9) = v16 + *((WORD *)v1 + 7);
                            //v23 = v2 + *((WORD *)v1 + 6);
                            //v24 = v9;
                            //PSX_4F6A70(v9, &v23, (unsigned __int8 *)v1);
                            break;
                        case 124: // 0x7C
                        {
                            LOG_INFO("124");
                            //v13 = 16;
                            //v26 = 16;
                            //v12 = 16;
                        LABEL_35:
                            //v25 = v12;
                        LABEL_36: // e

                            itemToDrawType = *((BYTE *)v1 + 11); // TODO: LOBYTE() = 
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
                                b = *((BYTE *)v1 + 8);
                                g = *((BYTE *)v1 + 9);
                                r = *((BYTE *)v1 + 10);
                            }

                            //LOWORD(itemToDrawType) = *((WORD *)v1 + 9);

                            int p1 = v2 + *((signed __int16 *)v1 + 6); // offset + ?
                            int p2 = v16 + *((signed __int16 *)v1 + 7); // offset + ?;
                            int p3 = *((unsigned __int8 *)v1 + 16);
                            int p4 = *((unsigned __int8 *)v1 + 17);

                            dword_C2D04C(
                                p1,
                                p2,
                                p3,
                                p4,
                                128,
                                128,
                                128,
                                640,
                                480,
                                100,
                                100 & 2
                            );

                            /*
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
                            itemToDrawType & 2);*/
                        }
                        break;
                        default:
                            goto LABEL_45;
                        }
                    }
                    else if ((v5 & 0x40) == 64) // 0x40
                    {
                        //LOG_WARNING("64");
                        //PSX_4F7D90(v1, v2, v16);
                    }
                    else if ((v5 & 0x20) == 32) // 0x20
                    {

                        unsigned __int8 * v15 = PSX_4F7110((int)v1, v2, v16);
                        if (v15)
                        {
                            PSX_4F7960((int)v15, v2, v16);
                        }
                    }
                    break;
                }
            }
        LABEL_45:
            v1 = (int **)*v1;
        } while (v1);
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

        if (byte_BD1464)
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

        if (byte_BD1464)
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
