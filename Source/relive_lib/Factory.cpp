#include "BinaryPath.hpp"
#include "MapWrapper.hpp"
#include "stdafx.h"
#include "Factory.hpp"

#include "GameType.hpp"

#include "../AliveLibAE/stdlib.hpp"

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/Mine.hpp"
#include "../relive_lib/GameObjects/TimedMine.hpp"
#include "../relive_lib/GameObjects/BackgroundAnimation.hpp"
#include "../relive_lib/GameObjects/Alarm.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/GameObjects/RollingBallStopper.hpp"
#include "../relive_lib/GameObjects/ZzzSpawner.hpp"
#include "../relive_lib/GameObjects/BoomMachine.hpp"
#include "../relive_lib/GameObjects/ShadowZone.hpp"
#include "../relive_lib/GameObjects/TrapDoor.hpp"
#include "../relive_lib/GameObjects/TimerTrigger.hpp"
#include "../relive_lib/GameObjects/ElectricWall.hpp"
#include "../relive_lib/GameObjects/GasEmitter.hpp"
#include "../relive_lib/GameObjects/Dove.hpp"
#include "../relive_lib/GameObjects/GasCountDown.hpp"
#include "../relive_lib/GameObjects/Door.hpp"

#include "../AliveLibAO/Game.hpp"
#include "../AliveLibAO/Abe.hpp"
#include "../AliveLibAO/Elum.hpp"
#include "../AliveLibAO/PauseMenu.hpp"
#include "../AliveLibAO/ZBall.hpp"
#include "../AliveLibAO/SligSpawner.hpp"
#include "../AliveLibAO/Slig.hpp"
#include "../AliveLibAO/Bat.hpp"
#include "../AliveLibAO/BellHammer.hpp"
#include "../AliveLibAO/LiftMover.hpp"
#include "../AliveLibAO/DoorLight.hpp"
#include "../AliveLibAO/LightEffect.hpp"
#include "../AliveLibAO/ChimeLock.hpp"
#include "../AliveLibAO/CreditsController.hpp"
#include "../AliveLibAO/Meat.hpp"
#include "../AliveLibAO/Math.hpp"
#include "../AliveLibAO/MusicTrigger.hpp"
#include "../AliveLibAO/SecurityOrb.hpp"
#include "../AliveLibAO/SecurityClaw.hpp"
#include "../AliveLibAO/BeeSwarmHole.hpp"
#include "../AliveLibAO/BeeNest.hpp"
#include "../AliveLibAO/Honey.hpp"
#include "../AliveLibAO/HoneySack.hpp"
#include "../AliveLibAO/PullRingRope.hpp"
#include "../AliveLibAO/InvisibleSwitch.hpp"
#include "../AliveLibAO/FallingItem.hpp"
#include "../AliveLibAO/FootSwitch.hpp"
#include "../AliveLibAO/HoistRocksEffect.hpp"
#include "../AliveLibAO/RollingBall.hpp"
#include "../AliveLibAO/Lever.hpp"
#include "../AliveLibAO/SecurityDoor.hpp"
#include "../AliveLibAO/BackgroundGlukkon.hpp"
#include "../AliveLibAO/Well.hpp"
#include "../AliveLibAO/BirdPortal.hpp"
#include "../AliveLibAO/SlogSpawner.hpp"
#include "../AliveLibAO/DoorFlame.hpp"
#include "../AliveLibAO/MotionDetector.hpp"
#include "../AliveLibAO/LCDStatusBoard.hpp"
#include "../AliveLibAO/FlintLockFire.hpp"
#include "../AliveLibAO/MeatSaw.hpp"
#include "../AliveLibAO/Mudokon.hpp"
#include "../AliveLibAO/HintFly.hpp"
#include "../AliveLibAO/SlingMudokon.hpp"
#include "../AliveLibAO/MainMenu.hpp"
#include "../AliveLibAO/DDCheat.hpp"
#include "../AliveLibAO/Slog.hpp"
#include "../AliveLibAO/LCDScreen.hpp"
#include "../AliveLibAO/UXB.hpp"
#include "../AliveLibAO/Scrab.hpp"
#include "../AliveLibAO/Paramite.hpp"
#include "../AliveLibAO/MovingBomb.hpp"
#include "../AliveLibAO/LiftPoint.hpp"
#include "../AliveLibAO/RockSack.hpp"
#include "../AliveLibAO/MeatSack.hpp"
#include "../AliveLibAO/Path.hpp"

#include "../AliveLibAE/MainMenu.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAE/LCDScreen.hpp"
#include "../AliveLibAE/UXB.hpp"
#include "../AliveLibAE/LCDStatusBoard.hpp"
#include "../AliveLibAE/HoistRocksEffect.hpp"
#include "../AliveLibAE/WorkWheel.hpp"
#include "../AliveLibAE/StatusLight.hpp"
#include "../AliveLibAE/WheelSyncer.hpp"
#include "../AliveLibAE/LevelLoader.hpp"
#include "../AliveLibAE/InvisibleSwitch.hpp"
#include "../AliveLibAE/Lever.hpp"
#include "../AliveLibAE/MultiSwitchController.hpp"
#include "../AliveLibAE/Drill.hpp"
#include "../AliveLibAE/Teleporter.hpp"
#include "../AliveLibAE/Well.hpp"
#include "../AliveLibAE/Water.hpp"
#include "../AliveLibAE/SlamDoor.hpp"
#include "../AliveLibAE/MusicTrigger.hpp"
#include "../AliveLibAE/PauseMenu.hpp"
#include "../AliveLibAE/Abe.hpp"
#include "../AliveLibAE/LiftPoint.hpp"
#include "../AliveLibAE/PullRingRope.hpp"
#include "../AliveLibAE/LiftMover.hpp"
#include "../AliveLibAE/CreditsController.hpp"
#include "../AliveLibAE/FootSwitch.hpp"
#include "../AliveLibAE/BrewMachine.hpp"
#include "../AliveLibAE/FallingItem.hpp"
#include "../AliveLibAE/MineCar.hpp"
#include "../AliveLibAE/DemoPlayback.hpp"
#include "../AliveLibAE/SecurityOrb.hpp"
#include "../AliveLibAE/Greeter.hpp"
#include "../AliveLibAE/MotionDetector.hpp"
#include "../AliveLibAE/FlyingSlig.hpp"
#include "../AliveLibAE/FlyingSligSpawner.hpp"
#include "../AliveLibAE/Mudokon.hpp"
#include "../AliveLibAE/BirdPortal.hpp"
#include "../AliveLibAE/TorturedMudokon.hpp"
#include "../AliveLibAE/DoorBlocker.hpp"
#include "../AliveLibAE/SlapLock.hpp"
#include "../AliveLibAE/Glukkon.hpp"
#include "../AliveLibAE/MovingBomb.hpp"
#include "../AliveLibAE/Slurg.hpp"
#include "../AliveLibAE/DoorFlame.hpp"
#include "../AliveLibAE/CrawlingSlig.hpp"
#include "../AliveLibAE/Scrab.hpp"
#include "../AliveLibAE/Paramite.hpp"
#include "../AliveLibAE/Fleech.hpp"
#include "../AliveLibAE/Slog.hpp"
#include "../AliveLibAE/Slig.hpp"
#include "../AliveLibAE/GlukkonSwitch.hpp"
#include "../AliveLibAE/CrawlingSligButton.hpp"
#include "../AliveLibAE/SligGetPantsAndWings.hpp"
#include "../AliveLibAE/Meat.hpp"
#include "../AliveLibAE/Bone.hpp"
#include "../AliveLibAE/RockSack.hpp"
#include "../AliveLibAE/SligSpawner.hpp"
#include "../AliveLibAE/ScrabSpawner.hpp"
#include "../AliveLibAE/SlogSpawner.hpp"
#include "../AliveLibAE/ParamiteWebLine.hpp"
#include "../AliveLibAE/ExplosionSet.hpp"
#include "../AliveLibAE/ColourfulMeter.hpp"
#include "../AliveLibAE/SecurityDoor.hpp"
#include "../AliveLibAE/LaughingGas.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../AliveLibAE/MeatSack.hpp"
#include "../AliveLibAE/BoneBag.hpp"
#include "../AliveLibAE/RollingBall.hpp"

namespace AO {

static constexpr AnimId kAbeGibs[3] = {
    AnimId::Abe_Head_Gib,
    AnimId::Abe_Arm_Gib,
    AnimId::Abe_Body_Gib};

static constexpr AnimId kSlogGibs[2] = {
    AnimId::Slog_Head_Gib,
    AnimId::Slog_Body_Gib};

static constexpr AnimId kElumGibs[3] = {
    AnimId::Elum_Head_Gib,
    AnimId::Elum_Arm_Gib,
    AnimId::Elum_Body_Gib};

static constexpr AnimId kSligGibs[3] = {
    AnimId::Slig_Head_Gib,
    AnimId::Slig_Arm_Gib,
    AnimId::Slig_Body_Gib};

static void Factory_Hoist(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_HoistRock1);
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_HoistRock2);
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_HoistRock3);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::AO_HoistRock1);
                ResourceManagerWrapper::PendAnimation(AnimId::AO_HoistRock2);
                ResourceManagerWrapper::PendAnimation(AnimId::AO_HoistRock3);
                break;
        }
    }
    else
    {
        auto pHoistTlv = static_cast<relive::Path_Hoist*>(pTlv);
        if (pHoistTlv->mHoistType == relive::Path_Hoist::Type::eOffScreen)
        {
            relive_new HoistRocksEffect(pHoistTlv, tlvId);
            // OG issue, no reset on failure ??
        }
        else
        {
            Path::TLV_Reset(tlvId);
        }
    }
}

static void Factory_Edge(relive::Path_TLV* /*pTlv*/, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        // Empty
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_ShadowZone(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new ShadowZone(static_cast<relive::Path_ShadowZone*>(pTlv), tlvId);
    }
}

