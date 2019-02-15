#include "stdafx.h"
#include "Door.hpp"
#include "Function.hpp"
#include "Path.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "MusicTrigger.hpp"
#include "Midi.hpp"
#include "DDCheat.hpp"
#include "BaseAliveGameObject.hpp"

struct Door_Info
{
    int field_0_frameTableOffset_closed;
    int field_4_frameTableOffset_open;
    int field_8_maxW;
    int field_C_maxH;
};
ALIVE_ASSERT_SIZEOF(Door_Info, 0x10);

Door_Info stru_544888[19] =
{
    { 8692, 8704, 77, 69 },
    { 8692, 8704, 77, 69 },
    { 5436, 5448, 67, 62 },
    { 5436, 5448, 67, 62 },
    { 5436, 5448, 67, 62 },
    { 7636, 7648, 62, 70 },
    { 5652, 5664, 56, 62 },
    { 5436, 5448, 67, 62 },
    { 6304, 6316, 59, 58 },
    { 6224, 6236, 54, 71 },
    { 6224, 6236, 54, 71 },
    { 5436, 5448, 67, 62 },
    { 7636, 7648, 62, 70 },
    { 5652, 5664, 56, 62 },
    { 6304, 6316, 59, 58 },
    { 8692, 8704, 77, 69 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 }
};



