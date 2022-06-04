#include "stdafx_ao.h"
#include "Function.hpp"
#include "Mine.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "BaseBomb.hpp"
#include "ScreenManager.hpp"

namespace AO {

ALIVE_VAR(1, 0x507B88, Mine*, sMinePlayingSound_507B88, nullptr);

Mine::Mine(Path_Mine* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    mBaseGameObjectTypeId = ReliveTypes::eMine;
    
    ///////////////////////////////////////////////////////////////////////////
    const AnimRecord& rec = AO::AnimRec(AnimId::Mine);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    ///////////////////////////////////////////////////////////////////////////
    
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

    field_10C_detonating = 0;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.mRenderLayer = Layer::eLayer_BombRollingBall_Half_16;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_10_anim.mRenderLayer = Layer::eLayer_BombRollingBall_35;
        field_C6_scale = 1;
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24);
    field_110_tlv = tlvInfo;
    field_114_gnframe = sGnFrame;

    const AnimRecord& flashRec = AO::AnimRec(AnimId::Mine_Flash);
    u8** ppFLashRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, flashRec.mResourceId, 1, 0);
    field_118_animation.Init(
        flashRec.mFrameTableOffset,
        gObjList_animations_505564,
        this,
        flashRec.mMaxW,
        flashRec.mMaxH,
        ppFLashRes,
        1,
        0,
        0);

    field_118_animation.mRenderLayer = field_10_anim.mRenderLayer;
    field_118_animation.mAnimFlags.Set(AnimFlags::eBit16_bBlending);
    field_118_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
    field_118_animation.field_14_scale = field_BC_sprite_scale;

    field_118_animation.mRed = 128;
    field_118_animation.mGreen = 128;
    field_118_animation.mBlue = 128;

    field_118_animation.mRenderMode = TPageAbr::eBlend_0;
    field_10E_disabled_resources = pTlv->field_1E_disabled_resources;

    // TODO
    field_1B0_flags = 2 * (pTlv->field_20_persists_offscreen == Choice_short::eYes_1) | (field_1B0_flags & ~2);

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID, 1, 0);

    if (!(pTlv->field_1E_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }

    if (!(pTlv->field_1E_disabled_resources & 4))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 1, 0);
    }

    if (!(pTlv->field_1E_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    if (gMap.mCurrentLevel == LevelIds::eStockYards_5 || gMap.mCurrentLevel == LevelIds::eStockYardsReturn_6)
    {
        field_C4_b = 50;
        field_C2_g = 50;
        field_C0_r = 50;
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 1, 0);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 1, 0);
    }

    mBaseAliveGameObjectCollectionRect.x = field_A8_xpos - (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
    mBaseAliveGameObjectCollectionRect.y = field_AC_ypos - ScaleToGridSize(field_BC_sprite_scale);
    mBaseAliveGameObjectCollectionRect.w = field_A8_xpos + (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
    mBaseAliveGameObjectCollectionRect.h = field_AC_ypos;

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
}

Mine::~Mine()
{
    if (field_10C_detonating == 1)
    {
        gMap.TLV_Reset(field_110_tlv, -1, 0, 1);
    }
    else
    {
        gMap.TLV_Reset(field_110_tlv, -1, 0, 0);
    }

    field_118_animation.VCleanUp();

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbebombAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kDebrisID00AOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kBgexpldAOResID, 0, 0));

    if (!(field_10E_disabled_resources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0));
    }

    if (!(field_10E_disabled_resources & 4))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmblowAOResID_217, 0, 0));
    }

    if (!(field_10E_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kSlogBlowAOResID, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0));

    mBaseGameObjectFlags.Clear(Options::eInteractive_Bit8);

    if (sMinePlayingSound_507B88 == this)
    {
        sMinePlayingSound_507B88 = nullptr;
    }
}

void Mine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath || !(field_1B0_flags & 2))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 Mine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->mBaseGameObjectTypeId)
    {
        case AOTypes::eAbe_43:
        case AOTypes::eAbilityRing_69:
        case AOTypes::eExplosion_74:
        case AOTypes::eShrykull_85:
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            ao_new<BaseBomb>(
                field_A8_xpos,
                field_AC_ypos,
                0,
                field_BC_sprite_scale);
            field_10C_detonating = 1;
            field_114_gnframe = sGnFrame;
            return 1;
        }

        default:
            return 0;
    }
}

void Mine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    ao_new<BaseBomb>(
        field_A8_xpos,
        field_AC_ypos,
        0,
        field_BC_sprite_scale);
    field_10C_detonating = 1;
}

void Mine::VOnPickUpOrSlapped()
{
    if (field_10C_detonating != 1)
    {
        field_10C_detonating = 1;
        field_114_gnframe = sGnFrame + 5;
    }
}

void Mine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        field_118_animation.VRender(
            FP_GetExponent(field_A8_xpos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x)),
            FP_GetExponent(field_AC_ypos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + field_C8_yOffset)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void Mine::VUpdate()
{
    const s16 bInCamera = gMap.Is_Point_In_Current_Camera_4449C0(
        field_B2_lvl_number,
        field_B0_path_number,
        field_A8_xpos,
        field_AC_ypos,
        0);

    if (field_10C_detonating)
    {
        if (field_10C_detonating == 1 && static_cast<s32>(sGnFrame) >= field_114_gnframe)
        {
            ao_new<BaseBomb>(
                field_A8_xpos,
                field_AC_ypos,
                0,
                field_BC_sprite_scale);
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        if (field_10_anim.field_92_current_frame == 1)
        {
            if (sMinePlayingSound_507B88 == nullptr || sMinePlayingSound_507B88 == this)
            {
                if (bInCamera)
                {
                    SFX_Play_Mono(SoundEffect::RedTick_4, 35, 0);
                }
                sMinePlayingSound_507B88 = this;
            }
        }

        if (IsColliding())
        {
            field_10C_detonating = 1;
            field_114_gnframe = sGnFrame;
        }
    }

    if (field_10C_detonating != 1
        && (Event_Get(kEventDeathReset_4)
            || Event_Get(kEvent_9)
            || field_B2_lvl_number != gMap.mCurrentLevel
            || field_B0_path_number != gMap.mCurrentPath))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 Mine::IsColliding()
{
    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
        {
            if (pObj->field_10_anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                PSX_RECT bObjRect = {};
                pObj->VGetBoundingRect(&bObjRect, 1);

                if (FP_GetExponent(pObj->field_A8_xpos) > bRect.x && FP_GetExponent(pObj->field_A8_xpos) < bRect.w && FP_GetExponent(pObj->field_AC_ypos) < bRect.h + 5 && bRect.x <= bObjRect.w && bRect.w >= bObjRect.x && bRect.h >= bObjRect.y && bRect.y <= bObjRect.h && pObj->field_BC_sprite_scale == field_BC_sprite_scale)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

} // namespace AO
