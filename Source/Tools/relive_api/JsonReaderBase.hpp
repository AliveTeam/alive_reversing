#pragma once

#include "JsonModelTypes.hpp"
#include "PropertyCollection.hpp"
#include "../../AliveLibAO/Collisions.hpp"
#include "../../AliveLibAE/Collisions.hpp"
#include "TlvObjectBaseMacros.hpp"

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

        ADD("type", mLine.field_8_type);

        ADD("next", mLine.field_10_next);
        ADD("previous", mLine.field_C_previous);
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

        ADD("type", mLine.field_8_type);

        ADD("next", mLine.field_C_next);
        ADD("previous", mLine.field_A_previous);

        ADD("next2", mLine.field_10_next2);
        ADD("previous2", mLine.field_E_previous2);

        ADD("length", mLine.field_12_line_length);
    }

    PathLine mLine = {};
};

class JsonReaderBase
{
public:
    MapInfo mRootInfo;

protected:
    std::pair<std::vector<CameraNameAndTlvBlob>, jsonxx::Object> Load(TypesCollectionBase& types, const std::string& fileName);

    std::vector<AO::PathLine> ReadAOLines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray);
    std::vector<::PathLine> ReadAELines(TypesCollectionBase& types, const jsonxx::Array& collisionsArray);
};