Door* Door::ctor_41E250(Path_Door* pTlvData, int tlvInfo)
{
    NOT_IMPLEMENTED(); // Pretty bugged right now

    // TODO: BASE
    //BaseDoor::ctor_41E0D0(this);
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x5449BC); // vTbl_Door_5449BC
    field_4_typeId = BaseGameObject::Types::eDoor_33;

    // HACK/test
    //pTlvData->field_20_start_state = 1;

    field_F4_tlvInfo = tlvInfo;
    field_F8_door_type = pTlvData->field_1E_type;
    field_FE_start_state = pTlvData->field_20_start_state;

    if (pTlvData->field_40_close_after_use)
    {
        if (pTlvData->field_1_unknown)
        {
            field_FE_start_state = 1;
        }
    }

    field_100_switch_id = pTlvData->field_1A_id;
    if (pTlvData->field_1A_id == 1)
    {
        field_100_switch_id = 0;
    }

    field_FA_door_number = pTlvData->field_18_door_number;
    if (gMap_5C3030.sCurrentLevelId_5C3030 == LevelIds::eFeeCoDepot_5)
    {
        switch (field_FA_door_number)
        {
        case 30000:
            if (sVisitedBonewerks_5C1C02)
            {
                field_FE_start_state = eClosed;
            }
            break;

        case 30001:
        case 30004:
            if (sVisitedBarracks_5C1C04)
            {
                field_FE_start_state = eClosed;
            }
            break;

        case 30002:
            if (sVisitedBonewerks_5C1C02)
            {
                SwitchStates_Set_465FF0(255u, 1);
            }

            if (sVisitedBarracks_5C1C04)
            {
                SwitchStates_Set_465FF0(254u, 1);
            }

            if (sVisitedFeecoEnder_5C1C06)
            {
                SwitchStates_Set_465FF0(255u, 0);
                SwitchStates_Set_465FF0(254u, 0);
            }
            break;

        case 30003:
            if (sVisitedBonewerks_5C1C02)
            {
                if (sVisitedBarracks_5C1C04)
                {
                    if (sVisitedFeecoEnder_5C1C06)
                    {
                        field_FE_start_state = 0;
                    }
                }
                if (sVisitedBonewerks_5C1C02)
                {
                    SwitchStates_Set_465FF0(250u, 1);
                }
            }
            if (sVisitedBarracks_5C1C04)
            {
                SwitchStates_Set_465FF0(251u, 1);
            }
            if (sVisitedFeecoEnder_5C1C06)
            {
                SwitchStates_Set_465FF0(253u, 1);
            }
            break;
        default:
            break;
        }
    }

    if (field_F8_door_type == DoorTypes::eType_3)
    {
        field_102_hub_ids[0] = pTlvData->field_22_hubs[0];
        field_102_hub_ids[1] = pTlvData->field_22_hubs[1];
        field_102_hub_ids[2] = pTlvData->field_22_hubs[2];
        field_102_hub_ids[3] = pTlvData->field_22_hubs[3];
        field_102_hub_ids[4] = pTlvData->field_22_hubs[4];
        field_102_hub_ids[5] = pTlvData->field_22_hubs[5];
        field_102_hub_ids[6] = pTlvData->field_22_hubs[6];
        field_102_hub_ids[7] = pTlvData->field_22_hubs[7];

        if (SwitchStates_Get_466020(field_102_hub_ids[0]) &&
            SwitchStates_Get_466020(field_102_hub_ids[1]) &&
            SwitchStates_Get_466020(field_102_hub_ids[2]) &&
            SwitchStates_Get_466020(field_102_hub_ids[3]) &&
            SwitchStates_Get_466020(field_102_hub_ids[4]) &&
            SwitchStates_Get_466020(field_102_hub_ids[5]) &&
            SwitchStates_Get_466020(field_102_hub_ids[6]) &&
            SwitchStates_Get_466020(field_102_hub_ids[7]))
        {
            SwitchStates_Do_Operation_465F00(field_100_switch_id, eSetTrue);
        }
        else
        {
            SwitchStates_Do_Operation_465F00(field_100_switch_id, eSetFalse);
        }
    }

    if (field_FE_start_state == eOpen)
    {
        if (SwitchStates_Get_466020(field_100_switch_id))
        {
            // Its open but the switch id is on which causes it to close
            field_FE_start_state = eClosed;
        }
    }
    else
    {
        // Any other state is forced to closed
        field_FE_start_state = eClosed;
    }

    // TODO: Expose abe enums
    if ((sActiveHero_5C1B68->field_106_current_state == 114 || sActiveHero_5C1B68->field_106_current_state == 115)
        && field_FC_current_state == 1
        && field_FA_door_number == sActiveHero_5C1B68->field_1A0_door_id)
    {
        field_FC_current_state = eOpen;
    }

    if (field_F8_door_type == DoorTypes::eType_2)
    {
        field_102_hub_ids[0] = pTlvData->field_22_hubs[0];
        field_102_hub_ids[1] = pTlvData->field_22_hubs[1];
        field_102_hub_ids[2] = pTlvData->field_22_hubs[2];
        field_102_hub_ids[3] = pTlvData->field_22_hubs[3];
        field_102_hub_ids[4] = pTlvData->field_22_hubs[4];
        field_102_hub_ids[5] = pTlvData->field_22_hubs[5];
        field_102_hub_ids[6] = pTlvData->field_22_hubs[6];
        field_102_hub_ids[7] = pTlvData->field_22_hubs[7];
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kF2p3dorResID);
    if (!ppRes || stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0_frameTableOffset_closed == 0)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable);
        field_6_flags.Clear(BaseGameObject::eDead);
        return this;
    }

    if (field_FC_current_state == eOpen)
    {
        if (gMap_5C3030.field_22 == 108)
        {
            Animation_Init_424E10(
                6616,
                51,
                62,
                ppRes,
                1,
                1u);
        }
        else
        {
            Animation_Init_424E10(
                stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0_frameTableOffset_closed,
                static_cast<WORD>(stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_8_maxW),
                static_cast<WORD>(stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_C_maxH),
                ppRes,
                1,
                1u);
        }
    }
    else
    {
        if (gMap_5C3030.field_22 == 108)
        {
            Animation_Init_424E10(
                6604,
                51,
                62,
                ppRes,
                1,
                1u);
        }
        else
        {
            Animation_Init_424E10(
                stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_0_frameTableOffset_closed,
                static_cast<WORD>(stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_8_maxW),
                static_cast<WORD>(stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_C_maxH),
                ppRes,
                1,
                1u);
        }
    }

    if (pTlvData->field_16_scale)
    {
        if (pTlvData->field_16_scale == 1)
        {
            field_CC_sprite_scale = FP_FromDouble(0.5);
            field_D6_scale = 0;
            field_20_animation.field_C_render_layer = 6;
        }
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 25;
    }

    FP* xOff = &field_B8_xpos;
    FP* yOff = &field_BC_ypos;

    FP tlvXMid = FP_FromInteger((pTlvData->field_8_top_left.field_0_x + pTlvData->field_C_bottom_right.field_0_x) / 2);
    PathLine* pathLine = nullptr;

    if (sCollisions_DArray_5C1128->Raycast_417A60(
        tlvXMid,
        FP_FromInteger(pTlvData->field_8_top_left.field_2_y),
        tlvXMid,
        FP_FromInteger(pTlvData->field_C_bottom_right.field_2_y),
        &pathLine,
        xOff,
        yOff,
        (field_D6_scale == 1) ? 15 : 240))
    {
        // Move up off the line we hit
        *yOff -= FP_FromInteger(12) * field_CC_sprite_scale;

        // Snap on X
        *xOff = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(*xOff)));
    }
    else
    {
        // Couldn't glue to the floor.. just use the TLV pos
        *xOff = FP_FromInteger(pTlvData->field_8_top_left.field_0_x + 12);
        *yOff = FP_FromInteger(pTlvData->field_8_top_left.field_2_y + 24);
    }

    // Add on the TLV offset
    *xOff += FP_FromInteger(pTlvData->field_36_x_offset);
    *yOff += FP_FromInteger(pTlvData->field_38_y_offset);

    // Another OWI special
    FP yAdjustHack = {};
    if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eBarracks_6 &&
        gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eBarracks_Ender_13 ||
        gMap_5C3030.field_22 == 108)
    {
        if (gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eBonewerkz_8 &&
            gMap_5C3030.sCurrentLevelId_5C3030 != LevelIds::eBonewerkz_Ender_14)
        {
            goto LABEL_73;
        }

        yAdjustHack = FP_FromInteger(10) * field_CC_sprite_scale;
    }
    else
    {
        yAdjustHack = FP_FromInteger(14) * field_CC_sprite_scale;
    }
    *yOff += yAdjustHack;

