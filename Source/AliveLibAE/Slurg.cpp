#include "stdafx.h"
#include "Slurg.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0x5C1C08, u16, sSlurg_Step_Watch_Points_Idx_5C1C08, 0);
ALIVE_ARY(1, 0x5BD4DC, s8, 2, sSlurg_Step_Watch_Points_Count_5BD4DC, {});
ALIVE_ARY(1, 0x5C1B28, Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28, {});

void Slurg::Clear_Slurg_Step_Watch_Points()
{
    sSlurg_Step_Watch_Points_Idx_5C1C08 = !sSlurg_Step_Watch_Points_Idx_5C1C08;
    sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;

    for (auto& point : sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08].field_0_points)
    {
        point = {};
    }
}

TintEntry sSlurgTints_560BCC[18] = {
    {EReliveLevelIds::eMines, 102u, 127u, 118u},
    {EReliveLevelIds::eNecrum, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eNone, 102u, 127u, 118u}};

Slurg::Slurg(Path_Slurg* pTlv, u32 tlvInfo)
    : BaseAliveGameObject(0)
{
    mSlurgTlv = pTlv;

    mSlurgState = SlurgStates::eMoving_0;

    const AnimRecord& rec = AnimRec(AnimId::Slurg_Move);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Slurg_Move, ppRes);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    SetType(ReliveTypes::eSlurg);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);

    mTlvInfo = tlvInfo;
    if (pTlv->mSlurgData.field_4_scale == Scale_short::eHalf_1)
    {
        mSlurgSpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else if (pTlv->mSlurgData.field_4_scale == Scale_short::eFull_0)
    {
        mSlurgSpriteScale = FP_FromInteger(1);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    mMovingTimer = pTlv->mSlurgData.mMovingTimer;
    mRngForMovingTimer = pTlv->mSlurgData.mMovingTimer;

    SetTint(&sSlurgTints_560BCC[0], gMap.mCurrentLevel);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &mSlurgLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    mSlurgSwitchId = pTlv->mSlurgData.field_6_switch_id;

    mSlurgFlags.Clear();

    if (pTlv->mSlurgData.field_2_start_direction == XDirection_short::eRight_1)
    {
        mSlurgFlags.Set(SlurgFlags::eGoingRight);
    }

    VStackOnObjectsOfType(ReliveTypes::eSlurg);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mShadow = relive_new Shadow();
}

s32 Slurg::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const Slurg_State*>(pData);
    auto pTlv = static_cast<Path_Slurg*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLURG.BAN", nullptr);
    }

    auto pSlurg = relive_new Slurg(pTlv, pState->mTlvInfo);

    pSlurg->mBaseAnimatedWithPhysicsGameObject_XPos = pState->mXPos;
    pSlurg->mBaseAnimatedWithPhysicsGameObject_YPos = pState->mYPos;
    pSlurg->mBaseAnimatedWithPhysicsGameObject_VelX = pState->mVelX;
    pSlurg->mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter = pState->mFrameChangeCounter;

    // OG BUG: This wasn't restored
    pSlurg->mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame = pState->mAnimCurrentFrame;
    pSlurg->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->mFlipX & 1);
    pSlurg->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->mRender & 1);

    pSlurg->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->mDrawable & 1);

    if (IsLastFrame(&pSlurg->mBaseAnimatedWithPhysicsGameObject_Anim))
    {
        pSlurg->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pSlurg->mSlurgState = pState->mSlurgState;

    pSlurg->mSlurgFlags.Set(SlurgFlags::eGoingRight, pState->mSlurgFlags.Get(SlurgFlags::eGoingRight));
    pSlurg->mSlurgFlags.Set(SlurgFlags::eMoving, pState->mSlurgFlags.Get(SlurgFlags::eMoving));
    return sizeof(Slurg_State);
}

Slurg::~Slurg()
{
    if (mTlvInfo == -1)
    {
        Path::TLV_Reset(0xFFFFFFFF, -1, 0, mSlurgState == SlurgStates::eBurst_2);
    }
}

void Slurg::Burst()
{
    mSlurgState = SlurgStates::eBurst_2;
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Slurg_Burst, nullptr);

    relive_new Blood(mBaseAnimatedWithPhysicsGameObject_XPos,
                                mBaseAnimatedWithPhysicsGameObject_YPos,
                                FP_FromInteger(0),
                                FP_FromInteger(5),
                                mSlurgSpriteScale,
                                20);

    EventBroadcast(kEventLoudNoise, this);
    SfxPlayMono(SoundEffect::SlurgKill_89, 127, mSlurgSpriteScale);

    if (mSlurgSwitchId > 1)
    {
        SwitchStates_Add(mSlurgSwitchId, 1);
    }
}

