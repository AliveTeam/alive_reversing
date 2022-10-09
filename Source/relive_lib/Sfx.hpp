#pragma once

#include "../AliveLibCommon/Types.hpp"
#include "../AliveLibCommon/FixedPoint_common.hpp"

namespace relive {

struct SfxDefinition final
{
    s32 field_0_block_idx;
    s32 field_4_program;
    s32 field_8_note;
    s16 field_C_default_volume;
    s16 field_E_pitch_min;
    s16 field_10_pitch_max;
};

enum class SoundEffects
{
    Bullet1,
    Bullet2,
    GreenTick,
    RedTick,
    ZPop,
    SligShoot,
    BulletShell,
    MenuTransition,
    RingUnknownTrigger,
    Empty,
    SwitchUnknownTrigger,
    HandstoneTransition,
    Dove,
    FlyingDove,
    AbeDove,
    PossessEffect,
    ShrykullZap,
    WellExit,
    WellEnter,
    Leaf,
    AirStream,
    RockBounceOnMine,
    SackHit,
    RockBounce,
    ParticleBurst,
    PickupItem,
    SackWobble,
    LiftStop,
    WheelSqueak,
    AbeGenericMovement,
    SlogBite,
    AmbientEffect1,
    MeatBounce,
    Bloop,
    SecurityDoorDeny,
    ElectricZap,
    ElectricGateLoud,
    BirdPortalSpark,
    TrapdoorClose,
    TrapdoorOpen,
    AmbientEffect2,
    AmbientEffect3,
    AmbientEffect4,
    FallingItemHit,
    SecurityOrb,
    Zap1,
    Zap2,
    AmbientEffect5,
    MenuNavigation,
    AmbientEffect6,
    AmbientEffect7,
    FootSwitchPress,
    RingRopePull,
    DoorEffect,
    PortalOpening,
    Fire,
    AmbientEffect8,
    AmbientEffect9,
    FallingItemLand,
    LeverPull,
    KillEffect,
    Eating1,
    Eating2,
    GrenadeBounce,
    Clean1,
    Clean2,
    LCDScreen,
    FallingItemPresence1,
    FallingItemPresence2,
    IndustrialNoise1,
    IndustrialNoise2,
    IndustrialNoise3,
    Vaporize,
    IndustrialTrigger,
    Choke,
    Gas1,
    Gas2,
    IngameTransition,
    Shrykull1,
    Shrykull2,
    SecurityDoorLaugh,
    GlukkonSwitchBleh,
    SlurgKill,
    SlurgPause,
    Chisel,
    NakedSligTransformEnd,
    CrawlingSligTransformStart,
    WaterStart,
    WaterFall,
    WaterEnd,
    DrillMovement,
    DrillCollision,
    MinecarMovement,
    MinecarStop,
    MinecarStuck,
    WebDrop1,
    WebDrop2,
    SpiritLockShake,
    SpiritLockBreak,
    FlyingSpirit1,
    FlyingSpirit2,
    UnusedSpawn,
    ParamiteSpawn,
    ScrabSpawn,
    GlukkonSpawn,
    FlyingSligSpawn,
    SligSpawn,
    SlogSpawn,
    BrewMachineUseStart,
    BrewMachineUseMid,
    BrewMachineUseEmpty,
    BrewMachineUseEnd,
    GreeterLand,
    GreeterKnockback,

    // AO only SFX
    Unknown,
    RingBellHammer,
    UnknownCrystalSFX1,
    UnknownCrystalSFX2,
    SwitchBellHammer,
    Bees1,
    Bees2,
    RespawnDove,
    FlyingDoves,
    Respawn,
    HoneyDrip,
    MountingElum,
    LoudFire,
    Bat1,
    Bat2,
    Alarm,
    Trapdoor,
    BellChime_HighPitch,
    BellChime_MediumPitch,
    BellChime_LowPitch,
    HintFly,
    ZBall,
    FlintLock,
    PostFlint,
    RollingBallNoise1,
    RollingBallNoise2,
    SlingshotExtend,
    SlingshotShoot,
    MeatsawOffscreen,
    MeatsawIdle,
    MeatsawUp,
    MeatsawDown,
    GlukkonKillHim1,
    GlukkonKillHim2,
    GlukkonLaugh1,
    GlukkonLaugh2,
    SligLaugh,
    SligHereBoy,
    SligBleh,

};

const SfxDefinition& GetSfx(SoundEffects sfx);

// Randomly pick sfx1 or sfx2
SoundEffects RandomSfx(SoundEffects sfx1, SoundEffects sfx2);

s32 SFX_Play_Pitch(relive::SoundEffects sfxId, s32 volume, s32 pitch, FP scale = FP_FromInteger(1));
s32 SfxPlayMono(relive::SoundEffects sfxId, s32 volume, FP scale = FP_FromInteger(1));

} // namespace relive