LABEL_73:

    if (field_FC_current_state != eClosed)
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_DC_bApplyShadows |= 2u;

    return this;
}

BOOL Door::vIsOpen_41EB00()
{
    return field_FC_current_state == eOpen;
}

void Door::vOpen_41EB20()
{
    if (field_FC_current_state != eOpen)
    {
        field_FC_current_state = eOpening;
    }
}

void Door::vClose_41EB50()
{
    if (field_FC_current_state != eClosed)
    {
        field_FE_start_state = eClosed;
        field_FC_current_state = eClosing;
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_F4_tlvInfo);
        pTlv->field_1_unknown = 1;
    }
}

void Door::vSetOpen_41EBA0()
{
    field_FC_current_state = eOpen;
}

void Door::vSetClosed_41EBC0()
{
    field_FC_current_state = eClosed;
}

void Door::vScreenChange_41F080()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

Door* Door::vdtor_41E9D0(signed int flags)
{
    dtor_41EA00();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Door::PlaySound_41EA90()
{
    __int16 volume = 0;
    if (field_F8_door_type != DoorTypes::eType_0 || field_CC_sprite_scale != FP_FromInteger(1))
    {
        volume = 60;
    }
    else
    {
        volume = 90;
    }

    SFX_Play_46FBA0(0x39u, volume, 900, 0x10000);
    SFX_Play_46FBA0(0x39u, volume, 0, 0x10000);
}

void Door::dtor_41EA00()
{
    NOT_IMPLEMENTED();

    SetVTable(this, 0x5449BC); // vTbl_Door_5449BC
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    // TODO
    //dtor_41E130();
}

void Door::vUpdate_41EBE0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (sActiveHero_5C1B68->field_106_current_state == 114 || sActiveHero_5C1B68->field_106_current_state == 115)
    {
        if (field_FC_current_state == eClosed && field_FA_door_number == sActiveHero_5C1B68->field_1A0_door_id)
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_FC_current_state = eOpen;
        }
    }
    else
    {
        field_FA_door_number = -1;
        if (field_F8_door_type == DoorTypes::eType_2 || field_F8_door_type == DoorTypes::eType_3)
        {
            if (SwitchStates_Get_466020(field_102_hub_ids[0])
                && SwitchStates_Get_466020(field_102_hub_ids[1])
                && SwitchStates_Get_466020(field_102_hub_ids[2])
                && SwitchStates_Get_466020(field_102_hub_ids[3])
                && SwitchStates_Get_466020(field_102_hub_ids[4])
                && SwitchStates_Get_466020(field_102_hub_ids[5])
                && SwitchStates_Get_466020(field_102_hub_ids[6])
                && SwitchStates_Get_466020(field_102_hub_ids[7]))
            {
                if (!SwitchStates_Get_466020(field_100_switch_id) && field_F8_door_type == DoorTypes::eType_2)
                {
                    SND_SEQ_Play_4CAB10(0x20u, 1, 127, 127);
                    auto pMusicTrigger = alive_new<MusicTrigger>();
                    if (pMusicTrigger)
                    {
                        pMusicTrigger->ctor_47FF10(5, 0, 0, 0);
                    }
                }
                SwitchStates_Do_Operation_465F00(field_100_switch_id, eSetTrue);
            }
            else
            {
                SwitchStates_Do_Operation_465F00(field_100_switch_id, eSetFalse);
            }
        }
        switch (field_FC_current_state)
        {
        case eOpen:
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

            if (!field_FE_start_state && SwitchStates_Get_466020(field_100_switch_id)
                || field_FE_start_state == 1 && !SwitchStates_Get_466020(field_100_switch_id))
            {
                field_FC_current_state = eClosing;
                if (gMap_5C3030.field_22 == 108)
                {
                    field_20_animation.Set_Animation_Data_409C80(6616, nullptr);
                }
                else
                {
                    field_20_animation.Set_Animation_Data_409C80(
                        stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_4_frameTableOffset_open,
                        nullptr);
                }

                field_20_animation.field_4_flags.Clear(AnimFlags::eBit19_LoopBackwards);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
            break;

        case eClosed:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

            if (field_FE_start_state == eClosed && SwitchStates_Get_466020(field_100_switch_id) ||
                field_FE_start_state == eOpen && !SwitchStates_Get_466020(field_100_switch_id))
            {
                field_FC_current_state = eOpening;
                if (gMap_5C3030.field_22 == 108)
                {
                    field_20_animation.Set_Animation_Data_409C80(6616, nullptr);
                }
                else
                {
                    field_20_animation.Set_Animation_Data_409C80(
                        stru_544888[static_cast<int>(gMap_5C3030.sCurrentLevelId_5C3030)].field_4_frameTableOffset_open,
                        nullptr);
                }

                field_20_animation.SetFrame_409D50(3);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
                PlaySound_41EA90();
            }
            break;

        case eOpening:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_state = eOpen;
            }
            break;

        case eClosing:
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_FC_current_state = eClosed;
                PlaySound_41EA90();
            }
            break;

        default:
            return;
        }
    }
}

