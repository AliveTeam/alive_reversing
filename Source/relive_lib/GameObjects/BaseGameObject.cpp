#include "stdafx.h"
#include "BaseGameObject.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAO/Map.hpp"
#include "ResourceManagerWrapper.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "GameType.hpp"
#include "FatalError.hpp"
#include "../../AliveLibAO/Abe.hpp"
#include "../../AliveLibAE/Abe.hpp"

DynamicArrayT<BaseGameObject>* gBaseGameObjects = nullptr;

BaseGameObject::BaseGameObject(s16 bAddToObjectList, s16 resourceArraySize)
    : mBaseGameObjectResArray(resourceArraySize)
{
    // Pre-allocate the array with nullptr entries so that it can be
    // used like a fixed size array in the derived type.
    // TODO: If its never used dynamically in the derived type then it actually can become a fixed
    // compile time sized array at some point.
    for (s32 i = 0; i < resourceArraySize; i++)
    {
        mBaseGameObjectResArray.Push_Back(nullptr);
    }

    SetType(ReliveTypes::eNone);

    mListAddFailed = false;
    mDead = false;
    mIsBaseAnimatedWithPhysicsObj = false;
    mIsBaseAliveGameObject = false;
    mCanExplode = false;
    mInteractive = false;
    mSurviveDeathReset = false;
    mUpdateDuringCamSwap = false;
    mCantKill = false;
    mUpdatable = true;

    if (bAddToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            SetListAddFailed(true);
        }
    }

    Guid nextId = Guid::NewGuid();
    mBaseGameObjectTlvInfo = nextId;
    mBaseGameObjectId = nextId;
    sObjectIds.Insert(nextId, this);
}

BaseGameObject::~BaseGameObject()
{
    EventCancelForObj(this);

    sObjectIds.Remove(mBaseGameObjectId);
}

void BaseGameObject::VUpdate()
{

}

void BaseGameObject::VRender(OrderingTable& /*ot*/)
{

}

IMap& GetMap()
{
    if (GetGameType() == GameType::eAe)
    {
        return gMap;
    }
    else
    {
        return AO::gMap;
    }
}

BaseAliveGameObject* GetAbe()
{
    if (GetGameType() == GameType::eAe)
    {
        return gAbe;
    }
    else
    {
        return AO::gAbe;
    }
}

void BaseGameObject::VScreenChanged()
{
    auto& map = GetMap();
    if (map.LevelChanged() || map.PathChanged())
    {
        SetDead(true);
    }
}

void BaseGameObject::VStopAudio()
{

}

void BaseGameObject::VGetSaveState(SerializedObjectData&)
{
    // Not used in AO yet so needs a default impl
}

