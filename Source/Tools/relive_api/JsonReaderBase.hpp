#pragma once

#include "JsonModelTypes.hpp"
#include "PropertyCollection.hpp"
#include "../../AliveLibAO/Collisions.hpp"
#include "../../AliveLibAE/Collisions.hpp"
#include "TlvObjectBaseMacros.hpp"

namespace ReliveAPI {
class AOLine final : public PropertyCollection
{
public:
    AOLine(TypesCollectionBase& globalTypes, const AO::PathLine* line = nullptr)
    {
        if (line)
        {
            mLine = *line;
        }

        ADD("x1", mLine.field_0_rect.x);
        ADD("y1", mLine.field_0_rect.y);

        ADD("x2", mLine.field_0_rect.w);
        ADD("y2", mLine.field_0_rect.h);

        ADD("Type", mLine.field_8_type);

        ADD("Next", mLine.field_10_next);
        ADD("Previous", mLine.field_C_previous);
    }

    AO::PathLine mLine = {};
};

class AELine final : public PropertyCollection
{
public:
    AELine(TypesCollectionBase& globalTypes, const PathLine* line = nullptr)
    {
        if (line)
        {
            mLine = *line;
        }

        ADD("x1", mLine.field_0_rect.x);
        ADD("y1", mLine.field_0_rect.y);

        ADD("x2", mLine.field_0_rect.w);
        ADD("y2", mLine.field_0_rect.h);

        ADD("Type", mLine.field_8_type);

        ADD("Next", mLine.field_C_next);
        ADD("Previous", mLine.field_A_previous);

        ADD("Next 2", mLine.field_10_next2);
        ADD("Previous 2", mLine.field_E_previous2);

        ADD("Length", mLine.field_12_line_length);
    }

    PathLine mLine = {};
};

class IFileIO;
class Context;

class JsonReaderBase
{
public:
    MapInfo mRootInfo;

protected:
    std::pair<std::vector<CameraNameAndTlvBlob>, jsonxx::Object> Load(TypesCollectionBase& types, IFileIO& fileIO, const std::string& fileName, Context& context);

    std::vector<AO::PathLine> ReadAOLines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray, Context& context);
    std::vector<::PathLine> ReadAELines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray, Context& context);
};
} // namespace ReliveAPI