static void Factory_LiftPoint(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_R1);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_RuptureFarms);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_RuptureFarms);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_RuptureFarms);
                break;

            case EReliveLevelIds::eLines:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_Lines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Lines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Lines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Lines);
                break;

            case EReliveLevelIds::eDesert:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_R1);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Desert);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Desert);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Desert);
                break;

            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_R1);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Desert2);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Desert2);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Desert2);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_Lines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Forest);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Forest);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Forest);
                break;
        }
    }
    else
    {
        for (s32 idx = 0; idx < gBaseGameObjects->Size(); idx++)
        {
            BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(idx);
            if (!pObjIter)
            {
                break;
            }

            if (!pObjIter->GetDead() && pObjIter->Type() == ReliveTypes::eLiftPoint)
            {
                auto pLiftObj = static_cast<LiftPoint*>(pObjIter);

                const s16 xpos_i = FP_GetExponent(pLiftObj->mXPos);
                if (pTlv->mTopLeftX <= xpos_i
                    && xpos_i <= pTlv->mBottomRightX
                    && pLiftObj->mCurrentLevel == gMap.mCurrentLevel
                    && pLiftObj->mCurrentPath == gMap.mCurrentPath)
                {
                    Path::TLV_Reset(tlvId);
                    return;
                }
            }
        }

        if (pTlv->mTlvSpecificMeaning & 2 || (pTlv->mTlvSpecificMeaning == 0 && static_cast<relive::Path_LiftPoint*>(pTlv)->mIsStartPoint))
        {
            relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlv), tlvId);
        }
        else
        {
            TlvIterator tlvIterator = TlvIterator::Invalid();
            s16 pointNumber = 1;
            while (pointNumber < 8)
            {
                tlvIterator = gMap.Get_First_TLV_For_Offsetted_Camera(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));
                while (tlvIterator.GetTlv())
                {
                    if (tlvIterator.GetTlv()->mTlvType == ReliveTypes::eLiftPoint)
                    {
                        const auto tlv_x = pTlv->mTopLeftX;
                        const auto absX = tlvIterator.GetTlv()->mTopLeftX - tlv_x >= 0 ? tlvIterator.GetTlv()->mTopLeftX - tlv_x : tlv_x - tlvIterator.GetTlv()->mTopLeftX;
                        if (absX < 5)
                        {
                            if (tlvIterator.GetTlv()->mTlvSpecificMeaning & 2 || (tlvIterator.GetTlv()->mTlvSpecificMeaning == 0 && tlvIterator.GetTlv<relive::Path_LiftPoint>()->mIsStartPoint))
                            {
                                relive_new LiftPoint(tlvIterator.GetTlv<relive::Path_LiftPoint>(), tlvId);
                                return;
                            }
                        }
                    }
                    tlvIterator = tlvIterator.Next_TLV();
                }
                pointNumber++;
            }

            relive_new LiftPoint(static_cast<relive::Path_LiftPoint*>(pTlv), tlvId);
        }
    }
}

static void Factory_Well(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Well_Leaf);
    }
    else
    {
        const FP xpos = FP_FromInteger(pTlv->mTopLeftX);
        const FP ypos = FP_FromInteger(pTlv->mTopLeftY);
        relive_new Well(static_cast<relive::Path_WellBase*>(pTlv), xpos, ypos, tlvId);
    }
}

static void Factory_Dove(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {

        auto pDoveTlv = static_cast<relive::Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->Width();
        const s16 height = pDoveTlv->Height();

        for (s32 i = 0; i < pDoveTlv->mDoveCount; i++)
        {
            auto pDove = relive_new Dove(
                AnimId::Dove_Idle,
                tlvId,
                pDoveTlv->mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1));

            if (pDove)
            {
                s16 ypos = 0;
                if (pDoveTlv->mPixelPerfect)
                {
                    pDove->mXPos = FP_FromInteger(pDoveTlv->mTopLeftX);
                    ypos = pDoveTlv->mTopLeftY;
                }
                else
                {
                    pDove->mXPos = FP_FromInteger(pDoveTlv->mTopLeftX + width * Math_NextRandom() / 256);
                    ypos = pDoveTlv->mTopLeftY + height * Math_NextRandom() / 256;
                }

                pDove->mYPos = FP_FromInteger(ypos) + FP_FromInteger(10);
            }
        }
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Dove_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Dove_Flying);
    }
}

static void Factory_RockSack(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::RockSack_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::RockSack_SoftHit);
        ResourceManagerWrapper::PendAnimation(AnimId::RockSack_HardHit);

        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new RockSack(static_cast<relive::Path_RockSack*>(pTlv), tlvId);
    }
}

static void Factory_ZBall(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Swinging_Ball_Fast);
        ResourceManagerWrapper::PendAnimation(AnimId::Swinging_Ball_Normal);
        ResourceManagerWrapper::PendAnimation(AnimId::Swinging_Ball_Slow);
    }
    else
    {
        relive_new ZBall(static_cast<relive::Path_ZBall*>(pTlv), tlvId);
    }
}

static void Factory_FallingItem(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Stick);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManagerWrapper::PendAnimation(AnimId::FallingMeat_Waiting);
                ResourceManagerWrapper::PendAnimation(AnimId::FallingMeat_Falling);
                ResourceManagerWrapper::PendAnimation(AnimId::Meat_Gib);
                break;

            case EReliveLevelIds::eLines:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eStockYards:
            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManagerWrapper::PendAnimation(AnimId::AO_FallingRock_Falling);
                ResourceManagerWrapper::PendAnimation(AnimId::AO_FallingRock_Waiting);
                ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::AO_FallingRock_Falling);
                ResourceManagerWrapper::PendAnimation(AnimId::AO_FallingRock_Waiting);
                ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
                break;
        }
    }
    else
    {
        relive_new FallingItem(static_cast<relive::Path_FallingItem*>(pTlv), tlvId);
    }
}

static void Factory_PullRingRope(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_R1);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Farms_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Farms_UseBegin);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Farms_UseEnd);
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_R1);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Desert_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Desert_UseBegin);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Desert_UseEnd);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Rope_Lines);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Desert_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Desert_UseBegin);
                ResourceManagerWrapper::PendAnimation(AnimId::Pullring_Desert_UseEnd);
                break;
        }
    }
    else
    {
        relive_new PullRingRope(static_cast<relive::Path_PullRingRope*>(pTlv), tlvId);
    }
}

static void Factory_BackgroundAnimation(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        auto pBgAnim = static_cast<relive::Path_BackgroundAnimation*>(pTlv);
        //ResourceManagerWrapper::PendAnimation(static_cast<AnimId>(pBgAnim->mAnimId));
        relive_new BackgroundAnimation(pBgAnim, tlvId);
    }
}

static void Factory_Honey(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Honey);
        ResourceManagerWrapper::PendAnimation(AnimId::Honey_Drip);
    }
    else
    {

        const auto midPoint = pTlv->Width() / 2;
        auto pHoney = relive_new Honey(
            FP_FromInteger(midPoint + pTlv->mTopLeftX),
            FP_FromInteger(pTlv->mTopLeftY + 24));
        if (pHoney)
        {
            pHoney->mTlvInfo = tlvId;
        }
    }
}

static void Factory_TimedMine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pTimedMineTlv = static_cast<relive::Path_TimedMine*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnims(kElumGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::TimedMine_Activated);
        ResourceManagerWrapper::PendAnimation(AnimId::TimedMine_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_Flash);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_RedGreenTick);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::GroundExplosion);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }

    }
    else
    {
        relive_new TimedMine(pTimedMineTlv, tlvId);
    }
}

static void LoadWalkingSligResources(relive::LoadMode loadMode, BitField16<SligFlags_DisabledRes> disabledResources)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Slig::sSligMotionAnimIds);
        ResourceManagerWrapper::PendAnims(kSligGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::ObjectShadow);
        ResourceManagerWrapper::PendAnimation(AnimId::ShootingZFire_Particle);
        ResourceManagerWrapper::PendAnimation(AnimId::ShootingFire_Particle);
        ResourceManagerWrapper::PendAnimation(AnimId::Bullet_Shell);
        
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        if (!(disabledResources.Get(eDisabledRes_Bit1_SligLever)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit2_SligLift)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit3_SligZ)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit7_SligSleep)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit9_SligEdge)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit10_SligSmash)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit11_SligBeat)))
        {
        }

        if (!(disabledResources.Get(eDisabledRes_Bit8_SligKnfd)))
        {
        }

    }
}

static void Factory_Slig(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->mData.mDisabledResourcesAO);

    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        // Do nothing
    }
    else
    {
        relive_new Slig(pSligTlv, tlvId);
    }
}


static void Factory_Slog(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Slog::sSlogMotionAnimIds);
    }
    else
    {
        relive_new Slog(static_cast<relive::Path_Slog*>(pTlv), tlvId);
    }
}

static void Factory_Lever(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eStockYards:
            case EReliveLevelIds::eStockYardsReturn:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_Lever_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_Lever_Pull_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_Lever_Pull_Release_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_Lever_Pull_Right);
                ResourceManagerWrapper::PendAnimation(AnimId::RuptureFarms_Lever_Pull_Release_Right);
                break;

            case EReliveLevelIds::eLines:
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_Lever_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_Lever_Pull_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_Lever_Pull_Release_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_Lever_Pull_Right);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_Lever_Pull_Release_Right);
                break;

            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Release_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Right);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Release_Right);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Release_Left);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Right);
                ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Release_Right);
                break;
        }
    }
    else
    {
        relive_new Lever(static_cast<relive::Path_Lever*>(pTlv), tlvId);
    }
}

static void Factory_BellHammer(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new BellHammer(static_cast<relive::Path_BellHammer*>(pTlv), tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::BellHammer_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::BellHammer_Smashing);
        ResourceManagerWrapper::PendAnims(Elum::gElumMotionAnimIds);
    }
}

static void Factory_SecurityOrb(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pSecurityOrbTlv = static_cast<relive::Path_SecurityOrb*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Orb);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Blue);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Red);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnims(kElumGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
    }
    else
    {
        relive_new SecurityOrb(pSecurityOrbTlv, tlvId);
    }
}

static void Factory_LiftMud(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Mudokon::sMudMotionAnimIds);
    }
    else
    {
        relive_new Mudokon(pTlv, tlvId);
    }
}

static void Factory_BeeSwarmHole(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Bee_Swarm);
        if (gMap.mCurrentLevel == EReliveLevelIds::eForest || gMap.mCurrentLevel == EReliveLevelIds::eDesert)
        {
        }
    }
    else
    {
        relive_new BeeSwarmHole(static_cast<relive::Path_BeeSwarmHole*>(pTlv), tlvId);
    }
}

static void Factory_HoneySack(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Honey);
        ResourceManagerWrapper::PendAnimation(AnimId::Honey_Drip);
        ResourceManagerWrapper::PendAnimation(AnimId::HoneySack_Hanging);
        ResourceManagerWrapper::PendAnimation(AnimId::HoneySack_OnGround);
        ResourceManagerWrapper::PendAnimation(AnimId::HoneySack_Falling);
        ResourceManagerWrapper::PendAnimation(AnimId::HoneySack_FallingToSmashed);
    }
    else
    {
        relive_new HoneySack(static_cast<relive::Path_HoneySack*>(pTlv), tlvId);
    }
}

static void Factory_AbeStart(relive::Path_TLV* pTlv, const Guid& /*tlvId*/, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2 && !gAttract)
    {
        if (!gPauseMenu)
        {
            gPauseMenu = relive_new PauseMenu();
        }

        if (!gAbe)
        {
            gAbe = relive_new Abe();
            if (gAbe)
            {
                gAbe->mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
                gAbe->mYPos = FP_FromInteger(pTlv->mTopLeftY);
            }
        }
    }
    else
    {
        ResourceManagerWrapper::PendAnims(Abe::sAbeMotionAnimIds);
    }
}

static void Factory_ElumWall(relive::Path_TLV* /*pTlv*/, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_SlingMud(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new SlingMudokon(static_cast<relive::Path_SlingMudokon*>(pTlv), tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnims(SlingMudokon::sSlingMudMotionAnimIds);
        // kSparksAOResID was loaded but there is no AnimId entry for it?...
    }
}

static void Factory_BeeNest(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Bee_Swarm);
    }
    else
    {
        relive_new BeeNest(static_cast<relive::Path_BeeNest*>(pTlv), tlvId);
    }
}

