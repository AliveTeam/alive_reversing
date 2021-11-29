#pragma once

#include "../../AliveLibCommon/Types.hpp"

#include <map>
#include <unordered_map>
#include <memory>
#include <string>

namespace jsonxx {
class Array;
}

class TlvObjectBase;
class TypesCollectionBase;

template <class TlvType>
using FnTlvFactory = std::unique_ptr<TlvObjectBase> (*)(TypesCollectionBase&, TlvType*, s32);

template <typename TlvEnumType, typename PathTlvType>
class TlvFactory final
{
private:
    template <typename TlvWrapperType>
    [[nodiscard]] static std::unique_ptr<TlvObjectBase> fnCreate(TypesCollectionBase& types, PathTlvType* pTlv, s32 instanceCount)
    {
        // Using `std::make_unique` here unfortunately significantly increases compilation time on MinGW + GCC.
        auto* ret = new TlvWrapperType(types, pTlv);
        ret->SetInstanceNumber(instanceCount);
        return std::unique_ptr<TlvObjectBase>{static_cast<TlvObjectBase*>(ret)};
    }

public:
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvByEnum(TypesCollectionBase& typesCollection, TlvEnumType tlvType, PathTlvType* pTlv, s32 instanceCount) const;
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvByName(TypesCollectionBase& typesCollection, const std::string& tlvTypeName, PathTlvType* pTlv) const;

    void AddTlvsToJsonArray(TypesCollectionBase& typesCollection, jsonxx::Array& array);

    template <typename TlvWrapperType>
    void DoRegisterType(TypesCollectionBase& constructingTypes)
    {
        TlvWrapperType tmp;
        tmp.AddTypes(constructingTypes);

        const TlvEnumType tlvType = tmp.TlvType();

        mReverseTlvFactory[tmp.Name()] = mTlvFactory[tlvType] = &fnCreate<TlvWrapperType>;
    }

    std::unordered_map<TlvEnumType, FnTlvFactory<PathTlvType>> mTlvFactory;
    std::unordered_map<std::string, FnTlvFactory<PathTlvType>> mReverseTlvFactory;
};
