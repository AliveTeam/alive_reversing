#pragma once

#include "JsonModelTypes.hpp"
#include "PropertyCollection.hpp"
#include "../../relive_lib/Collisions.hpp"
#include "TlvObjectBaseMacros.hpp"

namespace ReliveAPI {
class ReliveLine final : public PropertyCollection
{
public:
    ReliveLine(TypesCollectionBase& globalTypes, const ::PathLine* line = nullptr)
    {
        if (line)
        {
            mLine = *line;
        }

        ADD("x1", mLine.mRect.x);
        ADD("y1", mLine.mRect.y);

        ADD("x2", mLine.mRect.w);
        ADD("y2", mLine.mRect.h);

        ADD("Type", mLine.mLineType);

        ADD("Next", mLine.mNext);
        ADD("Previous", mLine.mPrevious);

        ADD("Length", mLine.mLineLength);
    }

    PathLine mLine = {};
};

class IFileIO;
class Context;


struct LoadedJsonBase final
{
    std::vector<CameraNameAndTlvBlob> mPerCamData;
    std::set<AnimId> mResourcesRequiredInLvl;
    jsonxx::Object mMapJson;
};

class JsonReaderBase
{
public:
    MapInfo mRootInfo;

protected:
    LoadedJsonBase Load(TypesCollectionBase& types, IFileIO& fileIO, const std::string& fileName, Context& context);

    std::vector<::PathLine> ReadReliveLines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray, Context& context);
};
} // namespace ReliveAPI
