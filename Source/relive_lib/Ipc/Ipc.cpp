#include "Ipc.hpp"
#include "../logger.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <vector>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

struct PacketHeader final
{
    u8 mMagic = 0x2A;
    relive::PacketTypes mType = relive::PacketTypes::None;
    u32 mLength = 0;
};

#define PIPE_PATH L"\\\\.\\pipe\\relive_pipe"

class [[nodiscard]] Win32PipeRAII final
{
public:
    Win32PipeRAII()
    {

    }

    ~Win32PipeRAII()
    {
        Close();
    }

    void Close()
    {
        if (mHandle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(mHandle);
            mHandle = INVALID_HANDLE_VALUE;
        }
    }

    bool Connect(const std::string& socketPath)
    {
        Close();
        mHandle = ::CreateFileW(
            socketPath.c_str(), // TODO: to wstring
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (mHandle == INVALID_HANDLE_VALUE)
        {
            LOG_INFO("CreateFileW failed GLE %d", ::GetLastError());
            return false;
        }
        return true;
    }

    int Accept()
    {
        // TODO: Requires Bind - and the mHandle is the client
        if (::ConnectNamedPipe(mHandle, NULL) != 0)
        {
            LOG_INFO("ConnectNamedPipe failed GLE %d", ::GetLastError());
            return false;
        }

        return true;
    }

    bool Bind(const std::string& socketPath)
    {
        Close();
        mHandle = ::CreateNamedPipeW(
            socketPath.c_str(), // TODO: Convert to wstring
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,          // max instances
            4096, 4096, // out/in buffer
            0,
            NULL
        );

        if (mHandle == INVALID_HANDLE_VALUE) 
        {
            LOG_ERROR("CreateNamedPipe failed GLE %d", ::GetLastError());
            return false;
        }

        return true;
    }

    bool Listen()
    {
        // Bind() already listens
        return true;
    }

    // TODO: Write

    // TODO: Read

private:
    HANDLE mHandle = INVALID_HANDLE_VALUE;
};


class [[nodiscard]] UnixSocketRAII final
{
public:
    explicit UnixSocketRAII(int socket) 
     : mSocket(socket)
    {

    }

    UnixSocketRAII()
    {
        mSocket = ::socket(AF_UNIX, SOCK_STREAM, 0);
        if (mSocket < 0)
        {
            LOG_ERROR("socket failed %d", errno);
            return;
        }

        const int enable = 1;
        if (::setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        {
            LOG_ERROR("Set SO_REUSEADDR failed %d", errno);
        }
    }

    ~UnixSocketRAII()
    {
        Close();
    }

    void Close()
    {
        if (mSocket)
        {
            ::close(mSocket);
            mSocket = 0;
        }
    }

    bool Connect(const std::string& socketPath)
    {
        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, socketPath.c_str());

        if (::connect(mSocket, (sockaddr*)&addr, sizeof(addr)) < 0) 
        {
            LOG_ERROR("Connect failed %d", errno);
            return false;
        }
        return true;
    }

    int Accept()
    {
        return ::accept(mSocket, nullptr, nullptr);
    }

    bool Bind(const std::string& socketPath)
    {
        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, socketPath.c_str());

        if (::bind(mSocket, (sockaddr*)&addr, sizeof(addr)) < 0) 
        {
            return false;
        }
        return true;
    }

    bool Listen()
    {
        if (listen(mSocket, 1) < 0) 
        {
            return false;
        }
        return true;
    }

    int GetSocket() const
    {
        return mSocket;
    }

    bool Write(const void* buffer, size_t bufferLenBytes)
    {
        if (::write(mSocket, buffer, bufferLenBytes) < 0)
        {
            LOG_ERROR("Write failed %d", errno);
            return false;
        }
        return true;
    }

    // TODO: Read

private:
    int mSocket = 0;
};

#define SOCKET_PATH "/tmp/relive_ipc.sock"

