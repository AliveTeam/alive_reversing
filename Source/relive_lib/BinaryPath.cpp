#include "BinaryPath.hpp"
#include "data_conversion/relive_tlvs_serialization.hpp"
#include "../AliveLibAO/Path.hpp"
#include "../AliveLibCommon/Sys_common.hpp"

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

            if (type == "light_effect")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LightEffect>());
            }
            else if (type == "main_menu_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MainMenuController>());
            }
            else if (type == "shadow_zone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ShadowZone>());
            }
            else if (type == "security_orb")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SecurityOrb>());
            }
            else if (type == "continue_point")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ContinuePoint>());
            }
            else if (type == "lift_point")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LiftPoint>());
            }
            else if (type == "dove")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Dove>());
            }
            else if (type == "rock_sack")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RockSack>());
            }
            else if (type == "z_ball")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ZBall>());
            }
            else if (type == "falling_item")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FallingItem>());
            }
            else if (type == "pull_ring_rope")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_PullRingRope>());
            }
            else if (type == "timed_mine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TimedMine>());
            }
            else if (type == "hoist")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Hoist>());
            }
            else if (type == "trap_door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TrapDoor>());
            }
            else if (type == "lcd_screen")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LCDScreen>());
            }
            else if (type == "mine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Mine>());
            }
            else if (type == "invisible_switch")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_InvisibleSwitch>());
            }
            else if (type == "electric_wall")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElectricWall>());
            }
            else if (type == "boom_machine")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BoomMachine>());
            }
            else if (type == "uxb")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_UXB>());
            }
            else if (type == "meat_saw")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MeatSaw>());
            }
            else if (type == "lever")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Lever>());
            }
            else if (type == "edge")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Edge>());
            }
            else if (type == "bird_portal")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BirdPortal>());
            }
            else if (type == "bird_portal_exit")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BirdPortalExit>());
            }
            else if (type == "music_trigger")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MusicTrigger>());
            }
            else if (type == "soft_landing")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SoftLanding>());
            }
            else if (type == "lift_mover")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LiftMover>());
            }
            else if (type == "hint_fly")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HintFly>());
            }
            else if (type == "timer_trigger")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_TimerTrigger>());
            }
            else if (type == "flint_lock_fire")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FlintLockFire>());
            }
            else if (type == "honey_sack")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HoneySack>());
            }
            else if (type == "bat")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Bat>());
            }
            else if (type == "rolling_ball_stopper")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RollingBallStopper>());
            }
            else if (type == "rolling_ball")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RollingBall>());
            }
            else if (type == "motion_detector")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MotionDetector>());
            }
            else if (type == "bell_hammer")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BellHammer>());
            }
            else if (type == "slig_bound_left")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligBoundLeft>());
            }
            else if (type == "slig_bound_right")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligBoundRight>());
            }
            else if (type == "slig_persist")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligPersist>());
            }
            else if (type == "background_animation")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BackgroundAnimation>());
            }
            else if (type == "elum_wall")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElumWall>());
            }
            else if (type == "elum_start")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElumStart>());
            }
            else if (type == "kill_unsaved_muds")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_KillUnsavedMuds>());
            }
            else if (type == "invisible_zone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_InvisibleZone>());
            }
            else if (type == "start_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_StartController>());
            }
            else if (type == "scrab_no_fall")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabNoFall>());
            }
            else if (type == "scrab_bound_left")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabBoundLeft>());
            }
            else if (type == "scrab_bound_right")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ScrabBoundRight>());
            }
            else if (type == "honey_drip_target")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HoneyDripTarget>());
            }
            else if (type == "z_slig_cover")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ZSligCover>());
            }
            else if (type == "death_drop")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_DeathDrop>());
            }
            else if (type == "chime_lock")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ChimeLock>());
            }
            else if (type == "lcd_status_board")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LCDStatusBoard>());
            }
            else if (type == "credits_controller")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_CreditsController>());
            }
            else if (type == "reset_path")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ResetPath>());
            }
            else if (type == "meat_sack")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MeatSack>());
            }
            else if (type == "bee_nest")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BeeNest>());
            }
            else if (type == "bellsong_stone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BellsongStone>());
            }
            else if (type == "movie_stone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MovieStone>());
            }
            else if (type == "hand_stone")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_HandStone>());
            }
            else if (type == "path_transition")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_PathTransition>());
            }
            else if (type == "pulley")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Pulley>());
            }
            else if (type == "honey")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Honey>());
            }
            else if (type == "bee_swarm_hole")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BeeSwarmHole>());
            }
            else if (type == "door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Door>());
            }
            else if (type == "enemy_stopper")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_EnemyStopper>());
            }
            else if (type == "moving_bomb_stopper")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MovingBombStopper>());
            }
            else if (type == "door_flame")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_DoorFlame>());
            }
            else if (type == "mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Mudokon>());
            }
            else if (type == "moving_bomb")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MovingBomb>());
            }
            else if (type == "elum_path_trans")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ElumPathTrans>());
            }
            else if (type == "mudokon_path_trans")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_MudokonPathTrans>());
            }
            else if (type == "security_claw")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SecurityClaw>());
            }
            else if (type == "sling_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlingMudokon>());
            }
            else if (type == "foot_switch")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_FootSwitch>());
            }
            else if (type == "paramite")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Paramite>());
            }
            else if (type == "zzz_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_ZzzSpawner>());
            }
            else if (type == "background_glukkon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_BackgroundGlukkon>());
            }
            else if (type == "gas_emitter")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_GasEmitter>());
            }
            else if (type == "gas_countdown")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_GasCountDown>());
            }
            else if (type == "ring_cancel")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RingCancel>());
            }
            else if (type == "security_door")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SecurityDoor>());
            }
            else if (type == "lift_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_LiftMudokon>());
            }
            else if (type == "ring_mudokon")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_RingMudokon>());
            }
            else if (type == "well_local")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_WellLocal>());
            }
            else if (type == "well_express")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_WellExpress>());
            }
            else if (type == "slog")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Slog>());
            }
            else if (type == "slog_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SlogSpawner>());
            }
            else if (type == "scrab")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Scrab>());
            }
            else if (type == "slig")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_Slig>());
            }
            else if (type == "slig_spawner")
            {
                mapObjects.at(i).get_to(*camEntry->AllocTLV<relive::Path_SligSpawner>());
            }
            else
            {
                ALIVE_FATAL("Unknown TLV");
            }

            camEntry->mLastAllocated->mLength = camEntry->mLastAllocatedSize;

            if (i == mapObjects.size() - 1)
            {
                camEntry->mLastAllocated->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
            }
        }
        mCameras.emplace_back(std::move(camEntry));
    }
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
