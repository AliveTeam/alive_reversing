#include "stdafx.h"
#include "WorkWheel.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "PathData.hpp"
#include "GameEnderController.hpp"

BaseGameObject* WorkWheel::VDestructor(signed int flags)
{
    return vdtor_4E3820(flags);
}

void WorkWheel::VUpdate()
{
    vUpdate_4E38E0();
}

void WorkWheel::VScreenChanged()
{
    vScreenChanged_4E3AD0();
}

int WorkWheel::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_4E3C40(reinterpret_cast<WorkWheel_SaveState*>(pSaveBuffer));
}

void WorkWheel::VStartTurning()
{
    vStartTurning_4E3A20();
}

void WorkWheel::VStopTurning(__int16 bResetSwitch)
{
    vStopTurning_4E3A60(bResetSwitch);
}

WorkWheel* WorkWheel::ctor_4E35D0(Path_Wheel* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x548064); // vTbl_WorkWheel_548064

    field_4_typeId = Types::eWheel_148;

    field_C_objectId = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Work_Wheel_Idle);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_4_flags.Set(eBit15_bSemiTrans);

    field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    if (pTlv->field_10_scale)
    {
        if (pTlv->field_10_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_20_animation.field_C_render_layer = 6;
            field_D6_scale = 0;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 25;
        field_D6_scale = 1;
    }

    field_F8_id = pTlv->field_12_id;
    field_FE_duration = pTlv->field_14_duration;
    field_102_off_time = pTlv->field_16_off_time;
    field_100_on_counter = 0;
    field_104_off_when_stopped = pTlv->field_18_off_when_stopped;

    PathLine* pathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos + FP_FromInteger(24),
        &pathLine,
        &hitX,
        &hitY,
        (field_D6_scale == 1) ? 15 : 240))
    {
        field_BC_ypos = hitY;
    }
    else
    {
        field_BC_ypos += FP_FromInteger(20) * field_CC_sprite_scale;
    }


    field_DC_bApplyShadows |= 2u;
    field_FC_state = WheelStates::eIdle_0;
    field_F4_tlv_info = tlvInfo;

    return this;
}

void WorkWheel::dtor_4E3850()
{
    SetVTable(this, 0x548064); // vTbl_WorkWheel_548064
    Path::TLV_Reset_4DB8E0(field_F4_tlv_info, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

WorkWheel* WorkWheel::vdtor_4E3820(signed int flags)
{
    dtor_4E3850();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

int CC WorkWheel::CreateFromSaveState_4E3B10(const BYTE* pState)
{
    const WorkWheel_SaveState* pData = reinterpret_cast<const WorkWheel_SaveState*>(pState);

    Path_Wheel* pTlv = static_cast<Path_Wheel*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pData->field_4_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kWorkwhelResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("WORKWHEL.BAN", nullptr);
    }

    auto pWheel = ae_new<WorkWheel>();
    if (pWheel)
    {
        pWheel->ctor_4E35D0(pTlv, pData->field_4_tlvInfo);

        if (pData->field_C_state == WheelStates::eTurning_1)
        {
            pWheel->vStartTurning_4E3A20();
        }

        pWheel->field_100_on_counter = pData->field_8_snd_counter;
    }
    return sizeof(WorkWheel_SaveState);
}

int WorkWheel::vGetSaveState_4E3C40(WorkWheel_SaveState* pState)
{
    pState->field_0_id = Types::eWheel_148;
    pState->field_4_tlvInfo = field_F4_tlv_info;
    pState->field_8_snd_counter = field_100_on_counter;
    pState->field_C_state = field_FC_state;
    return sizeof(WorkWheel_SaveState);
}

void WorkWheel::vUpdate_4E38E0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_FC_state == WheelStates::eTurning_1)
    {
        ++field_100_on_counter;

        if (!(field_100_on_counter % 10)
            && gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
                field_C2_lvl_number,
                field_C0_path_number,
                field_B8_xpos,
                field_BC_ypos,
                0))
        {
            const short randomVol = Math_RandomRange_496AB0(-30, 0);
            SND_SEQ_Play_4CAB10(SeqId::WheelSqueak_19, 1, randomVol + 127, randomVol + 127);
        }
    }
    else if (field_FC_state == WheelStates::eIdle_0)
    {
        field_100_on_counter = 0;
    }

    if (field_F8_id)
    {
        if (field_100_on_counter > field_FE_duration)
        {
            if (gMap_5C3030.field_0_current_level == LevelIds::eBrewery_Ender_10 && field_F8_id == 100)
            {
                CreateGameEnderController_43B7A0();
            }

            if (field_102_off_time > 0 && field_100_on_counter > field_102_off_time)
            {
                SwitchStates_Set_465FF0(field_F8_id, 0);
            }
            else
            {
                SwitchStates_Set_465FF0(field_F8_id, 1);
            }
        }
    }
}

void WorkWheel::vScreenChanged_4E3AD0()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level ||
        gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path ||
        field_FC_state == WheelStates::eIdle_0)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void WorkWheel::vStartTurning_4E3A20()
{
    if (field_FC_state == WheelStates::eIdle_0)
    {
        field_FC_state = WheelStates::eTurning_1;
        const AnimRecord& animRec = AnimRec(AnimId::Work_Wheel_Turning);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    }
}

void WorkWheel::vStopTurning_4E3A60(__int16 bResetSwitch)
{
    if (field_FC_state == WheelStates::eTurning_1)
    {
        field_FC_state = WheelStates::eIdle_0;

        // Spin it.
        const AnimRecord& animRec = AnimRec(AnimId::Work_Wheel_Idle);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);

        if (field_104_off_when_stopped & 1)
        {
            if (bResetSwitch)
            {
                SwitchStates_Set_465FF0(field_F8_id, 0);
            }
        }
    }
}