static void Factory_Mine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pMineTlv = static_cast<relive::Path_Mine*>(pTlv);

    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnims(kElumGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Flash);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::GroundExplosion);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new Mine(pMineTlv, tlvId);
    }
}

static void Factory_Uxb(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pUxbTlv = static_cast<relive::Path_UXB*>(pTlv);

    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnims(kElumGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::UXB_Active);
        ResourceManagerWrapper::PendAnimation(AnimId::UXB_Toggle);
        ResourceManagerWrapper::PendAnimation(AnimId::UXB_Disabled);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_Flash);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_RedGreenTick);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::GroundExplosion);
        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new UXB(pUxbTlv, tlvId);
    }
}

static void Factory_Paramite(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Paramite::sParamiteMotionAnimIds);
        ResourceManagerWrapper::PendAnimation(AnimId::ParamiteWeb);
        return;
    }
    relive_new Paramite(static_cast<relive::Path_Paramite*>(pTlv), tlvId);
}

static void Factory_Bat(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Bat_Flying);
        ResourceManagerWrapper::PendAnimation(AnimId::Bat);
        ResourceManagerWrapper::PendAnimation(AnimId::Bat_Unknown);
    }
    else
    {
        relive_new Bat(static_cast<relive::Path_Bat*>(pTlv), tlvId);
    }
}

static void Factory_RingMud(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Mudokon::sMudMotionAnimIds);
    }
    else
    {
        relive_new Mudokon(pTlv, tlvId);
    }
}

static void Factory_BirdPortal(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<relive::Path_BirdPortal*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorShrink);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorIdle);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorGrow);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorShrink);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_Sparks);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_Flash);

        if (pBirdPortalTlv->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
        {
            ResourceManagerWrapper::PendAnimation(AnimId::ShrykullStart);
            ResourceManagerWrapper::PendAnimation(AnimId::ShrykullTransform);
            ResourceManagerWrapper::PendAnimation(AnimId::ShrykullDetransform);
            ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Blue);
            ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Red);
        }
    }
    else
    {
        relive_new BirdPortal(pBirdPortalTlv, tlvId);
    }
}

static void Factory_BellSongStone(relive::Path_TLV* /*pTlv*/, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        Path::TLV_Reset(tlvId);
    }
}


static void Factory_TrapDoor(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eRuptureFarms:
            case EReliveLevelIds::eBoardRoom:
            case EReliveLevelIds::eRuptureFarmsReturn:
                ResourceManagerWrapper::PendAnimation(AnimId::R1_TrapDoor_Open);
                ResourceManagerWrapper::PendAnimation(AnimId::R1_TrapDoor_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::R1_TrapDoor_Opening);
                ResourceManagerWrapper::PendAnimation(AnimId::R1_TrapDoor_Closing);
                break;
            case EReliveLevelIds::eLines:
            case EReliveLevelIds::eStockYards:
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Open);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Opening);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Closing);
                break;
            case EReliveLevelIds::eDesert:
            case EReliveLevelIds::eDesertTemple:
            case EReliveLevelIds::eDesertEscape:
                ResourceManagerWrapper::PendAnimation(AnimId::Desert_TrapDoor_Open);
                ResourceManagerWrapper::PendAnimation(AnimId::Desert_TrapDoor_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Desert_TrapDoor_Opening);
                ResourceManagerWrapper::PendAnimation(AnimId::Desert_TrapDoor_Closing);
                break;
            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Open);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Opening);
                ResourceManagerWrapper::PendAnimation(AnimId::Lines_TrapDoor_Closing);
                break;
        }
    }
    else
    {
        relive_new TrapDoor(static_cast<relive::Path_TrapDoor*>(pTlv), tlvId);
    }
}


static void Factory_RollingBall(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Stone_Ball);
        ResourceManagerWrapper::PendAnimation(AnimId::Stone_Ball_Rolling);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
    }
    else
    {
        relive_new RollingBall(static_cast<relive::Path_RollingBall*>(pTlv), tlvId);
    }
}

static TlvIterator FindMatchingSligTLV(TlvIterator pTlvIter, relive::Path_SligBound* pTlv)
{
    while (pTlvIter.GetTlv())
    {
        if (pTlvIter.GetTlv()->mTlvType == ReliveTypes::eSlig && pTlv->mSligBoundId == pTlvIter.GetTlv<relive::Path_Slig>()->mData.mSligBoundId && !pTlvIter.GetTlv()->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = pTlvIter.Next_TLV();
    }
    return TlvIterator::Invalid();
}

static void Factory_SligBoundLeft(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pBound = static_cast<relive::Path_SligBound*>(pTlv);
    LoadWalkingSligResources(loadMode, pBound->mDisabledResourcesAO);

    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            TlvIterator pTlvIter = gMap.Get_First_TLV_For_Offsetted_Camera(camX_idx, 0);
            relive::Path_TLV* pSligTlv = FindMatchingSligTLV(pTlvIter, pBound).GetTlv();
            if (pSligTlv)
            {
                pSligTlv->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                pSligTlv->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
                // AO OG bug tlvId not recalculated??
                relive_new Slig(static_cast<relive::Path_Slig*>(pSligTlv), tlvId);
                return;
            }
        }
    }
}

static void Factory_RollingBallStopper(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Stone_Ball_Stopper);
    }
    else
    {
        relive_new RollingBallStopper(static_cast<relive::Path_RollingBallStopper*>(pTlv), tlvId);
    }
}

static void Factory_FootSwitch(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Temple);
        ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Temple_Pressed);
    }
    else
    {
        relive_new FootSwitch(static_cast<relive::Path_FootSwitch*>(pTlv), tlvId);
    }
}

static void Factory_SecurityClaw(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pSecurityClawTlv = static_cast<relive::Path_SecurityClaw*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Claw_Lower_Open);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Claw_Lower_Close);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Claw_Lower_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Claw_Upper_Rotating);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Claw_Upper_NoRotation);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Blue);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Red);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnims(kElumGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
    }
    else
    {
        relive_new SecurityClaw(pSecurityClawTlv, tlvId);
    }
}


static void Factory_MotionDector(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new MotionDetector(static_cast<relive::Path_MotionDetector*>(pTlv), tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::MotionDetector_Flare);
        ResourceManagerWrapper::PendAnimation(AnimId::MotionDetector_Laser);
    }
}

static void Factory_SligSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_SligSpawner*>(pTlv);
    LoadWalkingSligResources(loadMode, pSligTlv->mData.mDisabledResourcesAO);

    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        // Do nothing, handled in LoadSligResources
    }
    else
    {
        relive_new SligSpawner(pTlv, &pSligTlv->mData, tlvId);
    }
}

static void Factory_ElectricWall(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new ElectricWall(static_cast<relive::Path_ElectricWall*>(pTlv), tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Electric_Wall);
    }
}


static void Factory_LiftMover(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new LiftMover(static_cast<relive::Path_LiftMover*>(pTlv), tlvId);
    }
}


static void Factory_ChimeLock(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new ChimeLock(static_cast<relive::Path_ChimeLock*>(pTlv), tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Chime_Ball);
        ResourceManagerWrapper::PendAnimation(AnimId::BigChime);
        ResourceManagerWrapper::PendAnimation(AnimId::MediumChime);
        ResourceManagerWrapper::PendAnimation(AnimId::SmallChime);
        ResourceManagerWrapper::PendAnimation(AnimId::BigChime_Moving);
        ResourceManagerWrapper::PendAnimation(AnimId::MediumChime_Moving);
        ResourceManagerWrapper::PendAnimation(AnimId::SmallChime_Moving);
    }
}

static void Factory_Elum(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Elum::gElumMotionAnimIds);
    }
    else
    {
        Elum::Spawn(tlvId);
        gElum->mXPos = FP_FromInteger(pTlv->mTopLeftX);
        gElum->mYPos = FP_FromInteger(pTlv->mTopLeftY);
    }
}


static void Factory_MeatSack(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Meat);
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSack_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSack_Hit);
    }
    else
    {
        relive_new MeatSack(static_cast<relive::Path_MeatSack*>(pTlv), tlvId);
    }
}


static void Factory_Scrab(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Scrab::sScrabMotionAnimIds);
        return;
    }
    relive_new Scrab(static_cast<relive::Path_Scrab*>(pTlv), tlvId);
}


static void Factory_FlintLockFire(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Fire);
        ResourceManagerWrapper::PendAnimation(AnimId::FlintLock_Gourd);
        ResourceManagerWrapper::PendAnimation(AnimId::FlintLock_Hammers_Disabled);
        ResourceManagerWrapper::PendAnimation(AnimId::FlintLock_Hammers_Activating);
    }
    else
    {
        relive_new FlintLockFire(static_cast<relive::Path_FlintLockFire*>(pTlv), tlvId);
    }
}

static void Factory_SligBoundRight(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    Factory_SligBoundLeft(pTlv, tlvId, loadMode);
}

static void Factory_InvisibleSwitch(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new InvisibleSwitch(static_cast<relive::Path_InvisibleSwitch*>(pTlv), tlvId);
    }
}

static void Factory_WorkerMudokon(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pMudTlv = static_cast<relive::Path_Mudokon*>(pTlv);
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        switch (pMudTlv->mJob)
        {
            case relive::Path_Mudokon::MudJobs::eStandScrub:
                break;

            case relive::Path_Mudokon::MudJobs::eSitChant:
                break;
        }

        relive_new Mudokon(pTlv, tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnims(Mudokon::sMudMotionAnimIds);
        switch (pMudTlv->mJob)
        {
            case relive::Path_Mudokon::MudJobs::eStandScrub:
            {
            }
            break;

            case relive::Path_Mudokon::MudJobs::eSitScrub:
            {
            }
            break;

            case relive::Path_Mudokon::MudJobs::eSitChant:
            {
            }
            break;
        }
    }
}

static void Factory_DoorFlame(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Fire);
        ResourceManagerWrapper::PendAnimation(AnimId::Door_FireBackgroundGlow);
    }
    else
    {
        relive_new DoorFlame(static_cast<relive::Path_DoorFlame*>(pTlv), tlvId);
    }
}

static void Factory_MovingBomb(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<relive::Path_MovingBomb*>(pTlv);

    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::MovingBomb);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kElumGibs);

        if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
        {
        }
    }
    else
    {
        relive_new MovingBomb(pMovingBombTlv, tlvId);
    }
}

static void Factory_MeatSaw(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSawMotor);
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSaw_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSaw_Moving);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
    }
    else
    {
        relive_new MeatSaw(static_cast<relive::Path_MeatSaw*>(pTlv), tlvId);
    }
}

static void Factory_MainMenuController(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (gMainMenuInstanceCount == 0)
    {
        if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
        {
            ResourceManagerWrapper::PendAnims(Menu::kAnims);
        }
        else
        {
            relive_new Menu(pTlv, tlvId);
        }
    }
}

