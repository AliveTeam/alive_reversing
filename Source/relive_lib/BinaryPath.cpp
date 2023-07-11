#include "stdafx.h"
#include "BinaryPath.hpp"
#include "data_conversion/relive_tlvs_serialization.hpp"
#include "../AliveLibAO/Path.hpp"
#include "nlohmann/json.hpp"

#include <typeindex>

static void from_json(const nlohmann::json& j, PathSoundInfo& s)
{
    j.at("vh_file").get_to(s.mVhFile);
    j.at("vb_file").get_to(s.mVbFile);
    j.at("seq_files").get_to(s.mSeqFiles);
}
class ITlvAllocator
{
public:
    virtual void Alloc(nlohmann::json& j, BinaryPath::CamEntry* pCamEntry) = 0;
};

template<typename TlvType>
class Allocator : public ITlvAllocator
{
public:
    Allocator() = default;

    void Alloc(nlohmann::json& j, BinaryPath::CamEntry* pCamEntry) override
    {
        j.get_to(*pCamEntry->AllocTLV<TlvType>());
    }
};


class TlvJsonTypeMap final
{
public:
    TlvJsonTypeMap()
    {
        PopulateTlvJsonMap();
    }

    void DeseralizeTlvJsonType(const std::string& name, nlohmann::json& j, BinaryPath::CamEntry* pCamEntry)
    {
        // This avoids a huge if if if chain in MSVC (compiler limit on huge if/else chains) and should
        // be a little faster at the expensive of more work at program start up.
        auto it = mTlvJsonMap.find(name);
        if (it != std::end(mTlvJsonMap))
        {
            it->second->Alloc(j, pCamEntry);
        }
        else
        {
            // ALIVE_FATAL("%s is not a known TLV type", name.c_str());
        }
    }

private :
    void PopulateTlvJsonMap()
    {
#define InsertTlvType(x) {std::string(x::kClassName), std::make_unique<Allocator<x>>()}

        mTlvJsonMap.insert(InsertTlvType(relive::Path_ShadowZone));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SecurityOrb));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ContinuePoint));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LiftPoint));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Dove));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_RockSack));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ZBall));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_FallingItem));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_PullRingRope));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_TimedMine));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Hoist));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_TrapDoor));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LCDScreen));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Mine));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_InvisibleSwitch));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ElectricWall));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BoomMachine));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_UXB));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MeatSaw));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Lever));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Edge));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BirdPortal));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BirdPortalExit));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LightEffect));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MusicTrigger));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SoftLanding));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LiftMover));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_HintFly));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_TimerTrigger));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_FlintLockFire));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_HoneySack));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Bat));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_RollingBallStopper));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_RollingBall));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MotionDetector));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BellHammer));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SligBoundLeft));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SligBoundRight));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SligPersist));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BackgroundAnimation));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MainMenuController));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ElumWall));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ElumStart));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_KillUnsavedMuds));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_InvisibleZone));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_StartController));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ScrabNoFall));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ScrabBoundLeft));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ScrabBoundRight));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_HoneyDripTarget));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ZSligCover));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_DeathDrop));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ChimeLock));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LCDStatusBoard));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_CreditsController));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ResetPath));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MeatSack));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BeeNest));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BellsongStone));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MovieStone));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_HandStone));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_PathTransition));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Pulley));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Honey));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BeeSwarmHole));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Door));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_AbeStart));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_EnemyStopper));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MovingBombStopper));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_DoorFlame));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Mudokon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MovingBomb));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ElumPathTrans));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MudokonPathTrans));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SecurityClaw));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SlingMudokon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_FootSwitch));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Paramite));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ZzzSpawner));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BackgroundGlukkon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_GasEmitter));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_GasCountDown));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_RingCancel));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SecurityDoor));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LiftMudokon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_RingMudokon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_WellLocal));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_WellExpress));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Slog));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SlogSpawner));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Scrab));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ScrabSpawner));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Slig));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SligSpawner));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_TrainDoor));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_TorturedMudokon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_DoorBlocker));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_GlukkonSwitch));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Greeter));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BrewMachine));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Alarm));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ParamiteWebLine));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SlapLock));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_StatusLight));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MultiSwitchController));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ExplosionSet));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_BoneBag));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_MineCar));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_ColourfulMeter));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_DemoSpawnPoint));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LevelLoader));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SlamDoor));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Slurg));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_LaughingGas));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_WorkWheel));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Water));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_WheelSyncer));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Fleech));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SlurgSpawner));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Drill));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Teleporter));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_Glukkon));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_CrawlingSligButton));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_FlyingSlig));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_FlyingSligSpawner));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_CrawlingSlig));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SligGetWings));
        mTlvJsonMap.insert(InsertTlvType(relive::Path_SligGetPants));

#undef InsertTlvType
    }

    std::map<std::string, std::unique_ptr<ITlvAllocator>> mTlvJsonMap = {};
};

