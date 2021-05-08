#include "JsonWriterAO.hpp"
#include <jsonxx/jsonxx.h>
#include "../AliveLibAO/Map.hpp"
#include "TypesCollectionAO.hpp"
#include "TlvObjectBase.hpp"
#include "JsonReaderBase.hpp"

JsonWriterAO::JsonWriterAO(std::unique_ptr<TypesCollectionAO>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(*typesCollection, pathId, pathBndName, info)
    , mTypesCollection{ std::move(typesCollection) }
{
    mMapRootInfo.mGame = "AO";
}

JsonWriterAO::JsonWriterAO(s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterAO{ std::make_unique<TypesCollectionAO>(), pathId, pathBndName, info }
{
}


[[nodiscard]] jsonxx::Array JsonWriterAO::ReadTlvStream(u8* ptr)
{
    jsonxx::Array mapObjects;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
    pPathTLV->RangeCheck();

    if (static_cast<s32>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            mTypeCounterMap[pPathTLV->field_4_type.mType]++;

            auto obj = mTypesCollection->MakeTlvAO(pPathTLV->field_4_type.mType, pPathTLV, mTypeCounterMap[pPathTLV->field_4_type.mType]);
            if (obj)
            {
                if (pPathTLV->field_2_length != obj->TlvLen())
                {
                    LOG_ERROR(magic_enum::enum_name(pPathTLV->field_4_type.mType) << " size should be " << pPathTLV->field_2_length << " but got " << obj->TlvLen());
                    throw ReliveAPI::WrongTLVLengthException();
                }
                mapObjects << obj->InstanceToJson(*mTypesCollection);
            }
            else
            {
                LOG_WARNING("Ignoring type: " << magic_enum::enum_name(pPathTLV->field_4_type.mType));
            }

            pPathTLV = AO::Path_TLV::Next_446460(pPathTLV);
            if (pPathTLV)
            {
                pPathTLV->RangeCheck();
            }
        }
    }

    return mapObjects;
}

[[nodiscard]] jsonxx::Array JsonWriterAO::AddCollisionLineStructureJson()
{
    AOLine tmpLine(*mTypesCollection);
    return tmpLine.PropertiesToJson();
}

void JsonWriterAO::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

[[nodiscard]] jsonxx::Array JsonWriterAO::ReadCollisionStream(u8* ptr, s32 numItems)
{
    jsonxx::Array collisionsArray;
    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(ptr);
    TypesCollectionAO types;

    for (s32 i = 0; i < numItems; i++)
    {
        AOLine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties);

        collisionsArray << properties;
    }
    return collisionsArray;
}

void JsonWriterAO::DebugDumpTlvs(const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource)
{
    u8* pStart = pathResource.data() + info.mObjectOffset;
    u8* pEnd = pathResource.data() + info.mIndexTableOffset;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(pStart);
    pPathTLV->RangeCheck();
    s32 idx = 0;
    if (static_cast<s32>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            idx++;
            DebugDumpTlv(prefix, idx, *pPathTLV);
            pPathTLV = AO::Path_TLV::Next_NoCheck(pPathTLV);
            if (pPathTLV)
            {
                pPathTLV->RangeCheck();
                if (pPathTLV->field_2_length == 0)
                {
                    // Dont get stuck in a loop
                    break;
                }

                if (reinterpret_cast<u8*>(pPathTLV) == pEnd)
                {
                    break;
                }
            }
        }
    }
}

JsonWriterAO::~JsonWriterAO() = default;
