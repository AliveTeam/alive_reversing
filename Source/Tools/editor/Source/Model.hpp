#pragma once

#include <string>
#include <vector>
#include <memory>
#include <jsonxx.h>

class ModelException
{
public:
    virtual ~ModelException() { }
    ModelException() = default;
    explicit ModelException(const std::string& what) : mWhat(what) { }
    const std::string& what() const
    {
        return mWhat;
    }
protected:
    std::string mWhat;
};

// Error opening json file on disk
class IOReadException final : public ModelException { public: using ModelException::ModelException; };

// Json data failed to parse
class InvalidJsonException final : public ModelException {};

// Game name in the json isn't AO or AE
class InvalidGameException final : public ModelException { public: using ModelException::ModelException; };

// In the json schema the "typeName" couldn't be found in basic or enum types for "structureName"
class ObjectPropertyTypeNotFoundException final : public ModelException
{
public:
    explicit ObjectPropertyTypeNotFoundException(const std::string& structureName, const std::string& typeName)
        : ModelException(structureName + ":" + typeName), mStructName(structureName), mTypeName(typeName)
    {

    }

    const std::string& StructName() const { return mStructName; }
    const std::string& TypeName() const { return mTypeName; }

private:
    std::string mStructName;
    std::string mTypeName;
};

// Expected to read "key" in the json but it either didn't exist or was the wrong type
class JsonKeyNotFoundException final : public ModelException
{
public:
    explicit JsonKeyNotFoundException(const std::string& key)
        : ModelException(key), mKey(key)
    {

    }

    const std::string& Key() const { return mKey; }

private:
    std::string mKey;
};

struct ObjectProperty final
{
    ObjectProperty() = default;
    ObjectProperty(const ObjectProperty&) = default;

    std::string mName;
    std::string mTypeName;
    int mBasicTypeValue = 0;
    std::string mEnumValue;
    bool mVisible = true;
    enum class Type
    {
        Enumeration,
        BasicType,
    };
    Type mType = {};
};
using UP_ObjectProperty = std::unique_ptr<ObjectProperty>;

ObjectProperty* PropertyByName(const std::string& name, std::vector<UP_ObjectProperty>& props);

const ObjectProperty* PropertyByName(const std::string& name, const std::vector<UP_ObjectProperty>& props);

struct MapObject final
{
    MapObject() = default;

    MapObject(const MapObject& rhs);

    std::string mName;
    std::string mObjectStructureType;
    std::vector<UP_ObjectProperty> mProperties;

    int XPos() const 
    {
        return PropertyByName("xpos", mProperties)->mBasicTypeValue;
    }

    void SetXPos(int xpos)
    {
        PropertyByName("xpos", mProperties)->mBasicTypeValue = xpos;
    }

    int YPos() const
    {
        return PropertyByName("ypos", mProperties)->mBasicTypeValue;
    }

    void SetYPos(int ypos)
    {
        PropertyByName("ypos", mProperties)->mBasicTypeValue = ypos;
    }

    int Width() const
    {
        return PropertyByName("width", mProperties)->mBasicTypeValue;
    }

    void SetWidth(int width)
    {
        PropertyByName("width", mProperties)->mBasicTypeValue = width;
    }

    int Height() const
    {
        return PropertyByName("height", mProperties)->mBasicTypeValue;
    }

    void SetHeight(int height)
    {
        PropertyByName("height", mProperties)->mBasicTypeValue = height;
    }
};
using UP_MapObject = std::unique_ptr<MapObject>;

struct Camera final
{
    std::string mName;
    int mId = 0;
    int mX = 0;
    int mY = 0;
    std::vector<UP_MapObject> mMapObjects;

    class CameraImageAndLayers final
    {
    public:
        std::string mCameraImage;
        std::string mForegroundLayer;
        std::string mBackgroundLayer;
        std::string mForegroundWellLayer;
        std::string mBackgroundWellLayer;
    };
    CameraImageAndLayers mCameraImageandLayers;

};
using UP_Camera = std::unique_ptr<Camera>;

class CollisionObject final
{
public:
    explicit CollisionObject(int id) : mId(id) { }
    
    CollisionObject(const CollisionObject&) = delete;

    CollisionObject(int id, const CollisionObject& rhs);

    std::vector<UP_ObjectProperty> mProperties;

    // The previous/next in the collision data is the index of the next/previous line. Removing or adding line
    // will cause this to break so we remap to a generated Id that then gets normalized back to indicies on save
    // by looking up the index of the line with the given Id.
    int mId = 0;

    int X1() const
    {
        return PropertyByName("x1", mProperties)->mBasicTypeValue;
    }

    void SetX1(int x1)
    {
        PropertyByName("x1", mProperties)->mBasicTypeValue = x1;
    }

    int Y1() const
    {
        return PropertyByName("y1", mProperties)->mBasicTypeValue;
    }

    void SetY1(int y1)
    {
        PropertyByName("y1", mProperties)->mBasicTypeValue = y1;
    }

    int X2() const
    {
        return PropertyByName("x2", mProperties)->mBasicTypeValue;
    }

    void SetX2(int x2)
    {
        PropertyByName("x2", mProperties)->mBasicTypeValue = x2;
    }

    int Y2() const
    {
        return PropertyByName("y2", mProperties)->mBasicTypeValue;
    }

    void SetY2(int y2)
    {
        PropertyByName("y2", mProperties)->mBasicTypeValue = y2;
    }

