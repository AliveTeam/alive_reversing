#include "stdafx.h"
#include "TrapDoor.hpp"
#include "Function.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Path.hpp"
#include "stdlib.hpp"

struct TrapDoor_Data final
{
    AnimId field_0_open;
    AnimId field_4_closed;
    AnimId field_8_opening;
    AnimId field_C_closing;
    //s16 field_10_maxW;
    //s16 field_12_maxH;
};

const TrapDoor_Data sTrapDoorData_547B78[18] = {
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing}};

const TintEntry sTrapDoorTints_5639AC[18] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

TrapDoor::TrapDoor(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo)
{
    SetType(ReliveTypes::eTrapDoor);
    mBaseGameObjectTlvInfo = tlvInfo;

    field_12C_unused &= ~1u;

    field_130_stay_open_time2 = field_13C_stay_open_time;
    field_134_switch_id = pTlv->field_10_switch_id;
    field_138_switch_state = pTlv->field_12_start_state;

    const s32 levelIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    s32 frameTableOffset = 0;
    const AnimRecord& closedRec = AnimRec(sTrapDoorData_547B78[levelIdx].field_4_closed);
    if (field_138_switch_state == SwitchStates_Get(field_134_switch_id))
    {
        field_136_state = TrapDoorState::eOpen_2;
        const AnimRecord& openRec = AnimRec(sTrapDoorData_547B78[levelIdx].field_0_open);
        frameTableOffset = openRec.mFrameTableOffset;
    }
    else
    {
        field_136_state = TrapDoorState::eClosed_0;
        frameTableOffset = closedRec.mFrameTableOffset;
    }

    field_13E_self_closing = pTlv->field_14_self_closing;
    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID);

    AddDynamicCollision(
        closedRec.mFrameTableOffset,
        closedRec.mMaxW,
        closedRec.mMaxH,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_26;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        field_124_pCollisionLine->field_8_type = eLineTypes::eUnknown_36;
    }

    SetTint(sTrapDoorTints_5639AC, gMap.mCurrentLevel);

    field_140_x = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_144_y = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(frameTableOffset, 0);

    if (pTlv->field_1A_direction == XDirection_short::eRight_1) // TODO: check if this is the correct direction
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->field_8_top_left.field_0_x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->field_C_bottom_right.field_0_x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    field_13A_xOff = pTlv->field_1C_xOff;

    if (field_136_state == TrapDoorState::eOpen_2)
    {
        Open();
    }

    field_148_bounding_rect.x = pTlv->field_8_top_left.field_0_x;
    field_148_bounding_rect.y = pTlv->field_8_top_left.field_2_y;

    field_148_bounding_rect.w = pTlv->field_C_bottom_right.field_0_x;
    field_148_bounding_rect.h = pTlv->field_C_bottom_right.field_2_y;

    mApplyShadows |= 2u;
    field_13C_stay_open_time = pTlv->field_1E_stay_open_time;
}

s32 TrapDoor::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const TrapDoor_State*>(pData);
    auto pTlv = static_cast<Path_TrapDoor*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_8_tlvInfo));

    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eMudomoVault:
        case EReliveLevelIds::eMudancheeVault:
        case EReliveLevelIds::eMudancheeVault_Ender:
        case EReliveLevelIds::eMudomoVault_Ender:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("VLTSTRAP.BAN", nullptr);
            }
            break;

        default:
            if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("TRAPDOOR.BAN", nullptr);
            }
            break;
    }

    auto pTrapDoor = relive_new TrapDoor(pTlv, nullptr, pState->field_8_tlvInfo);
    if (pTrapDoor)
    {
        pTrapDoor->field_130_stay_open_time2 = pState->field_4_open_time;
        pTrapDoor->field_136_state = pState->field_2_state;

        if (pState->field_2_state == TrapDoorState::eClosing_3 || pState->field_2_state == TrapDoorState::eOpening_1)
        {
            pTrapDoor->Open();
        }
    }

    return sizeof(TrapDoor_State);
}

