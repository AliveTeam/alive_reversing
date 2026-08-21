#include "Ipc.hpp"
#include "../logger.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <unistd.h>
#endif

struct PacketHeader final
{
    u8 mMagic = 0x2A;
    relive::PacketTypes mType = relive::PacketTypes::None;
    u32 mLength = 0;
};


#ifdef _WIN32

#define PIPE_PATH L"\\\\.\\pipe\\relive_pipe"

class [[nodiscard]] Win32PipeRAII final
{
public:
    Win32PipeRAII()
        : mHandle(INVALID_HANDLE_VALUE)
    {
    }

    explicit Win32PipeRAII(HANDLE handle)
        : mHandle(handle)
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
            ::FlushFileBuffers(mHandle);
            ::DisconnectNamedPipe(mHandle);
            ::CloseHandle(mHandle);
            mHandle = INVALID_HANDLE_VALUE;
        }
    }

    bool Connect(const std::wstring& socketPath)
    {
        Close();
        mHandle = ::CreateFileW(
            socketPath.c_str(),
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

    HANDLE Accept()
    {
        if (mHandle == INVALID_HANDLE_VALUE)
        {
            return INVALID_HANDLE_VALUE;
        }

        BOOL connected = ::ConnectNamedPipe(mHandle, NULL);
        if (!connected)
        {
            DWORD error = ::GetLastError();
            // If a client connected between CreateNamedPipe and ConnectNamedPipe,
            // it's still a successful connection.
            if (error != ERROR_PIPE_CONNECTED)
            {
                LOG_INFO("ConnectNamedPipe failed GLE %d", error);
                return INVALID_HANDLE_VALUE;
            }
        }

        return mHandle;
    }

    bool Bind(const std::wstring& socketPath)
    {
        Close();
        mHandle = ::CreateNamedPipeW(
            socketPath.c_str(),
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
        // Named Pipes automatically listen once bound
        return mHandle != INVALID_HANDLE_VALUE;
    }

    bool Write(const void* buffer, size_t bufferLenBytes) const
    {
        DWORD bytesWritten = 0;
        if (!::WriteFile(mHandle, buffer, static_cast<DWORD>(bufferLenBytes), &bytesWritten, NULL) || bytesWritten != bufferLenBytes)
        {
            LOG_ERROR("WriteFile failed GLE %d", ::GetLastError());
            return false;
        }
        return true;
    }

    bool Read(void* buffer, size_t bufferLenBytes) const
    {
        DWORD totalBytesRead = 0;
        while (totalBytesRead < bufferLenBytes)
        {
            DWORD bytesRead = 0;
            BOOL result = ::ReadFile(
                mHandle,
                static_cast<char*>(buffer) + totalBytesRead,
                static_cast<DWORD>(bufferLenBytes - totalBytesRead),
                &bytesRead,
                NULL);

            if (!result || bytesRead == 0)
            {
                LOG_ERROR("ReadFile failed GLE %d", ::GetLastError());
                return false;
            }
            totalBytesRead += bytesRead;
        }
        return true;
    }

private:
    HANDLE mHandle = INVALID_HANDLE_VALUE;
};

class Win32Ipc final : public relive::IIpcInterface
{
public:
    Win32Ipc()
        : mAcceptConnections(false)
        , mAcceptConnectionsThread(nullptr)
    {
    }

    ~Win32Ipc()
    {
        Cancel();
    }

    void Listen(relive::TOnPacket fnOnPacket) final
    {
        mOnPacket = fnOnPacket;

        mAcceptConnections = true;
        mAcceptConnectionsThread = std::make_unique<std::thread>(&Win32Ipc::AcceptClientsThread, this);
    }

    void Cancel() final
    {
        mAcceptConnections = false;

        // Unblock ConnectNamedPipe if it is listening
        mListeningPipe.Close();

        if (mAcceptConnectionsThread)
        {
            if (mAcceptConnectionsThread->joinable())
            {
                LOG_INFO("Wait for thread to exit...");
                mAcceptConnectionsThread->join();
                LOG_INFO("Thread exited");
            }
            mAcceptConnectionsThread.reset();
        }
    }

    [[nodiscard]] bool Connect() final
    {
        return mClientPipe.Connect(PIPE_PATH);
    }

    void SendLevelChanged(const std::string& fileName) final
    {
        PacketHeader header;
        header.mLength = static_cast<uint32_t>(fileName.size());
        header.mType = relive::PacketTypes::LevelPathJsonChanged;

        if (mClientPipe.Write(&header, sizeof(header)))
        {
            mClientPipe.Write(fileName.data(), fileName.size());
        }
    }

private:
    void AcceptClientsThread()
    {
        while (mAcceptConnections)
        {
            if (!mListeningPipe.Bind(PIPE_PATH))
            {
                LOG_ERROR("Failed to bind named pipe");
                break;
            }

            if (mListeningPipe.Accept() == INVALID_HANDLE_VALUE)
            {
                continue;
            }

            PacketHeader header;
            if (!mListeningPipe.Read(&header, sizeof(header)))
            {
                LOG_ERROR("Read header failed");
                continue;
            }

            if (header.mMagic != 0x2A)
            {
                LOG_ERROR("Bad packet magic in header");
                continue;
            }

            std::vector<u8> payload(header.mLength);
            if (header.mLength > 0)
            {
                if (!mListeningPipe.Read(payload.data(), payload.size()))
                {
                    LOG_ERROR("Read payload failed");
                    continue;
                }
            }

            if (header.mType == relive::PacketTypes::LevelPathJsonChanged)
            {
                if (mOnPacket)
                {
                    mOnPacket(header.mType, payload);
                }
            }
            else
            {
                LOG_ERROR("Unknown packet type %d", static_cast<u8>(header.mType));
            }
        }
    }

    std::atomic<bool> mAcceptConnections{false};
    std::unique_ptr<std::thread> mAcceptConnectionsThread;
    Win32PipeRAII mListeningPipe;
    Win32PipeRAII mClientPipe;
    relive::TOnPacket mOnPacket;
};

#endif

#ifndef _WIN32
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
#endif

namespace relive
{
    std::unique_ptr<IIpcInterface> MakeIpcInterface()
    {
        #ifdef _WIN32
            return std::make_unique<Win32Ipc>();
        #else
            return std::make_unique<LinuxIpc>();
        #endif
    }
}
