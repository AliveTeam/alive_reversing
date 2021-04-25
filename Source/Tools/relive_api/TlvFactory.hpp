#pragma once

template<class TlvType>
using FnTlvFactory = std::function<std::unique_ptr<TlvObjectBase>(class TypesCollectionBase&, TlvType*, s32)>;

template<typename TlvEnumType, typename PathTlvType>
class TlvFactory
{
public:
    std::unique_ptr<TlvObjectBase> MakeTlvByEnum(TypesCollectionBase& typesCollection, TlvEnumType tlvType, PathTlvType* pTlv, s32 instanceCount)
    {
        auto it = mTlvFactory.find(tlvType);
        if (it == std::end(mTlvFactory))
        {
            LOG_WARNING("Type " << magic_enum::enum_name(tlvType) << " unknown");
            return nullptr;
        }
        return it->second(typesCollection, pTlv, instanceCount);
    }

    std::unique_ptr<TlvObjectBase> MakeTlvByName(TypesCollectionBase& typesCollection, const std::string& tlvTypeName, PathTlvType* pTlv)
    {
        auto it = mReverseTlvFactory.find(tlvTypeName);
        if (it == std::end(mReverseTlvFactory))
        {
            LOG_WARNING("Type " << tlvTypeName << " unknown");
            return nullptr;
        }
        return it->second(typesCollection, pTlv, 0);
    }

    void AddTlvsToJsonArray(TypesCollectionBase& typesCollection, jsonxx::Array& array)
    {
        for (auto&[key, value] : mTlvFactory)
        {
            array << value(typesCollection, nullptr, 0)->StructureToJson();
        }
    }

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
