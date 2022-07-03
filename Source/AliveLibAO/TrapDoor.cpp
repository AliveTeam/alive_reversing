#include "stdafx_ao.h"
#include "Function.hpp"
#include "TrapDoor.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Collisions.hpp"

namespace AO {

struct TrapDoor_Data final
{
    AnimId field_0_open;
    AnimId field_4_closed;
    AnimId field_8_opening;
    AnimId field_C_closing;
    //s16 field_10_maxW;
    //s16 field_12_maxH;
};
//ALIVE_ASSERT_SIZEOF(TrapDoor_Data, 0x14);


const TrapDoor_Data sTrapDoorData_4BD4A0[16] = {
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // menu
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // rapture farms
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // lines
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest temple
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // stock yards
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // stock yards return
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // removed
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // desert
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // desert temple
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // credits
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // removed
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // board room
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // rupture farms return
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest chase
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}}; // desert escape


void TrapDoor::VRender(PrimHeader** ppOt)
{
    mBaseAnimatedWithPhysicsGameObject_XPos += FP_FromInteger(field_13A_xOff);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    mBaseAnimatedWithPhysicsGameObject_XPos -= FP_FromInteger(field_13A_xOff);
}

void TrapDoor::VRemove(BaseAliveGameObject* pObj)
{
    PlatformBase::VRemove(pObj);
}

void TrapDoor::VAdd(BaseAliveGameObject* pObj)
{
    PlatformBase::VAdd(pObj);
}

void TrapDoor::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        if (field_13C_self_closing == Choice_short::eYes_1)
        {
            SwitchStates_Set(field_134_switch_id, field_138_switch_state == 0);
        }
    }
}

PSX_RECT TrapDoor::VGetBoundingRect(s32 /*pointIdx*/)
{
    return field_148_bounding_rect;
}

TrapDoor::~TrapDoor()
{
    gMap.TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);
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

        // Find alive objects..
        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // That are on this trap door
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (pAliveObj->mLiftPoint == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened
                if (mPlatformBaseCollisionLine == pAliveObj->BaseAliveGameObjectCollisionLine)
                {
                    pAliveObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }
        }
    }

    Rect_Clear(&mPlatformBaseCollisionLine->mRect);
    mPlatformBaseCollisionLine = nullptr;
    ObjListPlatforms_50766C->Remove_Item(this);
}

TrapDoor::TrapDoor(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo)
{
    field_12C_flag &= ~1u;

    mBaseGameObjectTypeId = ReliveTypes::eTrapDoor;
    field_134_switch_id = pTlv->field_18_switch_id;
    field_138_switch_state = pTlv->field_1A_start_state;

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    AnimId animId = AnimId::None;
    if (field_138_switch_state == SwitchStates_Get(pTlv->field_18_switch_id))
    {
        field_136_state = TrapDoorState::eOpen_2;
        animId = sTrapDoorData_4BD4A0[cur_lvl].field_0_open;
    }
    else
    {
        field_136_state = TrapDoorState::eClosed_0;
        animId = sTrapDoorData_4BD4A0[cur_lvl].field_4_closed;
    }

    field_13C_self_closing = pTlv->field_1C_self_closing;
    if (pTlv->field_1E_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, 1, 0);

    AddDynamicCollision(
        sTrapDoorData_4BD4A0[cur_lvl].field_4_closed,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    field_144_y = FP_FromInteger(pTlv->mTopLeft.y);
    field_140_x = FP_FromInteger(pTlv->mTopLeft.x);

    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animId, nullptr);
    if (pTlv->field_22_direction == XDirection_short::eRight_1)
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mBaseAnimatedWithPhysicsGameObject_XPos);
    field_13A_xOff = pTlv->field_24_xOff;

    if (field_136_state == TrapDoorState::eOpen_2)
    {
        // Inlined
        Open();
    }

    field_148_bounding_rect.x = pTlv->mTopLeft.x;
    field_148_bounding_rect.y = pTlv->mTopLeft.y;

    field_148_bounding_rect.w = pTlv->mBottomRight.x;
    field_148_bounding_rect.h = pTlv->mBottomRight.y;

    field_10C = 4;
}

void TrapDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const CameraPos direction = gMap.GetDirection(
        mBaseAnimatedWithPhysicsGameObject_LvlNumber,
        mBaseAnimatedWithPhysicsGameObject_PathNumber,
        field_140_x,
        field_144_y);

    switch (field_136_state)
    {
        case TrapDoorState::eClosed_0:
            if (SwitchStates_Get(field_134_switch_id) == field_138_switch_state)
            {
                Open();
                field_136_state = TrapDoorState::eOpening_1;

                const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
                    sTrapDoorData_4BD4A0[cur_lvl].field_8_opening,
                    nullptr);

                SFX_Play_Camera(SoundEffect::Trapdoor_49, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialTrigger_97, 45, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise1_93, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_136_state = TrapDoorState::eOpen_2;
                field_130_stay_open_time = 20;
            }
            break;

        case TrapDoorState::eOpen_2:
            field_130_stay_open_time--;
            if ((field_13C_self_closing == Choice_short::eYes_1 && !field_130_stay_open_time) || SwitchStates_Get(field_134_switch_id) != SwitchStates_Get(field_138_switch_state))
            {
                const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
                    sTrapDoorData_4BD4A0[cur_lvl].field_C_closing,
                    nullptr);
                field_136_state = TrapDoorState::eClosing_3;

                SFX_Play_Camera(SoundEffect::Trapdoor_49, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialNoise3_95, 60, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise2_94, 90, direction);
                }
            }
            break;

        case TrapDoorState::eClosing_3:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mPlatformBaseCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
                    field_148_bounding_rect.x,
                    field_148_bounding_rect.y,
                    field_148_bounding_rect.w,
                    field_148_bounding_rect.y,
                    eLineTypes::eDynamicCollision_32
                );
                ObjListPlatforms_50766C->Push_Back(this);
                field_136_state = TrapDoorState::eClosed_0;
                SwitchStates_Set(field_134_switch_id, field_138_switch_state == 0);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