static void Factory_HintFly(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new HintFly(static_cast<relive::Path_HintFly*>(pTlv), tlvId);
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::HintFly);
    }
}

static void Factory_TimerTrigger(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new TimerTrigger(static_cast<relive::Path_TimerTrigger*>(pTlv), tlvId);
    }
}


static void Factory_SecurityDoor(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Door_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Door_Speak);
    }
    else
    {
        relive_new SecurityDoor(static_cast<relive::Path_SecurityDoor*>(pTlv), tlvId);
    }
}

static void Factory_BoomMachine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pBoomMachineTlv = static_cast<relive::Path_BoomMachine*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Grenade);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Pipe_DropGrenade);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Pipe_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Button_Off);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Button_On);
        return;
    }

    relive_new BoomMachine(pBoomMachineTlv, tlvId);
}


static void Factory_LCDScreen(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new LCDScreen(static_cast<relive::Path_LCDScreen*>(pTlv), tlvId);
    }
}

static void Factory_HandStone(relive::Path_TLV* /*pTlv*/, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Circular_Fade);
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}


static void Factory_CreditsController(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        if (!gCreditsControllerExists)
        {
            relive_new CreditsController(static_cast<relive::Path_CreditsController*>(pTlv), tlvId);
        }
    }
}

static void Factory_LCDStatusBoard(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new LCDStatusBoard(static_cast<relive::Path_LCDStatusBoard*>(pTlv), tlvId);
    }
}


static void Factory_SwitchStateBooleanLogic(relive::Path_TLV* /*pTlv*/, const Guid& /*tlvId*/, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        LOG_WARNING("Factory_SwitchStateBooleanLogic_487B80 tlv converter not implemented");
        //relive_new SwitchStateBooleanLogic(static_cast<relive::Path_SwitchStateBooleanLogic*>(pTlv), tlvId);
    }
}

static void Factory_MusicTrigger(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new MusicTrigger(static_cast<relive::Path_MusicTrigger*>(pTlv), tlvId);
    }
}

static void Factory_LightEffect(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pPathLightTlv = static_cast<relive::Path_LightEffect*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (pPathLightTlv->mType)
        {
            case relive::Path_LightEffect::Type::Star:
                ResourceManagerWrapper::PendAnimation(AnimId::Star);
                break;

            case relive::Path_LightEffect::Type::GoldGlow:
                ResourceManagerWrapper::PendAnimation(AnimId::GoldGlow);
                break;

            case relive::Path_LightEffect::Type::GreenGlow:
                ResourceManagerWrapper::PendAnimation(AnimId::GreenGlow);
                break;

            case relive::Path_LightEffect::Type::FlintGlow:
                ResourceManagerWrapper::PendAnimation(AnimId::FlintGlow);
                break;

            case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
                ResourceManagerWrapper::PendAnimation(AnimId::RedDoorLight);
                ResourceManagerWrapper::PendAnimation(AnimId::GreenDoorLight);
                break;

            case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
                ResourceManagerWrapper::PendAnimation(AnimId::RedHubLight);
                ResourceManagerWrapper::PendAnimation(AnimId::GreenHubLight);
                break;

            default:
                return;
        }
    }
    else
    {
        switch (pPathLightTlv->mType)
        {
            case relive::Path_LightEffect::Type::Star:
            {
                relive_new LightEffect(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::GoldGlow:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::GreenGlow:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::FlintGlow:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            case relive::Path_LightEffect::Type::Switchable_RedGreenDoorLights:
            case relive::Path_LightEffect::Type::Switchable_RedGreenHubLight:
            {
                relive_new DoorLight(pPathLightTlv, tlvId);
                break;
            }

            default:
                return;
        }
    }
}


static void Factory_SlogSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Slog::sSlogMotionAnimIds);
    }
    else
    {
        relive_new SlogSpawner(static_cast<relive::Path_SlogSpawner*>(pTlv), tlvId);
    }
}


static void Factory_GasCountDown(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
    }
    else
    {
        relive_new GasCountDown(static_cast<relive::Path_GasCountDown*>(pTlv), tlvId);
    }
}



static void Factory_RingCancel(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        auto pRingCancel = static_cast<relive::Path_RingCancel*>(pTlv);
        if (pRingCancel->mRemovesShrykull)
        {
            if (gAbe->mRingPulseTimer)
            {
                if (gAbe->mHaveShrykull)
                {
                    gAbe->mRingPulseTimer = 0;
                }
            }
        }
        else
        {
            if (!gAbe->mHaveShrykull)
            {
                gAbe->mRingPulseTimer = 0;
            }
        }
        Path::TLV_Reset(tlvId);
    }
}


static void Factory_GasEmitter(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new GasEmitter(static_cast<relive::Path_GasEmitter*>(pTlv), tlvId);
    }
}


static void Factory_ZzzSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new ZzzSpawner(static_cast<relive::Path_ZzzSpawner*>(pTlv), tlvId);
    }
}


static void Factory_BackgroundGlukkon(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Background_Glukkon_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Background_Glukkon_Laugh);
        ResourceManagerWrapper::PendAnimation(AnimId::Background_Glukkon_KillHim1);
        ResourceManagerWrapper::PendAnimation(AnimId::Background_Glukkon_KillHim2);
        ResourceManagerWrapper::PendAnimation(AnimId::Background_Glukkon_Dying);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
    }
    else
    {
        relive_new BackgroundGlukkon(static_cast<relive::Path_BackgroundGlukkon*>(pTlv), tlvId);
    }
}


static void Factory_KillUnsavedMuds(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        // TODO: Check removed OG bug fix - added an extra check that checks if the map has changed
        // which prevents that the killed mudokon count becomes inaccurate or even negative.
        auto pTlvKillMuds = static_cast<relive::Path_KillUnsavedMuds*>(pTlv);
        if (!pTlvKillMuds->mTlvSpecificMeaning)
        {
            gKilledMudokons = static_cast<s16>(pTlvKillMuds->mMudsToKillCount - gRescuedMudokons);
            pTlvKillMuds->mTlvSpecificMeaning = 1;
            Path::TLV_Delete(tlvId);
        }
    }
}
} // namespace AO

namespace AE
{

static constexpr AnimId kAbeGibs[3] = {
    AnimId::Abe_Head_Gib,
    AnimId::Abe_Arm_Gib,
    AnimId::Abe_Body_Gib};

static constexpr AnimId kSlogGibs[2] = {
    AnimId::Slog_Head_Gib,
    AnimId::Slog_Body_Gib};

static constexpr AnimId kSligGibs[3] = {
    AnimId::Slig_Head_Gib,
    AnimId::Slig_Arm_Gib,
    AnimId::Slig_Body_Gib};

static void Factory_MainMenuController(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (sMainMenuObjectCounter_BB4400 == 0)
    {
        if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
        {
            for (const auto& info : MainMenuController::sMainMenuFrameTable_561CC8)
            {
                if (info.field_0_animation != AnimId::None)
                {
                    ResourceManagerWrapper::PendAnimation(info.field_0_animation);
                }
            }
            ResourceManagerWrapper::PendAnims(MainMenuController::kMenuAnims);

            // Originally loaded in the PauseMenu::Init()
            ResourceManagerWrapper::PendAnimation(AnimId::NormalMudIcon);
            ResourceManagerWrapper::PendAnimation(AnimId::AngryMudIcon);
            ResourceManagerWrapper::PendAnimation(AnimId::HappyMudIcon);

            // Originally loaded in Game_Init_LoadingIcon()
            ResourceManagerWrapper::PendAnimation(AnimId::Loading_Icon2);
        }
        else
        {
            relive_new MainMenuController(pTlv, tlvId);
        }
    }
}

static void Factory_Hoist(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    relive::Path_Hoist* pHoistTlv = static_cast<relive::Path_Hoist*>(pTlv);
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::HoistRock1);
        ResourceManagerWrapper::PendAnimation(AnimId::HoistRock2);
        ResourceManagerWrapper::PendAnimation(AnimId::HoistRock3);
    }
    else if (pHoistTlv->mHoistType == relive::Path_Hoist::Type::eOffScreen)
    {
        // Its an off screen hoist so create the falling rocks effect
        relive_new HoistRocksEffect(pHoistTlv, tlvId);
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_Edge(relive::Path_TLV* /*pTlv*/, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        // Empty
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_Door(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        auto pDoorTlv = static_cast<relive::Path_Door*>(pTlv);
        ResourceManagerWrapper::PendAnimation(AnimId::Door_Themed_Closed, pDoorTlv->mTheme);
        ResourceManagerWrapper::PendAnimation(AnimId::Door_Themed_Open, pDoorTlv->mTheme);
    }
    else
    {
        relive_new Door(static_cast<relive::Path_Door*>(pTlv), tlvId);
    }
}

static void Factory_ShadowZone(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new ShadowZone(static_cast<relive::Path_ShadowZone*>(pTlv), tlvId);
    }
}

static void Factory_LiftPoint(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::AE_Rope);
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudomoVault_Ender:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Necrum);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Necrum);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Necrum);
                break;

            case EReliveLevelIds::eFeeCoDepot:
            case EReliveLevelIds::eFeeCoDepot_Ender:
            case EReliveLevelIds::eBarracks:
            case EReliveLevelIds::eBarracks_Ender:
            case EReliveLevelIds::eBonewerkz:
            case EReliveLevelIds::eBonewerkz_Ender:
            case EReliveLevelIds::eBrewery:
            case EReliveLevelIds::eBrewery_Ender:
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Mines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Mines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Mines);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::LiftPlatform_Mines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftBottomWheel_Mines);
                ResourceManagerWrapper::PendAnimation(AnimId::LiftTopWheel_Mines);
                break;
        }
    }
    else
    {
        relive::Path_LiftPoint* pLiftTlv = static_cast<relive::Path_LiftPoint*>(pTlv);
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (!(pObj->GetDead()) && pObj->Type() == ReliveTypes::eLiftPoint)
            {
                // Is there already an existing LiftPoint object for this TLV?
                LiftPoint* pLiftPoint = static_cast<LiftPoint*>(pObj);
                const s16 xpos = FP_GetExponent(pLiftPoint->mXPos);
                if (pTlv->mTopLeftX <= xpos && xpos <= pTlv->mBottomRightX && pLiftPoint->mLiftPointId == pLiftTlv->mLiftPointId && pLiftPoint->mCurrentLevel == gMap.mCurrentLevel && pLiftPoint->mCurrentPath == gMap.mCurrentPath)
                {
                    // Yes so just reset its data
                    Path::TLV_Reset(tlvId);
                    return;
                }
            }
        }

        // TODO: Meaning of the data in mTlvSpecificMeaning for lift point
        if (pLiftTlv->mTlvSpecificMeaning & 2 || (pLiftTlv->mTlvSpecificMeaning == 0 && pLiftTlv->mIsStartPoint))
        {
            relive_new LiftPoint(pLiftTlv, tlvId);
            return;
        }
        else
        {
            // Find out where to create the lift point
            s16 pointNumber = 1;
            while (pointNumber < 8)
            {
                TlvIterator tlvIterator = gPathInfo->Get_First_TLV_For_Offsetted_Camera(
                    0,
                    pointNumber / 2 * (pointNumber % 2 != 0 ? -1 : 1));

                while (tlvIterator.GetTlv())
                {
                    if (tlvIterator.GetTlv()->mTlvType == ReliveTypes::eLiftPoint)
                    {
                        auto pLiftPointIter = tlvIterator.GetTlv<relive::Path_LiftPoint>();

                        const s32 tlvX = pTlv->mTopLeftX;
                        const s32 absX = tlvIterator.GetTlv()->mTopLeftX - tlvX >= 0 ? tlvIterator.GetTlv()->mTopLeftX - tlvX : tlvX - tlvIterator.GetTlv()->mTopLeftX;

                        if (absX < 5 && pLiftPointIter->mLiftPointId == pLiftTlv->mLiftPointId && (pLiftPointIter->mTlvSpecificMeaning & 2 || pLiftPointIter->mTlvSpecificMeaning == 0) && pLiftPointIter->mIsStartPoint)
                        {
                            relive_new LiftPoint(pLiftPointIter, tlvId);
                            return;
                        }
                    }

                    tlvIterator = tlvIterator.Next_TLV();
                }
                pointNumber++;
            }

            // Default to original
            relive_new LiftPoint(pLiftTlv, tlvId);
        }
    }
}

