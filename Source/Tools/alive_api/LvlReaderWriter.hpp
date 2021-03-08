#pragma once

#include <vector>
#include <optional>
#include "ByteStream.hpp"
#include "../AliveLibAE/LvlArchive.hpp"

inline std::string ToString(const LvlFileRecord& rec)
{
    int i = 0;
    for (i = 0; i < ALIVE_COUNTOF(LvlFileRecord::field_0_file_name); i++)
    {
        if (!rec.field_0_file_name[i])
        {
            break;
        }
    }
    return std::string(rec.field_0_file_name, i);
}

class LvlFileChunk
{
public:
    LvlFileChunk(DWORD id, ResourceManager::ResourceType resType, const std::vector<BYTE>& data)
        : mData(data)
    {
        mHeader.field_0_size = static_cast<DWORD>(data.size());
        mHeader.field_C_id = id;
        mHeader.field_8_type = resType;
    }

    DWORD Id() const
    {
        return mHeader.field_C_id;
    }

    DWORD Size() const
    {
        return mHeader.field_0_size;
    }

    const ResourceManager::Header& Header() const
    {
        return mHeader;
    }

    const std::vector<BYTE>& Data() const
    {
        return mData;
    }

private:
    ResourceManager::Header mHeader = {};
    std::vector<BYTE> mData;
};

class ChunkedLvlFile
{
public:
    explicit ChunkedLvlFile(const std::vector<BYTE>& data)
    {
        Read(data);
    }

    std::optional<LvlFileChunk> ChunkById(DWORD id) const
    {
        for (auto& chunk : mChunks)
        {
            if (chunk.Id() == id)
            {
                return { chunk };
            }
        }
        return {};
    }

    void AddChunk(LvlFileChunk& chunkToAdd)
    {
        for (auto& chunk : mChunks)
        {
            if (chunk.Id() == chunkToAdd.Id())
            {
                chunk = chunkToAdd;
                return;
            }
        }
        mChunks.push_back(chunkToAdd);
    }

    std::vector<BYTE> Data() const
    {
        std::size_t neededSize = 0;
        for (auto& chunk : mChunks)
        {
            neededSize += chunk.Size();
        }
        neededSize += sizeof(ResourceManager::Header) * mChunks.size();

        ByteStream s;
        s.ReserveSize(neededSize);
        for (auto& chunk : mChunks)
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
        return s.GetBuffer();
    }

private:
    void Read(const std::vector<BYTE>& data)
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

            std::vector<BYTE> tmpData(resHeader.field_0_size);
            if (resHeader.field_0_size > 0)
            {
                tmpData.resize(tmpData.size() - sizeof(ResourceManager::Header));
                s.Read(tmpData);
            }

            LvlFileChunk chunk(resHeader.field_C_id, static_cast<ResourceManager::ResourceType>(resHeader.field_8_type), tmpData);
            mChunks.push_back(chunk);

            if (resHeader.field_8_type == ResourceManager::ResourceType::Resource_End)
            {
                break;
            }

        } while (!s.AtReadEnd());
    }

    std::vector<LvlFileChunk> mChunks;
};

class LvlReader
{
public:
    explicit LvlReader(const char* lvlFile)
    {
        mFileHandle = ::fopen(lvlFile, "rb");
        if (mFileHandle)
        {
            if (!ReadTOC())
            {
                Close();
            }
        }
    }

    bool IsOpen() const { return mFileHandle != nullptr; }

    void Close()
    {
        if (IsOpen())
        {
            ::fclose(mFileHandle);
            mFileHandle = nullptr;
        }
    }

    ~LvlReader()
    {
        Close();
    }

    std::optional<std::vector<BYTE>> ReadFile(const char* fileName)
    {
        if (!IsOpen())
        {
            return {};
        }

        for (const auto& rec : mFileRecords)
        {
            if (strncmp(rec.field_0_file_name, fileName, ALIVE_COUNTOF(LvlFileRecord::field_0_file_name)) == 0)
            {
                const auto fileOffset = rec.field_C_start_sector * 2048;
                if (::fseek(mFileHandle, fileOffset, SEEK_SET) != 0)
                {
                    return {};
                }

                std::vector<BYTE> ret(rec.field_14_file_size);
                if (::fread(ret.data(), 1, ret.size(), mFileHandle) != ret.size())
                {
                    return {};
                }

                return { ret };
            }
        }
        return {};
    }

    int FileCount() const
    {
        return mHeader.field_10_sub.field_0_num_files;
    }

    std::string FileNameAt(int idx) const
    {
        return ToString(mFileRecords[idx]);
    }

    int FileSizeAt(int idx) const
    {
        return mFileRecords[idx].field_14_file_size;
    }

protected:
    bool ReadTOC()
    {
        if (::fread(&mHeader, sizeof(LvlHeader) - sizeof(LvlFileRecord), 1, mFileHandle) != 1)
        {
            return false;
        }

        // TODO: Should probably validate the header, but the real game does not give a toss
        // so probably is not that important.

        mFileRecords.resize(mHeader.field_10_sub.field_0_num_files);

        if (::fread(mFileRecords.data(), sizeof(LvlFileRecord), mFileRecords.size(), mFileHandle) != mFileRecords.size())
        {
            return false;
        }

        return true;
    }