// TODO: over kill remove this class
class LinuxAutoFileDeleter final
{
public:
    explicit LinuxAutoFileDeleter(const std::string& filePath)
     : mPath(filePath)
    {

    }

    ~LinuxAutoFileDeleter()
    {
    }

    void Delete()
    {
        if (::unlink(mPath.c_str()) < 0 && errno != 2)
        {
            LOG_ERROR("Delete of %s failed with %d", mPath.c_str(), errno);
        }
    }

private:
    std::string mPath;
};


class LinuxIpc final : public relive::IIpcInterface
{
public:
    LinuxIpc()
    {
    }

    void Listen(relive::TOnPacket fnOnPacket) final
    {
        mOnPacket = fnOnPacket;

        if (!mSocket.Bind(SOCKET_PATH))
        {
            if (errno == 98)
            {
                LinuxAutoFileDeleter socketDeleter(SOCKET_PATH);
                socketDeleter.Delete();
                if (!mSocket.Bind(SOCKET_PATH))
                {
                    LOG_ERROR("Socket bind failed %d", errno);
                    return;
                }
            }
            else
            {
                LOG_ERROR("Socket bind failed %d", errno);
                return;
            }
        }

        LOG_INFO("Socket bound");

        if (!mSocket.Listen())
        {
            LOG_ERROR("Socket listen failed %d", errno);
            return;
        }

        LOG_INFO("Listening for connections...");

        mAcceptConnections = true;
        mAcceptConnectionsThread = std::make_unique<std::thread>(&LinuxIpc::AcceptClientsThread, this);
    }

    void Cancel() final
    {
        mAcceptConnections = false;
        mSocket.Close();
        if (mAcceptConnectionsThread)
        {
            if (mAcceptConnectionsThread->joinable())
            {
                LOG_INFO("Wait for thread to exit...");
                mAcceptConnectionsThread->join();
                LOG_INFO("Thread exited");
            }
            else
            {
                LOG_INFO("Thread already ended");
            }
        }
        else
        {
            LOG_INFO("Not accepting connections");
        }
    }

    [[nodiscard]] bool Connect() final
    {
        return mSocket.Connect(SOCKET_PATH);
    }

    void SendLevelChanged(const std::string& fileName) final
    {
        PacketHeader header;
        header.mLength = static_cast<uint32_t>(fileName.size());
        header.mType = relive::PacketTypes::LevelPathJsonChanged;
        mSocket.Write(&header, sizeof(header));
        mSocket.Write(fileName.c_str(), fileName.size());
    }

private:

    void AcceptClientsThread()
    {
        while (mAcceptConnections)
        {
            UnixSocketRAII client(mSocket.Accept());

            // We expect 1 message from a client and then nothing - and only really support 1 client
            PacketHeader header;
            const ssize_t n = read(client.GetSocket(), &header, sizeof(header));
            if (n <= 0)
            {
                LOG_ERROR("Read header failed %d", errno);
            }
            else
            {
                if (header.mMagic != 0x2A)
                {
                    LOG_ERROR("Bad packet magic in header");
                    continue;
                }

                std::vector<u8> payload;
                payload.resize(header.mLength);
                size_t totalBytesRead = 0;
                while (totalBytesRead < header.mLength)
                {
                    const size_t readBytes = read(client.GetSocket(), payload.data() + totalBytesRead, payload.size() - totalBytesRead);
                    if (readBytes <= 0)
                    {
                        LOG_ERROR("Read payload failed %d", errno);
                        break;
                    }
                    totalBytesRead += readBytes;
                }

                if (totalBytesRead >= header.mLength)
                {
                    switch(header.mType)
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
            }
        }
    }

    std::atomic<bool> mAcceptConnections;
    std::unique_ptr<std::thread> mAcceptConnectionsThread;
    UnixSocketRAII mSocket;
};

namespace relive
{
    std::unique_ptr<IIpcInterface> MakeIpcInterface()
    {
        return std::make_unique<LinuxIpc>();
    }
}
