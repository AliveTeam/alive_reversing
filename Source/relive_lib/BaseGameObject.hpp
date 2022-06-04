#pragma once

#include "../AliveLibCommon/Types.hpp"
#include "../AliveLibCommon/BitField.hpp"

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
    eRingOrLiftMud,
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

enum class AETypes : s16;

namespace AO {
enum class AOTypes : s16;
}

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

    static ReliveTypes FromAO(AO::AOTypes aoType);
    static AO::AOTypes ToAO(ReliveTypes reliveType);

    static ReliveTypes FromAE(AETypes aeType);

    static AETypes ToAE(ReliveTypes reliveType);

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
