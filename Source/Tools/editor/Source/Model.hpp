#pragma once

#include <string>
#include <vector>
#include <memory>
#include "nlohmann/json.hpp"
#include "../../relive_lib/ObjectTypes.hpp"
#include "../../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../../relive_lib/Collisions.hpp"

class MapObjectBase
{
public:
    MapObjectBase(ReliveTypes tlvType)
        : mType(tlvType)
    {

    }


    ReliveTypes mType = ReliveTypes::eNone;
};

struct Path_TimedMine final : public MapObjectBase
{
    Path_TimedMine()
        : MapObjectBase(ReliveTypes::eTimedMine)
    {
        //ADD("Scale", mTlv.mScale);
        //ADD("Ticks Before Explosion", mTlv.mTicksUntilExplosion);
    }

    relive::Path_TimedMine mTlv = {};
};

using UP_MapObjectBase = std::unique_ptr<MapObjectBase>;

class Model final
{
public:
    class ModelException
    {
    public:
        virtual ~ModelException()
        { }
        ModelException() = default;
        explicit ModelException(const std::string& what)
            : mWhat(what)
        { }
        const std::string& what() const
        {
            return mWhat;
        }

    protected:
        std::string mWhat;
    };

    // Error opening json file on disk
    class IOReadException final : public ModelException
    {
    public:
        using ModelException::ModelException;
    };

    // Json data failed to parse
    class InvalidJsonException final : public ModelException
    { };

    // Game name in the json isn't AO or AE
    class InvalidGameException final : public ModelException
    {
    public:
        using ModelException::ModelException;
    };

    // In the json schema the "typeName" couldn't be found in basic or enum types for "structureName"
    class ObjectPropertyTypeNotFoundException final : public ModelException
    {
    public:
        explicit ObjectPropertyTypeNotFoundException(const std::string& structureName, const std::string& typeName)
            : ModelException(structureName + ":" + typeName)
            , mStructName(structureName)
            , mTypeName(typeName)
        {
        }

        const std::string& StructName() const
        {
            return mStructName;
        }
        const std::string& TypeName() const
        {
            return mTypeName;
        }

    private:
        std::string mStructName;
        std::string mTypeName;
    };

    // Expected to read "key" in the json but it either didn't exist or was the wrong type
    class JsonKeyNotFoundException final : public ModelException
    {
    public:
        explicit JsonKeyNotFoundException(const std::string& key)
            : ModelException(key)
            , mKey(key)
        {
        }

        const std::string& Key() const
        {
            return mKey;
        }

    private:
        std::string mKey;
    };


    struct Camera final
    {
        std::string mName;
        int mId = 0;
        int mX = 0;
        int mY = 0;
        std::vector<UP_MapObjectBase> mMapObjects;

        /*
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
        */
    };
    using UP_Camera = std::unique_ptr<Camera>;

    class CollisionObject final
    {
    public:
        explicit CollisionObject(int id)
            : mId(id)
        { }

        CollisionObject(const CollisionObject&) = delete;

        CollisionObject(int id, const CollisionObject& rhs);

        int X1() const
        {
            return mLine.mRect.x;
        }

        void SetX1(int x1)
        {
            mLine.mRect.x = x1;
        }

        int Y1() const
        {
            return mLine.mRect.y;
        }

        void SetY1(int y1)
        {
            mLine.mRect.y = y1;
        }

        int X2() const
        {
            return mLine.mRect.w;
        }

        void SetX2(int x2)
        {
            mLine.mRect.w = x2;
        }

        int Y2() const
        {
            return mLine.mRect.h;
        }

        void SetY2(int y2)
        {
            mLine.mRect.h = y2;
        }

        int Next() const
        {
            return mLine.mNext;
        }

        int Previous() const
        {
            return mLine.mPrevious;
        }

        // The previous/next in the collision data is the index of the next/previous line. Removing or adding line
        // will cause this to break so we remap to a generated Id that then gets normalized back to indicies on save
        // by looking up the index of the line with the given Id.
        int mId = 0;

        PathLine mLine = {};
    };
    using UP_CollisionObject = std::unique_ptr<CollisionObject>;

    struct Enum final
    {
        std::string mName;
        std::vector<std::string> mValues;
    };
    using UP_Enum = std::unique_ptr<Enum>;


    void LoadJsonFromString(const std::string& json);
    void LoadJsonFromFile(const std::string& jsonFile);
    void CreateAsNewPath(int newPathId);
    std::string ToJson() const;

    Camera* GetContainingCamera(MapObjectBase* pMapObject);

    UP_MapObjectBase TakeFromContainingCamera(MapObjectBase* pMapObject);

    UP_Camera RemoveCamera(Camera* pCamera);
    void AddCamera(UP_Camera pCamera);

    void SwapContainingCamera(MapObjectBase* pMapObject, Camera* pTargetCamera);

    const std::vector<UP_Camera>& GetCameras() const 
    {
        return mCameras; 
    }

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

    std::vector<UP_Camera> mCameras;
    std::vector<UP_CollisionObject> mCollisions;
};
using UP_Model = std::unique_ptr<Model>;
