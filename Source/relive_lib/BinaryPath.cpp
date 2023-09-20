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
    virtual ~ITlvAllocator() = default;
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

    nlohmann::json& collisions = pathJson["map"]["collisions"];
    mCollisions.resize(collisions.size());
    s32 idx = 0;
    for (auto& collision : collisions)
    {
        collision.get_to(mCollisions[idx]);
        idx++;
    }

    for (auto& cam : pathJson["map"]["cameras"])
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

            kTlvJsonTypeMap.DeseralizeTlvJsonType(type, mapObjects.at(i), camEntry.get());

            camEntry->mLastAllocated->mLength = camEntry->mLastAllocatedSize;

            if (i == mapObjects.size() - 1)
            {
                camEntry->mLastAllocated->mTlvFlags.Set(relive::TlvFlags::eBit3_End_TLV_List);
            }
        }
        mCameras.emplace_back(std::move(camEntry));
    }

    mSoundInfo = std::make_shared<PathSoundInfo>();
    from_json(pathJson["map"]["sound_info"], *mSoundInfo);
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
