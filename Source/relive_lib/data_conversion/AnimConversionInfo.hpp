#pragma once
#include "../AnimResources.hpp"
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
    BackgroundAnimation,
};

const char_type* AnimRecName(AnimId anim);
const char_type* AnimRecGroupName(AnimId anim);
const char_type* AnimRecThemeName(AnimId anim);
const char* ToString(EAnimGroup animGroup);

struct PalRecConversionInfo final
{
    PalId mPalId;         // which pal?
    bool mConverted;
};

struct AnimRecThemeInfo final
{
    const char* mThemeName; // Theme name of the item, e.g if a door could be RuptureFarms, Feeco etc
    AnimId mAnimIdRemapTo;  // Resulting AnimId e.g map a unique specific anim id to a generic one 
};

struct AnimRecConversionInfo final
{
    AnimId mAnimId;         // which anim?
    EAnimGroup mGroup;      // abe, doors etc
    AnimRecThemeInfo mThemeInfo;
    EReliveLevelIds mAeLvl; // LVL this anim exists in for AE
    EReliveLevelIds mAoLvl; // LVL this anim exists in for AO
    bool mConverted;
};

struct AnimRecNames final
{
    AnimId mAnimId;
    const char_type* mAnimName;
};

const char* ToString(PalId pal);

extern AnimRecNames kAnimRecNames[1027];
extern AnimRecConversionInfo kAnimRecConversionInfo[1024];
