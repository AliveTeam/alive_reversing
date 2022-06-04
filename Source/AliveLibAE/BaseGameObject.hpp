#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "DynamicArray.hpp"
#include "../AliveLibCommon/BitField.hpp"

ALIVE_VAR_EXTERN(u32, sGnFrame);

enum class AETypes : s16
{
    eNone_0 = 0,
    eAlarm_1 = 1,
    eSligSpawner_2 = 2,
    eScreenManager_6 = 6,
    eBackgroundAnimation_7 = 7,
    eLiftMover_9 = 9,
    eTimedMine_or_MovingBomb_10 = 10,
    eBone_11 = 11,
    eBoneBag_12 = 12,
    eBrewMachine_13 = 13,
    eBullet_15 = 15,
    eSligButton_16 = 16,
    eExplosionSet_18 = 18,
    eDDCheat_19 = 19,
    eSparks_22 = 22,
    eGasClock_23 = 23,
    eMetal_24 = 24,
    eMinesAlarm_25 = 25,
    eCrawlingSlig_26 = 26,
    eParticleBurst_29 = 29,
    eDrill_30 = 30,
    eLaughingGas_31 = 31,
    eDoor_33 = 33,
    eDoorLock_34 = 34,
    eDove_35 = 35,
    eGameSpeak_38 = 38,
    eElectricWall_39 = 39,
    ePalOverwriter_44 = 44,
    eEvilFart_45 = 45,
    eBaseBomb_46 = 46,
    eSecurityClaw_47 = 47,
    eRockSpawner_48 = 48,
    eFleech_50 = 50,
    ePossessionFlicker_51 = 51,
    eThrowableTotalIndicator_53 = 53,
    eFlyingSlig_54 = 54,
    eFlyingSligSpawner_55 = 55,
    eFootSwitch_56 = 56,
    eGameEnderController_57 = 57,
    eColourfulMeter_59 = 59,
    eSlapLock_OrbWhirlWind_60 = 60,
    eLockedSoul_61 = 61,
    eDeathBird_62 = 62,
    eGreeter_64 = 64,
    eGrenade_65 = 65,
    eGrenadeMachine_66 = 66,
    eGlukkon_67 = 67,
    eHelpPhone_68 = 68,
    eAbe_69 = 69,
    eEyeOrbPart_74 = 74,
    eInvisibleEffect_75 = 75,
    ePulley_76 = 76,
    eLiftPoint_78 = 78,
    eResourceManager_70 = 79,
    eSligGetPantsOrWings_80 = 80,
    eMudokon2_81 = 81,
    eSecurityOrb_83 = 83,
    eMeat_84 = 84,
    eMeatSack_85 = 85,
    eMeatSaw_86 = 86, // from AO
    eText_87 = 87,
    eMine_88 = 88,
    eMineCar_89 = 89,
    eGreeterBody_91 = 91,
    eMusicController_93 = 93,
    eMusicTrigger_94 = 94,
    ePauseMenu_95 = 95,
    eParamite_96 = 96,
    eDemoPlayback_98 = 98,
    eBirdPortal_99 = 99,
    eBirdPortalTerminator_100 = 100,
    eFG1_101 = 101,
    ePullRope_103 = 103,
    eAbilityRing_104 = 104,
    eRock_105 = 105,
    eRockSack_106 = 106,
    eLiftRope_108 = 108,
    eExplosion_109 = 109,
    eMudokon_110 = 110,
    eRedLaser_111 = 111,
    eScrab_112 = 112,
    eScrabSpawner_113 = 113,
    eScreenClipper_114 = 114,
    eEffectBase_115 = 115,
    eMainMenuTransistion_116 = 116,
    eFlash_117 = 117,
    eScreenShake_118 = 118,
    eShrykull_121 = 121,
    eSlamDoor_122 = 122,
    eSnoozeParticle_124 = 124,
    eSlig_125 = 125,
    eSlog_126 = 126,
    eSlurgSpawner_128 = 128,
    eSlurg_129 = 129,
    eParticle_134 = 134,
    eZapLine_135 = 135,
    eTimerTrigger_136 = 136,
    eLever_139 = 139,
    eTorturedMud_141 = 141,
    eTrapDoor_142 = 142,
    eUXB_143 = 143,
    eMovie_145 = 145,
    eWebLine_146 = 146,
    eWell_147 = 147,
    eWheel_148 = 148,
    eCameraSwapper_149 = 149,
    eElectrocute_150 = 150,

    eDebugHelper_1001 = 1001, // NOTE: Not part of the original game - a debug helper only
    eDebugConsole_1002 = 1002,
    eDebugPathRenderer_1003 = 1003,

    eNeverSet_107 = 107, // taking damage from something in AO? maybe bees, bats etc?
    eNeverSet_8 = 8,
    eNeverSet_40 = 40,
    eNevetSet_127 = 127,

};