    int Next() const
    {
        return PropertyByName("Next", mProperties)->mBasicTypeValue;
    }

    int Previous() const
    {
        return PropertyByName("Previous", mProperties)->mBasicTypeValue;
    }
};
using UP_CollisionObject = std::unique_ptr<CollisionObject>;

struct Enum final
{
    std::string mName;
    std::vector<std::string> mValues;
};
using UP_Enum = std::unique_ptr<Enum>;

struct EnumOrBasicTypeProperty final
{
    std::string mName;
    std::string mType;
    bool mVisible = true;
};

struct ObjectStructure final
{
    std::string mName;
    std::vector<EnumOrBasicTypeProperty> mEnumAndBasicTypeProperties;
};
using UP_ObjectStructure = std::unique_ptr<ObjectStructure>;

struct BasicType final
{
    std::string mName;
    int mMinValue = 0;
    int mMaxValue = 0;
};
using UP_BasicType = std::unique_ptr<BasicType>;

struct MapInfo final
{
    int mApiVersion = 0;
    std::string mGame;
    std::string mPathBnd;
    int mPathId = 0;
    int mXGridSize = 0;
    int mXSize = 0;
    int mYGridSize = 0;
    int mYSize = 0;

    int mAbeStartXPos = 0;
    int mAbeStartYPos = 0;
    
    int mNumMudsInPath = 0;

    int mTotalMuds = 0;
    int mBadEndingMuds = 0;
    int mGoodEndingMuds = 0;

    std::vector<std::string> mLCDScreenMessages;
    std::vector<std::string> mHintFlyMessages;
};

class Model final
{
public:
    void LoadJsonFromString(const std::string& json);
    void LoadJsonFromFile(const std::string& jsonFile);
    void CreateAsNewPath(int newPathId);
    const MapInfo& GetMapInfo() const { return mMapInfo; }
    MapInfo& GetMapInfo() { return mMapInfo; }

    Camera* GetContainingCamera(MapObject* pMapObject);

    UP_MapObject TakeFromContainingCamera(MapObject* pMapObject);

    UP_Camera RemoveCamera(Camera* pCamera);
    void AddCamera(UP_Camera pCamera);

    void SwapContainingCamera(MapObject* pMapObject, Camera* pTargetCamera);

    struct FoundType;
    UP_ObjectProperty MakeProperty(const Model::FoundType foundTypes, const EnumOrBasicTypeProperty& property, const ObjectStructure* pObjStructure);

    const std::vector<UP_ObjectStructure>& GetObjectStructures() const 
    {
        return mObjectStructures;
    }

    const std::vector<UP_Camera>& GetCameras() const { return mCameras; }

    Camera* CameraAt(int x, int y) const
    {
        for (auto& cam : mCameras)
        {
            if (cam->mX == x && cam->mY == y)
            {
                return cam.get();
            }
        }
        return nullptr;
    }

    std::vector<UP_CollisionObject>& CollisionItems()
    {
        return mCollisions;
    }

    struct FoundType final
    {
        Enum* mEnum = nullptr;
        BasicType* mBasicType = nullptr;
    };

    Enum* FindEnum(const std::string& toFind)
    {
        for (const auto& enumType : mEnums)
        {
            if (enumType->mName == toFind)
            {
                return enumType.get();
            }
        }
        return nullptr;
    }

    BasicType* FindBasicType(const std::string& toFind)
    {
        for (const auto& basicType : mBasicTypes)
        {
            if (basicType->mName == toFind)
            {
                return basicType.get();
            }
        }
        return nullptr;
    }

    FoundType FindType(const std::string& toFind)
    {
        Enum* enumType = FindEnum(toFind);
        if (enumType)
        {
            return { enumType, nullptr };
        }

        BasicType* basicType = FindBasicType(toFind);
        if (basicType)
        {
            return { nullptr, basicType };
        }

        return { nullptr, nullptr };
    }

    std::string ToJson() const;

    const ObjectStructure& CollisionStructure() const
    {
        return *mCollisionStructure;
    }

    UP_CollisionObject RemoveCollisionItem(CollisionObject* pItem);

    int NextCollisionId() const
    {
        int biggestId = 0;
        for (auto& item : mCollisions)
        {
            if (item->mId > biggestId)
            {
                biggestId = item->mId;
            }
        }
        return biggestId + 1;
    }

    int IndexOfCollisionId(int id) const
    {
        if (id == -1)
        {
            return -1;
        }

        for (size_t i=0; i<mCollisions.size(); i++)
        {
            if (mCollisions[i]->mId == id)
            {
                return static_cast<int>(i);
            }
        }

        // Id wasn't found, bad input json ?
        return -1;
    }

private:
    void CreateEmptyCameras();

    std::vector<UP_ObjectProperty> ReadProperties(const ObjectStructure* pObjStructure, jsonxx::Object& properties);

    MapInfo mMapInfo;
    std::vector<UP_Camera> mCameras;
    std::vector<UP_CollisionObject> mCollisions;
    UP_ObjectStructure mCollisionStructure;

    std::vector<UP_Enum> mEnums;
    std::vector<UP_ObjectStructure> mObjectStructures;
    std::vector<UP_BasicType> mBasicTypes;

    // Keep a copy of this so we can save it back out
    jsonxx::Object mSchema;
    jsonxx::Array mCollisionStructureSchema;
};
using UP_Model = std::unique_ptr<Model>;