ReliveTypes BaseGameObject::FromAO(AOTypes aoType)
{
    switch (aoType)
    {
        case AOTypes::eNone_0:
            return ReliveTypes::eNone;
        case AOTypes::eAlarm_1:
            return ReliveTypes::eAlarm;
        case AOTypes::eScreenManager_4:
            return ReliveTypes::eScreenManager;
        case AOTypes::eBackgroundAnimation_5:
            return ReliveTypes::eBackgroundAnimation;
        case AOTypes::eBat_6:
            return ReliveTypes::eBat;
        case AOTypes::eLiftMover_7:
            return ReliveTypes::eLiftMover;
        case AOTypes::eTimedMine_8:
            return ReliveTypes::eTimedMine;
        case AOTypes::eBullet_10:
            return ReliveTypes::eBullet;
        case AOTypes::eDDCheat_12:
            return ReliveTypes::eDDCheat;
        case AOTypes::eBells_13:
            return ReliveTypes::eBells;
        case AOTypes::eChimeLock_14:
            return ReliveTypes::eChimeLock;
        case AOTypes::eZapSpark_15:
            return ReliveTypes::eZapSpark;
        case AOTypes::eGasClock_16:
            return ReliveTypes::eGasCountDown;
        case AOTypes::eParticleBurst_19:
            return ReliveTypes::eParticleBurst;
        case AOTypes::eDoor_21:
            return ReliveTypes::eDoor;
        case AOTypes::eDove_22:
            return ReliveTypes::eDove;
        case AOTypes::eGameSpeak_24:
            return ReliveTypes::eGameSpeak;
        case AOTypes::eElectricWall_25:
            return ReliveTypes::eElectricWall;
        case AOTypes::eElum_26:
            return ReliveTypes::eElum;
        case AOTypes::eBellHammer_27:
            return ReliveTypes::eBellHammer;
        case AOTypes::ePalOverwriter_29:
            return ReliveTypes::ePalOverwriter;
        case AOTypes::eBaseBomb_30:
            return ReliveTypes::eGroundExplosion;
        case AOTypes::eSecurityClaw_31:
            return ReliveTypes::eSecurityClaw;
        case AOTypes::eRockSpawner_32:
            return ReliveTypes::eRockSpawner;
        case AOTypes::eFlintLockFire_34:
            return ReliveTypes::eFlintLockFire;
        case AOTypes::eThrowableTotalIndicator_35:
            return ReliveTypes::eThrowableTotalIndicator;
        case AOTypes::eFootSwitch36:
            return ReliveTypes::eFootSwitch;
        case AOTypes::eGameEnderController_37:
            return ReliveTypes::eGameEnderController;
        case AOTypes::eDeathBird_38:
            return ReliveTypes::eDeathBird;
        case AOTypes::eLoadingFile_39:
            return ReliveTypes::eLoadingFile;
        case AOTypes::eGrenade_40:
            return ReliveTypes::eGrenade;
        case AOTypes::eGrenadeMachine_41:
            return ReliveTypes::eBoomMachine;
        case AOTypes::eBackgroundGlukkon_42:
            return ReliveTypes::eBackgroundGlukkon;
        case AOTypes::eAbe_43:
            return ReliveTypes::eAbe;
        case AOTypes::MainMenuFade_44:
            return ReliveTypes::MainMenuFade;
        case AOTypes::eHoneySack_45:
            return ReliveTypes::eHoneySack;
        case AOTypes::eHoney_47:
            return ReliveTypes::eHoney;
        case AOTypes::eClawOrBirdPortalTerminator_48:
            return ReliveTypes::eClawOrBirdPortalTerminator;
        case AOTypes::eRingOrLiftMud_52:
            return ReliveTypes::eRingOrLiftMud;
        case AOTypes::eLiftPoint_51:
            return ReliveTypes::eLiftPoint;
        case AOTypes::SecurityOrb_53:
            return ReliveTypes::eSecurityOrb;
        case AOTypes::eMeat_54:
            return ReliveTypes::eMeat;
        case AOTypes::eMeatStack_55:
            return ReliveTypes::eMeatSack;
        case AOTypes::eMeatSaw_56:
            return ReliveTypes::eMeatSaw;
        case AOTypes::eMine_57:
            return ReliveTypes::eMine;
        case AOTypes::eRollingBallStopperShaker_58:
            return ReliveTypes::eRollingBallStopperShaker;
        case AOTypes::eMotionDetector_59:
            return ReliveTypes::eMotionDetector;
        case AOTypes::eRollingBallStopper_60:
            return ReliveTypes::eRollingBallStopper;
        case AOTypes::ePauseMenu_61:
            return ReliveTypes::ePauseMenu;
        case AOTypes::eParamite_62:
            return ReliveTypes::eParamite;
        case AOTypes::eDemoPlayback:
            return ReliveTypes::eDemoPlayback;
        case AOTypes::eBirdPortal_65:
            return ReliveTypes::eBirdPortal;
        case AOTypes::eBirdPortalTerminator_66:
            return ReliveTypes::eBirdPortalTerminator;
        case AOTypes::eFG1_67:
            return ReliveTypes::eFG1;
        case AOTypes::ePullRingRope_68:
            return ReliveTypes::ePullRingRope;
        case AOTypes::eAbilityRing_69:
            return ReliveTypes::eAbilityRing;
        case AOTypes::eRock_70:
            return ReliveTypes::eRock;
        case AOTypes::eRockSack_71:
            return ReliveTypes::eRockSack;
        case AOTypes::eRollingBall_72:
            return ReliveTypes::eRollingBall;
        case AOTypes::eRope_73:
            return ReliveTypes::eRope;
        case AOTypes::eExplosion_74:
            return ReliveTypes::eAirExplosion;
        case AOTypes::eMudokon_75:
            return ReliveTypes::eMudokon;
        case AOTypes::eRedLaser_76:
            return ReliveTypes::eRedLaser;
        case AOTypes::eScrab_77:
            return ReliveTypes::eScrab;
        case AOTypes::eScreenClipper_78:
            return ReliveTypes::eScreenClipper;
        case AOTypes::eEffectBase_79:
            return ReliveTypes::eEffectBase;
        case AOTypes::eDeathFadeOut_80:
            return ReliveTypes::eFade;
        case AOTypes::eFlash_81:
            return ReliveTypes::eFlash;
        case AOTypes::ScreenShake_82:
            return ReliveTypes::eScreenShake;
        case AOTypes::eScreenWave_83:
            return ReliveTypes::eScreenWave;
        case AOTypes::eUnknown_84:
            return ReliveTypes::eUnknown;
        case AOTypes::eShrykull_85:
            return ReliveTypes::eShrykull;
        case AOTypes::eSnoozParticle_87:
            return ReliveTypes::eSnoozeParticle;
        case AOTypes::eSlig_88:
            return ReliveTypes::eSlig;
        case AOTypes::eSlog_89:
            return ReliveTypes::eSlog;
        case AOTypes::SlingMud_90:
            return ReliveTypes::SlingMud;
        case AOTypes::eSligSpawner_91:
            return ReliveTypes::eSligSpawner;
        case AOTypes::eZBall_92:
            return ReliveTypes::eZBall;
        case AOTypes::eParticle_93:
            return ReliveTypes::eParticle;
        case AOTypes::eZapLine_94:
            return ReliveTypes::eZapLine;
        case AOTypes::eBeeSwarm_95:
            return ReliveTypes::eBeeSwarm;
        case AOTypes::eBeeNest_96:
            return ReliveTypes::eBeeNest;
        case AOTypes::eLever_97:
            return ReliveTypes::eLever;
        case AOTypes::eTrapDoor_98:
            return ReliveTypes::eTrapDoor;
        case AOTypes::eUXB_99:
            return ReliveTypes::eUXB;
        case AOTypes::eMovie_100:
            return ReliveTypes::eMovie;
        case AOTypes::eWell_101:
            return ReliveTypes::eWell;
        case AOTypes::eCameraSwapper_102:
            return ReliveTypes::eCameraSwapper;
        case AOTypes::eElectrocute_103:
            return ReliveTypes::eElectrocute;
        default:
            ALIVE_FATAL("Unknown AO type");
    }
}