enum class ReliveTypes : s16
{
    eNone = 0, // TODO: Do not depend on this value in the future
    eAlarm,
    eScreenManager,
    eBackgroundAnimation,
    eBat,
    eLiftMover,
    eTimedMine,
    eBullet,
    eDDCheat,
    eBells,
    eChimeLock,
    eSpark,
    eGasClock,
    eParticleBurst,
    eDoor,
    eBird,
    eGameSpeak,
    eElectricWall,
    eElum,
    eBellHammer,
    ePalOverwriter,
    eBaseBomb,
    eSecurityClaw,
    eRockSpawner,
    eFlintLockFire,
    eThrowableTotalIndicator,
    eFootSwitch,
    eGameEnderController,
    eDeathBird,
    eLoadingFile,
    eGrenade,
    eGrenadeMachine,
    eBackgroundGlukkon,
    eAbe,
    MainMenuFade,
    eHoneySack,
    eHoney,
    eClawOrBirdPortalTerminator,
    eMudokon,
    eLiftPoint,
    SecurityOrb,
    eMeat,
    eMeatStack,
    eMeatSaw,
    eMine,
    eRollingBallStopperShaker,
    eMotionDetector,
    eRollingBallStopper,
    ePauseMenu,
    eParamite,
    eDemoPlayback,
    eBirdPortal,
    eBirdPortalTerminator,
    eFG1,
    ePullRingRope,
    eAbilityRing,
    eRock,
    eRockSack,
    eRollingBall,
    eRope,
    eExplosion,
    eRedLaser,
    eScrab,
    eScreenClipper,
    eEffectBase,
    eDeathFadeOut,
    eFlash,
    ScreenShake,
    eScreenWave,
    eUnknown, // todo: remove ?
    eShrykull,
    eSnoozParticle,
    eSlig,
    eSlog,
    SlingMud,
    eSligSpawner,
    eZBall,
    eParticle,
    eZapLine,
    eBeeSwarm,
    eBeeNest,
    eLever,
    eTrapDoor,
    eUXB,
    eMovie,
    eCameraSwapper,
    eElectrocute,
    eTimedMine_or_MovingBomb,
    eBone,
    eBoneBag,
    eBrewMachine,
    eSligButton,
    eExplosionSet,
    eSparks,
    eMetal,
    eMinesAlarm,
    eCrawlingSlig,
    eDrill,
    eLaughingGas,
    eDoorLock,
    eDove,
    eEvilFart,
    eFleech,
    ePossessionFlicker,
    eFlyingSlig,
    eFlyingSligSpawner,
    eColourfulMeter,
    eSlapLock_OrbWhirlWind,
    eLockedSoul,
    eGreeter,
    eGlukkon,
    eHelpPhone,
    eEyeOrbPart,
    eInvisibleEffect,
    ePulley,
    eResourceManager,
    eSligGetPantsOrWings,
    eCtorMudokon,
    eSecurityOrb,
    eMeatSack,
    eText,
    eMineCar,
    eGreeterBody,
    eMusicController,
    eMusicTrigger,
    ePullRope,
    eLiftRope,
    eScrabSpawner,
    eMainMenuTransistion,
    eScreenShake,
    eSlamDoor,
    eSnoozeParticle,
    eSlurgSpawner,
    eSlurg,
    eTimerTrigger,
    eTorturedMud,
    eWebLine,
    eWell,
    eWheel,
};

struct PrimHeader;

// Temp interface till we have 1 copy of BaseGameObject
class IBaseGameObject
{
public:
    enum Options
    {
        eListAddFailed_Bit1 = 0x001,
        eUpdatable_Bit2 = 0x002,
        eDead = 0x004,
        eDrawable_Bit4 = 0x008,
        eIsBaseAnimatedWithPhysicsObj_Bit5 = 0x010,
        eIsBaseAliveGameObject_Bit6 = 0x020,
        eCanExplode_Bit7 = 0x040,
        eInteractive_Bit8 = 0x080,
        eSurviveDeathReset_Bit9 = 0x100,
        eUpdateDuringCamSwap_Bit10 = 0x200,
        eCantKill_Bit11 = 0x400
    };
    virtual ~IBaseGameObject()    { }

    virtual void VUpdate() = 0;
    virtual void VRender(PrimHeader** pOrderingTable) = 0;
    virtual void VScreenChanged() = 0;
    virtual void VStopAudio() = 0;

    virtual s32 VGetSaveState(u8* /*pSaveBuffer*/)
    {
        // Not used in AO yet so needs a default impl
        return 0;
    }

    ReliveTypes FromAE(AETypes aeType)
    {
        switch (aeType)
        {
            case AETypes::eCrawlingSlig_26: return ReliveTypes::eCrawlingSlig;
        }
        return ReliveTypes::eNone;
    }

    AETypes ToAE(ReliveTypes reliveType)
    {
        switch (reliveType)
        {
            case ReliveTypes::eCrawlingSlig:
                return AETypes::eCrawlingSlig_26;
        }
        return AETypes::eNone_0;
    }

    void SetType(ReliveTypes type)
    {
        mBaseGameObjectTypeId = type;
    }

    ReliveTypes Type() const
    {
        return mBaseGameObjectTypeId;
    }

public:
    ReliveTypes mBaseGameObjectTypeId;
    BitField16<Options> mBaseGameObjectFlags;
    s32 mBaseGameObjectUpdateDelay;
};

class BaseGameObject : public IBaseGameObject
{
public:
   
    BaseGameObject(s16 bAddToObjectList, s16 resourceArraySize);
    virtual ~BaseGameObject();
    virtual void VUpdate();
    virtual void VRender(PrimHeader** pOrderingTable);
    virtual void VScreenChanged();
    virtual void VStopAudio();

    virtual s32 VGetSaveState(u8* pSaveBuffer);

protected:
    u8** Add_Resource(u32 type, s32 resourceID);

    static s32 RefreshId(s32 objectId);

    // Helper to check if a timer has expired
    template <class T>
    static inline bool Expired(const T& value)
    {
        return static_cast<s32>(sGnFrame) > value;
    }

    template <class T>
    static s32 MakeTimer(const T value)
    {
        return static_cast<s32>(sGnFrame) + value;
    }

public:
    void SetUpdateDelay(s32 updateDelay)
    {
        mBaseGameObjectUpdateDelay = updateDelay;
    }

    s32 UpdateDelay() const
    {
        return mBaseGameObjectUpdateDelay;
    }

public:

    s32 field_8_object_id;
    s32 mBaseGameObjectTlvInfo;
    DynamicArrayT<u8*> field_10_resources_array;
private:
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x20);


ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObjects);
