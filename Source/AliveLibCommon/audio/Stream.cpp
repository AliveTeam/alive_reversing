#include <sstream>
#include <algorithm>
#include <fstream>
#include <iterator>
#include "logger.hpp"
#include "Stream.hpp"
#include "Exceptions.hpp"

namespace psx {

Stream::Stream(std::vector<Uint8>&& data)
{
    mSize = data.size();
    auto s = std::make_unique<std::stringstream>();
    std::copy(data.begin(), data.end(), std::ostream_iterator<unsigned char>(*s));
    mStream.reset(s.release());
    Seek(0);
}

void Stream::ReadUInt8(Uint8& output)
{
    if (!mStream->read(reinterpret_cast<char*>(&output), sizeof(output)))
    {
        throw Exception("ReadUInt8 failure");
    }
}

void Stream::ReadUInt32(Uint32& output)
{
    if (!mStream->read(reinterpret_cast<char*>(&output), sizeof(output)))
    {
        throw Exception("ReadUInt32 failure");
    }
}

void Stream::ReadUInt16(Uint16& output)
{
    if (!mStream->read(reinterpret_cast<char*>(&output), sizeof(output)))
    {
        throw Exception("ReadUInt32 failure");
    }
}

void Stream::ReadSInt16(Sint16& output)
{
    if (!mStream->read(reinterpret_cast<char*>(&output), sizeof(output)))
    {
        throw Exception("ReadSInt16 failure");
    }
}

void Stream::ReadBytes(Sint8* pDest, size_t destSize)
{
    if (!mStream->read(reinterpret_cast<char*>(pDest), destSize))
    {
        throw Exception("ReadBytes failure");
    }
}

void Stream::ReadBytes(Uint8* pDest, size_t destSize)
{
    if (!mStream->read(reinterpret_cast<char*>(pDest), destSize))
    {
        throw Exception("ReadBytes failure");
    }
}

void Stream::Seek(size_t pos)
{
    if (!mStream->seekg(pos))
    {
        throw Exception("Seek failure");
    }
}

bool Stream::AtEnd() const
{
    const int c = mStream->peek();
    return (c == EOF);
}

size_t Stream::Pos() const
{
    const size_t pos = static_cast<size_t>(mStream->tellg());
    return pos;
}

size_t Stream::Size() const
{
    return mSize;
}

} // namespace psx