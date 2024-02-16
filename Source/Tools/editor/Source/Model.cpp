#include "Model.hpp"
#include "ReliveApiWrapper.hpp"
#include <optional>
#include "../../relive_lib/data_conversion/relive_tlvs_serialization.hpp"
#include <QDebug>

static std::optional<std::string> LoadFileToString(const std::string& fileName)
{
    EditorFileIO fileIo;
    auto file = fileIo.Open(fileName, ReliveAPI::IFileIO::Mode::ReadBinary);
    if (!file->IsOpen())
    {
        return {};
    }
    std::string s;
    file->ReadInto(s);
    return { s };
}

static nlohmann::json ReadArray(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key) || !o.at(key).is_array())
    {
        throw Model::JsonKeyNotFoundException(key);
    }
    return o.at(key);
}

static nlohmann::json ReadObject(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key) || !o.at(key).is_object())
    {
        throw Model::JsonKeyNotFoundException(key);
    }
    return o.at(key);
}

static int ReadNumber(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key) || !o.at(key).is_number())
    {
        throw Model::JsonKeyNotFoundException(key);
    }
    return o.at(key).get<s32>();
}

static std::string ReadString(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key) || !o.at(key).is_string())
    {
        throw Model::JsonKeyNotFoundException(key);
    }
    return o.at(key);   
}

static std::string ReadStringOptional(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key) || !o.at(key).is_string())
    {
        return "";
    }
    return o.at(key);
}

static bool ReadBool(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key) || !o.at(key).is_boolean())
    {
        throw Model::JsonKeyNotFoundException(key);
    }
    return o.at(key);
}

Model::Camera* Model::GetContainingCamera(MapObjectBase* pMapObject)
{
    Model::Camera* pContainingCamera = nullptr;
    for (auto& camera : mCameras)
    {
        for (auto& mapObj : camera->mMapObjects)
        {
            if (mapObj.get() == pMapObject)
            {
                pContainingCamera = camera.get();
                break;
            }
        }
    }
    return pContainingCamera;
}

UP_MapObjectBase Model::TakeFromContainingCamera(MapObjectBase* pMapObject)
{
    for (auto& camera : mCameras)
    {
        for (auto it = camera->mMapObjects.begin(); it != camera->mMapObjects.end(); )
        {
            if ((*it).get() == pMapObject)
            {
                UP_MapObjectBase takenObj((*it).release());
                camera->mMapObjects.erase(it);
                return takenObj;
            }
            it++;
        }
    }
    return nullptr;
}

Model::UP_Camera Model::RemoveCamera(Model::Camera* pCamera)
{
    for (auto it = mCameras.begin(); it != mCameras.end(); )
    {
        if ((*it).get() == pCamera)
        {
            auto ret = std::move(*it);
            mCameras.erase(it);
            return ret;
        }
        it++;
    }
    return nullptr;
}

void Model::AddCamera(UP_Camera pCamera)
{
    mCameras.push_back(std::move(pCamera));
}

void Model::SwapContainingCamera(MapObjectBase* pMapObject, Camera* pTargetCamera)
{
    pTargetCamera->mMapObjects.push_back(TakeFromContainingCamera(pMapObject));
}

template<class T>
static void DeserializeObject(std::vector<UP_MapObjectBase>& objects, const nlohmann::json& mapObject)
{
    auto tmpMapObject = std::make_unique<T>();
    from_json(mapObject, tmpMapObject->mTlv);

    // Re-purpose to width/height for the editor
    tmpMapObject->mTlv.mBottomRightX = tmpMapObject->mTlv.Width();
    tmpMapObject->mTlv.mBottomRightY = tmpMapObject->mTlv.Height();

    objects.push_back(std::move(tmpMapObject));
}