static void Factory_Well(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Well_Leaf);
    }
    else
    {
        relive::Path_WellBase* pWellTlv = static_cast<relive::Path_WellBase*>(pTlv);
        const FP xpos = FP_FromInteger(pWellTlv->mTopLeftX);
        const FP ypos = FP_FromInteger(pWellTlv->mTopLeftY + 5);
        relive_new Well(pWellTlv, xpos, ypos, tlvId);
    }
}

static void Factory_Dove(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        auto pDoveTlv = static_cast<relive::Path_Dove*>(pTlv);

        const s16 width = pDoveTlv->Width();
        const s16 height = pDoveTlv->Height();

        for (s32 i = 0; i < pDoveTlv->mDoveCount; i++)
        {
            auto pDove = relive_new Dove(
                AnimId::Dove_Idle,
                tlvId,
                pDoveTlv->mScale != relive::reliveScale::eFull ? FP_FromDouble(0.5) : FP_FromInteger(1));

            s16 ypos = 0;
            if (pDoveTlv->mPixelPerfect)
            {
                pDove->mXPos = FP_FromInteger(pTlv->mTopLeftX);
                ypos = pTlv->mTopLeftY;
            }
            else
            {
                pDove->mXPos = FP_FromInteger(pTlv->mTopLeftX + width * Math_NextRandom() / 256);
                ypos = pTlv->mTopLeftY + height * Math_NextRandom() / 256;
            }
            pDove->mYPos = FP_FromInteger(ypos) + FP_FromInteger(10);
        }
    }
}

static void Factory_RockSack(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::RockSack_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::RockSack_SoftHit);
        ResourceManagerWrapper::PendAnimation(AnimId::RockSack_HardHit);
    }
    else
    {
        relive_new RockSack(static_cast<relive::Path_RockSack*>(pTlv), tlvId);
    }
}

static void Factory_FallingItem(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Stick);
        ResourceManagerWrapper::PendAnimation(AnimId::ObjectShadow);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
        {
            ResourceManagerWrapper::PendAnimation(AnimId::FallingCrate_Falling);
            ResourceManagerWrapper::PendAnimation(AnimId::FallingCrate_Waiting);
            ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        }
        else
        {
            ResourceManagerWrapper::PendAnimation(AnimId::AE_FallingRock_Falling);
            ResourceManagerWrapper::PendAnimation(AnimId::AE_FallingRock_Waiting);
        }
    }
    else
    {
        relive_new FallingItem(static_cast<relive::Path_FallingItem*>(pTlv), tlvId);
    }
}

static void Factory_PullRingRope(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::AE_Rope);
        ResourceManagerWrapper::PendAnimation(AnimId::PullRingRope_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::PullRingRope_UseBegin);
        ResourceManagerWrapper::PendAnimation(AnimId::PullRingRope_UseEnd);
    }
    else
    {
        relive_new PullRingRope(static_cast<relive::Path_PullRingRope*>(pTlv), tlvId);
    }
}

static void Factory_TimedMine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    auto mine_tlv = static_cast<relive::Path_TimedMine*>(pTlv);

    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::TimedMine_Activated);
        ResourceManagerWrapper::PendAnimation(AnimId::TimedMine_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_Flash);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_RedGreenTick);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::GroundExplosion);
    }
    else
    {
        relive_new TimedMine(mine_tlv, tlvId);
    }
}

static void LoadWalkingSligResources()
{
    ResourceManagerWrapper::PendAnims(Slig::sSligAnimIdTable);
    ResourceManagerWrapper::PendAnimation(AnimId::ShootingZFire_Particle);
    ResourceManagerWrapper::PendAnimation(AnimId::ShootingFire_Particle);
    ResourceManagerWrapper::PendAnimation(AnimId::Bullet_Shell);
    ResourceManagerWrapper::PendAnims(kSligGibs);
    ResourceManagerWrapper::PendAnimation(AnimId::ObjectShadow);
}

static void Factory_Slig(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources();
    }
    else
    {
        relive_new Slig(pSligTlv, tlvId);
    }
}

static void LoadSlogResources()
{
    ResourceManagerWrapper::PendAnims(Slog::sSlogAnimIdTable);
}

static void Factory_Slog(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadSlogResources();
    }
    else
    {
        relive_new Slog(static_cast<relive::Path_Slog*>(pTlv), tlvId);
    }
}

static void Factory_Lever(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Release_Left);
        ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Release_Right);
        ResourceManagerWrapper::PendAnimation(AnimId::Lever_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Left);
        ResourceManagerWrapper::PendAnimation(AnimId::Lever_Pull_Right);
    }
    else
    {
        relive_new Lever(static_cast<relive::Path_Lever*>(pTlv), tlvId);
    }
}

static void Factory_SecurityOrb(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    relive::Path_SecurityOrb* pSecurityOrbTlv = static_cast<relive::Path_SecurityOrb*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Orb);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Blue);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Red);
        ResourceManagerWrapper::PendAnimation(AnimId::AE_ZapSpark);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
    }
    else
    {
        relive_new SecurityOrb(pSecurityOrbTlv, tlvId);
    }
}

static void Factory_AbeStart(relive::Path_TLV* pTlv,  const Guid& /*tlvId*/, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2 && !gAttract)
    {
        if (!gPauseMenu)
        {
            gPauseMenu = relive_new PauseMenu();
        }

        if (!gAbe)
        {
            gAbe = relive_new Abe();
            if (gAbe)
            {
                gAbe->mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
                gAbe->mYPos = FP_FromInteger(pTlv->mTopLeftY);
            }
        }
    }
}

static void Factory_Mine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    auto mine_tlv = static_cast<relive::Path_Mine*>(pTlv);

    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Flash);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::GroundExplosion);
    }
    else
    {
        relive_new Mine(mine_tlv, tlvId);
    }
}

static void Factory_UXB(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto uxb_tlv = static_cast<relive::Path_UXB*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::UXB_Active);
        ResourceManagerWrapper::PendAnimation(AnimId::UXB_Toggle);
        ResourceManagerWrapper::PendAnimation(AnimId::UXB_Disabled);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_Flash);
        ResourceManagerWrapper::PendAnimation(AnimId::Bomb_RedGreenTick);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::GroundExplosion);
    }
    else
    {
        relive_new UXB(uxb_tlv, tlvId);
    }
}

static void Factory_Paramite(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Paramite::sParamiteMotionAnimIds);
        ResourceManagerWrapper::PendAnimation(AnimId::ParamiteWeb);
    }
    else
    {
         relive_new Paramite(static_cast<relive::Path_Paramite*>(pTlv), tlvId);
    }
}

static void Factory_MovieHandStone(relive::Path_TLV*, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        // Empty
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_BirdPortal(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pBirdPortalTlv = static_cast<relive::Path_BirdPortal*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorShrink);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorIdle);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_TerminatorGrow);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_Sparks);
        ResourceManagerWrapper::PendAnimation(AnimId::BirdPortal_Flash);

        if (pBirdPortalTlv->mPortalType == relive::Path_BirdPortal::PortalType::eShrykull)
        {
            ResourceManagerWrapper::PendAnimation(AnimId::ShrykullStart);
            ResourceManagerWrapper::PendAnimation(AnimId::ShrykullTransform);
            ResourceManagerWrapper::PendAnimation(AnimId::ShrykullDetransform);
            ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Blue);
            ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Red);
        }
    }
    else if (SwitchStates_Get(pBirdPortalTlv->mCreatePortalSwitchId))
    {
        relive_new BirdPortal(pBirdPortalTlv, tlvId);
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_TrapDoor(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Tribal_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Tribal_Closing);
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Tribal_Open);
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Tribal_Opening);
                break;

            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Open);
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Closing);
                ResourceManagerWrapper::PendAnimation(AnimId::Trap_Door_Opening);
                break;
        }
    }
    else
    {
        relive_new TrapDoor(static_cast<relive::Path_TrapDoor*>(pTlv), tlvId);
    }
}

static TlvIterator FindMatchingSligTLV(TlvIterator pTlvIter, relive::Path_SligBound* pTlv)
{
    while (pTlvIter.GetTlv())
    {
        if (pTlvIter.GetTlv()->mTlvType == ReliveTypes::eSlig && pTlv->mSligBoundId == pTlvIter.GetTlv<relive::Path_Slig>()->mData.mSligBoundId && !pTlvIter.GetTlv()->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed))
        {
            return pTlvIter;
        }
        pTlvIter = pTlvIter.Next_TLV();
    }
    return TlvIterator::Invalid();
}

static void Factory_SligBoundLeft(relive::Path_TLV* pTlv,  const Guid& /*tlvId*/, relive::LoadMode loadMode)
{
    auto pBound = static_cast<relive::Path_SligBound*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources();
    }
    else
    {
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
        pBound->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);

        for (s16 camX_idx = -2; camX_idx < 3; camX_idx++)
        {
            TlvIterator pTlvIter = FindMatchingSligTLV( gPathInfo->Get_First_TLV_For_Offsetted_Camera(camX_idx, 0), pBound);
            if (pTlvIter.GetTlv())
            {
                pTlvIter.GetTlv()->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                pTlvIter.GetTlv()->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);

                relive_new Slig(pTlvIter.GetTlv<relive::Path_Slig>(), pTlvIter.GetTlv()->mId); // id of the slig to spawn at the bound, not the bound itself

                return;
            }
        }
    }
}

static void Factory_FootSwitch(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Vault_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Vault_Pressed);
                break;
            case EReliveLevelIds::eBonewerkz:
                ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Bonewerkz_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Bonewerkz_Pressed);
                break;
            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Industrial_Idle);
                ResourceManagerWrapper::PendAnimation(AnimId::Foot_Switch_Industrial_Pressed);
                break;
        }
    }
    else
    {
        relive_new FootSwitch(static_cast<relive::Path_FootSwitch*>(pTlv), tlvId);
    }
}

