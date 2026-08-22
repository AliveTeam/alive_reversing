#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace relive
{
    enum class PacketTypes : unsigned char 
    {
        None = 0x0,
        LevelPathJsonChanged = 0xA
    };

    using TOnPacket = std::function<void(relive::PacketTypes, const std::vector<unsigned char>&)>;

    class IIpcInterface
    {
    public:
        virtual ~IIpcInterface() { }
        virtual void Listen(TOnPacket fnOnPacket) = 0;
        virtual void Cancel() = 0;
        virtual void SendLevelChanged(const std::string& fileName) = 0;
    protected:
        TOnPacket mOnPacket;
    };

    std::unique_ptr<IIpcInterface> MakeIpcInterface();
}