static void DeserializeObjectFactory(std::vector<UP_MapObjectBase>& mapObjects, const nlohmann::json& mapObject, ReliveTypes type)
{
    switch (type)
    {
        case ReliveTypes::eTimedMine:
            DeserializeObject<relive::Editor_TimedMine>(mapObjects, mapObject);
            return;
        case ReliveTypes::eElectricWall:
            DeserializeObject<relive::Editor_ElectricWall>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMudokon:
            [[fallthrough]];
        case ReliveTypes::eRingOrLiftMud:
            DeserializeObject<relive::Editor_Mudokon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBirdPortal:
            DeserializeObject<relive::Editor_BirdPortal>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLCDStatusBoard:
            DeserializeObject<relive::Editor_LCDStatusBoard>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDoor:
            DeserializeObject<relive::Editor_Door>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLever:
            DeserializeObject<relive::Editor_Lever>(mapObjects, mapObject);
            return;
        case ReliveTypes::eHoist:
            DeserializeObject<relive::Editor_Hoist>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBoomMachine:
            DeserializeObject<relive::Editor_BoomMachine>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlig:
            DeserializeObject<relive::Editor_Slig>(mapObjects, mapObject);
            return;
        case ReliveTypes::eFleech:
            DeserializeObject<relive::Editor_Fleech>(mapObjects, mapObject);
            return;
        case ReliveTypes::eEnemyStopper:
            DeserializeObject<relive::Editor_EnemyStopper>(mapObjects, mapObject);
            return;
        case ReliveTypes::eTeleporter:
            DeserializeObject<relive::Editor_Teleporter>(mapObjects, mapObject);
            return;
        case ReliveTypes::eUXB:
            DeserializeObject<relive::Editor_UXB>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLCDScreen:
            DeserializeObject<relive::Editor_LCDScreen>(mapObjects, mapObject);
            return;
        case ReliveTypes::eEdge:
            DeserializeObject<relive::Editor_Edge>(mapObjects, mapObject);
            return;
        case ReliveTypes::eStatusLight:
            DeserializeObject<relive::Editor_StatusLight>(mapObjects, mapObject);
            return;
        case ReliveTypes::eShadowZone:
            DeserializeObject<relive::Editor_ShadowZone>(mapObjects, mapObject);
            return;
        case ReliveTypes::eWorkWheel:
            DeserializeObject<relive::Editor_WorkWheel>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMusicTrigger:
            DeserializeObject<relive::Editor_MusicTrigger>(mapObjects, mapObject);
            return;
        case ReliveTypes::eAbeStart:
            DeserializeObject<relive::Editor_AbeStart>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSoftLanding:
            DeserializeObject<relive::Editor_SoftLanding>(mapObjects, mapObject);
            return;
        case ReliveTypes::eWellExpress:
            DeserializeObject<relive::Editor_WellExpress>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlamDoor:
            DeserializeObject<relive::Editor_SlamDoor>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLaughingGas:
            DeserializeObject<relive::Editor_LaughingGas>(mapObjects, mapObject);
            return;
        case ReliveTypes::eInvisibleSwitch:
            DeserializeObject<relive::Editor_InvisibleSwitch>(mapObjects, mapObject);
            return;
        case ReliveTypes::eWater:
            DeserializeObject<relive::Editor_Water>(mapObjects, mapObject);
            return;
        case ReliveTypes::eGasEmitter:
            DeserializeObject<relive::Editor_GasEmitter>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBackgroundAnimation:
            DeserializeObject<relive::Editor_BackgroundAnimation>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLiftPoint:
            DeserializeObject<relive::Editor_LiftPoint>(mapObjects, mapObject);
            return;
        case ReliveTypes::ePullRingRope:
            DeserializeObject<relive::Editor_PullRingRope>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMultiSwitchController:
            DeserializeObject<relive::Editor_MultiSwitchController>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSecurityOrb:
            DeserializeObject<relive::Editor_SecurityOrb>(mapObjects, mapObject);
            return;
        case ReliveTypes::eInvisibleZone:
            DeserializeObject<relive::Editor_InvisibleZone>(mapObjects, mapObject);
            return;
        case ReliveTypes::eContinuePoint:
            DeserializeObject<relive::Editor_ContinuePoint>(mapObjects, mapObject);
            return;
        case ReliveTypes::eWheelSyncer:
            DeserializeObject<relive::Editor_WheelSyncer>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLevelLoader:
            DeserializeObject<relive::Editor_LevelLoader>(mapObjects, mapObject);
            return;
        case ReliveTypes::ePulley:
            DeserializeObject<relive::Editor_Pulley>(mapObjects, mapObject);
            return;
        case ReliveTypes::eFlyingSlig:
            DeserializeObject<relive::Editor_FlyingSlig>(mapObjects, mapObject);
            return;
        case ReliveTypes::eFlyingSligSpawner:
            DeserializeObject<relive::Editor_FlyingSligSpawner>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDeathDrop:
            DeserializeObject<relive::Editor_DeathDrop>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSligSpawner:
            DeserializeObject<relive::Editor_SligSpawner>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSligBoundLeft:
            DeserializeObject<relive::Editor_SligBoundLeft>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSligBoundRight:
            DeserializeObject<relive::Editor_SligBoundRight>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSligPersist:
            DeserializeObject<relive::Editor_SligPersist>(mapObjects, mapObject);
            return;
        case ReliveTypes::eZSligCover:
            DeserializeObject<relive::Editor_ZSligCover>(mapObjects, mapObject);
            return;
        case ReliveTypes::eWellLocal:
            DeserializeObject<relive::Editor_WellLocal>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBrewMachine:
            DeserializeObject<relive::Editor_BrewMachine>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDrill:
            DeserializeObject<relive::Editor_Drill>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMine:
            DeserializeObject<relive::Editor_Mine>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlog:
            DeserializeObject<relive::Editor_Slog>(mapObjects, mapObject);
            return;
        case ReliveTypes::eResetPath:
            DeserializeObject<relive::Editor_ResetPath>(mapObjects, mapObject);
            return;
        case ReliveTypes::eTrapDoor:
            DeserializeObject<relive::Editor_TrapDoor>(mapObjects, mapObject);
            return;
        case ReliveTypes::ePathTransition:
            DeserializeObject<relive::Editor_PathTransition>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLiftMover:
            DeserializeObject<relive::Editor_LiftMover>(mapObjects, mapObject);
            return;
        case ReliveTypes::eRockSack:
            DeserializeObject<relive::Editor_RockSack>(mapObjects, mapObject);
            return;
        case ReliveTypes::eTimerTrigger:
            DeserializeObject<relive::Editor_TimerTrigger>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMotionDetector:
            DeserializeObject<relive::Editor_MotionDetector>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMineCar:
            DeserializeObject<relive::Editor_MineCar>(mapObjects, mapObject);
            return;
        case ReliveTypes::eExplosionSet:
            DeserializeObject<relive::Editor_ExplosionSet>(mapObjects, mapObject);
            return;
        case ReliveTypes::eColourfulMeter:
            DeserializeObject<relive::Editor_ColourfulMeter>(mapObjects, mapObject);
            return;
        case ReliveTypes::eAlarm:
            DeserializeObject<relive::Editor_Alarm>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDemoSpawnPoint:
            DeserializeObject<relive::Editor_DemoSpawnPoint>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlapLock:
            DeserializeObject<relive::Editor_SlapLock>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlurg:
            DeserializeObject<relive::Editor_Slurg>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDoorBlocker:
            DeserializeObject<relive::Editor_DoorBlocker>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDove:
            DeserializeObject<relive::Editor_Dove>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBirdPortalExit:
            DeserializeObject<relive::Editor_BirdPortalExit>(mapObjects, mapObject);
            return;
        case ReliveTypes::eDoorFlame:
            DeserializeObject<relive::Editor_DoorFlame>(mapObjects, mapObject);
            return;
        case ReliveTypes::eTrainDoor:
            DeserializeObject<relive::Editor_TrainDoor>(mapObjects, mapObject);
            return;
        case ReliveTypes::eGreeter:
            DeserializeObject<relive::Editor_Greeter>(mapObjects, mapObject);
            return;
        case ReliveTypes::eScrabLeftBound:
            DeserializeObject<relive::Editor_ScrabBoundLeft>(mapObjects, mapObject);
            return;
        case ReliveTypes::eScrabRightBound:
            DeserializeObject<relive::Editor_ScrabBoundRight>(mapObjects, mapObject);
            return;
        case ReliveTypes::eCreditsController:
            DeserializeObject<relive::Editor_CreditsController>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMovieHandStone:
            DeserializeObject<relive::Editor_MovieStone>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMovingBomb:
            DeserializeObject<relive::Editor_MovingBomb>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMovingBombStopper:
            DeserializeObject<relive::Editor_MovingBombStopper>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSecurityDoor:
            DeserializeObject<relive::Editor_SecurityDoor>(mapObjects, mapObject);
            return;
        case ReliveTypes::eCrawlingSlig:
            DeserializeObject<relive::Editor_CrawlingSlig>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSligGetPants:
            DeserializeObject<relive::Editor_SligGetPants>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSligGetWings:
            DeserializeObject<relive::Editor_SligGetWings>(mapObjects, mapObject);
            return;
        case ReliveTypes::eCrawlingSligButton:
            DeserializeObject<relive::Editor_CrawlingSligButton>(mapObjects, mapObject);
            return;
        case ReliveTypes::eGlukkon:
            DeserializeObject<relive::Editor_Glukkon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eGlukkonSwitch:
            DeserializeObject<relive::Editor_GlukkonSwitch>(mapObjects, mapObject);
            return;
        case ReliveTypes::eGasCountDown:
            DeserializeObject<relive::Editor_GasCountDown>(mapObjects, mapObject);
            return;
        case ReliveTypes::eFallingItem:
            DeserializeObject<relive::Editor_FallingItem>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBoneBag:
            DeserializeObject<relive::Editor_BoneBag>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSecurityClaw:
            DeserializeObject<relive::Editor_SecurityClaw>(mapObjects, mapObject);
            return;
        case ReliveTypes::eFootSwitch:
            DeserializeObject<relive::Editor_FootSwitch>(mapObjects, mapObject);
            return;
        case ReliveTypes::eZzzSpawner:
            DeserializeObject<relive::Editor_ZzzSpawner>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlogSpawner:
            DeserializeObject<relive::Editor_SlogSpawner>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMainMenuController:
            DeserializeObject<relive::Editor_MainMenuController>(mapObjects, mapObject);
            return;
        case ReliveTypes::eScrab:
            DeserializeObject<relive::Editor_Scrab>(mapObjects, mapObject);
            return;
        case ReliveTypes::eScrabSpawner:
            DeserializeObject<relive::Editor_ScrabSpawner>(mapObjects, mapObject);
            return;
        case ReliveTypes::eSlurgSpawner:
            DeserializeObject<relive::Editor_SlurgSpawner>(mapObjects, mapObject);
            return;
        case ReliveTypes::eParamite:
            DeserializeObject<relive::Editor_Paramite>(mapObjects, mapObject);
            return;
        case ReliveTypes::eParamiteWebLine:
            DeserializeObject<relive::Editor_ParamiteWebLine>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMeatSack:
            DeserializeObject<relive::Editor_MeatSack>(mapObjects, mapObject);
            return;
        case ReliveTypes::eTorturedMud:
            DeserializeObject<relive::Editor_TorturedMudokon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eKillUnsavedMuds:
            DeserializeObject<relive::Editor_KillUnsavedMuds>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBackgroundGlukkon:
            DeserializeObject<relive::Editor_BackgroundGlukkon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBellSongStone:
            DeserializeObject<relive::Editor_BellsongStone>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLightEffect:
            DeserializeObject<relive::Editor_LightEffect>(mapObjects, mapObject);
            return;
        case ReliveTypes::eStartController:
            DeserializeObject<relive::Editor_StartController>(mapObjects, mapObject);
            return;
        case ReliveTypes::eHintFly:
            DeserializeObject<relive::Editor_HintFly>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBat:
            DeserializeObject<relive::Editor_Bat>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBellHammer:
            DeserializeObject<relive::Editor_BellHammer>(mapObjects, mapObject);
            return;
        case ReliveTypes::eElumPathTrans:
            DeserializeObject<relive::Editor_ElumPathTrans>(mapObjects, mapObject);
            return;
        case ReliveTypes::eElumStart:
            DeserializeObject<relive::Editor_ElumStart>(mapObjects, mapObject);
            return;
        case ReliveTypes::eElumWall:
            DeserializeObject<relive::Editor_ElumWall>(mapObjects, mapObject);
            return;
        case ReliveTypes::eRingMudokon:
            DeserializeObject<relive::Editor_RingMudokon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eRingCancel:
            DeserializeObject<relive::Editor_RingCancel>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMeatSaw:
            DeserializeObject<relive::Editor_MeatSaw>(mapObjects, mapObject);
            return;
        case ReliveTypes::eChimeLock:
            DeserializeObject<relive::Editor_ChimeLock>(mapObjects, mapObject);
            return;
        case ReliveTypes::eFlintLockFire:
            DeserializeObject<relive::Editor_FlintLockFire>(mapObjects, mapObject);
            return;
        case ReliveTypes::eMudokonPathTrans:
            DeserializeObject<relive::Editor_MudokonPathTrans>(mapObjects, mapObject);
            return;
        case ReliveTypes::eScrabNoFall:
            DeserializeObject<relive::Editor_ScrabNoFall>(mapObjects, mapObject);
            return;
        case ReliveTypes::eLiftMudokon:
            DeserializeObject<relive::Editor_LiftMudokon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eHoneySack:
            DeserializeObject<relive::Editor_HoneySack>(mapObjects, mapObject);
            return;
        case ReliveTypes::SlingMud:
            DeserializeObject<relive::Editor_SlingMudokon>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBeeSwarmHole:
            DeserializeObject<relive::Editor_BeeSwarmHole>(mapObjects, mapObject);
            return;
        case ReliveTypes::eRollingBall:
            DeserializeObject<relive::Editor_RollingBall>(mapObjects, mapObject);
            return;
        case ReliveTypes::eRollingBallStopper:
            DeserializeObject<relive::Editor_RollingBallStopper>(mapObjects, mapObject);
            return;
        case ReliveTypes::eBeeNest:
            DeserializeObject<relive::Editor_BeeNest>(mapObjects, mapObject);
            return;
        case ReliveTypes::eZBall:
            DeserializeObject<relive::Editor_ZBall>(mapObjects, mapObject);
            return;
        case ReliveTypes::eHoney:
            DeserializeObject<relive::Editor_Honey>(mapObjects, mapObject);
            return;
        case ReliveTypes::eHoneyDripTarget:
            DeserializeObject<relive::Editor_HoneyDripTarget>(mapObjects, mapObject);
            return;
        case ReliveTypes::eElum:
            DeserializeObject<relive::Editor_Elum>(mapObjects, mapObject);
            return;
        default:
            return;
    }
}

