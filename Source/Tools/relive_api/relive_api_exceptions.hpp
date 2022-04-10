#pragma once

#include <string>

namespace ReliveAPI {
class Exception
{
    // Common base for catch-all
};

// Error opening json file on disk
class IOReadException final : public Exception
{
public:
    explicit IOReadException(const std::string& fileName)
        : mFileName(fileName)
    {

    }

    const std::string& FileName() const
    {
        return mFileName;
    }

private:
    std::string mFileName;
};

class IOWriteException final : public Exception
{
public:
    explicit IOWriteException(const std::string& fileName)
        : mFileName(fileName)
    {

    }

    const std::string& FileName() const
    {
        return mFileName;
    }

private:
    std::string mFileName;
};

class UnknownEnumTypeException final : public Exception
{
public:
    UnknownEnumTypeException()
    {
        // Thrown when looking up an enum type string via its typeid, thus we don't know the enum name
    }

    explicit UnknownEnumTypeException(const std::string& enumName, const std::string& enumValue)
        : mEnumName(enumName)
        , mEnumValue(enumValue)
    {
    }

    const std::string& EnumName() const
    {
        return mEnumName;
    }

    const std::string& EnumValue() const
    {
        return mEnumValue;
    }

private:
    std::string mEnumName;
    std::string mEnumValue;
};

class IOReadPastEOFException final : public Exception
{ };

class EmptyPropertyNameException final : public Exception
{ };

class EmptyTypeNameException final : public Exception
{ };

class DuplicatePropertyKeyException final : public Exception
{ };

class DuplicatePropertyNameException final : public Exception
{
public:
    explicit DuplicatePropertyNameException(const std::string& propertyName)
        : mPropertyName(propertyName)
    {

    }

    const std::string& PropertyName() const
    {
        return mPropertyName;
    }

private:
    std::string mPropertyName;
};

class DuplicateEnumNameException final : public Exception
{
public:
    explicit DuplicateEnumNameException(const std::string& enumTypeName)
        : mEnumTypeName(enumTypeName)
    {

    }

    const std::string& EnumTypeName() const
    {
        return mEnumTypeName;
    }

private:
    std::string mEnumTypeName;
};

// When looking up via pointer
class PropertyNotFoundException final : public Exception
{ };

class InvalidGameException final : public Exception
{
public:
    explicit InvalidGameException(const std::string& gameName)
        : mGameName(gameName)
    {

    }

    const std::string& GameName() const
    {
        return mGameName;
    }

private:
    std::string mGameName;
};

// Json data failed to parse
class InvalidJsonException final : public Exception
{ };

// Json version is newer than what we support upgrade to (we know about up to v10, but given v11 json for example)
class JsonVersionTooNew final : public Exception
{ };

// Json version is older than min supported upgrade target
class JsonVersionTooOld final : public Exception
{ };

// Camera name wasn't 8 chars
class BadCameraNameException final : public Exception
{
public:
    explicit BadCameraNameException(const std::string& cameraName)
        : mCameraName(cameraName)
    {

    }

    const std::string& CameraName() const
    {
        return mCameraName;
    }

private:
    std::string mCameraName;
};

// Json version is less than the current API version
class JsonNeedsUpgradingException final : public Exception
{
public:
    JsonNeedsUpgradingException(int currentApiVersion, int yourJsonVersion)
        : mCurrentApiVersion(currentApiVersion)
        , mYourJsonVersion(yourJsonVersion)
    {

    }

    int CurrentApiVersion() const
    {
        return mCurrentApiVersion;
    }

    int YourJsonVersion() const
    {
        return mYourJsonVersion;
    }

private:
    int mCurrentApiVersion = 0;
    int mYourJsonVersion = 0;
};

// Opening a Path from the LVL failed
class OpenPathException final : public Exception
{ };


// The x,y of a camera in the json it outside of the map size (e.g camera 2,2 but the map size is 1,1)
class CameraOutOfBoundsException final : public Exception
{ };

// The TLV couldn't be created for this structure type because it isn't known
class UnknownStructureTypeException final : public Exception
{
public:
    explicit UnknownStructureTypeException(const std::string& structureTypeName)
        : mStructureTypeName(structureTypeName)
    {

    }

    const std::string& StructureTypeName() const
    {
        return mStructureTypeName;
    }

private:
    std::string mStructureTypeName;
};

// When reading binary path data the length field of the TLV is not the fixed size we expected it to be
class WrongTLVLengthException final : public Exception
{ };

// Expected to read "key" in the json but it either didn't exist or was the wrong type
class JsonKeyNotFoundException final : public Exception
{
public:
    explicit JsonKeyNotFoundException(const std::string& key)
        : mKey(key)
    {
    }

    const std::string& Key() const
    {
        return mKey;
    }

private:
    std::string mKey;
};

} // namespace ReliveAPI
