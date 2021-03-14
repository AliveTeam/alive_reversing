#pragma once

#include "ITypeBase.hpp"
#include "EnumType.hpp"
#include "BasicType.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAE/Path.hpp"

class TlvObjectBase;

enum class Game
{
    AO,
    AE
};

class TypesCollection
{
public:
    explicit TypesCollection(Game gameType);

    void AddAOTypes();
    void AddAETypes();

    void AddTlvsToJsonArray(jsonxx::Array& array);

    std::unique_ptr<TlvObjectBase> MakeTlvAE(TlvTypes tlvType, Path_TLV* pTlv);

    std::unique_ptr<TlvObjectBase> MakeTlvAO(AO::TlvTypes tlvType, AO::Path_TLV* pTlv);
    std::unique_ptr<TlvObjectBase> MakeTlvAO(const std::string& tlvTypeName, AO::Path_TLV* pTlv);

    jsonxx::Object EnumsToJson() const
    {
        jsonxx::Object ret;
        for (const auto& basicType : mTypes)
        {
            if (!basicType->IsBasicType())
            {
                basicType->ToJson(ret);
            }
        }
        jsonxx::Object t;
        t << "object_structure_property_enums" << ret;
        return t;
    }

    jsonxx::Object BasicTypesToJson() const
    {
        jsonxx::Object ret;
        for (const auto& basicType : mTypes)
        {
            if (basicType->IsBasicType())
            {
                basicType->ToJson(ret);
            }
        }
        jsonxx::Object t;
        t << "object_structure_property_basic_types" << ret;
        return t;
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
            return ret;
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
    BasicType<T>* AddBasicType(const std::string& typeName, int minVal, int maxVal)
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
    std::map<AO::TlvTypes, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, AO::Path_TLV*)>> mTlvFactoryAO;
    std::map<std::string, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, AO::Path_TLV*)>> mReverseTlvFactoryAO;

    std::map<TlvTypes, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, Path_TLV*)>> mTlvFactoryAE;
    std::map<std::string, std::function<std::unique_ptr<TlvObjectBase>(TypesCollection&, Path_TLV*)>> mReverseTlvFactoryAE;

    Game mGameType = {};
};