AOTypes BaseGameObject::ToAO(ReliveTypes reliveType)
{
    switch (reliveType)
    {
        case ReliveTypes::eNone:
            return AOTypes::eNone_0;
        case ReliveTypes::eAlarm:
            return AOTypes::eAlarm_1;
        case ReliveTypes::eScreenManager:
            return AOTypes::eScreenManager_4;
        case ReliveTypes::eBackgroundAnimation:
            return AOTypes::eBackgroundAnimation_5;
        case ReliveTypes::eBat:
            return AOTypes::eBat_6;
        case ReliveTypes::eLiftMover:
            return AOTypes::eLiftMover_7;
        case ReliveTypes::eTimedMine:
            return AOTypes::eTimedMine_8;
        case ReliveTypes::eBullet:
            return AOTypes::eBullet_10;
        case ReliveTypes::eDDCheat:
            return AOTypes::eDDCheat_12;
        case ReliveTypes::eBells:
            return AOTypes::eBells_13;
        case ReliveTypes::eChimeLock:
            return AOTypes::eChimeLock_14;
        case ReliveTypes::eZapSpark:
            return AOTypes::eZapSpark_15;
        case ReliveTypes::eGasCountDown:
            return AOTypes::eGasClock_16;
        case ReliveTypes::eParticleBurst:
            return AOTypes::eParticleBurst_19;
        case ReliveTypes::eDoor:
            return AOTypes::eDoor_21;
        case ReliveTypes::eDove:
            return AOTypes::eDove_22;
        case ReliveTypes::eGameSpeak:
            return AOTypes::eGameSpeak_24;
        case ReliveTypes::eElectricWall:
            return AOTypes::eElectricWall_25;
        case ReliveTypes::eElum:
            return AOTypes::eElum_26;
        case ReliveTypes::eBellHammer:
            return AOTypes::eBellHammer_27;
        case ReliveTypes::ePalOverwriter:
            return AOTypes::ePalOverwriter_29;
        case ReliveTypes::eGroundExplosion:
            return AOTypes::eBaseBomb_30;
        case ReliveTypes::eSecurityClaw:
            return AOTypes::eSecurityClaw_31;
        case ReliveTypes::eRockSpawner:
            return AOTypes::eRockSpawner_32;
        case ReliveTypes::eFlintLockFire:
            return AOTypes::eFlintLockFire_34;
        case ReliveTypes::eThrowableTotalIndicator:
            return AOTypes::eThrowableTotalIndicator_35;
        case ReliveTypes::eFootSwitch:
            return AOTypes::eFootSwitch36;
        case ReliveTypes::eGameEnderController:
            return AOTypes::eGameEnderController_37;
        case ReliveTypes::eDeathBird:
            return AOTypes::eDeathBird_38;
        case ReliveTypes::eLoadingFile:
            return AOTypes::eLoadingFile_39;
        case ReliveTypes::eGrenade:
            return AOTypes::eGrenade_40;
        case ReliveTypes::eBoomMachine:
            return AOTypes::eGrenadeMachine_41;
        case ReliveTypes::eBackgroundGlukkon:
            return AOTypes::eBackgroundGlukkon_42;
        case ReliveTypes::eAbe:
            return AOTypes::eAbe_43;
        case ReliveTypes::MainMenuFade:
            return AOTypes::MainMenuFade_44;
        case ReliveTypes::eHoneySack:
            return AOTypes::eHoneySack_45;
        case ReliveTypes::eHoney:
            return AOTypes::eHoney_47;
        case ReliveTypes::eClawOrBirdPortalTerminator:
            return AOTypes::eClawOrBirdPortalTerminator_48;
        case ReliveTypes::eRingOrLiftMud:
            return AOTypes::eRingOrLiftMud_52;
        case ReliveTypes::eLiftPoint:
            return AOTypes::eLiftPoint_51;
        case ReliveTypes::eSecurityOrb:
            return AOTypes::SecurityOrb_53;
        case ReliveTypes::eMeat:
            return AOTypes::eMeat_54;
        case ReliveTypes::eMeatSack:
            return AOTypes::eMeatStack_55;
        case ReliveTypes::eMeatSaw:
            return AOTypes::eMeatSaw_56;
        case ReliveTypes::eMine:
            return AOTypes::eMine_57;
        case ReliveTypes::eRollingBallStopperShaker:
            return AOTypes::eRollingBallStopperShaker_58;
        case ReliveTypes::eMotionDetector:
            return AOTypes::eMotionDetector_59;
        case ReliveTypes::eRollingBallStopper:
            return AOTypes::eRollingBallStopper_60;
        case ReliveTypes::ePauseMenu:
            return AOTypes::ePauseMenu_61;
        case ReliveTypes::eParamite:
            return AOTypes::eParamite_62;
        case ReliveTypes::eDemoPlayback:
            return AOTypes::eDemoPlayback;
        case ReliveTypes::eBirdPortal:
            return AOTypes::eBirdPortal_65;
        case ReliveTypes::eBirdPortalTerminator:
            return AOTypes::eBirdPortalTerminator_66;
        case ReliveTypes::eFG1:
            return AOTypes::eFG1_67;
        case ReliveTypes::ePullRingRope:
            return AOTypes::ePullRingRope_68;
        case ReliveTypes::eAbilityRing:
            return AOTypes::eAbilityRing_69;
        case ReliveTypes::eRock:
            return AOTypes::eRock_70;
        case ReliveTypes::eRockSack:
            return AOTypes::eRockSack_71;
        case ReliveTypes::eRollingBall:
            return AOTypes::eRollingBall_72;
        case ReliveTypes::eRope:
            return AOTypes::eRope_73;
        case ReliveTypes::eAirExplosion:
            return AOTypes::eExplosion_74;
        case ReliveTypes::eMudokon:
            return AOTypes::eMudokon_75;
        case ReliveTypes::eRedLaser:
            return AOTypes::eRedLaser_76;
        case ReliveTypes::eScrab:
            return AOTypes::eScrab_77;
        case ReliveTypes::eScreenClipper:
            return AOTypes::eScreenClipper_78;
        case ReliveTypes::eEffectBase:
            return AOTypes::eEffectBase_79;
        case ReliveTypes::eFade:
            return AOTypes::eDeathFadeOut_80;
        case ReliveTypes::eFlash:
            return AOTypes::eFlash_81;
        case ReliveTypes::eScreenShake:
            return AOTypes::ScreenShake_82;
        case ReliveTypes::eScreenWave:
            return AOTypes::eScreenWave_83;
        case ReliveTypes::eUnknown:
            return AOTypes::eUnknown_84;
        case ReliveTypes::eShrykull:
            return AOTypes::eShrykull_85;
        case ReliveTypes::eSnoozeParticle:
            return AOTypes::eSnoozParticle_87;
        case ReliveTypes::eSlig:
            return AOTypes::eSlig_88;
        case ReliveTypes::eSlog:
            return AOTypes::eSlog_89;
        case ReliveTypes::SlingMud:
            return AOTypes::SlingMud_90;
        case ReliveTypes::eSligSpawner:
            return AOTypes::eSligSpawner_91;
        case ReliveTypes::eZBall:
            return AOTypes::eZBall_92;
        case ReliveTypes::eParticle:
            return AOTypes::eParticle_93;
        case ReliveTypes::eZapLine:
            return AOTypes::eZapLine_94;
        case ReliveTypes::eBeeSwarm:
            return AOTypes::eBeeSwarm_95;
        case ReliveTypes::eBeeNest:
            return AOTypes::eBeeNest_96;
        case ReliveTypes::eLever:
            return AOTypes::eLever_97;
        case ReliveTypes::eTrapDoor:
            return AOTypes::eTrapDoor_98;
        case ReliveTypes::eUXB:
            return AOTypes::eUXB_99;
        case ReliveTypes::eMovie:
            return AOTypes::eMovie_100;
        case ReliveTypes::eWell:
            return AOTypes::eWell_101;
        case ReliveTypes::eCameraSwapper:
            return AOTypes::eCameraSwapper_102;
        case ReliveTypes::eElectrocute:
            return AOTypes::eElectrocute_103;
        default:
            ALIVE_FATAL("No AO type for this relive type");
    }
}