static void Factory_ChantSuppressor_WithArms(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    // TODO: Most of the code for the chant suppressor with arms is still in the game
    // check if it can be re-added at some point.
    Factory_SecurityOrb(pTlv, tlvId, loadMode);
}

static void Factory_MotionDetector(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new MotionDetector(static_cast<relive::Path_MotionDetector*>(pTlv), tlvId, nullptr);
    }
    else
    {
        ResourceManagerWrapper::PendAnimation(AnimId::MotionDetector_Flare);
        ResourceManagerWrapper::PendAnimation(AnimId::MotionDetector_Laser);
    }
}

static void Factory_SligSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pSligTlv = static_cast<relive::Path_Slig*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources();
    }
    else
    {
        relive_new SligSpawner(pSligTlv, tlvId);
    }
}

static void Factory_ElectricWall(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Electric_Wall);
    }
    else
    {
        relive_new ElectricWall(static_cast<relive::Path_ElectricWall*>(pTlv), tlvId);
    }
}

static void Factory_LiftMover(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new LiftMover(static_cast<relive::Path_LiftMover*>(pTlv), tlvId);
    }
}

static void Factory_MeatSack(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Meat);
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSack_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::MeatSack_Hit);
    }
    else
    {
        relive_new MeatSack(static_cast<relive::Path_MeatSack*>(pTlv), tlvId);
    }
}

static void Factory_Scrab(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Scrab::sScrabMotionAnimIds);
    }
    else
    {
        relive_new Scrab(static_cast<relive::Path_Scrab*>(pTlv), tlvId, relive::Path_ScrabSpawner::SpawnDirection::eNone);
    }
}

static void Factory_SligBoundRight(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    Factory_SligBoundLeft(pTlv, tlvId, loadMode);
}

static void Factory_InvisibleSwitch(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new InvisibleSwitch(static_cast<relive::Path_InvisibleSwitch*>(pTlv), tlvId);
    }
}

static void Factory_Mudokon(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pMudTlv = static_cast<relive::Path_Mudokon*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Mudokon::kMudMotionAnimIds);
        ResourceManagerWrapper::PendAnimation(AnimId::ObjectShadow);
    }
    else
    {
        relive_new Mudokon(pMudTlv, tlvId);
    }
}

static void Factory_DoorFlame(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Fire);
        ResourceManagerWrapper::PendAnimation(AnimId::Door_FireBackgroundGlow);
    }
    else
    {
        relive_new DoorFlame(static_cast<relive::Path_DoorFlame*>(pTlv), tlvId);
    }
}

static void Factory_MovingBomb(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pMovingBombTlv = static_cast<relive::Path_MovingBomb*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::MovingBomb);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
    }
    else
    {
        relive_new MovingBomb(pMovingBombTlv, tlvId);
    }
}

static void Factory_TimerTrigger(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::ConstructObject_0)
    {
        relive_new TimerTrigger(static_cast<relive::Path_TimerTrigger*>(pTlv), tlvId);
    }
}

static void Factory_SecurityDoor(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Door_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Door_Speak);
    }
    else
    {
        relive_new SecurityDoor(static_cast<relive::Path_SecurityDoor*>(pTlv), tlvId);
    }
}

static void Factory_BoomMachine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    auto pTlvBooMachine = static_cast<relive::Path_BoomMachine*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Grenade);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Pipe_DropGrenade);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Pipe_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Button_Off);
        ResourceManagerWrapper::PendAnimation(AnimId::BoomMachine_Button_On);
    }
    else
    {
        relive_new BoomMachine(pTlvBooMachine, tlvId);
    }
}

static void Factory_BackgroundAnimation(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new BackgroundAnimation(static_cast<relive::Path_BackgroundAnimation*>(pTlv), tlvId);
    }
}

static void Factory_LCDScreen(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        //Map::LoadResource("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, 2, loadmode); // TODO: Add to resource ID enum
        return;
    }

    relive_new LCDScreen(static_cast<relive::Path_LCDScreen*>(pTlv), tlvId);
}

static void Factory_HandStone(relive::Path_TLV*, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::SpotLight);
    }
    else
    {
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_CreditsController(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2 && !gCreditsControllerExists)
    {
        relive_new CreditsController(pTlv, tlvId);
    }
}

static void Factory_LCDStatusBoard(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        //Map::LoadResource("LCDFONT.FNT", ResourceManager::ResourceType::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
        return;
    }
    
    relive_new LCDStatusBoard(static_cast<relive::Path_LCDStatusBoard*>(pTlv), tlvId);
}

static void Factory_WheelSyncer(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new WheelSyncer(static_cast<relive::Path_WheelSyncer*>(pTlv), tlvId);
    }
}

static void Factory_MusicTrigger(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new MusicTrigger(static_cast<relive::Path_MusicTrigger*>(pTlv), tlvId);
    }
}

static void Factory_SlogSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadSlogResources();
    }
    else
    {
        relive_new SlogSpawner(static_cast<relive::Path_SlogSpawner*>(pTlv), tlvId);
    }
}

static void Factory_GasCountdown(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
        //gMap.LoadResource("ABEGAS.BAN", ResourceManager::Resource_Animation, AEResourceID::kAbegasResID, loadMode);
    }
    else
    {
        relive_new GasCountDown(static_cast<relive::Path_GasCountDown*>(pTlv), tlvId);
    }
}

static void Factory_GasEmitter(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new GasEmitter(static_cast<relive::Path_GasEmitter*>(pTlv), tlvId);
    }
}

static void Factory_ZzzSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        relive_new ZzzSpawner(static_cast<relive::Path_ZzzSpawner*>(pTlv), tlvId);
    }
}

static void Factory_Glukkon(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    relive::Path_Glukkon* pGlukkonTlv = static_cast<relive::Path_Glukkon*>(pTlv);
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        switch (pGlukkonTlv->mGlukkonType)
        {
            case relive::Path_Glukkon::GlukkonTypes::eStoryAslik:
                for (s32 i = 0; i < 25; i++)
                {
                    const auto& anim = Glukkon::sGlukkonsAnimIdTable[1][i];
                    ResourceManagerWrapper::PendAnimation(anim);
                }
                ResourceManagerWrapper::PendAnimation(AnimId::Aslik_Head_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Aslik_Arm_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Aslik_Body_Gib);
                break;

            case relive::Path_Glukkon::GlukkonTypes::eStoryDripik:
                for (s32 i = 0; i < 25; i++)
                {
                    const auto& anim = Glukkon::sGlukkonsAnimIdTable[2][i];
                    ResourceManagerWrapper::PendAnimation(anim);
                }
                ResourceManagerWrapper::PendAnimation(AnimId::Dripik_Head_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Dripik_Arm_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Dripik_Body_Gib);
                break;

            case relive::Path_Glukkon::GlukkonTypes::eStoryPhleg:
                for (s32 i = 0; i < 25; i++)
                {
                    const auto& anim = Glukkon::sGlukkonsAnimIdTable[3][i];
                    ResourceManagerWrapper::PendAnimation(anim);
                }
                ResourceManagerWrapper::PendAnimation(AnimId::Phleg_Head_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Phleg_Arm_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Phleg_Body_Gib);
                break;

            case relive::Path_Glukkon::GlukkonTypes::eNormal:
                for (s32 i = 0; i < 25; i++)
                {
                    const auto& anim = Glukkon::sGlukkonsAnimIdTable[0][i];
                    ResourceManagerWrapper::PendAnimation(anim);
                }
                ResourceManagerWrapper::PendAnimation(AnimId::Glukkon_Head_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Glukkon_Arm_Gib);
                ResourceManagerWrapper::PendAnimation(AnimId::Glukkon_Body_Gib);
                break;
        }
    }
    else
    {
        relive_new Glukkon(pGlukkonTlv, tlvId);
    }
}

static void Factory_Water(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::WaterDrop);
        ResourceManagerWrapper::PendAnimation(AnimId::WaterSplash);
    }
    else
    {
        relive_new Water(static_cast<relive::Path_Water*>(pTlv), tlvId);
    }
}

static void Factory_WorkWheel(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Work_Wheel_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Work_Wheel_Turning);
    }
    else
    {
        relive_new WorkWheel(static_cast<relive::Path_WorkWheel*>(pTlv), tlvId);
    }
}

static void Create_LaughingGas(Layer layer, relive::Path_TLV* pTlv, const Guid& tlvId)
{
    if (!gLaughingGasInstanceCount)
    {
        relive_new LaughingGas(layer, static_cast<relive::Path_LaughingGas*>(pTlv), tlvId);
    }
}

static void Factory_LaughingGas(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        Create_LaughingGas(Layer::eLayer_Above_FG1_39, pTlv, tlvId);
    }
}

static void LoadFlyingSligResources()
{
    ResourceManagerWrapper::PendAnims(FlyingSlig::sFlyingSligAnimIdTable);
    ResourceManagerWrapper::PendAnims(kSligGibs);
    ResourceManagerWrapper::PendAnimation(AnimId::Grenade);
    ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion_Small);
    ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
    ResourceManagerWrapper::PendAnims(kAbeGibs);
    ResourceManagerWrapper::PendAnimation(AnimId::ShootingFire_Particle);
    ResourceManagerWrapper::PendAnimation(AnimId::Vaporize_Particle);
    ResourceManagerWrapper::PendAnims(kSlogGibs);
}

static void Factory_FlyingSlig(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources();
    }
    else
    {
        relive_new FlyingSlig(static_cast<relive::Path_FlyingSlig*>(pTlv), tlvId);
    }
}

static void Factory_Fleech(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Fleech::sFleechAnimFromMotion);
        ResourceManagerWrapper::PendAnimation(AnimId::Fleech_Head_Gib);
        ResourceManagerWrapper::PendAnimation(AnimId::Fleech_Body_Gib);
    }
    else if (gMap.mCurrentLevel != EReliveLevelIds::eMudancheeVault_Ender || gMap.mCurrentPath != 9 || gMap.mCurrentCamera != 4)
    {
        relive_new Fleech(static_cast<relive::Path_Fleech*>(pTlv), tlvId);
    }
}

static void Factory_Slurg(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Slurg_Move);
        ResourceManagerWrapper::PendAnimation(AnimId::Slurg_Burst);
        ResourceManagerWrapper::PendAnimation(AnimId::Slurg_Turn_Around);
    }
    else
    {
        relive_new Slurg(static_cast<relive::Path_Slurg*>(pTlv), tlvId);
    }
}

static void Factory_SlamDoor(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                ResourceManagerWrapper::PendAnimation(AnimId::Slam_Door_Vault_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Slam_Door_Vault_Closing);
                ResourceManagerWrapper::PendAnimation(AnimId::Slam_Door_Vault_Opening);
                break;
            default:
                ResourceManagerWrapper::PendAnimation(AnimId::Slam_Door_Industrial_Closed);
                ResourceManagerWrapper::PendAnimation(AnimId::Slam_Door_Industrial_Closing);
                ResourceManagerWrapper::PendAnimation(AnimId::Slam_Door_Industrial_Opening);
                break;
        }
    }
    else
    {
        relive_new SlamDoor(static_cast<relive::Path_SlamDoor*>(pTlv), tlvId);
    }
}

