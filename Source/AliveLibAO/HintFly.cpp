#include "stdafx_ao.h"
#include "Function.hpp"
#include "HintFly.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"

void HintFly_ForceLink() {}

START_NS_AO

const char* gHintFlyMessages_4C6A10[] =
{
    "SNEAK TO BOMB",
    "ABE WAS HERE",
    "TEST THREE",
    "LEARN THE SECRET SONG",
    "BRAVE THE RING",
    "SNEAK IN AND OUT OF SHADOWS",
    "SEEK A ROCK",
    "SLOG GOES BOOM",
    "WATCH YOUR STEP",
    "FIND PLATFORM FOR ELUM",
    "DROP IN",
    "WATCH OUT FOR THAT BAT",
    "POWER WHISPERS FROM SHADOWS",
    "RESCUE LOST SOULS",
    "YOU CAN HOIST WHERE YOU SEE FALLING DEBRIS",
    "ALMOST HOME",
    "SNEAK UP BEHIND SLIG",
    "BLOW BOMBS WITH SPIRIT RINGS",
    "QUEST FOR SPIRIT RINGS",
    "THESE LIGHTS WILL GUIDE YOU",
    "TOUCH THE STONE",
    "FIND BELLSONG THEN POSSESS BELLS",
    "LEAP OF FAITH",
    "YOU WILL RETURN",
    "STEP LIGHTLY",
    "RETURN TO RUPTURE FARMS",
    "FAREWELL ELUM",
    "DO NOT FORGET YOUR ROCK",
    "ELUM WAIT HERE",
    "RUN ELUM RUN",
    "ELUMS LIKE HONEY BUT NOT BEES",
    "SNEAK BY SLIG",
    "WALK SOFTLY",
    "SCOUT FROM AFAR",
    "DO NOT FORGET THE FLINTLOCK",
    "THEY WONT CHASE WHAT THEY CANT SEE",
    nullptr
};

struct HintFlyParticle
{
    FP field_0_xpos;
    FP field_4_ypos;
    char field_8_state;
    char field_9_pad;
    __int16 field_A;
    FP field_C_velx;
    FP field_10_vely;
    FP field_14_targetX;
    FP field_18_targetY;
    __int16 field_1C_sound_pitch;
    __int16 field_1E_sound_pitch_speed;
    char field_20_angle;
    char field_21_angle_speed;
    char field_22_timer;
    char field_23_pad;
    Prim_Sprt field_24_sprt[2];
};
ALIVE_ASSERT_SIZEOF(HintFlyParticle, 0x54);

const BYTE* pData[] =
{
    (const BYTE*)0x4C6DB0,
    (const BYTE*)0x4C6DC8,
    (const BYTE*)0x4C6DE8,
    (const BYTE*)0x4C6E00,
    (const BYTE*)0x4C6E20,
    (const BYTE*)0x4C6E40,
    (const BYTE*)0x4C6E58,
    (const BYTE*)0x4C6E70,
    (const BYTE*)0x4C6E88,
    (const BYTE*)0x4C6EA0,
    (const BYTE*)0x4C6EB0,
    (const BYTE*)0x4C6EC8,
    (const BYTE*)0x4C6ED8,
    (const BYTE*)0x4C6EF8,
    (const BYTE*)0x4C6F10,
    (const BYTE*)0x4C6F28,
    (const BYTE*)0x4C6F40,
    (const BYTE*)0x4C6F58,
    (const BYTE*)0x4C6F78,
    (const BYTE*)0x4C6F90,
    (const BYTE*)0x4C6FA0,
    (const BYTE*)0x4C6FB8,
    (const BYTE*)0x4C6FC8,
    (const BYTE*)0x4C6FE8,
    (const BYTE*)0x4C7000,
    (const BYTE*)0x4C7010,
    /*
    byte_4C6DB0,
    byte_4C6DC8,
    byte_4C6DE8,
    byte_4C6E00,
    byte_4C6E20,
    byte_4C6E40,
    byte_4C6E58,
    byte_4C6E70,
    byte_4C6E88,
    byte_4C6EA0,
    byte_4C6EB0,
    byte_4C6EC8,
    byte_4C6ED8,
    byte_4C6EF8,
    byte_4C6F10,
    byte_4C6F28,
    byte_4C6F40,
    byte_4C6F58,
    byte_4C6F78,
    byte_4C6F90,
    byte_4C6FA0,
    byte_4C6FB8,
    byte_4C6FC8,
    byte_4C6FE8,
    byte_4C7000,
    byte_4C7010,
    */
};

ALIVE_VAR(1, 0x4C6AA4, BYTE, sHintFlyRndSeed_4C6AA4, 37);

static BYTE HintFly_NextRandom()
{
    return sRandomBytes_4BBE30[sHintFlyRndSeed_4C6AA4++];
}

