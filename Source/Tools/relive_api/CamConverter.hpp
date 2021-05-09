#pragma once

class ChunkedLvlFile;

class CamConverterAO final
{
public:
    CamConverterAO(const ChunkedLvlFile& camFile);

    std::vector<u8> ToBase64Png() const;
};

class CamConverterAE final
{
public:
    CamConverterAE(const ChunkedLvlFile& camFile);

    std::vector<u8> ToBase64Png() const;
};
