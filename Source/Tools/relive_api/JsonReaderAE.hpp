#pragma once

#include "JsonReaderBase.hpp"

namespace ReliveAPI {
class Context;
class JsonReaderAE final : public JsonReaderBase
{
public:
    std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<::PathLine>> Load(IFileIO& fileIO, const std::string& fileName, Context& context);
};
} // namespace ReliveAPI