template<class T>
static void SerializeObject(nlohmann::json& mapObjects, relive::Path_TLV* pTlv)
{
    // Re-purpose to mBottomRightX/mBottomRightY for ingame use
    pTlv->mBottomRightX += pTlv->mTopLeftX;
    pTlv->mBottomRightY += pTlv->mTopLeftY;

    nlohmann::json mapObj;
    to_json(mapObj, *static_cast<T*>(pTlv));
    mapObjects.push_back(mapObj);
}

static void SerializeObjectFactory(nlohmann::json& mapObjects, relive::Path_TLV* pTlv)
{
    switch (pTlv->mTlvType)
    {
        case ReliveTypes::eTimedMine:
            SerializeObject<relive::Path_TimedMine>(mapObjects, pTlv);
            return;
        case ReliveTypes::eElectricWall:
            SerializeObject<relive::Path_ElectricWall>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMudokon:
            [[fallthrough]];
        case ReliveTypes::eRingOrLiftMud:
            SerializeObject<relive::Path_Mudokon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBirdPortal:
            SerializeObject<relive::Path_BirdPortal>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLCDStatusBoard:
            SerializeObject<relive::Path_LCDStatusBoard>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDoor:
            SerializeObject<relive::Path_Door>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLever:
            SerializeObject<relive::Path_Lever>(mapObjects, pTlv);
            return;
        case ReliveTypes::eHoist:
            SerializeObject<relive::Path_Hoist>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBoomMachine:
            SerializeObject<relive::Path_BoomMachine>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlig:
            SerializeObject<relive::Path_Slig>(mapObjects, pTlv);
            return;
        case ReliveTypes::eFleech:
            SerializeObject<relive::Path_Fleech>(mapObjects, pTlv);
            return;
        case ReliveTypes::eEnemyStopper:
            SerializeObject<relive::Path_EnemyStopper>(mapObjects, pTlv);
            return;
        case ReliveTypes::eTeleporter:
            SerializeObject<relive::Path_Teleporter>(mapObjects, pTlv);
            return;
        case ReliveTypes::eUXB:
            SerializeObject<relive::Path_UXB>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLCDScreen:
            SerializeObject<relive::Path_LCDScreen>(mapObjects, pTlv);
            return;
        case ReliveTypes::eEdge:
            SerializeObject<relive::Path_Edge>(mapObjects, pTlv);
            return;
        case ReliveTypes::eStatusLight:
            SerializeObject<relive::Path_StatusLight>(mapObjects, pTlv);
            return;
        case ReliveTypes::eShadowZone:
            SerializeObject<relive::Path_ShadowZone>(mapObjects, pTlv);
            return;
        case ReliveTypes::eWorkWheel:
            SerializeObject<relive::Path_WorkWheel>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMusicTrigger:
            SerializeObject<relive::Path_MusicTrigger>(mapObjects, pTlv);
            return;
        case ReliveTypes::eAbeStart:
            SerializeObject<relive::Path_AbeStart>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSoftLanding:
            SerializeObject<relive::Path_SoftLanding>(mapObjects, pTlv);
            return;
        case ReliveTypes::eWellExpress:
            SerializeObject<relive::Path_WellExpress>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlamDoor:
            SerializeObject<relive::Path_SlamDoor>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLaughingGas:
            SerializeObject<relive::Path_LaughingGas>(mapObjects, pTlv);
            return;
        case ReliveTypes::eInvisibleSwitch:
            SerializeObject<relive::Path_InvisibleSwitch>(mapObjects, pTlv);
            return;
        case ReliveTypes::eWater:
            SerializeObject<relive::Path_Water>(mapObjects, pTlv);
            return;
        case ReliveTypes::eGasEmitter:
            SerializeObject<relive::Path_GasEmitter>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBackgroundAnimation:
            SerializeObject<relive::Path_BackgroundAnimation>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLiftPoint:
            SerializeObject<relive::Path_LiftPoint>(mapObjects, pTlv);
            return;
        case ReliveTypes::ePullRingRope:
            SerializeObject<relive::Path_PullRingRope>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMultiSwitchController:
            SerializeObject<relive::Path_MultiSwitchController>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSecurityOrb:
            SerializeObject<relive::Path_SecurityOrb>(mapObjects, pTlv);
            return;
        case ReliveTypes::eInvisibleZone:
            SerializeObject<relive::Path_InvisibleZone>(mapObjects, pTlv);
            return;
        case ReliveTypes::eContinuePoint:
            SerializeObject<relive::Path_ContinuePoint>(mapObjects, pTlv);
            return;
        case ReliveTypes::eWheelSyncer:
            SerializeObject<relive::Path_WheelSyncer>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLevelLoader:
            SerializeObject<relive::Path_LevelLoader>(mapObjects, pTlv);
            return;
        case ReliveTypes::ePulley:
            SerializeObject<relive::Path_Pulley>(mapObjects, pTlv);
            return;
        case ReliveTypes::eFlyingSlig:
            SerializeObject<relive::Path_FlyingSlig>(mapObjects, pTlv);
            return;
        case ReliveTypes::eFlyingSligSpawner:
            SerializeObject<relive::Path_FlyingSligSpawner>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDeathDrop:
            SerializeObject<relive::Path_DeathDrop>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSligSpawner:
            SerializeObject<relive::Path_SligSpawner>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSligBoundLeft:
            SerializeObject<relive::Path_SligBoundLeft>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSligBoundRight:
            SerializeObject<relive::Path_SligBoundRight>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSligPersist:
            SerializeObject<relive::Path_SligPersist>(mapObjects, pTlv);
            return;
        case ReliveTypes::eZSligCover:
            SerializeObject<relive::Path_ZSligCover>(mapObjects, pTlv);
            return;
        case ReliveTypes::eWellLocal:
            SerializeObject<relive::Path_WellLocal>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBrewMachine:
            SerializeObject<relive::Path_BrewMachine>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDrill:
            SerializeObject<relive::Path_Drill>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMine:
            SerializeObject<relive::Path_Mine>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlog:
            SerializeObject<relive::Path_Slog>(mapObjects, pTlv);
            return;
        case ReliveTypes::eResetPath:
            SerializeObject<relive::Path_ResetPath>(mapObjects, pTlv);
            return;
        case ReliveTypes::eTrapDoor:
            SerializeObject<relive::Path_TrapDoor>(mapObjects, pTlv);
            return;
        case ReliveTypes::ePathTransition:
            SerializeObject<relive::Path_PathTransition>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLiftMover:
            SerializeObject<relive::Path_LiftMover>(mapObjects, pTlv);
            return;
        case ReliveTypes::eRockSack:
            SerializeObject<relive::Path_RockSack>(mapObjects, pTlv);
            return;
        case ReliveTypes::eTimerTrigger:
            SerializeObject<relive::Path_TimerTrigger>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMotionDetector:
            SerializeObject<relive::Path_MotionDetector>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMineCar:
            SerializeObject<relive::Path_MineCar>(mapObjects, pTlv);
            return;
        case ReliveTypes::eExplosionSet:
            SerializeObject<relive::Path_ExplosionSet>(mapObjects, pTlv);
            return;
        case ReliveTypes::eColourfulMeter:
            SerializeObject<relive::Path_ColourfulMeter>(mapObjects, pTlv);
            return;
        case ReliveTypes::eAlarm:
            SerializeObject<relive::Path_Alarm>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDemoSpawnPoint:
            SerializeObject<relive::Path_DemoSpawnPoint>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlapLock:
            SerializeObject<relive::Path_SlapLock>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlurg:
            SerializeObject<relive::Path_Slurg>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDoorBlocker:
            SerializeObject<relive::Path_DoorBlocker>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDove:
            SerializeObject<relive::Path_Dove>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBirdPortalExit:
            SerializeObject<relive::Path_BirdPortalExit>(mapObjects, pTlv);
            return;
        case ReliveTypes::eDoorFlame:
            SerializeObject<relive::Path_DoorFlame>(mapObjects, pTlv);
            return;
        case ReliveTypes::eTrainDoor:
            SerializeObject<relive::Path_TrainDoor>(mapObjects, pTlv);
            return;
        case ReliveTypes::eGreeter:
            SerializeObject<relive::Path_Greeter>(mapObjects, pTlv);
            return;
        case ReliveTypes::eScrabLeftBound:
            SerializeObject<relive::Path_ScrabBoundLeft>(mapObjects, pTlv);
            return;
        case ReliveTypes::eScrabRightBound:
            SerializeObject<relive::Path_ScrabBoundRight>(mapObjects, pTlv);
            return;
        case ReliveTypes::eCreditsController:
            SerializeObject<relive::Path_CreditsController>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMovieHandStone:
            SerializeObject<relive::Path_MovieStone>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMovingBomb:
            SerializeObject<relive::Path_MovingBomb>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMovingBombStopper:
            SerializeObject<relive::Path_MovingBombStopper>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSecurityDoor:
            SerializeObject<relive::Path_SecurityDoor>(mapObjects, pTlv);
            return;
        case ReliveTypes::eCrawlingSlig:
            SerializeObject<relive::Path_CrawlingSlig>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSligGetPants:
            SerializeObject<relive::Path_SligGetPants>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSligGetWings:
            SerializeObject<relive::Path_SligGetWings>(mapObjects, pTlv);
            return;
        case ReliveTypes::eCrawlingSligButton:
            SerializeObject<relive::Path_CrawlingSligButton>(mapObjects, pTlv);
            return;
        case ReliveTypes::eGlukkon:
            SerializeObject<relive::Path_Glukkon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eGlukkonSwitch:
            SerializeObject<relive::Path_GlukkonSwitch>(mapObjects, pTlv);
            return;
        case ReliveTypes::eGasCountDown:
            SerializeObject<relive::Path_GasCountDown>(mapObjects, pTlv);
            return;
        case ReliveTypes::eFallingItem:
            SerializeObject<relive::Path_FallingItem>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBoneBag:
            SerializeObject<relive::Path_BoneBag>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSecurityClaw:
            SerializeObject<relive::Path_SecurityClaw>(mapObjects, pTlv);
            return;
        case ReliveTypes::eFootSwitch:
            SerializeObject<relive::Path_FootSwitch>(mapObjects, pTlv);
            return;
        case ReliveTypes::eZzzSpawner:
            SerializeObject<relive::Path_ZzzSpawner>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlogSpawner:
            SerializeObject<relive::Path_SlogSpawner>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMainMenuController:
            SerializeObject<relive::Path_MainMenuController>(mapObjects, pTlv);
            return;
        case ReliveTypes::eScrab:
            SerializeObject<relive::Path_Scrab>(mapObjects, pTlv);
            return;
        case ReliveTypes::eScrabSpawner:
            SerializeObject<relive::Path_ScrabSpawner>(mapObjects, pTlv);
            return;
        case ReliveTypes::eSlurgSpawner:
            SerializeObject<relive::Path_SlurgSpawner>(mapObjects, pTlv);
            return;
        case ReliveTypes::eParamite:
            SerializeObject<relive::Path_Paramite>(mapObjects, pTlv);
            return;
        case ReliveTypes::eParamiteWebLine:
            SerializeObject<relive::Path_ParamiteWebLine>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMeatSack:
            SerializeObject<relive::Path_MeatSack>(mapObjects, pTlv);
            return;
        case ReliveTypes::eTorturedMud:
            SerializeObject<relive::Path_TorturedMudokon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eKillUnsavedMuds:
            SerializeObject<relive::Path_KillUnsavedMuds>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBackgroundGlukkon:
            SerializeObject<relive::Path_BackgroundGlukkon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBellSongStone:
            SerializeObject<relive::Path_BellsongStone>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLightEffect:
            SerializeObject<relive::Path_LightEffect>(mapObjects, pTlv);
            return;
        case ReliveTypes::eStartController:
            SerializeObject<relive::Path_StartController>(mapObjects, pTlv);
            return;
        case ReliveTypes::eHintFly:
            SerializeObject<relive::Path_HintFly>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBat:
            SerializeObject<relive::Path_Bat>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBellHammer:
            SerializeObject<relive::Path_BellHammer>(mapObjects, pTlv);
            return;
        case ReliveTypes::eElumPathTrans:
            SerializeObject<relive::Path_ElumPathTrans>(mapObjects, pTlv);
            return;
        case ReliveTypes::eElumStart:
            SerializeObject<relive::Path_ElumStart>(mapObjects, pTlv);
            return;
        case ReliveTypes::eElumWall:
            SerializeObject<relive::Path_ElumWall>(mapObjects, pTlv);
            return;
        case ReliveTypes::eRingMudokon:
            SerializeObject<relive::Path_RingMudokon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eRingCancel:
            SerializeObject<relive::Path_RingCancel>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMeatSaw:
            SerializeObject<relive::Path_MeatSaw>(mapObjects, pTlv);
            return;
        case ReliveTypes::eChimeLock:
            SerializeObject<relive::Path_ChimeLock>(mapObjects, pTlv);
            return;
        case ReliveTypes::eFlintLockFire:
            SerializeObject<relive::Path_FlintLockFire>(mapObjects, pTlv);
            return;
        case ReliveTypes::eMudokonPathTrans:
            SerializeObject<relive::Path_MudokonPathTrans>(mapObjects, pTlv);
            return;
        case ReliveTypes::eScrabNoFall:
            SerializeObject<relive::Path_ScrabNoFall>(mapObjects, pTlv);
            return;
        case ReliveTypes::eLiftMudokon:
            SerializeObject<relive::Path_LiftMudokon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eHoneySack:
            SerializeObject<relive::Path_HoneySack>(mapObjects, pTlv);
            return;
        case ReliveTypes::SlingMud:
            SerializeObject<relive::Path_SlingMudokon>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBeeSwarmHole:
            SerializeObject<relive::Path_BeeSwarmHole>(mapObjects, pTlv);
            return;
        case ReliveTypes::eRollingBall:
            SerializeObject<relive::Path_RollingBall>(mapObjects, pTlv);
            return;
        case ReliveTypes::eRollingBallStopper:
            SerializeObject<relive::Path_RollingBallStopper>(mapObjects, pTlv);
            return;
        case ReliveTypes::eBeeNest:
            SerializeObject<relive::Path_BeeNest>(mapObjects, pTlv);
            return;
        case ReliveTypes::eZBall:
            SerializeObject<relive::Path_ZBall>(mapObjects, pTlv);
            return;
        case ReliveTypes::eHoney:
            SerializeObject<relive::Path_Honey>(mapObjects, pTlv);
            return;
        case ReliveTypes::eHoneyDripTarget:
            SerializeObject<relive::Path_HoneyDripTarget>(mapObjects, pTlv);
            return;
        case ReliveTypes::eElum:
            SerializeObject<relive::Path_Elum>(mapObjects, pTlv);
            return;
        default:
            return;
    }
}