// TODO: Move this out when its done
#include "PsxDisplay.hpp"
#include "Animation.hpp"

class Rope_Segment : public Animation
{
public:
    virtual void vDecode_40AC90() override
    {
        // VNull_409C20
    }

    virtual void vRender_40B820(int xpos, int ypos, int** pOt, __int16 width, signed int height) override
    {
        vRender_40C690(xpos, ypos, pOt, width, height);
    }

    virtual signed __int16 vCleanUp_40C630() override
    {
        vCleanUp_40C9C0();
    }

    EXPORT void GetRenderedSize_40C980(PSX_RECT* pRect)
    {
        Poly_FT4_Get_Rect_409DA0(pRect, &field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index]);
    }

private:
    EXPORT void vRender_40C690(int xpos, int width, int** pOt, int /*width*/, int /*height*/)
    {
        Poly_FT4* pPoly = &field_10_polys[gPsxDisplay_5C1130.field_C_buffer_index];
        if (field_4_flags.Get(AnimFlags::eBit3_Render))
        {
            // Copy from animation to local
            *pPoly = field_68_anim_ptr->field_2C_ot_data[gPsxDisplay_5C1130.field_C_buffer_index];
            FrameInfoHeader* pFrameInfoHeader = field_68_anim_ptr->Get_FrameHeader_40B730(-1);

            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit22_DeadMode) )
            {
                ALIVE_FATAL("Impossible branch");
            }

            FrameHeader* pFrameHeader = reinterpret_cast<FrameHeader*>(&(*field_68_anim_ptr->field_20_ppBlock)[pFrameInfoHeader->field_0_frame_header_offset]);

            int frameH = pFrameHeader->field_5_height;
            int frameW = pFrameHeader->field_4_width;
            
            int frameOffX = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.x;
            int frameOffY = pFrameInfoHeader->field_8_data.offsetAndRect.mOffset.y;

            if (field_6C_scale != FP_FromInteger(1))
            {
                frameOffX = FP_GetExponent((FP_FromInteger(frameOffX) * field_6C_scale));
                frameOffY = FP_GetExponent((FP_FromInteger(frameOffY) * field_6C_scale));

                frameH = FP_GetExponent(FP_FromInteger(frameH) * field_6C_scale);
                frameW = FP_GetExponent((FP_FromInteger(frameW) * field_6C_scale));
            }

            int polyX = 0;
            int polyY = 0;
            int xConverted = PsxToPCX(xpos);
            if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit7_SwapXY))
            {
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit6_FlipY))
                {
                    if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        polyX = xConverted - frameOffY - frameH;
                    }
                    else
                    {
                        polyX = frameOffY + xConverted;
                    }
                    polyY = frameOffX + width;
                }
                else
                {
                    if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        polyX = xConverted - frameOffY - frameH;
                    }
                    else
                    {
                        polyX = frameOffY + xConverted;
                    }
                    polyY = width - frameOffX - frameW;
                }
            }
            else if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit6_FlipY))
            {
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    polyX = xConverted - frameOffX - frameW;
                }
                else
                {
                    polyX = frameOffX + xConverted;
                }
                polyY = width - frameOffY - frameH;
            }
            else
            {
                if (field_68_anim_ptr->field_4_flags.Get(AnimFlags::eBit5_FlipX))
                {
                    polyX = xConverted - frameOffX - frameW;
                }
                else
                {
                    polyX = frameOffX + xConverted;
                }
                polyY = frameOffY + width;
            }

            if (!field_4_flags.Get(AnimFlags::eBit16_bBlending))
            {
                SetRGB0(pPoly, field_8_r, field_9_g, field_A_b);
            }

            SetXYWH(pPoly, 
                static_cast<short>(polyX),
                static_cast<short>(polyY),
                static_cast<short>(frameW - 1),
                static_cast<short>(frameH - 1));

            if (pFrameHeader->field_7_compression_type == 3 || pFrameHeader->field_7_compression_type == 6)
            {
                SetPrimExtraPointerHack(pPoly, &pFrameHeader->field_8_width2);
            }
            else
            {
                SetPrimExtraPointerHack(pPoly, nullptr);
            }
            OrderingTable_Add_4F8AA0(&pOt[field_C_render_layer], &pPoly->mBase.header);
        }
    }

    EXPORT void vCleanUp_40C9C0()
    {
        field_68_anim_ptr = nullptr;
    }

