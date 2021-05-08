#pragma once

#include "JsonReaderBase.hpp"

class JsonReaderAO final : public JsonReaderBase
{
public:
    std::pair<std::vector<CameraNameAndTlvBlob>, std::vector<AO::PathLine>> Load(const std::string& fileName);
};
