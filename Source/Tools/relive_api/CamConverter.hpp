#pragma once

class ChunkedLvlFile;

class CamConverterAO final
{
public:
    CamConverterAO(const std::string& fileName, const ChunkedLvlFile& camFile);
};

class CamConverterAE final
{
public:
    CamConverterAE(const std::string& fileName, const ChunkedLvlFile& camFile);
};
