#pragma once

#include "ITypeBase.hpp"
#include "EnumType.hpp"
#include "BasicType.hpp"
#include "../AliveLibAO/PathData.hpp"
#include "../AliveLibAE/Path.hpp"
#include <magic_enum/include/magic_enum.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

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

    [[nodiscard]] jsonxx::Array EnumsToJson() const;
    [[nodiscard]] jsonxx::Array BasicTypesToJson() const;
    [[nodiscard]] std::string TypeName(std::type_index typeIndex) const;

    template<class T>
    [[nodiscard]] std::string TypeName() const
    {
        return TypeName(typeid(T));
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

        const auto ret = newEnum.get();
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
        if (!TypeName<T>().empty())
        {
            // Type already exists
            return nullptr;
        }

        auto newType = std::make_unique<BasicType<T>>(typeName, minVal, maxVal);

        const auto ret = newType.get();
        mTypes.push_back(std::move(newType));
        return ret;
    }

private:
    std::vector<std::unique_ptr<ITypeBase>> mTypes;
};
