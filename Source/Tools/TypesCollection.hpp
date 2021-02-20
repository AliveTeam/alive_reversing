#pragma once

#include "ITypeBase.hpp"
#include "EnumType.hpp"
#include "BasicType.hpp"

class TypesCollection
{
public:
    TypesCollection()
    {
        AddBasicType<BYTE>("Byte", 0, 255);
        AddBasicType<short>("UInt16", 0, 65535);

        AddEnum<::Path_Hoist::Type>("Enum_HoistType",
            {
                {::Path_Hoist::Type::eNextEdge, "next_edge"},
                {::Path_Hoist::Type::eNextFloor, "next_floor"},
                {::Path_Hoist::Type::eOffScreen, "off_screen"},
            });

        AddEnum<::Path_Hoist::EdgeType>("Enum_HoistEdgeType",
            {
                {::Path_Hoist::EdgeType::eBoth, "both"},
                {::Path_Hoist::EdgeType::eLeft, "left"},
                {::Path_Hoist::EdgeType::eRight, "right"},
            });

        AddEnum<::Path_Hoist::Scale>("Enum_HoistScale",
            {
                {::Path_Hoist::Scale::eFull, "full"},
                {::Path_Hoist::Scale::eHalf, "half"}
            });
    }

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
    auto AddEnum(const std::string& enumName, const std::vector<EnumPair<T>>& enumItems)
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
    auto AddBasicType(const std::string& typeName, int minVal, int maxVal)
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
