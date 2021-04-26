#pragma once

#include "ITypeBase.hpp"
#include "EnumType.hpp"
#include "BasicType.hpp"

#include "../AliveLibAO/PathData.hpp"

#include "../AliveLibAE/Path.hpp"

#include <magic_enum/include/magic_enum.hpp>

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
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

    TypesCollectionBase(const TypesCollectionBase&) = delete;
    TypesCollectionBase(TypesCollectionBase&&) = delete;

    virtual void AddTlvsToJsonArray(jsonxx::Array& array) = 0;
    virtual std::unique_ptr<TlvObjectBase> MakeTlvFromString(const std::string& tlvTypeName) = 0;

    [[nodiscard]] jsonxx::Array EnumsToJson() const;
    [[nodiscard]] jsonxx::Array BasicTypesToJson() const;
    [[nodiscard]] const std::string& TypeName(const std::type_index& typeIndex) const;

    template<class T>
    [[nodiscard]] const std::string& TypeName() const
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

        // Using `std::make_unique` here unfortunately significantly increases compilation time on MinGW + GCC.
        auto* newEnum = new EnumType<T>(enumName);
        for (const auto& enumItem : enumItems)
        {
            newEnum->Add(enumItem.mEnumValue, enumItem.mName);
        }

        return static_cast<EnumType<T>*>(RegisterType(newEnum));
    }

    template<class T>
    [[nodiscard]] T EnumValueFromString(const std::string& enumTypeName, const std::string& enumValueString)
    {
        const ITypeBase* ptr = FindByTypeName(enumTypeName);

        return ptr != nullptr
            ? static_cast<const EnumType<T>*>(ptr)->ValueFromString(enumValueString)
            : throw ReliveAPI::UnknownEnumValueException(enumValueString.c_str());
    }

    template<class T>
    [[nodiscard]] const std::string& EnumValueToString(T enumValue)
    {
        const ITypeBase* ptr = FindByTypeIndex(typeid(T));

        return ptr != nullptr
            ? static_cast<const EnumType<T>*>(ptr)->ValueToString(enumValue)
            : throw ReliveAPI::UnknownEnumValueException();
    }

    template<class T>
    BasicType<T>* AddBasicType(const std::string& typeName, s32 minVal, s32 maxVal)
    {
        if (!TypeName<T>().empty())
        {
            // Type already exists
            return nullptr;
        }

        // Using `std::make_unique` here unfortunately significantly increases compilation time on MinGW + GCC.
        return static_cast<BasicType<T>*>(RegisterType(new BasicType<T>(typeName, minVal, maxVal)));
    }

private:
    std::vector<std::unique_ptr<ITypeBase>> mTypes;
    std::unordered_map<std::string, ITypeBase*> mTypesByName;
    std::unordered_map<std::type_index, ITypeBase*> mTypesByTypeIndex;
    const std::string mEmptyStr;

    [[nodiscard]] ITypeBase* RegisterType(ITypeBase* typeBase);
    [[nodiscard]] const ITypeBase* FindByTypeName(const std::string& typeName) const;
    [[nodiscard]] const ITypeBase* FindByTypeIndex(const std::type_index& typeIndex) const;
};