void Slurg::VUpdate()
{
    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mMovingTimer == 0)
    {
        mMovingTimer = Math_RandomRange(mRngForMovingTimer, mRngForMovingTimer + 20);
        mSlurgState = SlurgStates::ePausing_1;
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Slurg_Turn_Around, nullptr);
    }

    const PSX_RECT bRect = VGetBoundingRect();

    if (mSlurgState != SlurgStates::eBurst_2)
    {
        const s32 idx = sSlurg_Step_Watch_Points_Idx_5C1C08 == 0;
        const s32 max_count = sSlurg_Step_Watch_Points_Count_5BD4DC[idx];
        for (s32 i = 0; i < max_count; i++)
        {
            const Slurg_Step_Watch_Point* pPoint = &sSlurg_Step_Watch_Points_5C1B28[idx].field_0_points[i];
            if (pPoint->field_0_xPos > bRect.x - 2 && pPoint->field_0_xPos < bRect.w + 2 && pPoint->field_2_yPos > bRect.y - 4 && pPoint->field_2_yPos < bRect.h + 4)
            {
                Burst();
                break;
            }
        }
    }

    switch (mSlurgState)
    {
        case SlurgStates::eMoving_0:
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(1);
            mMovingTimer--;
            if (mSlurgFlags.Get(SlurgFlags::eGoingRight))
            {
                mBaseAnimatedWithPhysicsGameObject_VelX = -FP_FromInteger(1);
            }

            mSlurgFlags.Toggle(SlurgFlags::eMoving);

            if (mSlurgFlags.Get(SlurgFlags::eMoving))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            }
            break;

        case SlurgStates::ePausing_1:
            mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(0);
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame == 0
                && gMap.Is_Point_In_Current_Camera(mBaseAnimatedWithPhysicsGameObject_LvlNumber, mBaseAnimatedWithPhysicsGameObject_PathNumber, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                SfxPlayMono(SoundEffect::SlurgPause_90, 0);
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mSlurgState = SlurgStates::eMoving_0;
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Slurg_Move, nullptr);
            }
            break;

        case SlurgStates::eBurst_2:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            break;
    }

    if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        mSlurgTlv = sPath_dword_BB47C0->TlvGetAt(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);

        VOnTlvCollision(mSlurgTlv);
    }
}

s16 Slurg::VTakeDamage(BaseGameObject* pFrom)
{
    // Slurgs are tough little dudes, only Paramites can smack 'em up.
    if (pFrom->Type() == ReliveTypes::eParamite)
    {
        Burst();
        return 1;
    }

    return 0;
}

void Slurg::VOnTlvCollision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType32 == TlvTypes::ScrabLeftBound_43)
        {
            if (mSlurgFlags.Get(SlurgFlags::eGoingRight))
            {
                GoLeft();
            }
        }
        else if (pTlv->mTlvType32 == TlvTypes::ScrabRightBound_44)
        {
            if (!mSlurgFlags.Get(SlurgFlags::eGoingRight))
            {
                GoRight();
            }
        }
        pTlv = sPath_dword_BB47C0->TlvGetAt(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    }

    if (mSlurgFlags.Get(SlurgFlags::eGoingRight))
    {
        if (WallHit(mSlurgSpriteScale * FP_FromInteger(8), -(mSlurgSpriteScale * FP_FromInteger(6))) || Check_IsOnEndOfLine(1, 1))
        {
            GoLeft();
        }
    }
    else
    {
        if (WallHit(mSlurgSpriteScale * FP_FromInteger(8), mSlurgSpriteScale * FP_FromInteger(6)) || Check_IsOnEndOfLine(0, 1))
        {
            GoRight();
        }
    }
}

s32 Slurg::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Slurg_State*>(pSaveBuffer);

    pState->mType = AETypes::eSlurg_129;
    pState->mXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->mYPos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->mVelX = mBaseAnimatedWithPhysicsGameObject_VelX;
    pState->mSlurgSpriteScale = mSlurgSpriteScale;
    pState->mFlipX = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->mCurrentMotion = mCurrentMotion;
    pState->mAnimCurrentFrame = mBaseAnimatedWithPhysicsGameObject_Anim.mCurrentFrame;
    pState->mFrameChangeCounter = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter;
    pState->mDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->mRender = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->mFrameTableOffset = mBaseAnimatedWithPhysicsGameObject_Anim.mFrameTableOffset;
    pState->mTlvInfo = mTlvInfo;
    pState->mSlurgState = mSlurgState;
    pState->mSlurgFlags.Set(SlurgFlags::eGoingRight, mSlurgFlags.Get(SlurgFlags::eGoingRight));
    pState->mSlurgFlags.Set(SlurgFlags::eMoving, mSlurgFlags.Get(SlurgFlags::eMoving));
    return sizeof(Slurg_State);
}

void Slurg::GoLeft()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    mSlurgFlags.Clear(SlurgFlags::eGoingRight);

    mSlurgState = SlurgStates::ePausing_1;
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Slurg_Turn_Around, nullptr);
}

void Slurg::GoRight()
{
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    mSlurgFlags.Set(SlurgFlags::eGoingRight);

    mSlurgState = SlurgStates::ePausing_1;
    mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(AnimId::Slurg_Turn_Around, nullptr);
}
