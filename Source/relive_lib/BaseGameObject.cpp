#include "stdafx.h"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibAE/Map.hpp"
#include "../AliveLibAO/Map.hpp"
#include "ResourceManagerWrapper.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "GameType.hpp"
#include "../AliveLibCommon/FatalError.hpp"

DynamicArrayT<BaseGameObject>* gBaseGameObjects = nullptr;
s32 sAccumulatedObjectCount_5C1BF4 = 0;

BaseGameObject::BaseGameObject(s16 bAddToObjectList, s16 resourceArraySize)
    : field_10_resources_array(resourceArraySize)
{
    // Pre-allocate the array with nullptr entries so that it can be
    // used like a fixed size array in the derived type.
    // TODO: If its never used dynamically in the derived type then it actually can become a fixed
    // compile time sized array at some point.
    for (s32 i = 0; i < resourceArraySize; i++)
    {
        field_10_resources_array.Push_Back(nullptr);
    }

    SetType(ReliveTypes::eNone);

    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eListAddFailed_Bit1);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eDead);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eIsBaseAliveGameObject_Bit6);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eCanExplode_Bit7);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eInteractive_Bit8);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eSurviveDeathReset_Bit9);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eUpdateDuringCamSwap_Bit10);
    mBaseGameObjectFlags.Clear(BaseGameObject::Options::eCantKill_Bit11);
    mBaseGameObjectFlags.Set(BaseGameObject::eUpdatable_Bit2);

    if (bAddToObjectList)
    {
        if (!gBaseGameObjects->Push_Back(this))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
        }
    }

    Guid nextId = Guid::NewGuid();
    mBaseGameObjectTlvInfo = nextId;
    mBaseGameObjectId = nextId;
    sObjectIds.Insert(nextId, this);

    sAccumulatedObjectCount_5C1BF4++; // TODO: probably no longer needed
}

BaseGameObject::~BaseGameObject()
{
    EventCancelForObj(this);

    sObjectIds.Remove(mBaseGameObjectId);
}

void BaseGameObject::VUpdate()
{

}

void BaseGameObject::VRender(PrimHeader** /*pOrderingTable*/)
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