static void Factory_LevelLoader(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new LevelLoader(static_cast<relive::Path_LevelLoader*>(pTlv), tlvId);
    }
}

static void Factory_DemoSpawnPoint(relive::Path_TLV*,  const Guid& /*tlvId*/, relive::LoadMode loadMode)
{
    if (loadMode != relive::LoadMode::LoadResourceFromList_1 && loadMode != relive::LoadMode::LoadResource_2)
    {
        if (gAttract)
        {
            if (!gActiveDemoPlayback)
            {
                relive_new DemoPlayback();
            }
        }
    }
}

static void Factory_Teleporter(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new Teleporter(static_cast<relive::Path_Teleporter*>(pTlv), tlvId);
    }
}

static void Factory_SlurgSpawner(relive::Path_TLV* /*pTlv*/,  const Guid& /*tlvId*/, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Slurg_Move);
        ResourceManagerWrapper::PendAnimation(AnimId::Slurg_Burst);
        ResourceManagerWrapper::PendAnimation(AnimId::Slurg_Turn_Around);
    }
    else
    {
       /*
       // TODO/HACK disabled because it crashes the game when a quick load is done
       // after you save when a slurg is spawned as it has no TLV.
       // Its only used to spawn 1 slurg in 1 screen of the game. Fix in abi_break
       // branch by converting to a normal slurg.
        relive_new SlurgSpawner(static_cast<relive::Path_SlurgSpawner*>(pTlv), tlvId);
        */
    }
}

static void Factory_Drill(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Drill_Horizontal_Off);
        ResourceManagerWrapper::PendAnimation(AnimId::Drill_Horizontal_On);
        ResourceManagerWrapper::PendAnimation(AnimId::Drill_Vertical_Off);
        ResourceManagerWrapper::PendAnimation(AnimId::Drill_Vertical_On);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
    }
    else
    {
        relive_new Drill(static_cast<relive::Path_Drill*>(pTlv), tlvId);
    }
}

static void Factory_ColourfulMeter(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        //gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadMode);
    }
    else
    {
        relive_new ColourfulMeter(static_cast<relive::Path_ColourfulMeter*>(pTlv), tlvId);
    }
}

static void Factory_FlyingSligSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources();
    }
    else
    {
        relive_new FlyingSligSpawner(static_cast<relive::Path_FlyingSligSpawner*>(pTlv), tlvId);
    }
}

static void Factory_MineCar(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Closed);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Open);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Shake_A);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Shake_B);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Tread_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Tread_Move_A);
        ResourceManagerWrapper::PendAnimation(AnimId::Mine_Car_Tread_Move_B);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
    }
    else
    {
        relive_new MineCar(static_cast<relive::Path_MineCar*>(pTlv), tlvId, 0, 0, 0);
    }
}

static void Factory_BoneBag(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::BoneBag_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::BoneBag_HardHit);
        ResourceManagerWrapper::PendAnimation(AnimId::BoneBag_SoftHit);
        ResourceManagerWrapper::PendAnimation(AnimId::Bone);
    }
    else
    {
        relive_new BoneBag(static_cast<relive::Path_BoneBag*>(pTlv), tlvId);
    }
}

static void Factory_ExplosionSet(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {

        if (gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz)
        {
            ResourceManagerWrapper::PendAnimation(AnimId::FallingCrate_Falling);
            ResourceManagerWrapper::PendAnimation(AnimId::FallingCrate_Waiting);
        }
        else
        {
            ResourceManagerWrapper::PendAnimation(AnimId::AE_FallingRock_Falling);
            ResourceManagerWrapper::PendAnimation(AnimId::AE_FallingRock_Waiting);
        }
        ResourceManagerWrapper::PendAnimation(AnimId::HoistRock1);
        ResourceManagerWrapper::PendAnimation(AnimId::HoistRock2);
        ResourceManagerWrapper::PendAnimation(AnimId::HoistRock3);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Stick);
        ResourceManagerWrapper::PendAnimation(AnimId::ObjectShadow);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
    }
    else
    {
        if (!gExplosionSet)
        {
            // gExplosionSet is assigned in the ctor (OWI programming)
            relive_new ExplosionSet();
        }
        gExplosionSet->Init(static_cast<relive::Path_ExplosionSet*>(pTlv));
        Path::TLV_Reset(tlvId);
    }
}

static void Factory_MultiSwitchController(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode != relive::LoadMode::LoadResourceFromList_1 && loadmode != relive::LoadMode::LoadResource_2)
    {
        relive_new MultiSwitchController(static_cast<relive::Path_MultiSwitchController*>(pTlv), tlvId);
    }
}

static void Factory_StatusLight(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Status_Light_Green);
        ResourceManagerWrapper::PendAnimation(AnimId::Status_Light_Red);
    }
    else
    {
        relive_new StatusLight(static_cast<relive::Path_StatusLight*>(pTlv), tlvId);
    }
}

static void Factory_SlapLock(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::SlapLock_Initiate);
        ResourceManagerWrapper::PendAnimation(AnimId::SlapLock_Punched);
        ResourceManagerWrapper::PendAnimation(AnimId::SlapLock_Shaking);
    }
    else
    {
        relive_new SlapLock(static_cast<relive::Path_SlapLock*>(pTlv), tlvId);
    }
}

static void Factory_ParamiteWebLine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::ParamiteWeb);
    }
    else
    {
        relive_new ParamiteWebLine(static_cast<relive::Path_ParamiteWebLine*>(pTlv), tlvId);
    }
}

static void Factory_Alarm(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::ConstructObject_0)
    {
        relive_new Alarm(static_cast<relive::Path_Alarm*>(pTlv), tlvId);
    }
}

static void Factory_BrewMachine(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadmode)
{
    if (loadmode == relive::LoadMode::LoadResourceFromList_1 || loadmode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::BrewMachine_Button);
        ResourceManagerWrapper::PendAnimation(AnimId::Fart);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        //gMap.LoadResource("LCDFONT.FNT", ResourceManager::Resource_Font, AEResourceID::kLcdfontResID, loadmode);
    }
    else
    {
        relive_new BrewMachine(static_cast<relive::Path_BrewMachine*>(pTlv), tlvId);
    }
}

static void Factory_ScrabSpawner(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(Scrab::sScrabMotionAnimIds);
    }
    else
    {
        relive_new ScrabSpawner(static_cast<relive::Path_ScrabSpawner*>(pTlv), tlvId);
    }
}

static void Factory_CrawlingSlig(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnims(CrawlingSlig::sCrawlingSligAnimIdTable);
        ResourceManagerWrapper::PendAnims(kSligGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::Grenade);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion_Small);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
        ResourceManagerWrapper::PendAnimation(AnimId::ShootingFire_Particle);
        ResourceManagerWrapper::PendAnimation(AnimId::Vaporize_Particle);
        ResourceManagerWrapper::PendAnims(kSlogGibs);
    }
    else
    {
        relive_new CrawlingSlig(static_cast<relive::Path_CrawlingSlig*>(pTlv), tlvId);
    }
}

static void Factory_SligGetPants(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadWalkingSligResources();
        ResourceManagerWrapper::PendAnimation(AnimId::CrawlingSligLocker_Closed);
        ResourceManagerWrapper::PendAnimation(AnimId::CrawlingSligLocker_Open);
    }
    else
    {
        relive_new SligGetPantsAndWings(pTlv, tlvId);
    }
}

static void Factory_SligGetWings(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        LoadFlyingSligResources();
        ResourceManagerWrapper::PendAnimation(AnimId::CrawlingSligLocker_Closed);
        ResourceManagerWrapper::PendAnimation(AnimId::CrawlingSligLocker_Open);
    }
    else
    {
        relive_new SligGetPantsAndWings(pTlv, tlvId);
    }
}

static void Factory_Greeter(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::MotionDetector_Flare);
        ResourceManagerWrapper::PendAnimation(AnimId::MotionDetector_Laser);
        ResourceManagerWrapper::PendAnimation(AnimId::Greeter_Moving);
        ResourceManagerWrapper::PendAnimation(AnimId::Greeter_Turn);
        ResourceManagerWrapper::PendAnimation(AnimId::Greeter_Hit);
        ResourceManagerWrapper::PendAnimation(AnimId::Greeter_Speak);
        ResourceManagerWrapper::PendAnimation(AnimId::Greeter_Chase);
        ResourceManagerWrapper::PendAnimation(AnimId::Greeter_Falling);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Blue);
        ResourceManagerWrapper::PendAnimation(AnimId::Zap_Line_Red);
        ResourceManagerWrapper::PendAnimation(AnimId::Metal_Gib);
        ResourceManagerWrapper::PendAnimation(AnimId::AirExplosion);
        ResourceManagerWrapper::PendAnims(kAbeGibs);
    }
    else
    {
        relive_new Greeter(static_cast<relive::Path_Greeter*>(pTlv), tlvId);
    }
}

static void Factory_CrawlingSligButton(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::CrawlingSligButton);
        ResourceManagerWrapper::PendAnimation(AnimId::CrawlingSligButtonUse);
    }
    else
    {
        relive_new CrawlingSligButton(static_cast<relive::Path_CrawlingSligButton*>(pTlv), tlvId);
    }
}

static void Factory_GlukkonSwitch(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Door_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Security_Door_Speak);
    }
    else
    {
        relive_new GlukkonSwitch(static_cast<relive::Path_GlukkonSwitch*>(pTlv), tlvId);
    }
}

static void Factory_DoorBlocker(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Door_Lock_Idle);
        ResourceManagerWrapper::PendAnimation(AnimId::Door_Lock_Open);
    }
    else
    {
        relive_new DoorBlocker(static_cast<relive::Path_DoorBlocker*>(pTlv), tlvId);
    }
}

static void Factory_RollingBallStopper(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Stone_Ball_Stopper);
    }
    else
    {
        relive_new RollingBallStopper(static_cast<relive::Path_RollingBallStopper*>(pTlv), tlvId);
    }
}

static void Factory_TorturedMudokon(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (!pTlv->mTlvSpecificMeaning)
    {
        if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
        {
            ResourceManagerWrapper::PendAnimation(AnimId::Tortured_Mudokon);
            ResourceManagerWrapper::PendAnimation(AnimId::Tortured_Mudokon_Released);
            ResourceManagerWrapper::PendAnimation(AnimId::Tortured_Mudokon_Zap);
            ResourceManagerWrapper::PendAnimation(AnimId::Tortured_Mudokon_Tears);
            ResourceManagerWrapper::PendAnimation(AnimId::Electric_Wall);
        }
        else
        {
            relive_new TorturedMudokon(static_cast<relive::Path_TorturedMudokon*>(pTlv), tlvId);
        }
    }
}

