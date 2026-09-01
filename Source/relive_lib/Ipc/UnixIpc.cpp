#ifndef _WIN32

#include "Ipc.hpp"
#include "IpcTransportInterfaces.hpp"
#include "../logger.hpp"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <cstring>

static constexpr const char* kSocketPath = "/tmp/relive_ipc.sock";

class UnixConnection final : public IConnection
{
public:
    explicit UnixConnection(int fd)
        : mSocket(fd)
    {
    }

    ~UnixConnection() override
    {
        Close();
    }

    bool Read(void* buffer, size_t len) override
    {
        size_t totalBytesRead = 0;
        while (totalBytesRead < len)
        {
            ssize_t readBytes = ::read(mSocket,
                                       static_cast<u8*>(buffer) + totalBytesRead,
                                       len - totalBytesRead);
            if (readBytes <= 0)
            {
                LOG_ERROR("Read failed %d", errno);
                return false;
            }
            totalBytesRead += static_cast<size_t>(readBytes);
        }
        return true;
    }

    bool Write(const void* buffer, size_t len) override
    {
        ssize_t written = ::write(mSocket, buffer, len);
        if (written < 0 || static_cast<size_t>(written) != len)
        {
            LOG_ERROR("Write failed %d", errno);
            return false;
        }
        return true;
    }

private:
    void Close()
    {
        if (mSocket > 0)
        {
            ::close(mSocket);
            mSocket = 0;
        }
    }

    int mSocket = 0;
};

class UnixServer final : public IServer
{
public:
    explicit UnixServer(const std::string& serverPath)
     : mServerPath(serverPath)
    {
        mSocket = ::socket(AF_UNIX, SOCK_STREAM, 0);
        if (mSocket < 0)
        {
            LOG_ERROR("socket failed %d", errno);
            return;
        }
    }

    ~UnixServer() override
    {
        Close();
    }

    void Listen() override
    {
        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        std::strcpy(addr.sun_path, mServerPath.c_str());

        if (::bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        {
            if (errno == EADDRINUSE)
            {
                if (::unlink(mServerPath.c_str()) < 0 && errno != ENOENT)
                {
                    LOG_ERROR("Delete of %s failed with %d", mServerPath.c_str(), errno);
                    return;
                }

                if (::bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
                {
                    LOG_ERROR("Socket bind attempt 2 failed %d", errno);
                    return;
                }
            }
            else
            {
                LOG_ERROR("Socket bind failed %d", errno);
                return;
            }
        }

        if (::listen(mSocket, 1) < 0)
        {
            LOG_ERROR("Socket listen failed %d", errno);
            return;
        }
        LOG_INFO("IPC server listening %s", kSocketPath);        
    }

    std::unique_ptr<IConnection> WaitForClient() override
    {
        int clientFd = ::accept(mSocket, nullptr, nullptr);
        if (clientFd < 0)
        {
            LOG_ERROR("accept failed %d", errno);
            return nullptr;
        }
        LOG_INFO("New IPC client connected");
        return std::make_unique<UnixConnection>(clientFd);
    }

private:
    void Close()
    {
        if (mSocket > 0)
        {
            ::shutdown(mSocket, SHUT_RDWR);
            ::close(mSocket);
            mSocket = 0;
        }
    }

    std::string mServerPath;
    int mSocket = 0;
};

class UnixClient final : public IClient
{
public:
    explicit UnixClient(const std::string& serverPath)
     : mServerPath(serverPath)
    {
        mSocket = ::socket(AF_UNIX, SOCK_STREAM, 0);
        if (mSocket < 0)
        {
            LOG_ERROR("socket failed %d", errno);
        }
    }

    ~UnixClient() override
    {
        Close();
    }

    std::unique_ptr<IConnection> Connect() override
    {
        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        std::strcpy(addr.sun_path, mServerPath.c_str());

        if (::connect(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
        {
            LOG_ERROR("Connect to %s failed %d", mServerPath.c_str(), errno);
            return nullptr;
        }
        LOG_INFO("Connected to %s", mServerPath.c_str());
        return std::make_unique<UnixConnection>(mSocket);
    }

private:
    void Close()
    {
        if (mSocket > 0)
        {
            ::close(mSocket);
            mSocket = 0;
        }
    }

    int mSocket = 0;
    std::string mServerPath;
};

std::unique_ptr<IServer> MakeIpcServer()
{
    return std::make_unique<UnixServer>(kSocketPath);
}

std::unique_ptr<IClient> MakeIpcClient()
{
    return std::make_unique<UnixClient>(kSocketPath);
}

#endif // !_WIN32