void BaseGameObject::VScreenChanged()
{
    auto& map = GetMap();
    if (map.LevelChanged() || map.PathChanged())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void BaseGameObject::VStopAudio()
{

}

s32 BaseGameObject::VGetSaveState(u8*)
{
    // Not used in AO yet so needs a default impl
    return 0;
}

ReliveTypes BaseGameObject::FromAO(AO::AOTypes aoType)
{
    switch (aoType)
    {
        case AO::AOTypes::eNone_0:
            return ReliveTypes::eNone;
        case AO::AOTypes::eAlarm_1:
            return ReliveTypes::eAlarm;
        case AO::AOTypes::eScreenManager_4:
            return ReliveTypes::eScreenManager;
        case AO::AOTypes::eBackgroundAnimation_5:
            return ReliveTypes::eBackgroundAnimation;
        case AO::AOTypes::eBat_6:
            return ReliveTypes::eBat;
        case AO::AOTypes::eLiftMover_7:
            return ReliveTypes::eLiftMover;
        case AO::AOTypes::eTimedMine_8:
            return ReliveTypes::eTimedMine;
        case AO::AOTypes::eBullet_10:
            return ReliveTypes::eBullet;
        case AO::AOTypes::eDDCheat_12:
            return ReliveTypes::eDDCheat;
        case AO::AOTypes::eBells_13:
            return ReliveTypes::eBells;
        case AO::AOTypes::eChimeLock_14:
            return ReliveTypes::eChimeLock;
        case AO::AOTypes::eZapSpark_15:
            return ReliveTypes::eZapSpark;
        case AO::AOTypes::eGasClock_16:
            return ReliveTypes::eGasCountDown;
        case AO::AOTypes::eParticleBurst_19:
            return ReliveTypes::eParticleBurst;
        case AO::AOTypes::eDoor_21:
            return ReliveTypes::eDoor;
        case AO::AOTypes::eBird_22:
            return ReliveTypes::eBird;
        case AO::AOTypes::eGameSpeak_24:
            return ReliveTypes::eGameSpeak;
        case AO::AOTypes::eElectricWall_25:
            return ReliveTypes::eElectricWall;
        case AO::AOTypes::eElum_26:
            return ReliveTypes::eElum;
        case AO::AOTypes::eBellHammer_27:
            return ReliveTypes::eBellHammer;
        case AO::AOTypes::ePalOverwriter_29:
            return ReliveTypes::ePalOverwriter;
        case AO::AOTypes::eBaseBomb_30:
            return ReliveTypes::eGroundExplosion;
        case AO::AOTypes::eSecurityClaw_31:
            return ReliveTypes::eSecurityClaw;
        case AO::AOTypes::eRockSpawner_32:
            return ReliveTypes::eRockSpawner;
        case AO::AOTypes::eFlintLockFire_34:
            return ReliveTypes::eFlintLockFire;
        case AO::AOTypes::eThrowableTotalIndicator_35:
            return ReliveTypes::eThrowableTotalIndicator;
        case AO::AOTypes::eFootSwitch36:
            return ReliveTypes::eFootSwitch;
        case AO::AOTypes::eGameEnderController_37:
            return ReliveTypes::eGameEnderController;
        case AO::AOTypes::eDeathBird_38:
            return ReliveTypes::eDeathBird;
        case AO::AOTypes::eLoadingFile_39:
            return ReliveTypes::eLoadingFile;
        case AO::AOTypes::eGrenade_40:
            return ReliveTypes::eGrenade;
        case AO::AOTypes::eGrenadeMachine_41:
            return ReliveTypes::eBoomMachine;
        case AO::AOTypes::eBackgroundGlukkon_42:
            return ReliveTypes::eBackgroundGlukkon;
        case AO::AOTypes::eAbe_43:
            return ReliveTypes::eAbe;
        case AO::AOTypes::MainMenuFade_44:
            return ReliveTypes::MainMenuFade;
        case AO::AOTypes::eHoneySack_45:
            return ReliveTypes::eHoneySack;
        case AO::AOTypes::eHoney_47:
            return ReliveTypes::eHoney;
        case AO::AOTypes::eClawOrBirdPortalTerminator_48:
            return ReliveTypes::eClawOrBirdPortalTerminator;
        case AO::AOTypes::eRingOrLiftMud_52:
            return ReliveTypes::eRingOrLiftMud;
        case AO::AOTypes::eLiftPoint_51:
            return ReliveTypes::eLiftPoint;
        case AO::AOTypes::SecurityOrb_53:
            return ReliveTypes::eSecurityOrb;
        case AO::AOTypes::eMeat_54:
            return ReliveTypes::eMeat;
        case AO::AOTypes::eMeatStack_55:
            return ReliveTypes::eMeatSack;
        case AO::AOTypes::eMeatSaw_56:
            return ReliveTypes::eMeatSaw;
        case AO::AOTypes::eMine_57:
            return ReliveTypes::eMine;
        case AO::AOTypes::eRollingBallStopperShaker_58:
            return ReliveTypes::eRollingBallStopperShaker;
        case AO::AOTypes::eMotionDetector_59:
            return ReliveTypes::eMotionDetector;
        case AO::AOTypes::eRollingBallStopper_60:
            return ReliveTypes::eRollingBallStopper;
        case AO::AOTypes::ePauseMenu_61:
            return ReliveTypes::ePauseMenu;
        case AO::AOTypes::eParamite_62:
            return ReliveTypes::eParamite;
        case AO::AOTypes::eDemoPlayback:
            return ReliveTypes::eDemoPlayback;
        case AO::AOTypes::eBirdPortal_65:
            return ReliveTypes::eBirdPortal;
        case AO::AOTypes::eBirdPortalTerminator_66:
            return ReliveTypes::eBirdPortalTerminator;
        case AO::AOTypes::eFG1_67:
            return ReliveTypes::eFG1;
        case AO::AOTypes::ePullRingRope_68:
            return ReliveTypes::ePullRingRope;
        case AO::AOTypes::eAbilityRing_69:
            return ReliveTypes::eAbilityRing;
        case AO::AOTypes::eRock_70:
            return ReliveTypes::eRock;
        case AO::AOTypes::eRockSack_71:
            return ReliveTypes::eRockSack;
        case AO::AOTypes::eRollingBall_72:
            return ReliveTypes::eRollingBall;
        case AO::AOTypes::eRope_73:
            return ReliveTypes::eRope;
        case AO::AOTypes::eExplosion_74:
            return ReliveTypes::eAirExplosion;
        case AO::AOTypes::eMudokon_75:
            return ReliveTypes::eMudokon;
        case AO::AOTypes::eRedLaser_76:
            return ReliveTypes::eRedLaser;
        case AO::AOTypes::eScrab_77:
            return ReliveTypes::eScrab;
        case AO::AOTypes::eScreenClipper_78:
            return ReliveTypes::eScreenClipper;
        case AO::AOTypes::eEffectBase_79:
            return ReliveTypes::eEffectBase;
        case AO::AOTypes::eDeathFadeOut_80:
            return ReliveTypes::eDeathFadeOut;
        case AO::AOTypes::eFlash_81:
            return ReliveTypes::eFlash;
        case AO::AOTypes::ScreenShake_82:
            return ReliveTypes::ScreenShake;
        case AO::AOTypes::eScreenWave_83:
            return ReliveTypes::eScreenWave;
        case AO::AOTypes::eUnknown_84:
            return ReliveTypes::eUnknown;
        case AO::AOTypes::eShrykull_85:
            return ReliveTypes::eShrykull;
        case AO::AOTypes::eSnoozParticle_87:
            return ReliveTypes::eSnoozParticle;
        case AO::AOTypes::eSlig_88:
            return ReliveTypes::eSlig;
        case AO::AOTypes::eSlog_89:
            return ReliveTypes::eSlog;
        case AO::AOTypes::SlingMud_90:
            return ReliveTypes::SlingMud;
        case AO::AOTypes::eSligSpawner_91:
            return ReliveTypes::eSligSpawner;
        case AO::AOTypes::eZBall_92:
            return ReliveTypes::eZBall;
        case AO::AOTypes::eParticle_93:
            return ReliveTypes::eParticle;
        case AO::AOTypes::eZapLine_94:
            return ReliveTypes::eZapLine;
        case AO::AOTypes::eBeeSwarm_95:
            return ReliveTypes::eBeeSwarm;
        case AO::AOTypes::eBeeNest_96:
            return ReliveTypes::eBeeNest;
        case AO::AOTypes::eLever_97:
            return ReliveTypes::eLever;
        case AO::AOTypes::eTrapDoor_98:
            return ReliveTypes::eTrapDoor;
        case AO::AOTypes::eUXB_99:
            return ReliveTypes::eUXB;
        case AO::AOTypes::eMovie_100:
            return ReliveTypes::eMovie;
        case AO::AOTypes::eWell_101:
            return ReliveTypes::eWell;
        case AO::AOTypes::eCameraSwapper_102:
            return ReliveTypes::eCameraSwapper;
        case AO::AOTypes::eElectrocute_103:
            return ReliveTypes::eElectrocute;
        default:
            ALIVE_FATAL("Unknown AO type");
    }
}

AO::AOTypes BaseGameObject::ToAO(ReliveTypes reliveType)
{
    switch (reliveType)
    {
        case ReliveTypes::eNone:
            return AO::AOTypes::eNone_0;
        case ReliveTypes::eAlarm:
            return AO::AOTypes::eAlarm_1;
        case ReliveTypes::eScreenManager:
            return AO::AOTypes::eScreenManager_4;
        case ReliveTypes::eBackgroundAnimation:
            return AO::AOTypes::eBackgroundAnimation_5;
        case ReliveTypes::eBat:
            return AO::AOTypes::eBat_6;
        case ReliveTypes::eLiftMover:
            return AO::AOTypes::eLiftMover_7;
        case ReliveTypes::eTimedMine:
            return AO::AOTypes::eTimedMine_8;
        case ReliveTypes::eBullet:
            return AO::AOTypes::eBullet_10;
        case ReliveTypes::eDDCheat:
            return AO::AOTypes::eDDCheat_12;
        case ReliveTypes::eBells:
            return AO::AOTypes::eBells_13;
        case ReliveTypes::eChimeLock:
            return AO::AOTypes::eChimeLock_14;
        case ReliveTypes::eZapSpark:
            return AO::AOTypes::eZapSpark_15;
        case ReliveTypes::eGasCountDown:
            return AO::AOTypes::eGasClock_16;
        case ReliveTypes::eParticleBurst:
            return AO::AOTypes::eParticleBurst_19;
        case ReliveTypes::eDoor:
            return AO::AOTypes::eDoor_21;
        case ReliveTypes::eBird:
            return AO::AOTypes::eBird_22;
        case ReliveTypes::eGameSpeak:
            return AO::AOTypes::eGameSpeak_24;
        case ReliveTypes::eElectricWall:
            return AO::AOTypes::eElectricWall_25;
        case ReliveTypes::eElum:
            return AO::AOTypes::eElum_26;
        case ReliveTypes::eBellHammer:
            return AO::AOTypes::eBellHammer_27;
        case ReliveTypes::ePalOverwriter:
            return AO::AOTypes::ePalOverwriter_29;
        case ReliveTypes::eGroundExplosion:
            return AO::AOTypes::eBaseBomb_30;
        case ReliveTypes::eSecurityClaw:
            return AO::AOTypes::eSecurityClaw_31;
        case ReliveTypes::eRockSpawner:
            return AO::AOTypes::eRockSpawner_32;
        case ReliveTypes::eFlintLockFire:
            return AO::AOTypes::eFlintLockFire_34;
        case ReliveTypes::eThrowableTotalIndicator:
            return AO::AOTypes::eThrowableTotalIndicator_35;
        case ReliveTypes::eFootSwitch:
            return AO::AOTypes::eFootSwitch36;
        case ReliveTypes::eGameEnderController:
            return AO::AOTypes::eGameEnderController_37;
        case ReliveTypes::eDeathBird:
            return AO::AOTypes::eDeathBird_38;
        case ReliveTypes::eLoadingFile:
            return AO::AOTypes::eLoadingFile_39;
        case ReliveTypes::eGrenade:
            return AO::AOTypes::eGrenade_40;
        case ReliveTypes::eBoomMachine:
            return AO::AOTypes::eGrenadeMachine_41;
        case ReliveTypes::eBackgroundGlukkon:
            return AO::AOTypes::eBackgroundGlukkon_42;
        case ReliveTypes::eAbe:
            return AO::AOTypes::eAbe_43;
        case ReliveTypes::MainMenuFade:
            return AO::AOTypes::MainMenuFade_44;
        case ReliveTypes::eHoneySack:
            return AO::AOTypes::eHoneySack_45;
        case ReliveTypes::eHoney:
            return AO::AOTypes::eHoney_47;
        case ReliveTypes::eClawOrBirdPortalTerminator:
            return AO::AOTypes::eClawOrBirdPortalTerminator_48;
        case ReliveTypes::eRingOrLiftMud:
            return AO::AOTypes::eRingOrLiftMud_52;
        case ReliveTypes::eLiftPoint:
            return AO::AOTypes::eLiftPoint_51;
        case ReliveTypes::eSecurityOrb:
            return AO::AOTypes::SecurityOrb_53;
        case ReliveTypes::eMeat:
            return AO::AOTypes::eMeat_54;
        case ReliveTypes::eMeatSack:
            return AO::AOTypes::eMeatStack_55;
        case ReliveTypes::eMeatSaw:
            return AO::AOTypes::eMeatSaw_56;
        case ReliveTypes::eMine:
            return AO::AOTypes::eMine_57;
        case ReliveTypes::eRollingBallStopperShaker:
            return AO::AOTypes::eRollingBallStopperShaker_58;
        case ReliveTypes::eMotionDetector:
            return AO::AOTypes::eMotionDetector_59;
        case ReliveTypes::eRollingBallStopper:
            return AO::AOTypes::eRollingBallStopper_60;
        case ReliveTypes::ePauseMenu:
            return AO::AOTypes::ePauseMenu_61;
        case ReliveTypes::eParamite:
            return AO::AOTypes::eParamite_62;
        case ReliveTypes::eDemoPlayback:
            return AO::AOTypes::eDemoPlayback;
        case ReliveTypes::eBirdPortal:
            return AO::AOTypes::eBirdPortal_65;
        case ReliveTypes::eBirdPortalTerminator:
            return AO::AOTypes::eBirdPortalTerminator_66;
        case ReliveTypes::eFG1:
            return AO::AOTypes::eFG1_67;
        case ReliveTypes::ePullRingRope:
            return AO::AOTypes::ePullRingRope_68;
        case ReliveTypes::eAbilityRing:
            return AO::AOTypes::eAbilityRing_69;
        case ReliveTypes::eRock:
            return AO::AOTypes::eRock_70;
        case ReliveTypes::eRockSack:
            return AO::AOTypes::eRockSack_71;
        case ReliveTypes::eRollingBall:
            return AO::AOTypes::eRollingBall_72;
        case ReliveTypes::eRope:
            return AO::AOTypes::eRope_73;
        case ReliveTypes::eAirExplosion:
            return AO::AOTypes::eExplosion_74;
        case ReliveTypes::eMudokon:
            return AO::AOTypes::eMudokon_75;
        case ReliveTypes::eRedLaser:
            return AO::AOTypes::eRedLaser_76;
        case ReliveTypes::eScrab:
            return AO::AOTypes::eScrab_77;
        case ReliveTypes::eScreenClipper:
            return AO::AOTypes::eScreenClipper_78;
        case ReliveTypes::eEffectBase:
            return AO::AOTypes::eEffectBase_79;
        case ReliveTypes::eDeathFadeOut:
            return AO::AOTypes::eDeathFadeOut_80;
        case ReliveTypes::eFlash:
            return AO::AOTypes::eFlash_81;
        case ReliveTypes::ScreenShake:
            return AO::AOTypes::ScreenShake_82;
        case ReliveTypes::eScreenWave:
            return AO::AOTypes::eScreenWave_83;
        case ReliveTypes::eUnknown:
            return AO::AOTypes::eUnknown_84;
        case ReliveTypes::eShrykull:
            return AO::AOTypes::eShrykull_85;
        case ReliveTypes::eSnoozParticle:
            return AO::AOTypes::eSnoozParticle_87;
        case ReliveTypes::eSlig:
            return AO::AOTypes::eSlig_88;
        case ReliveTypes::eSlog:
            return AO::AOTypes::eSlog_89;
        case ReliveTypes::SlingMud:
            return AO::AOTypes::SlingMud_90;
        case ReliveTypes::eSligSpawner:
            return AO::AOTypes::eSligSpawner_91;
        case ReliveTypes::eZBall:
            return AO::AOTypes::eZBall_92;
        case ReliveTypes::eParticle:
            return AO::AOTypes::eParticle_93;
        case ReliveTypes::eZapLine:
            return AO::AOTypes::eZapLine_94;
        case ReliveTypes::eBeeSwarm:
            return AO::AOTypes::eBeeSwarm_95;
        case ReliveTypes::eBeeNest:
            return AO::AOTypes::eBeeNest_96;
        case ReliveTypes::eLever:
            return AO::AOTypes::eLever_97;
        case ReliveTypes::eTrapDoor:
            return AO::AOTypes::eTrapDoor_98;
        case ReliveTypes::eUXB:
            return AO::AOTypes::eUXB_99;
        case ReliveTypes::eMovie:
            return AO::AOTypes::eMovie_100;
        case ReliveTypes::eWell:
            return AO::AOTypes::eWell_101;
        case ReliveTypes::eCameraSwapper:
            return AO::AOTypes::eCameraSwapper_102;
        case ReliveTypes::eElectrocute:
            return AO::AOTypes::eElectrocute_103;
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
