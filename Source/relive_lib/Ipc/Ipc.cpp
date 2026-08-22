#include "Ipc.hpp"
#include "IpcTransportInterfaces.hpp"
#include "../logger.hpp"
#include <memory>
#include <thread>
#include <atomic>

static constexpr u8 kPacketHeaderMagic = 0x2A;

struct PacketHeader final
{
    u8 mMagic = kPacketHeaderMagic;
    relive::PacketTypes mType = relive::PacketTypes::None;
    u32 mLength = 0;
};

class CommonIpc final : public relive::IIpcInterface
{
public:
    CommonIpc(std::unique_ptr<IServer> server,
              std::unique_ptr<IClient> client)
        : mServer(std::move(server))
        , mClient(std::move(client))
    {

    }

    ~CommonIpc()
    {
        Cancel();
    }

    void Listen(relive::TOnPacket fnOnPacket)
    {
        mServer->Listen();

        mOnPacket = fnOnPacket;

        mAcceptConnections = true;
        mAcceptThread = std::make_unique<std::thread>(&CommonIpc::AcceptLoop, this);
    }

    void Cancel()
    {
        mAcceptConnections = false;

        // Destroy server to unblock WaitForClient if it uses blocking primitives
        mServer.reset();

        if (mAcceptThread)
        {
            if (mAcceptThread->joinable())
            {
                LOG_INFO("Wait for thread to exit...");
                mAcceptThread->join();
                LOG_INFO("Thread exited");
            }
            mAcceptThread.reset();
        }
    }

    void SendLevelChanged(const std::string& fileName)
    {
        auto conn = mClient->Connect();
        if (!conn)
        {
            LOG_ERROR("Client connect failed");
            return;
        }

        PacketHeader header;
        header.mMagic = kPacketHeaderMagic;
        header.mType = relive::PacketTypes::LevelPathJsonChanged;
        header.mLength = static_cast<u32>(fileName.size());

        if (!conn->Write(&header, sizeof(header)))
        {
            LOG_ERROR("Write header failed");
            return;
        }

        if (!fileName.empty())
        {
            if (!conn->Write(fileName.data(), fileName.size()))
            {
                LOG_ERROR("Write payload failed");
            }
        }
    }

private:
    void AcceptLoop()
    {
        while (mAcceptConnections)
        {
            auto conn = mServer->WaitForClient();
            if (!conn)
            {
                // Could be interrupted or failed; loop again
                continue;
            }

            HandleConnection(*conn);
        }
    }

    void HandleConnection(IConnection& conn)
    {
        PacketHeader header{};
        if (!conn.Read(&header, sizeof(header)))
        {
            LOG_ERROR("Read header failed");
            return;
        }

        if (header.mMagic != kPacketHeaderMagic)
        {
            LOG_ERROR("Bad packet magic in header");
            return;
        }

        std::vector<u8> payload;
        payload.resize(header.mLength);

        if (header.mLength > 0)
        {
            if (!conn.Read(payload.data(), payload.size()))
            {
                LOG_ERROR("Read payload failed");
                return;
            }
        }

        switch (header.mType)
        {
            case relive::PacketTypes::LevelPathJsonChanged:
                if (mOnPacket)
                {
                    mOnPacket(header.mType, payload);
                }
                break;

            default:
                LOG_ERROR("Unknown packet type %d", static_cast<u8>(header.mType));
                break;
        }
    }

    std::atomic<bool> mAcceptConnections{false};
    std::unique_ptr<std::thread> mAcceptThread;

    std::unique_ptr<IServer> mServer;
    std::unique_ptr<IClient> mClient;
    relive::TOnPacket mOnPacket;
};

std::unique_ptr<IServer> MakeIpcServer();
std::unique_ptr<IClient> MakeIpcClient();

namespace relive
{
    std::unique_ptr<IIpcInterface> MakeIpcInterface()
    {
        return std::make_unique<CommonIpc>(MakeIpcServer(), MakeIpcClient());
    }
}
