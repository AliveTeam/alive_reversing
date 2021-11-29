#pragma once

#include <jsonxx/jsonxx.h>
#include "relive_api_exceptions.hpp"

namespace ReliveAPI {
[[nodiscard]] inline const jsonxx::Array& ReadArray(jsonxx::Object&&, const std::string&) = delete;
[[nodiscard]] inline const jsonxx::Array& ReadArray(const jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Array>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.get<jsonxx::Array>(key);
}

[[nodiscard]] inline const jsonxx::Object& ReadObject(jsonxx::Object&&, const std::string&) = delete;
[[nodiscard]] inline const jsonxx::Object& ReadObject(const jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Object>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.get<jsonxx::Object>(key);
}

[[nodiscard]] inline s32 ReadNumber(jsonxx::Object&&, const std::string&) = delete;
[[nodiscard]] inline s32 ReadNumber(const jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::Number>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return static_cast<s32>(o.get<jsonxx::Number>(key));
}

[[nodiscard]] inline const std::string& ReadString(jsonxx::Object&&, const std::string&) = delete;
[[nodiscard]] inline const std::string& ReadString(const jsonxx::Object& o, const std::string& key)
{
    if (!o.has<jsonxx::String>(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.get<jsonxx::String>(key);
}
} // namespace ReliveAPI
