#pragma once

#include "relive_api.hpp"
#include "file_api.hpp"
#include <functional>
#include <QString>

// Implement ReliveAPI::IFileIO override that supports unicode from utf8 on windows

class File final : public ReliveAPI::IFile
{
public:
    File(const std::string& fileName, ReliveAPI::IFileIO::Mode mode)
    {
#ifdef _WIN32
        // Convert the uft8 string to a utf16 string for old man windows
        QString utf8 = QString::fromStdString(fileName);
        std::vector<wchar_t> buffer;
        buffer.resize(utf8.length() + 1);
        utf8.toWCharArray(buffer.data());

        switch (mode)
        {
        case ReliveAPI::IFileIO::Mode::Write:
            mFileHandle = ::_wfopen(buffer.data(), L"w");
            break;

        case ReliveAPI::IFileIO::Mode::Read:
            mFileHandle = ::_wfopen(buffer.data(), L"r");
            break;

        case ReliveAPI::IFileIO::Mode::ReadBinary:
            mFileHandle = ::_wfopen(buffer.data(), L"rb");
            break;

        case ReliveAPI::IFileIO::Mode::WriteBinary:
            mFileHandle = ::_wfopen(buffer.data(), L"wb");
            break;
    }

#else
        // OSX, Linux and most other OSes support utf8 directly, the future is now, old man
        switch (mode)
        {
        case ReliveAPI::IFileIO::Mode::Write:
            mFileHandle = ::fopen(fileName.c_str(), "w");
            break;

        case ReliveAPI::IFileIO::Mode::Read:
            mFileHandle = ::fopen(fileName.c_str(), "r");
            break;

        case ReliveAPI::IFileIO::Mode::ReadBinary:
            mFileHandle = ::fopen(fileName.c_str(), "rb");
            break;

        case ReliveAPI::IFileIO::Mode::WriteBinary:
            mFileHandle = ::fopen(fileName.c_str(), "wb");
            break;
        }
#endif
    }

    ~File() override
    {
        if (IsOpen())
        {
            ::fclose(mFileHandle);
        }
    }

    bool IsOpen() const override
    {
        return mFileHandle != nullptr;
    }

    bool Seek(std::size_t absPos) override
    {
        return ::fseek(mFileHandle, static_cast<long>(absPos), SEEK_SET) == 0;
    }

    bool Read(u8* buffer, std::size_t len) override
    {
        return ::fread(buffer, 1, len, mFileHandle) == len;
    }

    bool Write(const u8* buffer, std::size_t len) override
    {
        return ::fwrite(buffer, 1, len, mFileHandle) == len;
    }

    bool ReadInto(std::string& str) override
    {
        const auto oldPos = ::ftell(mFileHandle);
        if (::fseek(mFileHandle, 0, SEEK_END) != 0)
        {
            return false;
        }

        const auto fileSize = ftell(mFileHandle);
        if (fileSize == -1)
        {
            return false;
        }

        str.resize(fileSize);
        if (!Seek(oldPos))
        {
            return false;
        }

        return Read(reinterpret_cast<u8*>(str.data()), str.size());
    }

    bool PadEOF(u32 multiple) override
    {
        const auto pos = ::ftell(mFileHandle);
        if (pos == -1)
        {
            return false;
        }

        if (pos + 1 != RoundUp(static_cast<int>(pos), static_cast<int>(multiple)))
        {
            ::fseek(mFileHandle, RoundUp(pos) - 1, SEEK_SET);
            u8 emptyByte = 0;
            ::fwrite(&emptyByte, sizeof(u8), 1, mFileHandle);
        }

        return true;
    }

private:
    FILE* mFileHandle = nullptr;
};

class EditorFileIO final : public ReliveAPI::IFileIO
{
public:
    std::unique_ptr<ReliveAPI::IFile> Open(const std::string& fileName, ReliveAPI::IFileIO::Mode mode) override
    {
        auto ret = std::make_unique<File>(fileName, mode);
        if (!ret->IsOpen())
        {
            return nullptr;
        }
        return ret;
    }
};

// TODO: Add more context to each of these errors
template<typename ApiCall>
bool ExecApiCall(ApiCall apiCall, std::function<void(const QString)> onFailure)
{
    try
    {
        return apiCall();
    }
    catch (const ReliveAPI::IOReadException&)
    {
        onFailure(QString("IO read failure"));
        return false;
    }
    catch (const ReliveAPI::IOWriteException&)
    {
        onFailure(QString("IO write failure"));
        return false;
    }
    catch (const ReliveAPI::IOReadPastEOFException&)
    {
        onFailure(QString("IO read past EOF"));
        return false;
    }
    catch (const ReliveAPI::EmptyPropertyNameException&)
    {
        onFailure(QString("Empty property name"));
        return false;
    }
    catch (const ReliveAPI::EmptyTypeNameException&)
    {
        onFailure(QString("Empty type name"));
        return false;
    }
    catch (const ReliveAPI::DuplicatePropertyKeyException&)
    {
        onFailure(QString("Duplicated property key"));
        return false;
    }
    catch (const ReliveAPI::DuplicatePropertyNameException& e)
    {
        onFailure(QString("Duplicated property name: ") + e.PropertyName().c_str());
        return false;
    }
    catch (const ReliveAPI::DuplicateEnumNameException& e)
    {
        onFailure(QString("Duplicated enum name: ") + e.EnumTypeName().c_str());
        return false;
    }
    catch (const ReliveAPI::PropertyNotFoundException&)
    {
        onFailure(QString("Property not found"));
        return false;
    }
    catch (const ReliveAPI::InvalidGameException& e)
    {
        onFailure(QString("Invalid game name: ") + e.GameName().c_str());
        return false;
    }
    catch (const ReliveAPI::InvalidJsonException&)
    {
        onFailure(QString("Invalid json, can't parse"));
        return false;
    }
    catch (const ReliveAPI::JsonVersionTooNew&)
    {
        onFailure(QString("Json version too new"));
        return false;
    }
    catch (const ReliveAPI::JsonVersionTooOld&)
    {
        onFailure(QString("Json version too old"));
        return false;
    }
    catch (const ReliveAPI::BadCameraNameException& e)
    {
        onFailure(QString("Bad camera name: ") + e.CameraName().c_str());
        return false;
    }
    catch (const ReliveAPI::JsonNeedsUpgradingException& e)
    {
        onFailure(QString("Json needs upgrading from ") + QString::number(e.YourJsonVersion()) + " to " + QString::number(e.CurrentApiVersion()) + " you can export with the previous editor to .lvl and re export to json with this version to fix this");
        return false;
    }
    catch (const ReliveAPI::OpenPathException&)
    {
        onFailure(QString("Open path failure"));
        return false;
    }
    catch (const ReliveAPI::CameraOutOfBoundsException&)
    {
        onFailure(QString("Camera out of bounds"));
        return false;
    }
    catch (const ReliveAPI::UnknownStructureTypeException& e)
    {
        onFailure(QString("Unknown structure record: ") + e.StructureTypeName().c_str());
        return false;
    }
    catch (const ReliveAPI::WrongTLVLengthException&)
    {
        onFailure(QString("TLV length is wrong"));
        return false;
    }
    catch (const ReliveAPI::JsonKeyNotFoundException& e)
    {
        onFailure(QString("Missing json key: ") + e.Key().c_str());
        return false;
    }
    catch (const ReliveAPI::Exception&)
    {
        onFailure(QString("Unknown API exception"));
        return false;
    }
    return true;
}
