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

EditorCamera* Model::GetContainingCamera(MapObjectBase* pMapObject)
{
    EditorCamera* pContainingCamera = nullptr;
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

std::unique_ptr<EditorCamera> Model::RemoveCamera(EditorCamera* pCamera)
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

void Model::SwapContainingCamera(MapObjectBase* pMapObject, EditorCamera* pTargetCamera)
{
    pTargetCamera->mMapObjects.push_back(TakeFromContainingCamera(pMapObject));
}

void Model::LoadJsonFromString(const std::string& json)
{
    // TODO: Handle exception on bad data
    nlohmann::json root = nlohmann::json::parse(json);

    const std::string game =  ReadString(root, "game");
    mGame = game == "AO" ? GameType::eAo : GameType::eAe;
    mPathVersion = ReadNumber(root, "path_version");

    nlohmann::json map = ReadObject(root, "map");
    mPathId = ReadNumber(map, "path_id");
    mSoundInfo = ReadObject(map, "sound_info");

    nlohmann::json cameras = ReadArray(map, "cameras");

    for (size_t i = 0; i < cameras.size(); i++)
    {
        nlohmann::json camera = cameras.at(static_cast<unsigned int>(i));

        auto tmpCamera = std::make_unique<EditorCamera>();
        tmpCamera->mId = ReadNumber(camera, "id");
        tmpCamera->mName = ReadString(camera, "name");
        tmpCamera->mX = ReadNumber(camera, "x");
        tmpCamera->mY = ReadNumber(camera, "y");

        if (camera.contains("map_objects") && camera["map_objects"].is_array())
        {
            nlohmann::json mapObjects = ReadArray(camera, "map_objects");

            for (const auto& mapObject : mapObjects)
            {
                const ReliveTypes objType = mapObject.at("tlv_type").template get<ReliveTypes>();
                auto it = MapObjectBase::GetEditorFactoryRegistry().find(objType);
                if (it != std::end(MapObjectBase::GetEditorFactoryRegistry()))
                {
                    tmpCamera->mMapObjects.emplace_back(it->second.mDeserializeFunc(mapObject));
                }
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

    auto cam = std::make_unique<EditorCamera>();
    cam->mX = 0;
    cam->mY = 0;
    mCameras.emplace_back(std::move(cam));
}

std::string Model::ToJson() const
{
    nlohmann::json root = nlohmann::json::object();

    root["game"] = mGame == GameType::eAo ? "AO" : "AE";
    root["path_version"] = mPathVersion;

    nlohmann::json map = nlohmann::json::object();
    map["path_id"] = mPathId;
    map["sound_info"] = mSoundInfo;

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

        if (!camera->mMapObjects.empty() || camera->mId != 0)
        {
            nlohmann::json mapObjects = nlohmann::json::array();
            for (auto& mapObject : camera->mMapObjects)
            {
                mapObjects.push_back(mapObject->SerializeObject());
            }

            nlohmann::json camObj{
                {"name", camera->mName},
                {"id", camera->mId},
                {"x", camera->mX},
                {"y", camera->mY},
                {"map_objects", mapObjects}};

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

std::unique_ptr<CollisionObject> Model::RemoveCollisionItem(CollisionObject* pItem)
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
                auto cam = std::make_unique<EditorCamera>();
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

CollisionObject::CollisionObject(int id, const CollisionObject& rhs)
    : mId(id)
{
    mLine = rhs.mLine;
}
