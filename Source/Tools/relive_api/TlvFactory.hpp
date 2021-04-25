#pragma once

#include "../AliveLibCommon/Types.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace jsonxx { class Array; }

class TlvObjectBase;
class TypesCollectionBase;

template<class TlvType>
using FnTlvFactory = std::function<std::unique_ptr<TlvObjectBase>(TypesCollectionBase&, TlvType*, s32)>;

template<typename TlvEnumType, typename PathTlvType>
class TlvFactory
{
public:
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvByEnum(TypesCollectionBase& typesCollection, TlvEnumType tlvType, PathTlvType* pTlv, s32 instanceCount);
    [[nodiscard]] std::unique_ptr<TlvObjectBase> MakeTlvByName(TypesCollectionBase& typesCollection, const std::string& tlvTypeName, PathTlvType* pTlv);

    void AddTlvsToJsonArray(TypesCollectionBase& typesCollection, jsonxx::Array& array);

    template<typename TlvWrapperType>
    void DoRegisterType(TypesCollectionBase& constructingTypes)
    {
        TlvWrapperType tmp;
        tmp.AddTypes(constructingTypes);

        const TlvEnumType tlvType = tmp.TlvType();

        auto fnCreate = [](TypesCollectionBase& types, PathTlvType* pTlv, s32 instanceCount)
        {
            auto ret = std::make_unique<TlvWrapperType>(types, pTlv);
            ret->SetInstanceNumber(instanceCount);
            return ret;
        };

        mReverseTlvFactory[tmp.Name()] = fnCreate;
        mTlvFactory[tlvType] = fnCreate;
    }

    std::map<TlvEnumType, FnTlvFactory<PathTlvType>> mTlvFactory;
    std::map<std::string, FnTlvFactory<PathTlvType>> mReverseTlvFactory;
};
