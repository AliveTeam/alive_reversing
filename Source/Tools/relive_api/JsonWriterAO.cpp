#include "JsonWriterAO.hpp"
#include <jsonxx/jsonxx.h>
#include "../../AliveLibAO/Map.hpp"
#include "TypesCollectionAO.hpp"
#include "TlvObjectBase.hpp"
#include "JsonReaderBase.hpp"

namespace ReliveAPI {
JsonWriterAO::JsonWriterAO(std::unique_ptr<TypesCollectionAO>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(*typesCollection, pathId, pathBndName, info)
    , mTypesCollection{std::move(typesCollection)}
{
    mMapRootInfo.mGame = "AO";
}

JsonWriterAO::JsonWriterAO(s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterAO{std::make_unique<TypesCollectionAO>(), pathId, pathBndName, info}
{
}


[[nodiscard]] jsonxx::Array JsonWriterAO::ReadTlvStream(u8* ptr, Context& context)
{
    jsonxx::Array mapObjects;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(ptr);
    pPathTLV->RangeCheck();

    if (static_cast<s32>(pPathTLV->mTlvType32.mType) <= 0x100000 && pPathTLV->mLength <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            mTypeCounterMap[pPathTLV->mTlvType32.mType]++;

            auto obj = mTypesCollection->MakeTlvAO(pPathTLV->mTlvType32.mType, pPathTLV, mTypeCounterMap[pPathTLV->mTlvType32.mType]);
            if (obj)
            {
                if (pPathTLV->mLength != obj->TlvLen())
                {
                    LOG_ERROR(magic_enum::enum_name(pPathTLV->mTlvType32.mType) << " size should be " << pPathTLV->mLength << " but got " << obj->TlvLen());
                    throw ReliveAPI::WrongTLVLengthException();
                }
                mapObjects << obj->InstanceToJson(*mTypesCollection, context);
            }
            else
            {
                LOG_WARNING("Ignoring type: " << magic_enum::enum_name(pPathTLV->mTlvType32.mType));
            }

            pPathTLV = AO::Path_TLV::Next(pPathTLV);
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

[[nodiscard]] jsonxx::Array JsonWriterAO::ReadCollisionStream(u8* ptr, s32 numItems, Context& context)
{
    jsonxx::Array collisionsArray;
    AO::PathLine* pLineIter = reinterpret_cast<AO::PathLine*>(ptr);
    TypesCollectionAO types;

    for (s32 i = 0; i < numItems; i++)
    {
        AOLine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties, context);

        collisionsArray << properties;
    }
    return collisionsArray;
}

void JsonWriterAO::DebugDumpTlvs(IFileIO& fileIo, const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource)
{
    u8* pStart = pathResource.data() + info.mObjectOffset;
    u8* pEnd = pathResource.data() + info.mIndexTableOffset;

    AO::Path_TLV* pPathTLV = reinterpret_cast<AO::Path_TLV*>(pStart);
    pPathTLV->RangeCheck();
    s32 idx = 0;
    if (static_cast<s32>(pPathTLV->mTlvType32.mType) <= 0x100000 && pPathTLV->mLength <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (pPathTLV)
        {
            idx++;
            DebugDumpTlv(fileIo, prefix, idx, *pPathTLV);
            pPathTLV = AO::Path_TLV::Next_NoCheck(pPathTLV);
            if (pPathTLV)
            {
                pPathTLV->RangeCheck();
                if (pPathTLV->mLength == 0)
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
} // namespace ReliveAPI
