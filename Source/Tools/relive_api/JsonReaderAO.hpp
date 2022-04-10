#pragma once

#include "JsonReaderBase.hpp"

namespace ReliveAPI {
class Context;
class JsonReaderAO final : public JsonReaderBase
{
public:
    std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> Load(IFileIO& fileIO, const std::string& fileName, Context& context);
};
} // namespace ReliveAPI
