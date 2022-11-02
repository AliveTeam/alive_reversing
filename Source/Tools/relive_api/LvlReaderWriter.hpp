#pragma once

#include "ByteStream.hpp"

#include "../../AliveLibAE/LvlArchive.hpp"

#include "../../AliveLibCommon/FunctionFwd.hpp"
#include "relive_api_exceptions.hpp"
#include "file_api.hpp"
#include "RoundUp.hpp"

#include <cstddef>
#include <cstring>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace ReliveAPI {
[[nodiscard]] inline std::string ToString(const LvlFileRecord& rec)
{
    size_t i = 0;
    for (i = 0; i < ALIVE_COUNTOF(LvlFileRecord::field_0_file_name); ++i)
    {
        if (!rec.field_0_file_name[i])
        {
            break;
        }
    }
    return std::string(rec.field_0_file_name, i);
}

class LvlFileChunk final
{
public:
    LvlFileChunk(u32 id, ResourceManager::ResourceType resType, std::vector<u8>&& data)
        : mData(std::move(data))
    {
        mHeader.field_0_size = static_cast<u32>(mData.size());
        mHeader.field_C_id = id;
        mHeader.field_8_type = resType;
    }

    [[nodiscard]] u32 Id() const
    {
        return mHeader.field_C_id;
    }

    [[nodiscard]] u32 Size() const
    {
        return mHeader.field_0_size;
    }

    [[nodiscard]] const ResourceManager::Header& Header() const
    {
        return mHeader;
    }

    [[nodiscard]] const std::vector<u8>& Data() const
    {
        return mData;
    }

    [[nodiscard]] std::vector<u8>& Data()
    {
        return mData;
    }

private:
    ResourceManager::Header mHeader = {};
    std::vector<u8> mData;
};

class ChunkedLvlFile final
{
public:
    ChunkedLvlFile()
    {

    }

    explicit ChunkedLvlFile(const std::vector<u8>& data)
    {
        Read(data);
    }

    // TODO: return ptr?
    [[nodiscard]] std::optional<LvlFileChunk> ChunkById(u32 id) const
    {
        for (auto& chunk : mChunks)
        {
            if (chunk.Id() == id)
            {
                return {chunk};
            }
        }
        return {};
    }

    [[nodiscard]] std::optional<LvlFileChunk> ChunkByType(u32 type) const
    {
        for (auto& chunk : mChunks)
        {
            if (chunk.Header().field_8_type == type)
            {
                return {chunk};
            }
        }
        return {};
    }