static TlvJsonTypeMap kTlvJsonTypeMap;

void BinaryPath::CreateFromJson(nlohmann::json& pathJson)
{
    // TODO: Do a pass to collect the total required buffer size

    nlohmann::json& collisions = pathJson["collisions"];
    mCollisions.resize(collisions.size());
    s32 idx = 0;
    for (auto& collision : collisions)
    {
        collision.get_to(mCollisions[idx]);
        idx++;
    }

    for (auto& cam : pathJson["cameras"])
    {
        auto camEntry = std::make_unique<CamEntry>();
        camEntry->mX = cam["x"];
        camEntry->mY = cam["y"];
        camEntry->mId = cam["id"];
        camEntry->mName = cam["name"];

        auto& mapObjects = cam["map_objects"];
        for (auto i = 0u; i < mapObjects.size(); i++)
        {
            const auto& type = mapObjects.at(i)["tlv_type"];

            //kTlvJsonTypeMap.DeseralizeTlvJsonType(type, mapObjects.at(i), camEntry.get());

            if (type == "light_effect")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LightEffect>());
            }
            if (type == "main_menu_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MainMenuController>());
            }
            if (type == "shadow_zone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ShadowZone>());
            }
            if (type == "security_orb")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SecurityOrb>());
            }
            if (type == "continue_point")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ContinuePoint>());
            }
            if (type == "lift_point")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LiftPoint>());
            }
            if (type == "dove")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Dove>());
            }
            if (type == "rock_sack")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RockSack>());
            }
            if (type == "z_ball")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ZBall>());
            }
            if (type == "falling_item")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FallingItem>());
            }
            if (type == "pull_ring_rope")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_PullRingRope>());
            }
            if (type == "timed_mine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TimedMine>());
            }
            if (type == "hoist")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Hoist>());
            }
            if (type == "trap_door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TrapDoor>());
            }
            if (type == "lcd_screen")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LCDScreen>());
            }
            if (type == "mine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Mine>());
            }
            if (type == "invisible_switch")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_InvisibleSwitch>());
            }
            if (type == "electric_wall")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElectricWall>());
            }
            if (type == "boom_machine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BoomMachine>());
            }
            if (type == "uxb")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_UXB>());
            }
            if (type == "meat_saw")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MeatSaw>());
            }
            if (type == "lever")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Lever>());
            }
            if (type == "edge")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Edge>());
            }
            if (type == "bird_portal")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BirdPortal>());
            }
            if (type == "bird_portal_exit")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BirdPortalExit>());
            }
            if (type == "music_trigger")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MusicTrigger>());
            }
            if (type == "soft_landing")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SoftLanding>());
            }
            if (type == "lift_mover")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LiftMover>());
            }
            if (type == "hint_fly")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HintFly>());
            }
            if (type == "timer_trigger")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TimerTrigger>());
            }
            if (type == "flint_lock_fire")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FlintLockFire>());
            }
            if (type == "honey_sack")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HoneySack>());
            }
            if (type == "bat")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Bat>());
            }
            if (type == "rolling_ball_stopper")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RollingBallStopper>());
            }
            if (type == "rolling_ball")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RollingBall>());
            }
            if (type == "motion_detector")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MotionDetector>());
            }
            if (type == "bell_hammer")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BellHammer>());
            }
            if (type == "slig_bound_left")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligBoundLeft>());
            }
            if (type == "slig_bound_right")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligBoundRight>());
            }
            if (type == "slig_persist")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligPersist>());
            }
            if (type == "background_animation")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BackgroundAnimation>());
            }
            if (type == "elum_wall")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElumWall>());
            }
            if (type == "elum_start")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElumStart>());
            }
            if (type == "kill_unsaved_muds")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_KillUnsavedMuds>());
            }
            if (type == "invisible_zone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_InvisibleZone>());
            }
            if (type == "start_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_StartController>());
            }
            if (type == "scrab_no_fall")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabNoFall>());
            }
            if (type == "scrab_bound_left")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabBoundLeft>());
            }
            if (type == "scrab_bound_right")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabBoundRight>());
            }
            if (type == "honey_drip_target")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HoneyDripTarget>());
            }
            if (type == "z_slig_cover")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ZSligCover>());
            }
            if (type == "death_drop")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_DeathDrop>());
            }
            if (type == "chime_lock")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ChimeLock>());
            }
            if (type == "lcd_status_board")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LCDStatusBoard>());
            }
            if (type == "credits_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_CreditsController>());
            }
            if (type == "reset_path")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ResetPath>());
            }
            if (type == "meat_sack")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MeatSack>());
            }
            if (type == "bee_nest")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BeeNest>());
            }
            if (type == "bellsong_stone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BellsongStone>());
            }
            if (type == "movie_stone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MovieStone>());
            }
            if (type == "hand_stone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HandStone>());
            }
            if (type == "path_transition")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_PathTransition>());
            }
            if (type == "pulley")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Pulley>());
            }
            if (type == "honey")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Honey>());
            }
            if (type == "bee_swarm_hole")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BeeSwarmHole>());
            }
            if (type == "door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Door>());
            }
            if (type == "enemy_stopper")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_EnemyStopper>());
            }
            if (type == "moving_bomb_stopper")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MovingBombStopper>());
            }
            if (type == "door_flame")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_DoorFlame>());
            }
            if (type == "mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Mudokon>());
            }
            if (type == "moving_bomb")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MovingBomb>());
            }
            if (type == "elum_path_trans")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElumPathTrans>());
            }
            if (type == "mudokon_path_trans")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MudokonPathTrans>());
            }
            if (type == "security_claw")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SecurityClaw>());
            }
            if (type == "sling_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlingMudokon>());
            }
            if (type == "foot_switch")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FootSwitch>());
            }
            if (type == "paramite")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Paramite>());
            }
            if (type == "zzz_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ZzzSpawner>());
            }
            if (type == "background_glukkon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BackgroundGlukkon>());
            }
            if (type == "gas_emitter")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_GasEmitter>());
            }
            if (type == "gas_countdown")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_GasCountDown>());
            }
            if (type == "ring_cancel")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RingCancel>());
            }
            if (type == "security_door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SecurityDoor>());
            }
            if (type == "lift_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LiftMudokon>());
            }
            if (type == "ring_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RingMudokon>());
            }
            if (type == "well_local")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_WellLocal>());
            }
            if (type == "well_express")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_WellExpress>());
            }
            if (type == "slog")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Slog>());
            }
            if (type == "slog_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlogSpawner>());
            }
            if (type == "scrab")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Scrab>());
            }
            if (type == "slig")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Slig>());
            }
            if (type == "slig_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligSpawner>());
            }
            if (type == "train_door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TrainDoor>());
            }
            if (type == "tortured_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TorturedMudokon>());
            }
            if (type == "door_blocker")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_DoorBlocker>());
            }
            if (type == "glukkon_switch")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_GlukkonSwitch>());
            }
            if (type == "greeter")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Greeter>());
            }
            if (type == "brew_machine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BrewMachine>());
            }
            if (type == "alarm")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Alarm>());
            }
            if (type == "paramite_web_line")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ParamiteWebLine>());
            }
            if (type == "slap_lock")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlapLock>());
            }
            if (type == "status_light")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_StatusLight>());
            }
            if (type == "multi_switch_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MultiSwitchController>());
            }
            if (type == "explosion_set")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ExplosionSet>());
            }
            if (type == "bone_bag")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BoneBag>());
            }
            if (type == "mine_car")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MineCar>());
            }
            if (type == "colourful_meter")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ColourfulMeter>());
            }
            if (type == "demo_spawn_point")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_DemoSpawnPoint>());
            }
            if (type == "level_loader")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LevelLoader>());
            }
            if (type == "slam_door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlamDoor>());
            }
            if (type == "annoying_slurg")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Slurg>());
            }
            if (type == "laughing_gas")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LaughingGas>());
            }
            if (type == "work_wheel")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_WorkWheel>());
            }
            if (type == "water")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Water>());
            }
            if (type == "wheel_syncer")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_WheelSyncer>());
            }
            if (type == "fleech")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Fleech>());
            }
            if (type == "slurg_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlurgSpawner>());
            }
            if (type == "drill")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Drill>());
            }
            if (type == "teleporter")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Teleporter>());
            }
            if (type == "glukkon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Glukkon>());
            }
            if (type == "crawling_slig_button")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_CrawlingSligButton>());
            }
            if (type == "flying_slig")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FlyingSlig>());
            }
            if (type == "flying_slig_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FlyingSligSpawner>());
            }
            if (type == "scrab_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabSpawner>());
            }
            if (type == "crawling_slig")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_CrawlingSlig>());
            }
            if (type == "slig_get_wings")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligGetWings>());
            }
            if (type == "slig_get_pants")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligGetPants>());
            }
            if (type == "abe_start")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_AbeStart>());
            }

            camEntry->mLastAllocated->mLength = camEntry->mLastAllocatedSize;

            if (i == mapObjects.size() - 1)
            {
                camEntry->mLastAllocated->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
            }
        }
        mCameras.emplace_back(std::move(camEntry));
    }

    mSoundInfo = std::make_shared<PathSoundInfo>();
    from_json(pathJson["sound_info"], *mSoundInfo);
}

relive::Path_TLV* BinaryPath::TlvsById(const Guid& id)
{
    for (auto& cam : mCameras)
    {
        auto pPathTLV = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
        while (pPathTLV)
        {
            if (pPathTLV->mId == id)
            {
                return pPathTLV;
            }

            if (pPathTLV->mTlvFlags.Get(relive::eBit3_End_TLV_List))
            {
                break;
            }
            pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
        }
    }
    return nullptr;
}