HintFly* HintFly::ctor_42A820(Path_HintFly* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BB200);
    field_E4_ppRes = nullptr;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 62, 1, 0);
    if (ppRes)
    {
        Animation_Init_417FD0(556, 10, 7, ppRes, 1);

        field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_124_tlvInfo = tlvInfo;
        field_11E_msg_idx = 0;

        field_11C_message_id = pTlv->field_18_mesage_id;

        field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
        field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

        const char* pMsg = gHintFlyMessages_4C6A10[pTlv->field_18_mesage_id];

        field_118_counter = 20;
        field_11A_msg_len = 0;

        // Find the length of the longest word (in particles)
        short longestWordLen = 0;
        short curWordLen = 0;
        for(;;)
        {
            // End of word?
            if (*pMsg == ' ' || *pMsg == 0)
            {
                if (curWordLen > longestWordLen)
                {
                    longestWordLen = curWordLen;
                }
                curWordLen = 0;
                if (!*pMsg)
                {
                    break;
                }
            }
            else
            {
                curWordLen += pData[(*pMsg) - 'A'][0];
            }
            pMsg++;
        }

        field_11A_msg_len = longestWordLen;
        field_11A_msg_len += 12;

        field_E4_ppRes = ResourceManager::Allocate_New_Locked_Resource_454F80(ResourceManager::Resource_HintFly, 0, sizeof(HintFlyParticle) * field_11A_msg_len);
        if (field_E4_ppRes)
        {
            field_E8_pRes = reinterpret_cast<HintFlyParticle*>(*field_E4_ppRes);
            field_112_state = 1;
            field_10C_timer = 0;

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
            {
                field_110_bitMode = 1;
            }
            else if (field_10_anim.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                field_110_bitMode = 2;
            }
            else
            {
                field_110_bitMode = 0;
            }

            int vram_x = field_10_anim.field_84_vram_rect.x & 0x3F;
            if (field_110_bitMode == 1)
            {
                vram_x = 2 * vram_x;
            }
            else if (field_110_bitMode == 0)
            {
                vram_x = 4 * vram_x;
            }

            auto pHeader = reinterpret_cast<FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[field_10_anim.Get_FrameHeader_403A00(-1)->field_0_frame_header_offset]);

            for (int i = 0; i < field_11A_msg_len; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    Prim_Sprt* pSprt = &field_E8_pRes[i].field_24_sprt[j];

                    Sprt_Init(pSprt);

                    Poly_Set_SemiTrans_498A40(&pSprt->mBase.header, 1);
                    Poly_Set_Blending_498A00(&pSprt->mBase.header, 1);

                    SetClut(pSprt, static_cast<short>(PSX_getClut_496840(
                        field_10_anim.field_8C_pal_vram_xy.field_0_x,
                        field_10_anim.field_8C_pal_vram_xy.field_2_y)));

                    SetUV0(pSprt, vram_x & 0xFF, field_10_anim.field_84_vram_rect.y & 0xFF);

                    pSprt->field_14_w = pHeader->field_4_width - 1;
                    pSprt->field_16_h = pHeader->field_5_height - 1;
                }
            }


            field_114_xScreen = FP_GetExponent(field_A8_xpos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x);
            field_116_yScreen = FP_GetExponent(field_AC_ypos + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y);

            const PSX_RECT rect = { static_cast<short>(field_10_anim.field_8C_pal_vram_xy.field_0_x + 1), field_10_anim.field_8C_pal_vram_xy.field_2_y, 1, 1 };
            const BYTE data[] = { 0, 0, 0, 0 };
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                PSX_LoadImage16_4962A0(&rect, data);
            }
            else
            {
                PSX_LoadImage_496480(&rect, data);
            }

            for (int i = 0; i < field_118_counter; i++)
            {
                HintFlyParticle* pParticle = &field_E8_pRes[i];
                pParticle->field_8_state = 3;
                pParticle->field_0_xpos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + field_114_xScreen - 16);
                pParticle->field_4_ypos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + field_116_yScreen - 16);
                pParticle->field_20_angle = HintFly_NextRandom();
                pParticle->field_21_angle_speed = (HintFly_NextRandom() % 4) + 12;
                if (HintFly_NextRandom() % 2)
                {
                    pParticle->field_21_angle_speed = -pParticle->field_21_angle_speed;
                }
            }
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    else
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

BaseGameObject* HintFly::VDestructor(signed int flags)
{
    return Vdtor_42BCF0(flags);
}