    FILE* mFileHandle = nullptr;
    LvlHeader mHeader = {};
    std::vector<LvlFileRecord> mFileRecords;
};

template<class T>
inline T RoundUp(T numToRound, T multiple)
{
    if (multiple == 0)
    {
        return numToRound;
    }

    auto remainder = numToRound % multiple;
    if (remainder == 0)
    {
        return numToRound;
    }

    return numToRound + multiple - remainder;
}

template<class T>
inline T RoundUp(T offset)
{
    return RoundUp(offset, static_cast<T>(2048));
}

class LvlWriter
{
public:
    explicit LvlWriter(const char* lvlFile)
        : mReader(lvlFile)
    {

    }

    void Close()
    {
        mReader.Close();
    }

    bool IsOpen() const { return mReader.IsOpen(); }

    std::optional<std::vector<BYTE>> ReadFile(const char* fileName)
    {
        // Return added/edited file first
        auto rec = GetNewOrEditedFileRecord(fileName);
        if (rec)
        {
            return { rec->mFileData };
        }
        return mReader.ReadFile(fileName);
    }

    void AddFile(const char* fileNameInLvl, const std::vector<BYTE>& data)
    {
        if (mReader.IsOpen())
        {
            bool isEditingAFile = false;
            for (int i = 0; i < mReader.FileCount(); i++)
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
                mNewOrEditedFiles.push_back({ fileNameInLvl, data, isEditingAFile });
            }
        }
    }

    bool Save(const char* lvlName = nullptr)
    {
        if (mNewOrEditedFiles.empty())
        {
            return true;
        }

        int newFilesCount = 0;
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
        newHeader.field_10_sub.field_0_num_files = static_cast<int>(fileRecs.size());

        int totalFileOffset = RoundUp((newHeader.field_10_sub.field_0_num_files * sizeof(LvlFileRecord)) + (sizeof(LvlHeader) - sizeof(LvlFileRecord)));
        newHeader.field_10_sub.field_4_header_size_in_sectors = newHeader.field_0_first_file_offset / 2048;
        if (newHeader.field_10_sub.field_4_header_size_in_sectors < 5)
        {
            newHeader.field_10_sub.field_4_header_size_in_sectors = 5;
        }
        newHeader.field_0_first_file_offset = newHeader.field_10_sub.field_4_header_size_in_sectors * 2048;
        totalFileOffset = newHeader.field_0_first_file_offset;

        // Add existing LVL file records
        int i = 0;
        for (i = 0; i < mReader.FileCount(); i++)
        {
            const auto fileName = mReader.FileNameAt(i);
            strncpy(fileRecs[i].field_0_file_name, fileName.c_str(), ALIVE_COUNTOF(LvlFileRecord::field_0_file_name));
            auto rec = GetNewOrEditedFileRecord(fileName.c_str());
            if (rec)
            {
                fileRecs[i].field_14_file_size = static_cast<int>(rec->mFileData.size());
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
                memcpy(fileRecs[i].field_0_file_name, rec.mFileNameInLvl.c_str(), ALIVE_COUNTOF(LvlFileRecord::field_0_file_name));
                fileRecs[i].field_14_file_size = static_cast<int>(rec.mFileData.size());
                i++;
            }
        }

        FILE* outFile = ::fopen("new.lvl", "wb");
        if (!outFile)
        {
            return false;
        }

        // Write header
        fwrite(&newHeader, sizeof(LvlHeader) - sizeof(LvlFileRecord), 1, outFile);

        // Write file table
        for (const auto& fileRec : fileRecs)
        {
            ::fwrite(&fileRec, sizeof(LvlFileRecord), 1, outFile);
        }

        // Write out each file data
        for (const auto& fileRec : fileRecs)
        {
            ::fseek(outFile, fileRec.field_C_start_sector * 2048, SEEK_SET);

            const auto fileName = ToString(fileRec);
            auto rec = GetNewOrEditedFileRecord(fileName.c_str());
            if (rec)
            {
                ::fwrite(rec->mFileData.data(), 1, rec->mFileData.size(), outFile);
            }
            else
            {
                std::optional<std::vector<BYTE>> data = mReader.ReadFile(fileName.c_str());
                if (!data)
                {
                    abort();
                }

                ::fwrite(data->data(), 1, data->size(), outFile);
            }
        }

        // Ensure termination padding to a multiple of the sector size exists at EOF
        const long int pos = ::ftell(outFile);
        if (pos == -1)
        {
            abort();
        }

        if (pos+1 != RoundUp(pos))
        {
            ::fseek(outFile, RoundUp(pos)-1, SEEK_SET);
            BYTE emptyByte = 0;
            ::fwrite(&emptyByte, sizeof(BYTE), 1, outFile);
        }

        ::fclose(outFile);
        return true;
    }

private:
    struct NewOrEditedFileRecord
    {
        std::string mFileNameInLvl;
        std::vector<BYTE> mFileData;
        bool mEditOfExistingFile;
    };

    NewOrEditedFileRecord* GetNewOrEditedFileRecord(const char* fileName)
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

