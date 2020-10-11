#include "stdafx_ao.h"
#include "Function.hpp"
#include "FlintLockFire.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "MusicTrigger.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"

START_NS_AO

struct FlintLockFireData
{
    int field_0_resourceId;
    int field_4_frameTable2;
    int field_8_maxW2;
    int field_C_maxH2;
    int field_10_resId;
    int field_14_frameTable1;
    int field_18_fireFrameTable;
    int field_1C_maxW1;
    int field_20_maxH1;
    int field_24_bFire;
};
ALIVE_ASSERT_SIZEOF(FlintLockFireData, 0x28);

const FlintLockFireData sFlintLockFireData_4BAC70[] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 2028, 9760, 105, 84, 2017, 11836, 11848, 125, 59, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 2028, 9760, 105, 84, 2017, 11836, 11848, 125, 59, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 2028, 9760, 105, 84, 2017, 11836, 11848, 125, 59, 1 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};



void FlintLockFire::VUpdate()
{
    VUpdate_41AEE0();
}

void FlintLockFire::VScreenChanged_41B0B0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void FlintLockFire::VScreenChanged()
{
    VScreenChanged_41B0B0();
}

void FlintLockFire::VStopAudio_41B0C0()
{
    if (field_EC_sound)
    {
        SND_Stop_Channels_Mask_4774A0(field_EC_sound);
        field_EC_sound = 0;
    }
}

void FlintLockFire::VStopAudio()
{
    VStopAudio_41B0C0();
}

FlintLockFire* FlintLockFire::Vdtor_41B500(signed int flags)
{
    dtor_41AE20();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* FlintLockFire::VDestructor(signed int flags)
{
    return Vdtor_41B500(flags);
}

BaseGameObject* FlintLockFire::dtor_41AE20()
{
    SetVTable(this, 0x4BAEA0);

    gMap_507BA8.TLV_Reset_446870(field_E8_tlvInfo, -1, 0, 0);
    field_F0_anim.vCleanUp();

    if (sFlintLockFireData_4BAC70[static_cast<int>(gMap_507BA8.field_0_current_level)].field_24_bFire)
    {
        field_188_anim.vCleanUp();
        field_220_anim.vCleanUp();
        if (field_EC_sound)
        {
            SND_Stop_Channels_Mask_4774A0(field_EC_sound);
        }
    }
    return dtor_417D10();
}

FlintLockFire* FlintLockFire::ctor_41AA90(Path_FlintLockFire* pTlv, int tlvInfo)
{
    ctor_417C10();
    field_4_typeId = Types::eFlintLockFire_34;

    SetVTable(this, 0x4BAEA0);

    SetVTable(&field_F0_anim, 0x4BA2B8);
    SetVTable(&field_188_anim, 0x4BA2B8);
    SetVTable(&field_220_anim, 0x4BA2B8);

    const int cur_lvl = static_cast<int>(gMap_507BA8.field_0_current_level);

    BYTE** ppAnimData = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, sFlintLockFireData_4BAC70[cur_lvl].field_10_resId, 1, 0);
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(
        ResourceManager::Resource_Animation,
        sFlintLockFireData_4BAC70[cur_lvl].field_0_resourceId,
        1,
        0);

    Animation_Init_417FD0(
        sFlintLockFireData_4BAC70[cur_lvl].field_14_frameTable1,
        sFlintLockFireData_4BAC70[cur_lvl].field_1C_maxW1,
        sFlintLockFireData_4BAC70[cur_lvl].field_20_maxH1,
        ppAnimData,
        1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_F0_anim.Init_402D20(
        sFlintLockFireData_4BAC70[cur_lvl].field_4_frameTable2,
        gObjList_animations_505564,
        this,
        static_cast<short>(sFlintLockFireData_4BAC70[cur_lvl].field_8_maxW2),
        static_cast<short>(sFlintLockFireData_4BAC70[cur_lvl].field_C_maxH2),
        ppRes,
        1,
        0,
        0);

    field_F0_anim.field_B_render_mode = 0;
    field_F0_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_F0_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 304, 1, 0);
        BYTE** v16 = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 304, 1, 0);
        field_188_anim.Init_402D20(
            5072,
            gObjList_animations_505564,
            this,
            51,
            24,
            v16,
            1,
            0,
            0);
        field_188_anim.field_B_render_mode = 0;
        field_188_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_188_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_188_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_220_anim.Init_402D20(5072, gObjList_animations_505564, this, 51, 24, v16, 1, 0, 0);
        field_220_anim.field_B_render_mode = 0;
        field_220_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_220_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_220_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_220_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_220_anim.SetFrame_402AC0(3u);
    }

    field_E4_state = 0;
    field_EC_sound = 0;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);
    field_E8_tlvInfo = tlvInfo;
    field_E6_switch_id = pTlv->field_1A_id;

    __int16 layer = 0;
    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        layer = 6;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        layer = 25;
        field_C6_scale = 1;
    }

    field_10_anim.field_C_layer = layer;
    field_F0_anim.field_C_layer = layer;

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        field_188_anim.field_C_layer = layer;
        field_220_anim.field_C_layer = layer;
    }

    if (SwitchStates_Get(pTlv->field_1A_id))
    {
        field_E4_state = 2;
        field_10_anim.Set_Animation_Data_402A40(sFlintLockFireData_4BAC70[cur_lvl].field_18_fireFrameTable, nullptr);
        field_10_anim.SetFrame_402AC0(field_10_anim.Get_Frame_Count_403540() - 1);
        field_10_anim.vDecode();
        field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_F0_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_188_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_220_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_220_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_EC_sound = SFX_Play_43AD70(SoundEffect::Fire_69, 0, 0);
        }
    }
    return this;
}

void FlintLockFire::VUpdate_41AEE0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const int cur_lvl = static_cast<int>(gMap_507BA8.field_0_current_level);

    switch (field_E4_state)
    {
    case 0:
        if (SwitchStates_Get(field_E6_switch_id))
        {
            field_E4_state = 1;
            field_10_anim.Set_Animation_Data_402A40(
                sFlintLockFireData_4BAC70[cur_lvl].field_18_fireFrameTable,
                0);
        }
        break;

    case 1:
        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            if (field_10_anim.field_92_current_frame == 6)
            {
                SFX_Play_43AD70(SoundEffect::FlintLock_68, 0, 0);
                SFX_Play_43AD70(SoundEffect::PostFlint_70, 0, 0);
            }
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_E4_state = 2;

            field_F0_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                field_188_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_188_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

                field_220_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                field_220_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

                field_EC_sound = SFX_Play_43AD70(SoundEffect::Fire_69, 0, 0);
            }

            auto pMusicTrigger = ao_new<MusicTrigger>();
            if (pMusicTrigger)
            {
                pMusicTrigger->ctor_443A60(6, 1, 0, 15);
            }
        }
        break;

    case 2:
        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            if (!field_EC_sound)
            {
                field_EC_sound = SFX_Play_43AD70(SoundEffect::Fire_69, 0, 0);
            }
        }
        break;

    default:
        break;
    }
}

void FlintLockFire::VRender(int** pOrderingTable)
{
    VRender_41B0F0(pOrderingTable);
}

void FlintLockFire::VRender_41B0F0(int** /*ppOt*/)
{
    NOT_IMPLEMENTED();
}

END_NS_AO
