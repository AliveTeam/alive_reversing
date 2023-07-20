#include "Model.hpp"
#include "ReliveApiWrapper.hpp"
#include <optional>
#include <fstream>

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

static jsonxx::Array ReadArray(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Array>(key))
    {
        throw JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::Array>(key);
}

static jsonxx::Object ReadObject(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Object>(key))
    {
        throw JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::Object>(key);
}

static int ReadNumber(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Number>(key))
    {
        throw JsonKeyNotFoundException(key);
    }
    return static_cast<int>(o.get<jsonxx::Number>(key));
}

static std::string ReadString(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::String>(key))
    {
        throw JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::String>(key);
}

static std::string ReadStringOptional(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::String>(key))
    {
        return "";
    }
    return o.get<jsonxx::String>(key);
}

static bool ReadBool(jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Boolean>(key))
    {
        throw JsonKeyNotFoundException(key);
    }
    return o.get<jsonxx::Boolean>(key);
}

static std::vector<EnumOrBasicTypeProperty> ReadObjectStructureProperties(jsonxx::Array& enumAndBasicTypes)
{
    std::vector<EnumOrBasicTypeProperty> properties;
    for (size_t j = 0; j < enumAndBasicTypes.size(); j++)
    {
        jsonxx::Object enumOrBasicType = enumAndBasicTypes.get<jsonxx::Object>(static_cast<unsigned int>(j));
        EnumOrBasicTypeProperty tmpEnumOrBasicTypeProperty;
        tmpEnumOrBasicTypeProperty.mName = ReadString(enumOrBasicType, "name");
        tmpEnumOrBasicTypeProperty.mType = ReadString(enumOrBasicType, "Type");
        tmpEnumOrBasicTypeProperty.mVisible = ReadBool(enumOrBasicType, "Visible");
        properties.push_back(tmpEnumOrBasicTypeProperty);
    }
    return properties;
}

static UP_ObjectStructure ReadObjectStructure(jsonxx::Object& objectStructure)
{
    auto tmpObjectStructure = std::make_unique<ObjectStructure>();
    tmpObjectStructure->mName = ReadString(objectStructure, "name");

    jsonxx::Array enumAndBasicTypes = ReadArray(objectStructure, "enum_and_basic_type_properties");
    tmpObjectStructure->mEnumAndBasicTypeProperties = ReadObjectStructureProperties(enumAndBasicTypes);

    return tmpObjectStructure;
}