void Model::LoadJsonFromString(const std::string& json)
{
    // TODO: Handle exception on bad data
    nlohmann::json root = nlohmann::json::parse(json);


    const std::string game =  ReadString(root, "game");
    mGame = game == "AO" ? GameType::eAo : GameType::eAe;

    nlohmann::json map = ReadObject(root, "map");


    nlohmann::json cameras = ReadArray(map, "cameras");
    for (size_t i = 0; i < cameras.size(); i++)
    {
        nlohmann::json camera = cameras.at(static_cast<unsigned int>(i));

        auto tmpCamera = std::make_unique<Camera>();
        tmpCamera->mId = ReadNumber(camera, "id");
        tmpCamera->mName = ReadString(camera, "name");
        tmpCamera->mX = ReadNumber(camera, "x");
        tmpCamera->mY = ReadNumber(camera, "y");

        /*
        tmpCamera->mCameraImageandLayers.mCameraImage = ReadStringOptional(camera, "image");
        tmpCamera->mCameraImageandLayers.mForegroundLayer = ReadStringOptional(camera, "foreground_layer");
        tmpCamera->mCameraImageandLayers.mBackgroundLayer = ReadStringOptional(camera, "background_layer");
        tmpCamera->mCameraImageandLayers.mForegroundWellLayer = ReadStringOptional(camera, "foreground_well_layer");
        tmpCamera->mCameraImageandLayers.mBackgroundWellLayer = ReadStringOptional(camera, "background_well_layer");
        */

        if (camera.contains("map_objects") && camera["map_objects"].is_array())
        {
            nlohmann::json mapObjects = ReadArray(camera, "map_objects");



            for (const auto& mapObject : mapObjects)
            {
                const ReliveTypes objType = mapObject.at("tlv_type").template get<ReliveTypes>();

                DeserializeObjectFactory(tmpCamera->mMapObjects, mapObject, objType);
            }
        }
        mCameras.push_back(std::move(tmpCamera));
    }

    nlohmann::json collisionsArray = ReadArray(map, "collisions");
    for (size_t i = 0; i < collisionsArray.size(); i++)
    {
        nlohmann::json collision = collisionsArray.at(static_cast<u32>(i));
        auto tmpCollision = std::make_unique<CollisionObject>(static_cast<s32>(i));
        collision.get_to(tmpCollision->mLine);
        mCollisions.push_back(std::move(tmpCollision));
    }

    CalculateMapSize();
    CreateEmptyCameras();
}

