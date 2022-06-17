#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../AliveLibCommon/BitField.hpp"
#include "DynamicArray.hpp"

void AliveLibAO_ForceLink();

struct PrimHeader;

namespace AO {


enum class Types : s16
{
    eNone_0 = 0,
    eAlarm_1 = 1,

    eScreenManager_4 = 4,
    eBackgroundAnimation_5 = 5,
    eBat_6 = 6,
    eLiftMover_7 = 7,
    eTimedMine_8 = 8,

    eBullet_10 = 10,
    eDDCheat_12 = 12,
    eBells_13 = 13,
    eChimeLock_14 = 14,
    eSpark_15 = 15,
    eGasClock_16 = 16,

    eParticleBurst_19 = 19,

    eDoor_21 = 21,
    eBird_22 = 22,

    eGameSpeak_24 = 24,
    eElectricWall_25 = 25,
    eElum_26 = 26,
    eBellHammer_27 = 27,

    ePalOverwriter_29 = 29,
    eBaseBomb_30 = 30,
    eSecurityClaw_31 = 31,
    eRockSpawner_32 = 32,

    eFlintLockFire_34 = 34,
    eThrowableTotalIndicator_35 = 35,
    eFootSwitch36 = 36,
    eGameEnderController_37 = 37,
    eDeathBird_38 = 38,
    eLoadingFile_39 = 39,
    eGrenade_40 = 40,
    eGrenadeMachine_41 = 41,
    eBackgroundGlukkon_42 = 42,
    eAbe_43 = 43,
    MainMenuFade_44 = 44,
    eHoneySack_45 = 45,

    eHoney_47 = 47,
    eClawOrBirdPortalTerminator_48 = 48,

    eMudokon_52 = 52,
    eLiftPoint_51 = 51,
    SecurityOrb_53 = 53,
    eMeat_54 = 54,
    eMeatStack_55 = 55,
    eMeatSaw_56 = 56,
    eMine_57 = 57,
    eRollingBallStopperShaker_58 = 58,
    eMotionDetector_59 = 59,
    eRollingBallStopper_60 = 60,
    ePauseMenu_61 = 61,
    eParamite_62 = 62,
    eDemoPlayback = 64,
    eBirdPortal_65 = 65,
    eBirdPortalTerminator_66 = 66,
    eFG1_67 = 67,
    ePullRingRope_68 = 68,
    eAbilityRing_69 = 69,
    eRock_70 = 70,
    eRockSack_71 = 71,
    eRollingBall_72 = 72,
    eRope_73 = 73,
    eExplosion_74 = 74,
    eMudokon_75 = 75,
    eRedLaser_76 = 76,
    eScrab_77 = 77,
    eScreenClipper_78 = 78,
    eEffectBase_79 = 79,
    eDeathFadeOut_80 = 80, // TODO: Actually eMainMenuTransistion in AE, shared?
    eFlash_81 = 81,
    ScreenShake_82 = 82,
    eScreenWave_83 = 83,
    eUnknown_84 = 84,
    eShrykull_85 = 85,

    eSnoozParticle_87 = 87,
    eSlig_88 = 88,
    eSlog_89 = 89,
    SlingMud_90 = 90,
    eSligSpawner_91 = 91,
    eZBall_92 = 92,
    eParticle_93 = 93,
    eZapLine_94 = 94,
    eBeeSwarm_95 = 95,
    eBeeNest_96 = 96,
    eLever_97 = 97,
    eTrapDoor_98 = 98,
    eUXB_99 = 99,
    eMovie_100 = 100,
    eWell_101 = 101,
    eCameraSwapper_102 = 102,
    eElectrocute_103 = 103,
};

class BaseGameObject
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

    explicit BaseGameObject(s16 addToObjectList);
    virtual ~BaseGameObject();

    virtual void VUpdate()
    {
        // Empty
    }

    virtual void VRender(PrimHeader**)
    {
        // Empty
    }

    virtual void VScreenChanged();

    virtual void VStopAudio()
    {
        // Empty
    }

public:
    Types field_4_typeId;
    BitField16<Options> mFlags;
    s32 field_8_update_delay;
    s8 field_C_refCount;
    s8 field_D_padding;
    s16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x10);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObjects);

} // namespace AO