ReliveTypes BaseGameObject::FromAE(AETypes aeType)
{
    switch (aeType)
    {
        case AETypes::eNone_0:
            return ReliveTypes::eNone;
        case AETypes::eAlarm_1:
            return ReliveTypes::eAlarm;
        case AETypes::eSligSpawner_2:
            return ReliveTypes::eSligSpawner;
        case AETypes::eScreenManager_6:
            return ReliveTypes::eScreenManager;
        case AETypes::eBackgroundAnimation_7:
            return ReliveTypes::eBackgroundAnimation;
        case AETypes::eLiftMover_9:
            return ReliveTypes::eLiftMover;
        case AETypes::eTimedMine_or_MovingBomb_10:
            return ReliveTypes::eTimedMine_or_MovingBomb;
        case AETypes::eBone_11:
            return ReliveTypes::eBone;
        case AETypes::eBoneBag_12:
            return ReliveTypes::eBoneBag;
        case AETypes::eBrewMachine_13:
            return ReliveTypes::eBrewMachine;
        case AETypes::eBullet_15:
            return ReliveTypes::eBullet;
        case AETypes::eSligButton_16:
            return ReliveTypes::eSligButton;
        case AETypes::eExplosionSet_18:
            return ReliveTypes::eExplosionSet;
        case AETypes::eDDCheat_19:
            return ReliveTypes::eDDCheat;
        case AETypes::eZapSpark_22:
            return ReliveTypes::eZapSpark;
        case AETypes::eGasClock_23:
            return ReliveTypes::eGasCountDown;
        case AETypes::eMetal_24:
            return ReliveTypes::eMetal;
        case AETypes::eMinesAlarm_25:
            return ReliveTypes::eMinesAlarm;
        case AETypes::eCrawlingSlig_26:
            return ReliveTypes::eCrawlingSlig;
        case AETypes::eParticleBurst_29:
            return ReliveTypes::eParticleBurst;
        case AETypes::eDrill_30:
            return ReliveTypes::eDrill;
        case AETypes::eLaughingGas_31:
            return ReliveTypes::eLaughingGas;
        case AETypes::eDoor_33:
            return ReliveTypes::eDoor;
        case AETypes::eDoorLock_34:
            return ReliveTypes::eDoorLock;
        case AETypes::eDove_35:
            return ReliveTypes::eDove;
        case AETypes::eGameSpeak_38:
            return ReliveTypes::eGameSpeak;
        case AETypes::eElectricWall_39:
            return ReliveTypes::eElectricWall;
        case AETypes::ePalOverwriter_44:
            return ReliveTypes::ePalOverwriter;
        case AETypes::eEvilFart_45:
            return ReliveTypes::eEvilFart;
        case AETypes::eBaseBomb_46:
            return ReliveTypes::eGroundExplosion;
        case AETypes::eSecurityClaw_47:
            return ReliveTypes::eSecurityClaw;
        case AETypes::eRockSpawner_48:
            return ReliveTypes::eRockSpawner;
        case AETypes::eFleech_50:
            return ReliveTypes::eFleech;
        case AETypes::ePossessionFlicker_51:
            return ReliveTypes::ePossessionFlicker;
        case AETypes::eThrowableTotalIndicator_53:
            return ReliveTypes::eThrowableTotalIndicator;
        case AETypes::eFlyingSlig_54:
            return ReliveTypes::eFlyingSlig;
        case AETypes::eFlyingSligSpawner_55:
            return ReliveTypes::eFlyingSligSpawner;
        case AETypes::eFootSwitch_56:
            return ReliveTypes::eFootSwitch;
        case AETypes::eGameEnderController_57:
            return ReliveTypes::eGameEnderController;
        case AETypes::eColourfulMeter_59:
            return ReliveTypes::eColourfulMeter;
        case AETypes::eSlapLock_OrbWhirlWind_60:
            return ReliveTypes::eSlapLock_OrbWhirlWind;
        case AETypes::eSlapLock_61:
            return ReliveTypes::eSlapLock;
        case AETypes::eDeathBird_62:
            return ReliveTypes::eDeathBird;
        case AETypes::eGreeter_64:
            return ReliveTypes::eGreeter;
        case AETypes::eGrenade_65:
            return ReliveTypes::eGrenade;
        case AETypes::eGrenadeMachine_66:
            return ReliveTypes::eBoomMachine;
        case AETypes::eGlukkon_67:
            return ReliveTypes::eGlukkon;
        case AETypes::eHelpPhone_68:
            return ReliveTypes::eHelpPhone;
        case AETypes::eAbe_69:
            return ReliveTypes::eAbe;
        case AETypes::eEyeOrbPart_74:
            return ReliveTypes::eEyeOrbPart;
        case AETypes::eInvisibleEffect_75:
            return ReliveTypes::eInvisibleEffect;
        case AETypes::ePulley_76:
            return ReliveTypes::ePulley;
        case AETypes::eLiftPoint_78:
            return ReliveTypes::eLiftPoint;
        case AETypes::eResourceManager_70:
            return ReliveTypes::eResourceManager;
        case AETypes::eSligGetPantsOrWings_80:
            return ReliveTypes::eSligGetPantsOrWings;
        case AETypes::eRingOrLiftMud_81:
            return ReliveTypes::eRingOrLiftMud;
        case AETypes::eSecurityOrb_83:
            return ReliveTypes::eSecurityOrb;
        case AETypes::eMeat_84:
            return ReliveTypes::eMeat;
        case AETypes::eMeatSack_85:
            return ReliveTypes::eMeatSack;
        case AETypes::eMeatSaw_86:
            return ReliveTypes::eMeatSaw;
        case AETypes::eText_87:
            return ReliveTypes::eText;
        case AETypes::eMine_88:
            return ReliveTypes::eMine;
        case AETypes::eMineCar_89:
            return ReliveTypes::eMineCar;
        case AETypes::eGreeterBody_91:
            return ReliveTypes::eGreeterBody;
        case AETypes::eMusicController_93:
            return ReliveTypes::eMusicController;
        case AETypes::eMusicTrigger_94:
            return ReliveTypes::eMusicTrigger;
        case AETypes::ePauseMenu_95:
            return ReliveTypes::ePauseMenu;
        case AETypes::eParamite_96:
            return ReliveTypes::eParamite;
        case AETypes::eDemoPlayback_98:
            return ReliveTypes::eDemoPlayback;
        case AETypes::eBirdPortal_99:
            return ReliveTypes::eBirdPortal;
        case AETypes::eBirdPortalTerminator_100:
            return ReliveTypes::eBirdPortalTerminator;
        case AETypes::eFG1_101:
            return ReliveTypes::eFG1;
        case AETypes::ePullRope_103:
            return ReliveTypes::ePullRingRope;
        case AETypes::eAbilityRing_104:
            return ReliveTypes::eAbilityRing;
        case AETypes::eRock_105:
            return ReliveTypes::eRock;
        case AETypes::eRockSack_106:
            return ReliveTypes::eRockSack;
        case AETypes::eRope_108:
            return ReliveTypes::eRope;
        case AETypes::eExplosion_109:
            return ReliveTypes::eAirExplosion;
        case AETypes::eMudokon_110:
            return ReliveTypes::eMudokon;
        case AETypes::eRedLaser_111:
            return ReliveTypes::eRedLaser;
        case AETypes::eScrab_112:
            return ReliveTypes::eScrab;
        case AETypes::eScrabSpawner_113:
            return ReliveTypes::eScrabSpawner;
        case AETypes::eScreenClipper_114:
            return ReliveTypes::eScreenClipper;
        case AETypes::eEffectBase_115:
            return ReliveTypes::eEffectBase;
        case AETypes::eMainMenuTransistion_116:
            return ReliveTypes::eMainMenuTransistion;
        case AETypes::eFlash_117:
            return ReliveTypes::eFlash;
        case AETypes::eScreenShake_118:
            return ReliveTypes::eScreenShake;
        case AETypes::eShrykull_121:
            return ReliveTypes::eShrykull;
        case AETypes::eSlamDoor_122:
            return ReliveTypes::eSlamDoor;
        case AETypes::eSnoozeParticle_124:
            return ReliveTypes::eSnoozeParticle;
        case AETypes::eSlig_125:
            return ReliveTypes::eSlig;
        case AETypes::eSlog_126:
            return ReliveTypes::eSlog;
        case AETypes::eSlurgSpawner_128:
            return ReliveTypes::eSlurgSpawner;
        case AETypes::eSlurg_129:
            return ReliveTypes::eSlurg;
        case AETypes::eParticle_134:
            return ReliveTypes::eParticle;
        case AETypes::eZapLine_135:
            return ReliveTypes::eZapLine;
        case AETypes::eTimerTrigger_136:
            return ReliveTypes::eTimerTrigger;
        case AETypes::eLever_139:
            return ReliveTypes::eLever;
        case AETypes::eTorturedMud_141:
            return ReliveTypes::eTorturedMud;
        case AETypes::eTrapDoor_142:
            return ReliveTypes::eTrapDoor;
        case AETypes::eUXB_143:
            return ReliveTypes::eUXB;
        case AETypes::eMovie_145:
            return ReliveTypes::eMovie;
        case AETypes::eWebLine_146:
            return ReliveTypes::eWebLine;
        case AETypes::eWell_147:
            return ReliveTypes::eWell;
        case AETypes::eWorkWheel_148:
            return ReliveTypes::eWorkWheel;
        case AETypes::eCameraSwapper_149:
            return ReliveTypes::eCameraSwapper;
        case AETypes::eElectrocute_150:
            return ReliveTypes::eElectrocute;
        default:
            ALIVE_FATAL("Unknown AE type");
    }
}