void Model::LoadJsonFromFile(const std::string& jsonFile)
{
    std::optional<std::string> jsonString = LoadFileToString(jsonFile);
    if (!jsonString)
    {
        throw IOReadException(jsonFile);
    }

    LoadJsonFromString(*jsonString);
}

void Model::CreateAsNewPath(int newPathId)
{
    // Reset everything to a 1x1 empty map
    /*
    mMapInfo.mPathId = newPathId;
    mMapInfo.mXSize = 1;
    mMapInfo.mYSize = 1;
    */

    mCameras.clear();
    mCollisions.clear();

    auto cam = std::make_unique<Camera>();
    cam->mX = 0;
    cam->mY = 0;
    mCameras.emplace_back(std::move(cam));
}

std::string Model::ToJson() const
{
    nlohmann::json root = nlohmann::json::object();

   // root << "path_version" << mMapInfo.mPathVersion;
    root["game"] = mGame == GameType::eAo ? "AO" : "AE";

    nlohmann::json map = nlohmann::json::object();
    /*
    map << "path_bnd" << mMapInfo.mPathBnd;
    map << "path_id" << mMapInfo.mPathId;
    map << "x_grid_size" << mMapInfo.mXGridSize;
    map << "x_size" << mMapInfo.mXSize;
    map << "y_grid_size" << mMapInfo.mYGridSize;
    map << "y_size" << mMapInfo.mYSize;

    map << "abe_start_xpos" << mMapInfo.mAbeStartXPos;
    map << "abe_start_ypos" << mMapInfo.mAbeStartYPos;

    map << "num_muds_in_path" << mMapInfo.mNumMudsInPath;
    map << "total_muds" << mMapInfo.mTotalMuds;
    map << "num_muds_for_bad_ending" << mMapInfo.mBadEndingMuds;
    map << "num_muds_for_good_ending" << mMapInfo.mGoodEndingMuds;

    jsonxx::Array LCDScreenMessages;
    for (const auto& msg : mMapInfo.mLCDScreenMessages)
    {
        LCDScreenMessages << msg;
    }
    map << "lcdscreen_messages" << LCDScreenMessages;

    jsonxx::Array hintFlyMessages;
    for (const auto& msg : mMapInfo.mHintFlyMessages)
    {
        hintFlyMessages << msg;
    }
    map << "hintfly_messages" << hintFlyMessages;
    */

    nlohmann::json cameras = nlohmann::json::array();
    for (auto& camera : mCameras)
    {
        if (!camera->mMapObjects.empty() /*||  !camera->mCameraImageandLayers.mCameraImage.empty()*/)
        {

            /*
            nlohmann::json{
            {"continue_point_zone_number", p.mContinuePoint_ZoneNumber},
            */

           
            /*/
            if (!camera->mCameraImageandLayers.mCameraImage.empty())
            {
                camObj << "image" << camera->mCameraImageandLayers.mCameraImage;
            }

            if (!camera->mCameraImageandLayers.mForegroundLayer.empty())
            {
                camObj << "foreground_layer" << camera->mCameraImageandLayers.mForegroundLayer;
            }

            if (!camera->mCameraImageandLayers.mBackgroundLayer.empty())
            {
                camObj << "background_layer" << camera->mCameraImageandLayers.mBackgroundLayer;
            }

            if (!camera->mCameraImageandLayers.mForegroundWellLayer.empty())
            {
                camObj << "foreground_well_layer" << camera->mCameraImageandLayers.mForegroundWellLayer;
            }

            if (!camera->mCameraImageandLayers.mBackgroundWellLayer.empty())
            {
                camObj << "background_well_layer" << camera->mCameraImageandLayers.mBackgroundWellLayer;
            }
            */

            nlohmann::json mapObjects = nlohmann::json::array();
            for (auto& mapObject : camera->mMapObjects)
            {
                SerializeObjectFactory(mapObjects, mapObject->mBaseTlv);
            }

            nlohmann::json camObj{
                {"name", camera->mName},
                {"id", camera->mId},
                {"x", camera->mX},
                {"y", camera->mY},
                {"map_objects", mapObjects}
            };

            cameras.push_back(camObj);
        }
    }

    nlohmann::json collisionsArray = nlohmann::json::array();
    for (auto& collision : mCollisions)
    {
        nlohmann::json collisionObj = nlohmann::json::object();
        to_json(collisionObj, collision->mLine);
        collisionsArray.push_back(collisionObj);
    }

    map["collisions"] = collisionsArray;
    map["cameras"] = cameras;

    root["map"] = map;

    return root.dump(4);
}