HintFly* HintFly::Vdtor_42BCF0(signed int flags)
{
    dtor_42ADF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void HintFly::VScreenChanged()
{
    VScreenChanged_42BCE0();
}

void HintFly::VScreenChanged_42BCE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BaseGameObject* HintFly::dtor_42ADF0()
{
    SetVTable(this, 0x4BB200);
    if (field_E4_ppRes)
    {
        ResourceManager::FreeResource_455550(field_E4_ppRes);
    }
    gMap_507BA8.TLV_Reset_446870(field_124_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

void HintFly::FormWordAndAdvanceToNextWord_42AF90()
{
    const char* msgPtr = &gHintFlyMessages_4C6A10[field_11C_message_id][field_11E_msg_idx];
    LOG_INFO("Word is " << msgPtr);

    // Find how long the word is
    short letterCount = 0;
    const char* pEndWord = msgPtr;
    for (; *pEndWord != ' '; pEndWord++)
    {
        if (!*pEndWord)
        {
            break;
        }
        letterCount++;
    }

    // Next time start at the next word
    field_11E_msg_idx += letterCount;

    // Also skip the space if we had one else display will be screwed
    if (*pEndWord == ' ')
    {
        field_11E_msg_idx++;
    }

    const int xBase = field_114_xScreen - (16 * letterCount) / 2;
    const int yBase = field_116_yScreen - 8;
   
    FP xBaseFP = FP_FromInteger(xBase);
    const FP yBaseFP = FP_FromInteger(yBase);

    int particleIdx = 0;
    for (int i = 0; i < letterCount; i++)
    {
        const auto pArray = pData[msgPtr[i] - 'A'];
        // First element is the count of "pixels" that make up a word
        const int total = pArray[0];
        for (int j = 0; j < total; j++)
        {
            // Position each "pixel" of the word
            HintFlyParticle* pParticleIter = &field_E8_pRes[particleIdx++];

            auto xVal = FP_FromInteger(pArray[j+1] >> 4);
            auto yVal = FP_FromInteger((pArray[j+1]) & 0xF);

            pParticleIter->field_14_targetX = (FP_FromDouble(0.8) * xVal) + xBaseFP;
            pParticleIter->field_18_targetY = (FP_FromDouble(0.8) * yVal) + yBaseFP;
            pParticleIter->field_C_velx =  FP_FromRaw((pParticleIter->field_14_targetX.fpValue - pParticleIter->field_0_xpos.fpValue) >> 4);
            pParticleIter->field_10_vely = FP_FromRaw((pParticleIter->field_18_targetY.fpValue - pParticleIter->field_4_ypos.fpValue) >> 4);
            pParticleIter->field_8_state = 1;

            pParticleIter->field_1C_sound_pitch = Math_RandomRange_450F20(-127, 127);
            pParticleIter->field_1E_sound_pitch_speed = pParticleIter->field_1C_sound_pitch >> 4;
        }
        xBaseFP += FP_FromInteger(16);
    }

    while (particleIdx < field_118_counter)
    {
        field_E8_pRes[particleIdx++].field_8_state = 3;
    }
}

void HintFly::UpdateParticles_42B1B0()
{
    for (int i = 0; i < field_118_counter; i++)
    {
        HintFlyParticle* pParticle = &field_E8_pRes[i];
        if (pParticle->field_8_state == 1)
        {
            const FP xTargetDelta = FP_Abs(pParticle->field_14_targetX - pParticle->field_0_xpos);
            const FP yTargetDelta = FP_Abs(pParticle->field_18_targetY - pParticle->field_4_ypos);

            if (xTargetDelta >= FP_FromInteger(1) || yTargetDelta >= FP_FromInteger(1))
            {
                pParticle->field_0_xpos += pParticle->field_C_velx;
                pParticle->field_4_ypos += pParticle->field_10_vely;
                pParticle->field_1C_sound_pitch += pParticle->field_1E_sound_pitch_speed;
            }
            else
            {
                pParticle->field_8_state = 2;
                pParticle->field_1C_sound_pitch = 0;
            }

            if (i < 1 && Math_RandomRange_450F20(0, 100) < 40)
            {
                SFX_Play_43AE60(60u, Math_RandomRange_450F20(24, 30), pParticle->field_1C_sound_pitch, 0);
            }
        }
        else if (pParticle->field_8_state == 2)
        {
            if (!(HintFly_NextRandom() & 3))
            {
                pParticle->field_0_xpos = pParticle->field_14_targetX + FP_FromInteger(abs(HintFly_NextRandom()) & 1);
            }

            if (i < 1 && Math_RandomRange_450F20(0, 100) < 40)
            {
                SFX_Play_43AE60(60u, Math_RandomRange_450F20(24, 30), 0, 0);
            }
        }
        else if (pParticle->field_8_state == 3)
        {
            pParticle->field_20_angle = pParticle->field_20_angle + pParticle->field_21_angle_speed;
            pParticle->field_0_xpos += (Math_Sine_451110(pParticle->field_20_angle) * FP_FromInteger(5));
            pParticle->field_4_ypos += (Math_Cosine_4510A0(pParticle->field_20_angle) * FP_FromInteger(2));
            if (i < 1 && Math_RandomRange_450F20(0, 100) < 20)
            {
                SFX_Play_43AD70(60u, Math_RandomRange_450F20(18, 24), 0);
            }
        }
    }
}

END_NS_AO