AETypes BaseGameObject::ToAE(ReliveTypes reliveType)
{
    switch (reliveType)
    {
        case ReliveTypes::eNone:
            return AETypes::eNone_0;
        case ReliveTypes::eAlarm:
            return AETypes::eAlarm_1;
        case ReliveTypes::eSligSpawner:
            return AETypes::eSligSpawner_2;
        case ReliveTypes::eScreenManager:
            return AETypes::eScreenManager_6;
        case ReliveTypes::eBackgroundAnimation:
            return AETypes::eBackgroundAnimation_7;
        case ReliveTypes::eLiftMover:
            return AETypes::eLiftMover_9;
        case ReliveTypes::eTimedMine_or_MovingBomb:
            return AETypes::eTimedMine_or_MovingBomb_10;
        case ReliveTypes::eBone:
            return AETypes::eBone_11;
        case ReliveTypes::eBoneBag:
            return AETypes::eBoneBag_12;
        case ReliveTypes::eBrewMachine:
            return AETypes::eBrewMachine_13;
        case ReliveTypes::eBullet:
            return AETypes::eBullet_15;
        case ReliveTypes::eSligButton:
            return AETypes::eSligButton_16;
        case ReliveTypes::eExplosionSet:
            return AETypes::eExplosionSet_18;
        case ReliveTypes::eDDCheat:
            return AETypes::eDDCheat_19;
        case ReliveTypes::eZapSpark:
            return AETypes::eZapSpark_22;
        case ReliveTypes::eGasCountDown:
            return AETypes::eGasClock_23;
        case ReliveTypes::eMetal:
            return AETypes::eMetal_24;
        case ReliveTypes::eMinesAlarm:
            return AETypes::eMinesAlarm_25;
        case ReliveTypes::eCrawlingSlig:
            return AETypes::eCrawlingSlig_26;
        case ReliveTypes::eParticleBurst:
            return AETypes::eParticleBurst_29;
        case ReliveTypes::eDrill:
            return AETypes::eDrill_30;
        case ReliveTypes::eLaughingGas:
            return AETypes::eLaughingGas_31;
        case ReliveTypes::eDoor:
            return AETypes::eDoor_33;
        case ReliveTypes::eDoorLock:
            return AETypes::eDoorLock_34;
        case ReliveTypes::eDove:
            return AETypes::eDove_35;
        case ReliveTypes::eGameSpeak:
            return AETypes::eGameSpeak_38;
        case ReliveTypes::eElectricWall:
            return AETypes::eElectricWall_39;
        case ReliveTypes::ePalOverwriter:
            return AETypes::ePalOverwriter_44;
        case ReliveTypes::eEvilFart:
            return AETypes::eEvilFart_45;
        case ReliveTypes::eGroundExplosion:
            return AETypes::eBaseBomb_46;
        case ReliveTypes::eSecurityClaw:
            return AETypes::eSecurityClaw_47;
        case ReliveTypes::eRockSpawner:
            return AETypes::eRockSpawner_48;
        case ReliveTypes::eFleech:
            return AETypes::eFleech_50;
        case ReliveTypes::ePossessionFlicker:
            return AETypes::ePossessionFlicker_51;
        case ReliveTypes::eThrowableTotalIndicator:
            return AETypes::eThrowableTotalIndicator_53;
        case ReliveTypes::eFlyingSlig:
            return AETypes::eFlyingSlig_54;
        case ReliveTypes::eFlyingSligSpawner:
            return AETypes::eFlyingSligSpawner_55;
        case ReliveTypes::eFootSwitch:
            return AETypes::eFootSwitch_56;
        case ReliveTypes::eGameEnderController:
            return AETypes::eGameEnderController_57;
        case ReliveTypes::eColourfulMeter:
            return AETypes::eColourfulMeter_59;
        case ReliveTypes::eSlapLock_OrbWhirlWind:
            return AETypes::eSlapLock_OrbWhirlWind_60;
        case ReliveTypes::eSlapLock:
            return AETypes::eSlapLock_61;
        case ReliveTypes::eDeathBird:
            return AETypes::eDeathBird_62;
        case ReliveTypes::eGreeter:
            return AETypes::eGreeter_64;
        case ReliveTypes::eGrenade:
            return AETypes::eGrenade_65;
        case ReliveTypes::eBoomMachine:
            return AETypes::eGrenadeMachine_66;
        case ReliveTypes::eGlukkon:
            return AETypes::eGlukkon_67;
        case ReliveTypes::eHelpPhone:
            return AETypes::eHelpPhone_68;
        case ReliveTypes::eAbe:
            return AETypes::eAbe_69;
        case ReliveTypes::eEyeOrbPart:
            return AETypes::eEyeOrbPart_74;
        case ReliveTypes::eInvisibleEffect:
            return AETypes::eInvisibleEffect_75;
        case ReliveTypes::ePulley:
            return AETypes::ePulley_76;
        case ReliveTypes::eLiftPoint:
            return AETypes::eLiftPoint_78;
        case ReliveTypes::eResourceManager:
            return AETypes::eResourceManager_70;
        case ReliveTypes::eSligGetPantsOrWings:
            return AETypes::eSligGetPantsOrWings_80;
        case ReliveTypes::eRingOrLiftMud:
            return AETypes::eRingOrLiftMud_81;
        case ReliveTypes::eSecurityOrb:
            return AETypes::eSecurityOrb_83;
        case ReliveTypes::eMeat:
            return AETypes::eMeat_84;
        case ReliveTypes::eMeatSack:
            return AETypes::eMeatSack_85;
        case ReliveTypes::eMeatSaw:
            return AETypes::eMeatSaw_86;
        case ReliveTypes::eText:
            return AETypes::eText_87;
        case ReliveTypes::eMine:
            return AETypes::eMine_88;
        case ReliveTypes::eMineCar:
            return AETypes::eMineCar_89;
        case ReliveTypes::eGreeterBody:
            return AETypes::eGreeterBody_91;
        case ReliveTypes::eMusicController:
            return AETypes::eMusicController_93;
        case ReliveTypes::eMusicTrigger:
            return AETypes::eMusicTrigger_94;
        case ReliveTypes::ePauseMenu:
            return AETypes::ePauseMenu_95;
        case ReliveTypes::eParamite:
            return AETypes::eParamite_96;
        case ReliveTypes::eDemoPlayback:
            return AETypes::eDemoPlayback_98;
        case ReliveTypes::eBirdPortal:
            return AETypes::eBirdPortal_99;
        case ReliveTypes::eBirdPortalTerminator:
            return AETypes::eBirdPortalTerminator_100;
        case ReliveTypes::eFG1:
            return AETypes::eFG1_101;
        case ReliveTypes::ePullRingRope:
            return AETypes::ePullRope_103;
        case ReliveTypes::eAbilityRing:
            return AETypes::eAbilityRing_104;
        case ReliveTypes::eRock:
            return AETypes::eRock_105;
        case ReliveTypes::eRockSack:
            return AETypes::eRockSack_106;
        case ReliveTypes::eRope:
            return AETypes::eRope_108;
        case ReliveTypes::eAirExplosion:
            return AETypes::eExplosion_109;
        case ReliveTypes::eMudokon:
            return AETypes::eMudokon_110;
        case ReliveTypes::eRedLaser:
            return AETypes::eRedLaser_111;
        case ReliveTypes::eScrab:
            return AETypes::eScrab_112;
        case ReliveTypes::eScrabSpawner:
            return AETypes::eScrabSpawner_113;
        case ReliveTypes::eScreenClipper:
            return AETypes::eScreenClipper_114;
        case ReliveTypes::eEffectBase:
            return AETypes::eEffectBase_115;
        case ReliveTypes::eMainMenuTransistion:
            return AETypes::eMainMenuTransistion_116;
        case ReliveTypes::eFlash:
            return AETypes::eFlash_117;
        case ReliveTypes::eScreenShake:
            return AETypes::eScreenShake_118;
        case ReliveTypes::eShrykull:
            return AETypes::eShrykull_121;
        case ReliveTypes::eSlamDoor:
            return AETypes::eSlamDoor_122;
        case ReliveTypes::eSnoozeParticle:
            return AETypes::eSnoozeParticle_124;
        case ReliveTypes::eSlig:
            return AETypes::eSlig_125;
        case ReliveTypes::eSlog:
            return AETypes::eSlog_126;
        case ReliveTypes::eSlurgSpawner:
            return AETypes::eSlurgSpawner_128;
        case ReliveTypes::eSlurg:
            return AETypes::eSlurg_129;
        case ReliveTypes::eParticle:
            return AETypes::eParticle_134;
        case ReliveTypes::eZapLine:
            return AETypes::eZapLine_135;
        case ReliveTypes::eTimerTrigger:
            return AETypes::eTimerTrigger_136;
        case ReliveTypes::eLever:
            return AETypes::eLever_139;
        case ReliveTypes::eTorturedMud:
            return AETypes::eTorturedMud_141;
        case ReliveTypes::eTrapDoor:
            return AETypes::eTrapDoor_142;
        case ReliveTypes::eUXB:
            return AETypes::eUXB_143;
        case ReliveTypes::eMovie:
            return AETypes::eMovie_145;
        case ReliveTypes::eWebLine:
            return AETypes::eWebLine_146;
        case ReliveTypes::eWell:
            return AETypes::eWell_147;
        case ReliveTypes::eWorkWheel:
            return AETypes::eWorkWheel_148;
        case ReliveTypes::eCameraSwapper:
            return AETypes::eCameraSwapper_149;
        case ReliveTypes::eElectrocute:
            return AETypes::eElectrocute_150;
        case ReliveTypes::eThrowableArray:
            return AETypes::eThrowableArray_102;
        default:
            ALIVE_FATAL("No AE type for this relive type");
    }
}

