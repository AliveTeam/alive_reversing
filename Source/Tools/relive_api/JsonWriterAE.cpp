#include "JsonWriterAE.hpp"
#include "../../AliveLibAE/Path.hpp"
#include <jsonxx/jsonxx.h>
#include "TypesCollectionAE.hpp"
#include "JsonReaderBase.hpp"
#include "TlvObjectBase.hpp"
#include <magic_enum/include/magic_enum.hpp>

namespace ReliveAPI {
void JsonWriterAE::DebugDumpTlvs(IFileIO& fileIo, const std::string& prefix, const PathInfo& info, std::vector<u8>& pathResource)
{
    u8* pData = pathResource.data();
    u8* pStart = pData + info.mObjectOffset;
    u8* pEnd = pData + info.mIndexTableOffset;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(pStart);
    s32 idx = 0;
    while (pPathTLV && reinterpret_cast<u8*>(pPathTLV) < pEnd)
    {
        idx++;
        DebugDumpTlv(fileIo, prefix, idx, *pPathTLV);

        // Skip length bytes to get to the start of the next TLV
        u8* ptr = reinterpret_cast<u8*>(pPathTLV);
        u8* pNext = ptr + pPathTLV->mLength;
        pPathTLV = reinterpret_cast<Path_TLV*>(pNext);
    }
}

JsonWriterAE::JsonWriterAE(std::unique_ptr<TypesCollectionAE>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(*typesCollection, pathId, pathBndName, info)
    , mTypesCollection{std::move(typesCollection)}
{
    mMapRootInfo.mGame = "AE";
}

JsonWriterAE::JsonWriterAE(s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterAE{std::make_unique<TypesCollectionAE>(), pathId, pathBndName, info}
{
}

JsonWriterAE::~JsonWriterAE() = default;

void JsonWriterAE::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

[[nodiscard]] jsonxx::Array JsonWriterAE::ReadCollisionStream(u8* ptr, s32 numItems, Context& context)
{
    jsonxx::Array collisionsArray;
    PathLineAE* pLineIter = reinterpret_cast<PathLineAE*>(ptr);
    TypesCollectionAE types;

    for (s32 i = 0; i < numItems; i++)
    {
        AELine tmpLine(types, &pLineIter[i]);

        jsonxx::Object properties;
        tmpLine.PropertiesToJson(types, properties, context);

        collisionsArray << properties;
    }
    return collisionsArray;
}

// TODO: Not sure if we have a func to iterate OG TLVs somewhere, this is a local copy for now
inline Path_TLV* Next_TLV_Impl(Path_TLV* pTlv)
{
    if (pTlv->mTlvFlags.Get(TlvFlags::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    // Skip length bytes to get to the start of the next TLV
    u8* ptr = reinterpret_cast<u8*>(pTlv);
    u8* pNext = ptr + pTlv->mLength;
    return reinterpret_cast<Path_TLV*>(pNext);
}

[[nodiscard]] jsonxx::Array JsonWriterAE::ReadTlvStream(u8* ptr, Context& context)
{
    jsonxx::Array mapObjects;

    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        mTypeCounterMap[pPathTLV->mTlvType32.mType]++;
        auto obj = mTypesCollection->MakeTlvAE(pPathTLV->mTlvType32.mType, pPathTLV, mTypeCounterMap[pPathTLV->mTlvType32.mType]);
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
            LOG_WARNING("Ignoring type: " << pPathTLV->mTlvType32.mType);
        }

        pPathTLV = Next_TLV_Impl(pPathTLV); // TODO: Will skip the last entry ??
    }

    return mapObjects;
}

[[nodiscard]] jsonxx::Array JsonWriterAE::AddCollisionLineStructureJson()
{
    AELine tmpLine(*mTypesCollection);
    return tmpLine.PropertiesToJson();
}
} // namespace ReliveAPI
