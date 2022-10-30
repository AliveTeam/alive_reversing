#include "TlvFactory.hpp"

#include "TlvObjectBase.hpp"

#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/Map.hpp"

#include "../../AliveLibAE/Path.hpp"
#include "../../AliveLibAO/Path.hpp"
#include "../../relive_lib/data_conversion/PathTlvsAE.hpp"

#include <jsonxx/jsonxx.h>
#include <magic_enum/include/magic_enum.hpp>

namespace ReliveAPI {
template <typename TlvEnumType, typename PathTlvType>
[[nodiscard]] std::unique_ptr<TlvObjectBase> TlvFactory<TlvEnumType, PathTlvType>::MakeTlvByEnum(TypesCollectionBase& typesCollection, TlvEnumType tlvType, PathTlvType* pTlv, s32 instanceCount) const
{
    const auto it = mTlvFactory.find(tlvType);

    if (it == std::end(mTlvFactory))
    {
        LOG_WARNING("Type %s unknown", magic_enum::enum_name(tlvType));
        return nullptr;
    }

    return it->second(typesCollection, pTlv, instanceCount);
}

template <typename TlvEnumType, typename PathTlvType>
[[nodiscard]] std::unique_ptr<TlvObjectBase> TlvFactory<TlvEnumType, PathTlvType>::MakeTlvByName(TypesCollectionBase& typesCollection, const std::string& tlvTypeName, PathTlvType* pTlv) const
{
    const auto it = mReverseTlvFactory.find(tlvTypeName);

    if (it == std::end(mReverseTlvFactory))
    {
        LOG_WARNING("Type %s unknown", tlvTypeName.c_str());
        return nullptr;
    }

    return it->second(typesCollection, pTlv, 0);
}

template <typename TlvEnumType, typename PathTlvType>
void TlvFactory<TlvEnumType, PathTlvType>::AddTlvsToJsonArray(TypesCollectionBase& typesCollection, jsonxx::Array& array)
{
    for (auto& [key, value] : mTlvFactory)
    {
        array << value(typesCollection, nullptr, 0)->StructureToJson();
    }
}

template class TlvFactory<TlvTypes, Path_TLV>;
template class TlvFactory<AO::TlvTypes, AO::Path_TLV>;
} // namespace ReliveAPI
