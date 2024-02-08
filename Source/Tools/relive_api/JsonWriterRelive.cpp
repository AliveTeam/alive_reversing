#include "JsonWriterRelive.hpp"
#include "../../AliveLibAE/Path.hpp"
#include <nlohmann/json.hpp>
#include "TypesCollectionRelive.hpp"
#include "JsonReaderBase.hpp"
#include "TlvObjectBase.hpp"

namespace ReliveAPI {

JsonWriterRelive::JsonWriterRelive(Game gameType, std::unique_ptr<TypesCollectionRelive>&& typesCollection, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterBase(*typesCollection, pathId, pathBndName, info)
    , mTypesCollection{std::move(typesCollection)}
{
    mMapRootInfo.mGame = (gameType == Game::AE) ? "AE" : "AO";
}

JsonWriterRelive::JsonWriterRelive(Game gameType, s32 pathId, const std::string& pathBndName, const PathInfo& info)
    : JsonWriterRelive{gameType, std::make_unique<TypesCollectionRelive>(), pathId, pathBndName, info}
{
}

JsonWriterRelive::~JsonWriterRelive() = default;

void JsonWriterRelive::ResetTypeCounterMap()
{
    mTypeCounterMap.clear();
}

[[nodiscard]] nlohmann::json JsonWriterRelive::ReadCollisionStream(u8* ptr, s32 numItems, Context& context)
{
    nlohmann::json collisionsArray = nlohmann::json::array();
    PathLine* pLineIter = reinterpret_cast<PathLine*>(ptr);
    TypesCollectionRelive types;

    for (s32 i = 0; i < numItems; i++)
    {
        ReliveLine tmpLine(types, &pLineIter[i]);

        nlohmann::json properties = nlohmann::json::object();
        tmpLine.PropertiesToJson(types, properties, context);

        collisionsArray.push_back(properties);
    }
    return collisionsArray;
}

// TODO: Not sure if we have a func to iterate OG TLVs somewhere, this is a local copy for now
inline relive::Path_TLV* Next_TLV_Impl(relive::Path_TLV* pTlv)
{
    if (pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    // Skip length bytes to get to the start of the next TLV
    u8* ptr = reinterpret_cast<u8*>(pTlv);
    u8* pNext = ptr + pTlv->mLength;
    return reinterpret_cast<relive::Path_TLV*>(pNext);
}

[[nodiscard]] nlohmann::json JsonWriterRelive::ReadTlvStream(u8* ptr, Context& context)
{
    nlohmann::json mapObjects = nlohmann::json::array();

    relive::Path_TLV* pPathTLV = reinterpret_cast<relive::Path_TLV*>(ptr);
    while (pPathTLV)
    {
        mTypeCounterMap[pPathTLV->mTlvType]++;
        auto obj = mTypesCollection->MakeTlvRelive(pPathTLV->mTlvType, pPathTLV, mTypeCounterMap[pPathTLV->mTlvType]);
        if (obj)
        {
            if (pPathTLV->mLength != obj->TlvLen())
            {
                LOG_ERROR("path tlv size of type %d should be %d but got %d", static_cast<s32>(pPathTLV->mTlvType), pPathTLV->mLength, obj->TlvLen());
                throw ReliveAPI::WrongTLVLengthException();
            }

            mapObjects.push_back(obj->InstanceToJson(*mTypesCollection, context));
        }
        else
        {
            LOG_WARNING("Ignoring type: %d", static_cast<u32>(pPathTLV->mTlvType));
        }

        pPathTLV = Next_TLV_Impl(pPathTLV);
    }

    return mapObjects;
}

[[nodiscard]] nlohmann::json JsonWriterRelive::AddCollisionLineStructureJson()
{
    ReliveLine tmpLine(*mTypesCollection);
    return tmpLine.PropertiesToJson();
}
} // namespace ReliveAPI