Model::UP_CollisionObject Model::RemoveCollisionItem(Model::CollisionObject* pItem)
{
    for (auto it = mCollisions.begin(); it != mCollisions.end(); )
    {
        if ((*it).get() == pItem)
        {
            auto ret = std::move(*it);
            mCollisions.erase(it);
            return ret;
        }
        it++;
    }
    return nullptr;
}

void Model::CreateEmptyCameras()
{
    // Make sure every cell in the "map" has a camera object
    for (u32 x = 0; x < XSize(); x++)
    {
        for (u32 y = 0; y < YSize(); y++)
        {
            if (!CameraAt(x, y))
            {
                auto cam = std::make_unique<Camera>();
                cam->mX = x;
                cam->mY = y;
                mCameras.emplace_back(std::move(cam));
            }
        }
    }
}

void Model::CalculateMapSize()
{
    mXSize = 0;
    mYSize = 0;

    for (const auto& cam : mCameras)
    {
        if (cam->mX > static_cast<s32>(mXSize))
        {
            mXSize = cam->mX;
        }

        if (cam->mY > static_cast<s32>(mYSize))
        {
            mYSize = cam->mY;
        }
    }

    mXSize++;
    mYSize++;
}

Model::CollisionObject::CollisionObject(int id, const Model::CollisionObject& rhs)
    : mId(id)
{

}
