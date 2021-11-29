#pragma once

#include <string>

namespace ReliveAPI {
class Exception
{
public:
    virtual ~Exception()
    { }
    Exception() = default;
    explicit Exception(const std::string& what)
        : mWhat(what)
    { }
    const std::string& what() const
    {
        return mWhat;
    }

protected:
    std::string mWhat;
};

// Error opening json file on disk
class IOReadException final : public Exception
{
public:
    using Exception::Exception;
};
class IOWriteException final : public Exception
{
public:
    using Exception::Exception;
};

// The value of the enum isn't anything we know about
class UnknownEnumValueException final : public Exception
{
public:
    using Exception::Exception;
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
    using Exception::Exception;
};
class DuplicateEnumNameException final : public Exception
{
public:
    using Exception::Exception;
};

class PropertyNotFoundException final : public Exception
{ };

class InvalidGameException final : public Exception
{
public:
    using Exception::Exception;
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
    using Exception::Exception;
};

// Json version is less than the current API version
class JsonNeedsUpgradingException final : public Exception
{ };

// Opening a Path from the LVL failed
class OpenPathException final : public Exception
{ };

// Count of collision items doesn't match hard coded data
class CollisionsCountChangedException final : public Exception
{ };

// The x,y of a camera in the json it outside of the map size (e.g camera 2,2 but the map size is 1,1)
class CameraOutOfBoundsException final : public Exception
{ };

// The TLV couldn't be created for this structure type because it isn't known
class UnknownStructureTypeException final : public Exception
{
public:
    using Exception::Exception;
};

// When reading binary path data the length field of the TLV is not the fixed size we expected it to be
class WrongTLVLengthException final : public Exception
{ };

// Expected to read "key" in the json but it either didn't exist or was the wrong type
class JsonKeyNotFoundException final : public Exception
{
public:
    explicit JsonKeyNotFoundException(const std::string& key)
        : Exception(key)
        , mKey(key)
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
