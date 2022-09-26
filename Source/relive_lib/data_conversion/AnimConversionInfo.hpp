#pragma once
#include "../../AliveLibCommon/AnimResources.hpp"
#include "../MapWrapper.hpp"

enum class EAnimGroup
{
    Ungrouped,
    Gibs,
    Ropes,
    Throwables,
    Abe,
    Lights,
    Bat,
    Fleech,
    FlyingSlig,
    Glukkons,
    Greeter,
    MineCar,
    Paramite,
    Scrab,
    CrawlingSlig,
    Slig,
    Shrykull,
    Icons,
    MenuHighlight,
    MenuAbeSpeak,
    MenuGlukkonSpeak,
    MenuScrabSpeak,
    MenuParamiteSpeak,
    MenuSligSpeak,
    Slog,
    Slurg,
    Elum,
    BirdPortal,
    PullRingRope,
    BoneBag,
    Drill,
    Chime,
    HoistRocks,
    ExplosionRocks,
    ExplosionSticks,
    CrawlingSligLocker,
    Lifts,
    Doors,
    Dove,
    ElectricWall,
    Explosions,
    FallingItems,
    FootSwitches,
    Grenade,
    HoneySack,
    MeatSack,
    MeatSaw,
    MotionDetector,
    MovingBomb,
    RockSack,
    SecurityClaw,
    SecurityDoor,
    SecurityOrb,
    SlamDoors,
    SlapLock,
    StatusLight,
    SwingingBall,
    Lever,
    TimedMine,
    TorturedMudokon,
    TrapDoors,
    UXB,
    WorkWheel,
    ZapLine,
    FlintLock,
    BoomMachine,
    CrawlingSligButton,
    BellHammer,
    DoorLock,
    RollingBall,
};

const char_type* AnimBaseName(AnimId id);
const char_type* AnimRecName(AnimId anim);
const char* ToString(EAnimGroup animGroup);

struct AnimRecConversionInfo final
{
    AnimId mAnimId;         // which anim?
    EAnimGroup mGroup;      // abe, doors etc
    EReliveLevelIds mAeLvl; // LVL this anim exists in for AE
    EReliveLevelIds mAoLvl; // LVL this anim exists in for AO
    bool mConverted;
};

struct BgAnimRecConversionInfo final
{
    u32 mBgAnimId;
    EReliveLevelIds mAeLvl; // LVL this anim exists in for AE
    EReliveLevelIds mAoLvl; // LVL this anim exists in for AO
    bool mConverted;
};

struct AnimRecNames final
{
    AnimId mAnimId;
    const char_type* mAnimName;
};


