#pragma once

#include <nlohmann/json.hpp>
#include "relive_api_exceptions.hpp"

namespace ReliveAPI {
[[nodiscard]] inline const nlohmann::json& ReadArray(nlohmann::json&&, const std::string&) = delete;
[[nodiscard]] inline const nlohmann::json& ReadArray(const nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.at(key);
}

[[nodiscard]] inline const nlohmann::json& ReadObject(nlohmann::json&&, const std::string&) = delete;
[[nodiscard]] inline const nlohmann::json& ReadObject(const nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.at(key);
}

[[nodiscard]] inline nlohmann::json& ReadObject(nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.at(key);
}
[[nodiscard]] inline s32 ReadNumber(nlohmann::json&&, const std::string&) = delete;
[[nodiscard]] inline s32 ReadNumber(const nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.at(key).get<s32>();
}

[[nodiscard]] inline const std::string ReadString(nlohmann::json&&, const std::string&) = delete;
[[nodiscard]] inline const std::string ReadString(const nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key))
    {
        throw ReliveAPI::JsonKeyNotFoundException(key);
    }

    return o.at(key);
}

[[nodiscard]] inline std::string ReadOptionalString(const nlohmann::json& o, const std::string& key)
{
    if (!o.contains(key))
    {
        return "";
    }

    return o.at(key);
}

} // namespace ReliveAPI
