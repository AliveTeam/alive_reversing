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

class TypesCollectionBase
{
public:
    TypesCollectionBase();
    virtual ~TypesCollectionBase();

    virtual void AddTlvsToJsonArray(jsonxx::Array& array) = 0;
    virtual std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) = 0;

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
            throw ReliveAPI::DuplicateEnumNameException(enumName.c_str());
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
        throw ReliveAPI::UnknownEnumValueException(enumValueString.c_str());
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
        throw ReliveAPI::UnknownEnumValueException();
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
};