Camera* Model::GetContainingCamera(MapObject* pMapObject)
{
    Camera* pContainingCamera = nullptr;
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

UP_MapObject Model::TakeFromContainingCamera(MapObject* pMapObject)
{
    for (auto& camera : mCameras)
    {
        for (auto it = camera->mMapObjects.begin(); it != camera->mMapObjects.end(); )
        {
            if ((*it).get() == pMapObject)
            {
                UP_MapObject takenObj((*it).release());
                camera->mMapObjects.erase(it);
                return takenObj;
            }
            it++;
        }
    }
    return nullptr;
}

UP_Camera Model::RemoveCamera(Camera* pCamera)
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

void Model::SwapContainingCamera(MapObject* pMapObject, Camera* pTargetCamera)
{
    pTargetCamera->mMapObjects.push_back(TakeFromContainingCamera(pMapObject));
}

UP_ObjectProperty Model::MakeProperty(const Model::FoundType foundTypes, const EnumOrBasicTypeProperty& property, const ObjectStructure* pObjStructure)
{
    auto tmpProperty = std::make_unique<ObjectProperty>();
    tmpProperty->mName = property.mName;
    tmpProperty->mTypeName = property.mType;
    tmpProperty->mVisible = property.mVisible;
    if (foundTypes.mBasicType)
    {
        tmpProperty->mType = ObjectProperty::Type::BasicType;
    }
    else
    {
        tmpProperty->mType = ObjectProperty::Type::Enumeration;
    }
    return tmpProperty;
}

std::vector<UP_ObjectProperty> Model::ReadProperties(const ObjectStructure* pObjStructure, jsonxx::Object& properties)
{
    std::vector<UP_ObjectProperty> tmpProperties;
    for (const EnumOrBasicTypeProperty& property : pObjStructure->mEnumAndBasicTypeProperties)
    {
        const FoundType foundTypes = FindType(property.mType);
        if (!foundTypes.mEnum && !foundTypes.mBasicType)
        {
            // corrupted schema type name has no definition
            throw ObjectPropertyTypeNotFoundException(property.mName, property.mType);
        }

        auto tmpProperty = MakeProperty(foundTypes, property, pObjStructure);
        if (foundTypes.mBasicType)
        {
            tmpProperty->mBasicTypeValue = ReadNumber(properties, property.mName);
        }
        else
        {
            tmpProperty->mEnumValue = ReadString(properties, property.mName);
        }
        tmpProperties.push_back(std::move(tmpProperty));
    }
    return tmpProperties;
}

void Model::LoadJsonFromString(const std::string& json)
{
    jsonxx::Object root;
    if (!root.parse(json))
    {
        throw InvalidJsonException();
    }

    mMapInfo.mPathVersion = ReadNumber(root, "path_version");
    mMapInfo.mGame = ReadString(root, "game");

    jsonxx::Object map = ReadObject(root, "map");

    mMapInfo.mPathBnd = ReadString(map, "path_bnd");
    mMapInfo.mPathId = ReadNumber(map, "path_id");
    mMapInfo.mXGridSize = ReadNumber(map, "x_grid_size");
    mMapInfo.mXSize = ReadNumber(map, "x_size");
    mMapInfo.mYGridSize = ReadNumber(map, "y_grid_size");
    mMapInfo.mYSize = ReadNumber(map, "y_size");

    mMapInfo.mAbeStartXPos = ReadNumber(map, "abe_start_xpos");
    mMapInfo.mAbeStartYPos = ReadNumber(map, "abe_start_ypos");
    mMapInfo.mNumMudsInPath = ReadNumber(map, "num_muds_in_path");
    mMapInfo.mTotalMuds = ReadNumber(map, "total_muds");
    mMapInfo.mBadEndingMuds = ReadNumber(map, "num_muds_for_bad_ending");
    mMapInfo.mGoodEndingMuds = ReadNumber(map, "num_muds_for_good_ending");

    jsonxx::Array LCDScreenMessages = ReadArray(map, "lcdscreen_messages");
    for (size_t i = 0; i < LCDScreenMessages.size(); i++)
    {
        mMapInfo.mLCDScreenMessages.emplace_back(LCDScreenMessages.get<jsonxx::String>(static_cast<unsigned int>(i)));
    }

    jsonxx::Array hintFlyMessages = ReadArray(map, "hintfly_messages");
    for (size_t i = 0; i < hintFlyMessages.size(); i++)
    {
        mMapInfo.mHintFlyMessages.emplace_back(hintFlyMessages.get<jsonxx::String>(static_cast<unsigned int>(i)));
    }

    mSchema = ReadObject(root, "schema");

    jsonxx::Array basicTypes = ReadArray(mSchema, "object_structure_property_basic_types");
    for (size_t i = 0; i < basicTypes.size(); i++)
    {
        jsonxx::Object basicType = basicTypes.get<jsonxx::Object>(static_cast<unsigned int>(i));
        auto tmpBasicType = std::make_unique<BasicType>();
        tmpBasicType->mName = ReadString(basicType, "name");
        tmpBasicType->mMaxValue = ReadNumber(basicType, "max_value");
        tmpBasicType->mMinValue = ReadNumber(basicType, "min_value");
        mBasicTypes.push_back(std::move(tmpBasicType));
    }

    jsonxx::Array enums = ReadArray(mSchema, "object_structure_property_enums");
    for (size_t i = 0; i < enums.size(); i++)
    {
        jsonxx::Object enumObject = enums.get<jsonxx::Object>(static_cast<unsigned int>(i));
        auto tmpEnum = std::make_unique<Enum>();
        tmpEnum->mName = ReadString(enumObject, "name");

        jsonxx::Array enumValuesArray = ReadArray(enumObject, "values");
        for (size_t j = 0; j < enumValuesArray.size(); j++)
        {
            tmpEnum->mValues.push_back(enumValuesArray.get<jsonxx::String>(static_cast<unsigned int>(j)));
        }
        mEnums.push_back(std::move(tmpEnum));
    }

    jsonxx::Array objectStructures = ReadArray(mSchema, "object_structures");
    for (size_t i = 0; i < objectStructures.size(); i++)
    {
        jsonxx::Object objectStructure = objectStructures.get<jsonxx::Object>(static_cast<unsigned int>(i));
        auto tmpObjectStructure = ReadObjectStructure(objectStructure);
        mObjectStructures.push_back(std::move(tmpObjectStructure));
    }

    jsonxx::Array cameras = ReadArray(map, "cameras");
    for (size_t i = 0; i < cameras.size(); i++)
    {
        jsonxx::Object camera = cameras.get<jsonxx::Object>(static_cast<unsigned int>(i));

        auto tmpCamera = std::make_unique<Camera>();
        tmpCamera->mId = ReadNumber(camera, "id");
        tmpCamera->mName = ReadString(camera, "name");
        tmpCamera->mX = ReadNumber(camera, "x");
        tmpCamera->mY = ReadNumber(camera, "y");

        tmpCamera->mCameraImageandLayers.mCameraImage = ReadStringOptional(camera, "image");
        tmpCamera->mCameraImageandLayers.mForegroundLayer = ReadStringOptional(camera, "foreground_layer");
        tmpCamera->mCameraImageandLayers.mBackgroundLayer = ReadStringOptional(camera, "background_layer");
        tmpCamera->mCameraImageandLayers.mForegroundWellLayer = ReadStringOptional(camera, "foreground_well_layer");
        tmpCamera->mCameraImageandLayers.mBackgroundWellLayer = ReadStringOptional(camera, "background_well_layer");

        if (camera.has<jsonxx::Array>("map_objects"))
        {
            jsonxx::Array mapObjects = ReadArray(camera, "map_objects");

            for (size_t j = 0; j < mapObjects.size(); j++)
            {
                jsonxx::Object mapObject = mapObjects.get<jsonxx::Object>(static_cast<unsigned int>(j));
                auto tmpMapObject = std::make_unique<MapObject>();
                tmpMapObject->mName = ReadString(mapObject, "name");
                tmpMapObject->mObjectStructureType = ReadString(mapObject, "object_structures_type");

                if (mapObject.has<jsonxx::Object>("properties"))
                {
                    const ObjectStructure* pObjStructure = nullptr;
                    for (const auto& objStruct : mObjectStructures)
                    {
                        if (objStruct->mName == tmpMapObject->mObjectStructureType)
                        {
                            pObjStructure = objStruct.get();
                            break;
                        }
                    }

                    if (!pObjStructure)
                    {
                        throw JsonKeyNotFoundException(tmpMapObject->mObjectStructureType);
                    }

                    jsonxx::Object properties = ReadObject(mapObject, "properties");
                    tmpMapObject->mProperties = ReadProperties(pObjStructure, properties);
                }

                tmpCamera->mMapObjects.push_back(std::move(tmpMapObject));
            }
        }
        mCameras.push_back(std::move(tmpCamera));
    }


    jsonxx::Object collisionObject = ReadObject(map, "collisions");
    jsonxx::Array collisionsArray = ReadArray(collisionObject, "items");
    mCollisionStructureSchema = ReadArray(collisionObject, "structure");

    mCollisionStructure = std::make_unique<ObjectStructure>();
    mCollisionStructure->mName = "Collision";
    mCollisionStructure->mEnumAndBasicTypeProperties = ReadObjectStructureProperties(mCollisionStructureSchema);

    for (size_t i = 0; i < collisionsArray.size(); i++)
    {
        jsonxx::Object collision = collisionsArray.get<jsonxx::Object>(static_cast<unsigned int>(i));

        auto tmpCollision = std::make_unique<CollisionObject>(static_cast<int>(i));
        tmpCollision->mProperties = ReadProperties(mCollisionStructure.get(), collision);
        mCollisions.push_back(std::move(tmpCollision));
    }

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
    mMapInfo.mPathId = newPathId;
    mMapInfo.mXSize = 1;
    mMapInfo.mYSize = 1;

    mCameras.clear();
    mCollisions.clear();

    auto cam = std::make_unique<Camera>();
    cam->mX = 0;
    cam->mY = 0;
    mCameras.emplace_back(std::move(cam));
}

std::string Model::ToJson() const
{
    jsonxx::Object root;

    root << "api_version" << mMapInfo.mPathVersion;
    root << "game" << mMapInfo.mGame;

    jsonxx::Object map;
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

    jsonxx::Array cameras;
    for (auto& camera : mCameras)
    {
        if (!camera->mMapObjects.empty() || !camera->mCameraImageandLayers.mCameraImage.empty())
        {
            jsonxx::Object camObj;
            camObj << "id" << camera->mId;
            camObj << "name" << camera->mName;
            camObj << "x" << camera->mX;
            camObj << "y" << camera->mY;

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

            jsonxx::Array mapObjects;
            for (auto& mapObject : camera->mMapObjects)
            {
                jsonxx::Object mapObj;
                mapObj << "name" << mapObject->mName;
                mapObj << "object_structures_type" << mapObject->mObjectStructureType;
                jsonxx::Object propertiesObject;
                for (auto& property : mapObject->mProperties)
                {
                    switch (property->mType)
                    {
                    case ObjectProperty::Type::BasicType:
                        propertiesObject << property->mName << property->mBasicTypeValue;
                        break;

                    case ObjectProperty::Type::Enumeration:
                        propertiesObject << property->mName << property->mEnumValue;
                        break;
                    }
                }
                mapObj << "properties" << propertiesObject;
                mapObjects << mapObj;
            }
            camObj << "map_objects" << mapObjects;

            cameras << camObj;
        }
    }

    jsonxx::Object collisionsObject;
    jsonxx::Array collisionItems;
    for (auto& collision : mCollisions)
    {
        jsonxx::Object collisionObj;
        for (auto& property : collision->mProperties)
        {
            switch (property->mType)
            {
            case ObjectProperty::Type::BasicType:
                // Special case handling for next/previous property links, map line Ids to line index
                if (property->mName == "Previous" || property->mName == "Next")
                {
                    collisionObj << property->mName << IndexOfCollisionId(property->mBasicTypeValue);
                }
                else
                {
                    collisionObj << property->mName << property->mBasicTypeValue;
                }
                break;

            case ObjectProperty::Type::Enumeration:
                collisionObj << property->mName << property->mEnumValue;
                break;
            }
        }
        collisionItems << collisionObj;
    }

    collisionsObject << "items" << collisionItems;
    collisionsObject << "structure" << mCollisionStructureSchema;
    map << "collisions" << collisionsObject;

    map << "cameras" << cameras;
    root << "map" << map;

    root << "schema" << mSchema;

    return root.json();
}

UP_CollisionObject Model::RemoveCollisionItem(CollisionObject* pItem)
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
    for (int x = 0; x < mMapInfo.mXSize; x++)
    {
        for (int y = 0; y < mMapInfo.mYSize; y++)
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

ObjectProperty* PropertyByName(const std::string& name, std::vector<UP_ObjectProperty>& props)
{
    for (auto& prop : props)
    {
        if (prop->mName == name)
        {
            return prop.get();
        }
    }
    return nullptr;
}

const ObjectProperty* PropertyByName(const std::string& name, const std::vector<UP_ObjectProperty>& props)
{
    for (const auto& prop : props)
    {
        if (prop->mName == name)
        {
            return prop.get();
        }
    }
    return nullptr;
}

MapObject::MapObject(const MapObject& rhs)
    : mName(rhs.mName),
      mObjectStructureType(rhs.mObjectStructureType)
{
    for (auto& prop : rhs.mProperties)
    {
        mProperties.emplace_back(std::make_unique<ObjectProperty>(*prop));
    }
}

CollisionObject::CollisionObject(int id, const CollisionObject& rhs)
    : mId(id)
{
    for (auto& prop : rhs.mProperties)
    {
        mProperties.emplace_back(std::make_unique<ObjectProperty>(*prop));
    }
}