void TrapDoor::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const CameraPos direction = gMap.GetDirection_4811A0(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, field_140_x, field_144_y);

    switch (field_136_state)
    {
        case TrapDoorState::eClosed_0:
            if (SwitchStates_Get(field_134_switch_id) == field_138_switch_state)
            {
                Open();
                field_136_state = TrapDoorState::eOpening_1;
                const AnimRecord& openingRec = AnimRec(sTrapDoorData_547B78[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].field_8_opening);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(openingRec.mFrameTableOffset, 0);

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialTrigger_80, 45, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise1_76, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                SFX_Play_Camera(SoundEffect::TrapdoorOpen_43, 70, direction);
                field_136_state = TrapDoorState::eOpen_2;
                field_130_stay_open_time2 = field_13C_stay_open_time;
            }
            break;

        case TrapDoorState::eOpen_2:
            field_130_stay_open_time2--;

            if ((field_13E_self_closing == Choice_short::eYes_1 && field_130_stay_open_time2 + 1 <= 0) || SwitchStates_Get(field_134_switch_id) != field_138_switch_state)
            {
                const AnimRecord& closingRec = AnimRec(sTrapDoorData_547B78[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].field_C_closing);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(closingRec.mFrameTableOffset, 0);

                field_136_state = TrapDoorState::eClosing_3;

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialNoise3_78, 60, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise2_77, 90, direction);
                }
            }
            break;

        case TrapDoorState::eClosing_3:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                SFX_Play_Camera(SoundEffect::TrapdoorClose_42, 70, direction);
                Add_To_Collisions_Array();
                field_136_state = TrapDoorState::eClosed_0;
                SwitchStates_Set(field_134_switch_id, field_138_switch_state == 0);
            }
            break;

        default:
            return;
    }
}

void TrapDoor::VRender(PrimHeader** ppOt)
{
    mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(field_13A_xOff);
    BaseAliveGameObject::VRender(ppOt);
    mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(field_13A_xOff);
}

void TrapDoor::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        if (field_13E_self_closing == Choice_short::eYes_1)
        {
            SwitchStates_Set(field_134_switch_id, field_138_switch_state == 0);
        }
    }
}

s32 TrapDoor::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<TrapDoor_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eTrapDoor_142;
    pState->field_4_open_time = field_130_stay_open_time2;
    pState->field_2_state = field_136_state;
    pState->field_8_tlvInfo = mPlatformBaseTlvInfo;
    return sizeof(TrapDoor_State);
}

PSX_RECT* TrapDoor::VGetBoundingRect(PSX_RECT* pRect, s32 /*not_used*/)
{
    *pRect = field_148_bounding_rect;
    return pRect;
}

void TrapDoor::VAdd(BaseAliveGameObject* pObj)
{
    PlatformBase::VAdd(pObj);
}

void TrapDoor::VRemove(BaseAliveGameObject* pObj)
{
    PlatformBase::VRemove(pObj);
}

void TrapDoor::Add_To_Collisions_Array()
{
    field_124_pCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        field_148_bounding_rect.x,
        field_148_bounding_rect.y,
        field_148_bounding_rect.w,
        field_148_bounding_rect.y,
        32);

    if (mBaseAnimatedWithPhysicsGameObject_SpriteScale != FP_FromInteger(1))
    {
        field_124_pCollisionLine->field_8_type = eLineTypes::eUnknown_36;
    }

    ObjList_5C1B78->Push_Back(this);
}

void TrapDoor::Open()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find alive objects.
        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // That are on this trap door.
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (sObjectIds.Find_Impl(pAliveObj->BaseAliveGameObjectId) == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened.
                if (field_124_pCollisionLine == pAliveObj->BaseAliveGameObjectCollisionLine)
                {
                    pAliveObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }
        }
    }

    Rect_Clear(&field_124_pCollisionLine->field_0_rect);
    field_124_pCollisionLine = nullptr;
    ObjList_5C1B78->Remove_Item(this);
}

TrapDoor::~TrapDoor()
{
    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);
}