public:
    Poly_FT4 field_10_polys[2];
    AnimationEx* field_68_anim_ptr;
    FP field_6C_scale;
};
ALIVE_ASSERT_SIZEOF(Rope_Segment, 0x70);

const TintEntry stru_55FD68[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


class LiftRope : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT LiftRope* ctor_4A0A70(unsigned __int16 left, __int16 top, unsigned __int16 bottom, FP scale)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x546C70); // vTbl_LiftRope_00546C70

        field_4_typeId = Types::eLiftRope_108;

        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kRopesResID);
        Animation_Init_424E10(748, 9, 16, ppRes, 1, 1);
        SetTint_425600(stru_55FD68, gMap_5C3030.sCurrentLevelId_5C3030);

        field_20_animation.field_14_scale = scale;
        field_CC_sprite_scale = scale;

        if (scale == FP_FromInteger(1))
        {
            field_F6_rope_length = 15;
            field_20_animation.field_C_render_layer = 24;
            field_D6_scale = 1;
        }
        else
        {
            field_F6_rope_length = 7;
            field_20_animation.field_C_render_layer = 5;
            field_20_animation.field_14_scale = FP_FromDouble(0.7);
            field_CC_sprite_scale = FP_FromDouble(0.7);
            field_D6_scale = 0;
        };

        field_20_animation.field_8_r = static_cast<BYTE>(field_D0_r);
        field_20_animation.field_9_g = static_cast<BYTE>(field_D2_g);
        field_20_animation.field_A_b = static_cast<BYTE>(field_D4_b);

        field_102_top = top;
        field_106_bottom = bottom;
        field_100_left = left;
        field_104_right = left;

        field_B8_xpos = FP_FromInteger(left);
        field_BC_ypos = FP_FromInteger(bottom);
        
        field_F4_rope_segment_count = (240 / field_F6_rope_length) + 1; // psx screen height

        field_F8_ppRopeRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Rope, 0, field_F4_rope_segment_count * sizeof(Rope_Segment));
        field_FC_pRopeRes = reinterpret_cast<Rope_Segment*>(*field_F8_ppRopeRes);


        for (int i = 0; i < field_F4_rope_segment_count; i++)
        {
            Rope_Segment* pSegment = &field_FC_pRopeRes[i];
            SetVTable(pSegment, 0x5447CC); // vTbl_RopeSegment_5447CC
            pSegment->field_4_flags.Set(AnimFlags::eBit3_Render);
            pSegment->field_68_anim_ptr = &field_20_animation;
            pSegment->field_C_render_layer = field_20_animation.field_C_render_layer;
            pSegment->field_6C_scale = scale;
            pSegment->field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            pSegment->field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        }

        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4A0D80(flags);
    }

    virtual void VUpdate() override
    {
        // nullsub@4A11E0
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_4A0E30(pOrderingTable);
    }

private:
    EXPORT void dtor_4A0DB0()
    {
        SetVTable(this, 0x546C70); // vTbl_LiftRope_00546C70
        ResourceManager::FreeResource_49C330(field_F8_ppRopeRes);
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT LiftRope* vdtor_4A0D80(signed int flags)
    {
        dtor_4A0DB0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

    EXPORT void vRender_4A0E30(int** /*pOt*/)
    {
        NOT_IMPLEMENTED();
    }

private:
    int field_E4_not_used[4];
    __int16 field_F4_rope_segment_count;
    __int16 field_F6_rope_length;
    BYTE **field_F8_ppRopeRes;
    Rope_Segment* field_FC_pRopeRes;
    __int16 field_100_left;
    __int16 field_102_top;
    __int16 field_104_right;
    __int16 field_106_bottom;
};
ALIVE_ASSERT_SIZEOF(LiftRope, 0x108);