static void Factory_TrainDoor(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Door_Train_Closed);
        ResourceManagerWrapper::PendAnimation(AnimId::Door_Train_Closing);
    }
    else
    {
        relive_new TrainDoor(static_cast<relive::Path_TrainDoor*>(pTlv), tlvId);
    }
}

static void Factory_RollingBall(relive::Path_TLV* pTlv, const Guid& tlvId, relive::LoadMode loadMode)
{
    if (loadMode == relive::LoadMode::LoadResourceFromList_1 || loadMode == relive::LoadMode::LoadResource_2)
    {
        ResourceManagerWrapper::PendAnimation(AnimId::Stone_Ball);
        ResourceManagerWrapper::PendAnimation(AnimId::Stone_Ball_Rolling);
        ResourceManagerWrapper::PendAnimation(AnimId::Explosion_Rock);
    }
    else
    {
        relive_new RollingBall(static_cast<relive::Path_RollingBall*>(pTlv), tlvId);
    }
}

} // namespace AE

namespace relive
{
void ConstructTLVObject(relive::Path_TLV* pTlv, const Guid& tlvInfo, relive::LoadMode loadMode)
{
    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eBackgroundAnimation:
            if (GetGameType() == GameType::eAo)
            AO::Factory_BackgroundAnimation(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_BackgroundAnimation(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLiftMover:
            if (GetGameType() == GameType::eAo)
            AO::Factory_LiftMover(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_LiftMover(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTimedMine:
            if (GetGameType() == GameType::eAo)
            AO::Factory_TimedMine(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_TimedMine(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGasCountDown:
            if (GetGameType() == GameType::eAo)    
            AO::Factory_GasCountDown(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_GasCountdown(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDoor:
            //if (GetGameType() == GameType::eAo)
            //AO::Factory_Door(pTlv, tlvInfo, loadMode);
            //else
            AE::Factory_Door(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eElectricWall:
            if (GetGameType() == GameType::eAo)
            AO::Factory_ElectricWall(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_ElectricWall(pTlv, tlvInfo, loadMode);
            break;            
        case ReliveTypes::eSecurityClaw:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SecurityClaw(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_ChantSuppressor_WithArms(pTlv, tlvInfo, loadMode);
            break;            
        case ReliveTypes::eBoomMachine:
            if (GetGameType() == GameType::eAo)
            AO::Factory_BoomMachine(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_BoomMachine(pTlv, tlvInfo, loadMode);
            break;           
        case ReliveTypes::eMudokon:
            if (GetGameType() == GameType::eAo)
            AO::Factory_WorkerMudokon(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Mudokon(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLiftPoint:
            if (GetGameType() == GameType::eAo)
            AO::Factory_LiftPoint(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_LiftPoint(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSecurityOrb:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SecurityOrb(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_SecurityOrb(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMeatSack:
            if (GetGameType() == GameType::eAo)
            AO::Factory_MeatSack(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_MeatSack(pTlv, tlvInfo, loadMode);
            break;           
        case ReliveTypes::eMine:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Mine(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Mine(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMotionDetector:
            if (GetGameType() == GameType::eAo)
            AO::Factory_MotionDector(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_MotionDetector(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRollingBallStopper:
            if (GetGameType() == GameType::eAo)
            AO::Factory_RollingBallStopper(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_RollingBallStopper(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eParamite:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Paramite(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Paramite(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBirdPortal:
            if (GetGameType() == GameType::eAo)
            AO::Factory_BirdPortal(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_BirdPortal(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::ePullRingRope:
            if (GetGameType() == GameType::eAo)
            AO::Factory_PullRingRope(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_PullRingRope(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRockSack:
            if (GetGameType() == GameType::eAo)
            AO::Factory_RockSack(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_RockSack(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRollingBall:
            if (GetGameType() == GameType::eAo)
            AO::Factory_RollingBall(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_RollingBall(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eScrab:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Scrab(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Scrab(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlig:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Slig(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Slig(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlog:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Slog(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Slog(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligSpawner:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SligSpawner(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_SligSpawner(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLever:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Lever(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Lever(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTrapDoor:
            if (GetGameType() == GameType::eAo)
            AO::Factory_TrapDoor(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_TrapDoor(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eUXB:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Uxb(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_UXB(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMusicTrigger:
            if (GetGameType() == GameType::eAo)
            AO::Factory_MusicTrigger(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_MusicTrigger(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eWellLocal:
        case ReliveTypes::eWellExpress:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Well(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Well(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eZzzSpawner:
            if (GetGameType() == GameType::eAo)
            AO::Factory_ZzzSpawner(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_ZzzSpawner(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHoist:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Hoist(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Hoist(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eEdge:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Edge(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Edge(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eShadowZone:
            if (GetGameType() == GameType::eAo)
            AO::Factory_ShadowZone(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_ShadowZone(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDove:
            if (GetGameType() == GameType::eAo)
            AO::Factory_Dove(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_Dove(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFallingItem:
            if (GetGameType() == GameType::eAo)
            AO::Factory_FallingItem(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_FallingItem(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eAbeStart:
            if (GetGameType() == GameType::eAo)
            AO::Factory_AbeStart(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_AbeStart(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFootSwitch:
            if (GetGameType() == GameType::eAo)
            AO::Factory_FootSwitch(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_FootSwitch(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligBoundRight:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SligBoundRight(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_SligBoundRight(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligBoundLeft:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SligBoundLeft(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_SligBoundLeft(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGasEmitter:
            if (GetGameType() == GameType::eAo)
            AO::Factory_GasEmitter(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_GasEmitter(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlogSpawner:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SlogSpawner(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_SlogSpawner(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLCDStatusBoard:
            if (GetGameType() == GameType::eAo)
            AO::Factory_LCDStatusBoard(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_LCDStatusBoard(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eCreditsController:
            if (GetGameType() == GameType::eAo)
            AO::Factory_CreditsController(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_CreditsController(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHandStone:
            if (GetGameType() == GameType::eAo)
            AO::Factory_HandStone(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_HandStone(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSecurityDoor:
            if (GetGameType() == GameType::eAo)
            AO::Factory_SecurityDoor(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_SecurityDoor(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMainMenuController:
            if (GetGameType() == GameType::eAo)
            AO::Factory_MainMenuController(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_MainMenuController(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMovingBomb:
            if (GetGameType() == GameType::eAo)
            AO::Factory_MovingBomb(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_MovingBomb(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDoorFlame:
            // TODO: Almost exactly the same - can be merged
            if (GetGameType() == GameType::eAo)
            AO::Factory_DoorFlame(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_DoorFlame(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eInvisibleSwitch:
            if (GetGameType() == GameType::eAo)
            AO::Factory_InvisibleSwitch(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_InvisibleSwitch(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTimerTrigger:
            if (GetGameType() == GameType::eAo)
            AO::Factory_TimerTrigger(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_TimerTrigger(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLCDScreen:
            if (GetGameType() == GameType::eAo)
            AO::Factory_LCDScreen(pTlv, tlvInfo, loadMode);
            else
            AE::Factory_LCDScreen(pTlv, tlvInfo, loadMode);
            break;

        // AO only currently
        case ReliveTypes::eRingCancel:
            AO::Factory_RingCancel(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::SlingMud:
            AO::Factory_SlingMud(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eKillUnsavedMuds:
            AO::Factory_KillUnsavedMuds(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBat:
            AO::Factory_Bat(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eChimeLock:
            AO::Factory_ChimeLock(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eElum:
            AO::Factory_Elum(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBellHammer:
            AO::Factory_BellHammer(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFlintLockFire:
            AO::Factory_FlintLockFire(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBackgroundGlukkon:
            AO::Factory_BackgroundGlukkon(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHoneySack:
            AO::Factory_HoneySack(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHoney:
            AO::Factory_Honey(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMeatSaw:
            AO::Factory_MeatSaw(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eZBall:
            AO::Factory_ZBall(pTlv, tlvInfo, loadMode);
            break;          
        case ReliveTypes::eElumWall:
            AO::Factory_ElumWall(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBeeNest:
            AO::Factory_BeeNest(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eRingMudokon:
            AO::Factory_RingMud(pTlv, tlvInfo, loadMode);
            break;        
        case ReliveTypes::eLiftMudokon:
            AO::Factory_LiftMud(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBeeSwarmHole:
            AO::Factory_BeeSwarmHole(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLightEffect:
            AO::Factory_LightEffect(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSwitchStateBooleanLogic:
            AO::Factory_SwitchStateBooleanLogic(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eHintFly:
            AO::Factory_HintFly(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBellSongStone:
            AO::Factory_BellSongStone(pTlv, tlvInfo, loadMode);
            break;

        // AE only currently
        case ReliveTypes::eMovieHandStone:
            AE::Factory_MovieHandStone(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eWheelSyncer:
            AE::Factory_WheelSyncer(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGlukkon:
            AE::Factory_Glukkon(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eWater:
            AE::Factory_Water(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eWorkWheel:
            AE::Factory_WorkWheel(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLaughingGas:
            AE::Factory_LaughingGas(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFlyingSlig:
            AE::Factory_FlyingSlig(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFleech:
            AE::Factory_Fleech(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlurg:
            AE::Factory_Slurg(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlamDoor:
            AE::Factory_SlamDoor(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eLevelLoader:
            AE::Factory_LevelLoader(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDemoSpawnPoint:
            AE::Factory_DemoSpawnPoint(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTeleporter:
            AE::Factory_Teleporter(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlurgSpawner:
            AE::Factory_SlurgSpawner(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDrill:
            AE::Factory_Drill(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eColourfulMeter:
            AE::Factory_ColourfulMeter(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eFlyingSligSpawner:
            AE::Factory_FlyingSligSpawner(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMineCar:
            AE::Factory_MineCar(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBoneBag:
            AE::Factory_BoneBag(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eExplosionSet:
            AE::Factory_ExplosionSet(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eMultiSwitchController:
            AE::Factory_MultiSwitchController(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eStatusLight:
            AE::Factory_StatusLight(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSlapLock:
            AE::Factory_SlapLock(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eParamiteWebLine:
            AE::Factory_ParamiteWebLine(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eAlarm:
            AE::Factory_Alarm(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eBrewMachine:
            AE::Factory_BrewMachine(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eScrabSpawner:
            AE::Factory_ScrabSpawner(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eCrawlingSlig:
            AE::Factory_CrawlingSlig(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligGetPants:
            AE::Factory_SligGetPants(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eSligGetWings:
            AE::Factory_SligGetWings(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGreeter:
            AE::Factory_Greeter(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eCrawlingSligButton:
            AE::Factory_CrawlingSligButton(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eGlukkonSwitch:
            AE::Factory_GlukkonSwitch(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eDoorBlocker:
            AE::Factory_DoorBlocker(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTorturedMud:
            AE::Factory_TorturedMudokon(pTlv, tlvInfo, loadMode);
            break;
        case ReliveTypes::eTrainDoor:
            AE::Factory_TrainDoor(pTlv, tlvInfo, loadMode);
            break;

    }
}

} 