    void RemoveChunksOfType(u32 type)
    {
        auto it = mChunks.begin();
        while (it != mChunks.end())
        {
            if (it->Header().field_8_type == type)
            {
                it = mChunks.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    void AddChunk(const LvlFileChunk& chunkToAdd)
    {
        LvlFileChunk copy = chunkToAdd;
        AddChunk(std::move(copy));
    }

    void AddChunk(LvlFileChunk&& chunkToAdd)
    {
        for (auto& chunk : mChunks)
        {
            if (chunk.Id() == chunkToAdd.Id())
            {
                chunk = std::move(chunkToAdd);
                return;
            }
        }

        bool hasEndChunkAtEnd = false;
        if (!mChunks.empty())
        {
            if (mChunks[mChunks.size() - 1].Header().field_8_type == ResourceManager::Resource_End)
            {
                hasEndChunkAtEnd = true;
            }
        }

        if (hasEndChunkAtEnd)
        {
            mChunks.insert(mChunks.end() - 1, std::move(chunkToAdd));
        }
        else
        {
            mChunks.push_back(std::move(chunkToAdd));
        }
    }

    [[nodiscard]] std::vector<u8> Data() const
    {
        std::size_t neededSize = 0;
        for (auto& chunk : mChunks)
        {
            neededSize += chunk.Size();
        }
        neededSize += sizeof(ResourceManager::Header) * mChunks.size();

        ByteStream s;
        s.ReserveSize(neededSize);

        for (const auto& chunk : mChunks)
        {
            auto adjustedHeader = chunk.Header();
            if (adjustedHeader.field_0_size > 0)
            {
                adjustedHeader.field_0_size += sizeof(ResourceManager::Header);
            }

            s.Write(adjustedHeader.field_0_size);
            s.Write(adjustedHeader.field_4_ref_count);
            s.Write(adjustedHeader.field_6_flags);
            s.Write(adjustedHeader.field_8_type);
            s.Write(adjustedHeader.field_C_id);

            if (chunk.Size() > 0)
            {
                s.Write(chunk.Data());
            }
        }

        return std::move(s).GetBuffer();
    }

    const u32 ChunkCount() const
    {
        return static_cast<u32>(mChunks.size());
    }

    const LvlFileChunk& ChunkAt(u32 idx) const
    {
        return mChunks[idx];
    }

private:
    void Read(const std::vector<u8>& data)
    {
        ByteStream s(data);

        do
        {
            ResourceManager::Header resHeader = {};
            s.Read(resHeader.field_0_size);
            s.Read(resHeader.field_4_ref_count);
            s.Read(resHeader.field_6_flags);
            s.Read(resHeader.field_8_type);
            s.Read(resHeader.field_C_id);

            std::vector<u8> tmpData(resHeader.field_0_size);
            if (resHeader.field_0_size > 0)
            {
                tmpData.resize(tmpData.size() - sizeof(ResourceManager::Header));
                s.Read(tmpData);
            }

            mChunks.emplace_back(resHeader.field_C_id, static_cast<ResourceManager::ResourceType>(resHeader.field_8_type), std::move(tmpData));

            if (resHeader.field_8_type == ResourceManager::ResourceType::Resource_End)
            {
                break;
            }
        }
        while (!s.AtReadEnd());
    }

    std::vector<LvlFileChunk> mChunks;
};

class IFileIO;

class LvlReader final
{
public:
    explicit LvlReader(IFileIO& fileIO, const char_type* lvlFile)
    {
        mFileHandle = fileIO.Open(lvlFile, IFileIO::Mode::ReadBinary);
        if (!mFileHandle)
        {
            throw ReliveAPI::IOReadException(lvlFile);
        }

        if (!ReadTOC())
        {
            Close();
        }
    }

    [[nodiscard]] bool IsOpen() const
    {
        return mFileHandle != nullptr;
    }

    void Close()
    {
        if (IsOpen())
        {
            mFileHandle = nullptr;
        }
    }

    ~LvlReader()
    {
        Close();
    }

    [[nodiscard]] bool ReadFileInto(std::vector<u8>& target, const char_type* fileName)
    {
        if (!IsOpen())
        {
            return false;
        }

        for (const auto& rec : mFileRecords)
        {
            if (std::strncmp(rec.field_0_file_name, fileName, ALIVE_COUNTOF(LvlFileRecord::field_0_file_name)) == 0)
            {
                const auto fileOffset = rec.field_C_start_sector * 2048;

                if (!mFileHandle->Seek(fileOffset))
                {
                    return false;
                }

                target.resize(rec.field_14_file_size);
                
                if (!mFileHandle->Read(target))
                {
                    return false;
                }

                return true;
            }
        }

        return false;
    }

    [[nodiscard]] std::optional<std::vector<u8>> ReadFile(const char_type* fileName)
    {
        std::optional<std::vector<u8>> result;
        result.emplace();

        if (!ReadFileInto(*result, fileName))
        {
            return {};
        }

        return result;
    }

    [[nodiscard]] s32 FileCount() const
    {
        return mHeader.field_10_sub.field_0_num_files;
    }

    [[nodiscard]] std::string FileNameAt(s32 idx) const
    {
        return ToString(mFileRecords[idx]);
    }

    [[nodiscard]] s32 FileSizeAt(s32 idx) const
    {
        return mFileRecords[idx].field_14_file_size;
    }

protected:
    [[nodiscard]] bool ReadTOC()
    {
        if (!mFileHandle->Read(reinterpret_cast<u8*>(&mHeader), sizeof(LvlHeader) - sizeof(LvlFileRecord)))
        {
            return false;
        }

        // TODO: Should probably validate the header, but the real game does not give a toss
        // so probably is not that important.

        mFileRecords.resize(mHeader.field_10_sub.field_0_num_files);

        if (!mFileHandle->Read(mFileRecords))
        {
            return false;
        }

        return true;
    }

    std::unique_ptr<IFile> mFileHandle;
    LvlHeader mHeader = {};
    std::vector<LvlFileRecord> mFileRecords;
};

class LvlWriter final
{
public:
    LvlWriter(IFileIO& fileIO, const char_type* lvlFile)
        : mReader(fileIO, lvlFile)
    {
    }

    void Close()
    {
        mReader.Close();
    }

    [[nodiscard]] bool IsOpen() const
    {
        return mReader.IsOpen();
    }

    [[nodiscard]] bool ReadFileInto(std::vector<u8>& target, const char_type* fileName)
    {
        // Return added/edited file first
        auto rec = GetNewOrEditedFileRecord(fileName);
        if (rec)
        {
            target = rec->mFileData;
            return true;
        }

        return mReader.ReadFileInto(target, fileName);
    }

    [[nodiscard]] bool FindFile(const char_type* fileName)
    {
        return GetNewOrEditedFileRecord(fileName) != nullptr;
    }

    [[nodiscard]] std::optional<std::vector<u8>> ReadFile(const char_type* fileName)
    {
        // Return added/edited file first
        auto rec = GetNewOrEditedFileRecord(fileName);
        if (rec)
        {
            return {rec->mFileData};
        }
        return mReader.ReadFile(fileName);
    }

    void AddFile(const char_type* fileNameInLvl, const std::vector<u8>& data)
    {
        if (mReader.IsOpen())
        {
            bool isEditingAFile = false;
            for (s32 i = 0; i < mReader.FileCount(); i++)
            {
                const auto fileName = mReader.FileNameAt(i);
                if (fileName == fileNameInLvl)
                {
                    isEditingAFile = true;
                    break;
                }
            }

            auto rec = GetNewOrEditedFileRecord(fileNameInLvl);
            if (rec)
            {
                rec->mEditOfExistingFile = isEditingAFile;
                rec->mFileData = data;
            }
            else
            {
                mNewOrEditedFiles.push_back({fileNameInLvl, data, isEditingAFile});
            }
        }
    }

    [[nodiscard]] bool Save(IFileIO& fileIo, std::vector<u8>& fileDataBuffer, const char_type* lvlName = nullptr)
    {
        if (mNewOrEditedFiles.empty())
        {
            return true;
        }

        s32 newFilesCount = 0;
        for (const auto& rec : mNewOrEditedFiles)
        {
            if (!rec.mEditOfExistingFile)
            {
                newFilesCount++;
            }
        }

        LvlHeader newHeader = {};
        std::vector<LvlFileRecord> fileRecs(mReader.FileCount() + newFilesCount);
        newHeader.field_4_ref_count = 0;
        newHeader.field_8_magic = 0x78646e49; // Idx
        newHeader.field_C_id = 0;
        newHeader.field_10_sub.field_0_num_files = static_cast<s32>(fileRecs.size());

        s32 totalFileOffset = static_cast<s32>(RoundUp((newHeader.field_10_sub.field_0_num_files * sizeof(LvlFileRecord)) + (sizeof(LvlHeader) - sizeof(LvlFileRecord))));
        newHeader.field_10_sub.field_4_header_size_in_sectors = totalFileOffset / 2048;
        if (newHeader.field_10_sub.field_4_header_size_in_sectors < 5)
        {
            newHeader.field_10_sub.field_4_header_size_in_sectors = 5;
        }
        newHeader.field_0_first_file_offset = newHeader.field_10_sub.field_4_header_size_in_sectors * 2048;
        totalFileOffset = newHeader.field_0_first_file_offset;

        // Add existing LVL file records
        s32 i = 0;
        for (i = 0; i < mReader.FileCount(); i++)
        {
            const auto fileName = mReader.FileNameAt(i);
            strncpy(fileRecs[i].field_0_file_name, fileName.c_str(), ALIVE_COUNTOF(LvlFileRecord::field_0_file_name));
            auto rec = GetNewOrEditedFileRecord(fileName.c_str());
            if (rec)
            {
                fileRecs[i].field_14_file_size = static_cast<s32>(rec->mFileData.size());
            }
            else
            {
                fileRecs[i].field_14_file_size = mReader.FileSizeAt(i);
            }

            fileRecs[i].field_C_start_sector = totalFileOffset / 2048;
            fileRecs[i].field_10_num_sectors = RoundUp(fileRecs[i].field_14_file_size) / 2048;

            totalFileOffset += fileRecs[i].field_14_file_size;
            totalFileOffset = RoundUp(totalFileOffset);
        }

        // Add new file records
        for (const auto& rec : mNewOrEditedFiles)
        {
            if (!rec.mEditOfExistingFile)
            {
                std::memcpy(fileRecs[i].field_0_file_name, rec.mFileNameInLvl.c_str(), ALIVE_COUNTOF(LvlFileRecord::field_0_file_name));
                fileRecs[i].field_14_file_size = static_cast<s32>(rec.mFileData.size());

                fileRecs[i].field_C_start_sector = totalFileOffset / 2048;
                fileRecs[i].field_10_num_sectors = RoundUp(fileRecs[i].field_14_file_size) / 2048;

                totalFileOffset += fileRecs[i].field_14_file_size;
                totalFileOffset = RoundUp(totalFileOffset);

                i++;
            }
        }

        auto outFile = fileIo.Open(lvlName, IFileIO::Mode::WriteBinary);
        if (!outFile->IsOpen())
        {
            return false;
        }

        // Write header
        outFile->Write(reinterpret_cast<const u8*>(&newHeader), sizeof(LvlHeader) - sizeof(LvlFileRecord));

        // Write file table
        for (const auto& fileRec : fileRecs)
        {
            outFile->Write(fileRec);
        }

        // Write out each file data
        for (const auto& fileRec : fileRecs)
        {
            outFile->Seek(fileRec.field_C_start_sector * 2048);

            const auto fileName = ToString(fileRec);
            auto rec = GetNewOrEditedFileRecord(fileName.c_str());
            if (rec)
            {
                outFile->Write(rec->mFileData.data(), rec->mFileData.size());
            }
            else
            {
                const bool goodRead = mReader.ReadFileInto(fileDataBuffer, fileName.c_str());
                if (!goodRead)
                {
                    throw ReliveAPI::IOReadException(fileName);
                }

                outFile->Write(fileDataBuffer.data(), fileDataBuffer.size());
            }
        }

        // Ensure termination padding to a multiple of the sector size exists at EOF
        if (!outFile->PadEOF(2048))
        {
            throw ReliveAPI::IOWriteException(lvlName);
        }

        return true;
    }

private:
    struct NewOrEditedFileRecord
    {
        std::string mFileNameInLvl;
        std::vector<u8> mFileData;
        bool mEditOfExistingFile;
    };

    [[nodiscard]] NewOrEditedFileRecord* GetNewOrEditedFileRecord(const char_type* fileName)
    {
        for (auto& rec : mNewOrEditedFiles)
        {
            if (rec.mFileNameInLvl == fileName)
            {
                return &rec;
            }
        }
        return nullptr;
    }

    LvlReader mReader;
    std::vector<NewOrEditedFileRecord> mNewOrEditedFiles;
};
} // namespace ReliveAPI
