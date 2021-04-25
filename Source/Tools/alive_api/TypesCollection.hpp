#pragma once

#include "ITypeBase.hpp"
#include "EnumType.hpp"
#include "BasicType.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAE/Path.hpp"
#include <magic_enum/include/magic_enum.hpp>

class TlvObjectBase;

enum class Game
{
    AO,
    AE
};

template<class TlvType>
using FnTlvFactory = std::function<std::unique_ptr<TlvObjectBase>(class TypesCollection&, TlvType*, s32)>;

template<typename TlvEnumType, typename PathTlvType>
class TlvFactory
{
public:
    std::unique_ptr<TlvObjectBase> MakeTlvByEnum(TypesCollection& typesCollection, TlvEnumType tlvType, PathTlvType* pTlv, s32 instanceCount)
    {
        auto it = mTlvFactory.find(tlvType);
        if (it == std::end(mTlvFactory))
        {
            LOG_WARNING("Type " << magic_enum::enum_name(tlvType) << " unknown");
            return nullptr;
        }
        return it->second(typesCollection, pTlv, instanceCount);
    }

    std::unique_ptr<TlvObjectBase> MakeTlvByName(TypesCollection& typesCollection, const std::string& tlvTypeName, PathTlvType* pTlv)
    {
        auto it = mReverseTlvFactory.find(tlvTypeName);
        if (it == std::end(mReverseTlvFactory))
        {
            LOG_WARNING("Type " << tlvTypeName << " unknown");
            return nullptr;
        }
        return it->second(typesCollection, pTlv, 0);
    }

    void AddTlvsToJsonArray(TypesCollection& typesCollection, jsonxx::Array& array)
    {
        for (auto& [key, value] : mTlvFactory)
        {
            array << value(typesCollection, nullptr, 0)->StructureToJson();
        }
    }

    template<typename TlvWrapperType>
    void DoRegisterType(TypesCollection& constructingTypes)
    {
        TlvWrapperType tmp;
        tmp.AddTypes(constructingTypes);
        const TlvEnumType tlvType = tmp.TlvType();
        auto fnCreate = [](TypesCollection& types, PathTlvType* pTlv, s32 instanceCount)
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

class TypesCollection
{
public:
    explicit TypesCollection(Game gameType);

    void AddAOTypes();
    void AddAETypes();

    void AddTlvsToJsonArray(jsonxx::Array& array);

    std::unique_ptr<TlvObjectBase> MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv, s32 instanceCount);
    std::unique_ptr<TlvObjectBase> MakeTlvAE(const std::string& tlvTypeName, Path_TLV* pTlv);

    std::unique_ptr<TlvObjectBase> MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv, s32 instanceCount);
    std::unique_ptr<TlvObjectBase> MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv);

    jsonxx::Array EnumsToJson() const
    {
        jsonxx::Array ret;
        for (const auto& basicType : mTypes)
        {
            if (!basicType->IsBasicType())
            {
                basicType->ToJson(ret);
            }
        }
        return ret;
    }

    jsonxx::Array BasicTypesToJson() const
    {
        jsonxx::Array ret;
        for (const auto& basicType : mTypes)
        {
            if (basicType->IsBasicType())
            {
                basicType->ToJson(ret);
            }
        }
        return ret;
    }

    std::string TypeName(std::type_index typeIndex) const
    {
        for (const auto& e : mTypes)
        {
            if (e->TypeIndex() == typeIndex)
            {
                return e->Name();
            }
        }
        return "";
    }

    template<class T>
    std::string TypeName() const
    {
        for (const auto& e : mTypes)
        {
            if (e->TypeIndex() == typeid(T))
            {
                return e->Name();
            }
        }
        return "";
    }

    template<class T>
    struct EnumPair
    {
        T mEnumValue;
        std::string mName;
    };

    template<class T>
    EnumType<T>* AddEnum(const std::string& enumName, const std::vector<EnumPair<T>>& enumItems)
    {
        EnumType<T>* ret = nullptr;
        if (!TypeName<T>().empty())
        {
            // Type already exists
            LOG_ERROR(enumName << " exists more than once ");
            abort();
            //return ret;
        }

        auto newEnum = std::make_unique<EnumType<T>>(enumName);
        for (const auto& enumItem : enumItems)
        {
            newEnum->Add(enumItem.mEnumValue, enumItem.mName);
        }
        ret = newEnum.get();
        mTypes.push_back(std::move(newEnum));
        return ret;
    }

    template<class T>
    T EnumValueFromString(const std::string& enumTypeName, const std::string& enumValueString)
    {
        for (const auto& e : mTypes)
        {
            if (e->Name() == enumTypeName)
            {
                return static_cast<EnumType<T>*>(e.get())->ValueFromString(enumValueString);
            }
        }
        abort();
    }

    template<class T>
    std::string EnumValueToString(T enumValue)
    {
        for (const auto& e : mTypes)
        {
            if (e->TypeIndex() == typeid(T))
            {
                return static_cast<EnumType<T>*>(e.get())->ValueToString(enumValue);
            }
        }
        abort();
    }

    template<class T>
    BasicType<T>* AddBasicType(const std::string& typeName, s32 minVal, s32 maxVal)
    {
        BasicType<T>* ret = nullptr;
        if (!TypeName<T>().empty())
        {
            // Type already exists
            return ret;
        }

        auto newType = std::make_unique<BasicType<T>>(typeName, minVal, maxVal);
        ret = newType.get();
        mTypes.push_back(std::move(newType));
        return ret;
    }


private:
    std::vector<std::unique_ptr<ITypeBase>> mTypes;

    TlvFactory<AO::TlvTypes, AO::Path_TLV> mTlvFactoryAO;
    TlvFactory<TlvTypes, Path_TLV> mTlvFactoryAE;

    Game mGameType = {};
};