void BaseGameObject::SetType(ReliveTypes type)
{
    mBaseGameObjectTypeId = type;
}

ReliveTypes BaseGameObject::Type() const
{
    return mBaseGameObjectTypeId;
}

Guid BaseGameObject::RefreshId(const Guid& objectId)
{
    static Guid nullGuid;
    if (objectId != nullGuid)
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseGameObjectTlvInfo == objectId)
            {
                return pObj->mBaseGameObjectId;
            }
        }
    }
    return nullGuid;
}

void BaseGameObject::SetUpdateDelay(s32 updateDelay)
{
    mBaseGameObjectUpdateDelay = updateDelay;
}

s32 BaseGameObject::UpdateDelay() const
{
    return mBaseGameObjectUpdateDelay;
}

AnimResource& BaseGameObject::GetAnimRes(AnimId animId)
{
    for (u32 i = 0; i < mLoadedAnims.size(); i++)
    {
        if (mLoadedAnims[i].mId == animId)
        {
            return mLoadedAnims[i];
        }
    }
    ALIVE_FATAL("Anim res %d not found", static_cast<s32>(animId));
}

PalResource& BaseGameObject::GetPalRes(PalId palId)
{
    for (u32 i = 0; i < mLoadedPals.size(); i++)
    {
        if (mLoadedPals[i].mId == palId)
        {
            return mLoadedPals[i];
        }
    }
    ALIVE_FATAL("Pal res %d not found", static_cast<s32>(palId));
}
