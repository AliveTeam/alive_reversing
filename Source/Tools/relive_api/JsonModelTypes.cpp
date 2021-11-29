#include "JsonModelTypes.hpp"
#include <jsonxx/jsonxx.h>

namespace ReliveAPI {
[[nodiscard]] jsonxx::Object CameraObject::ToJsonObject(jsonxx::Array mapObjectsArray) const
{
    jsonxx::Object obj;

    obj << "name" << mName;
    obj << "x" << mX;
    obj << "y" << mY;
    obj << "id" << mId;
    obj << "map_objects" << mapObjectsArray;

    return obj;
}

[[nodiscard]] std::size_t CameraNameAndTlvBlob::TotalTlvSize() const
{
    std::size_t allTlvsLen = 0;

    for (const auto& tlv : mTlvBlobs)
    {
        allTlvsLen += tlv.size();
    }

    return allTlvsLen;
}
} // namespace ReliveAPI
