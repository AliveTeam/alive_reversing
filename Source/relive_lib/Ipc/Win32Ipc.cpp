#ifdef _WIN32

#include "Ipc.hpp"
#include "IpcTransportInterfaces.hpp"
#include "../logger.hpp"

#include <windows.h>

static constexpr const char* kPipePathStr = "\\\\.\\pipe\\relive_pipe";

static std::wstring Utf8ToWide(const std::string& s)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
    std::wstring ws(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), len);
    return ws;
}

class Win32Connection final : public IConnection
{
public:
    explicit Win32Connection(HANDLE h)
        : mHandle(h)
    {
    }

    ~Win32Connection() override
    {
        Close();
    }

    bool Read(void* buffer, size_t len) override
    {
        DWORD totalBytesRead = 0;
        while (totalBytesRead < len)
        {
            DWORD bytesRead = 0;
            BOOL result = ::ReadFile(
                mHandle,
                static_cast<char*>(buffer) + totalBytesRead,
                static_cast<DWORD>(len - totalBytesRead),
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

    bool Write(const void* buffer, size_t len) override
    {
        DWORD bytesWritten = 0;
        if (!::WriteFile(mHandle, buffer, static_cast<DWORD>(len), &bytesWritten, NULL) || bytesWritten != len)
        {
            LOG_ERROR("WriteFile failed GLE %d", ::GetLastError());
            return false;
        }
        return true;
    }

private:
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

    HANDLE mHandle = INVALID_HANDLE_VALUE;
};

class Win32Server final : public IServer
{
public:
    explicit Win32Client(const std::string& serverPath)
    {
        mServerPath = Utf8ToWide(serverPath);
    }

    void Listen() override
    {
        // Do nothing WaitForClient does the listen in Win32
    }

    std::unique_ptr<IConnection> WaitForClient() override
    {
        HANDLE hPipe = ::CreateNamedPipeW(
            mServerPath.c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            16,
            4096, 4096,
            0,
            NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            LOG_ERROR("CreateNamedPipeW failed GLE %d", ::GetLastError());
            return nullptr;
        }

        BOOL connected = ::ConnectNamedPipe(hPipe, NULL);
        if (!connected)
        {
            DWORD err = ::GetLastError();
            if (err != ERROR_PIPE_CONNECTED)
            {
                LOG_ERROR("ConnectNamedPipe failed GLE %d", err);
                ::CloseHandle(hPipe);
                return nullptr;
            }
        }
        return std::make_unique<Win32Connection>(hPipe);
    }
private:
    std::wstring mServerPath;    
};

class Win32Client final : public IClient
{
public:
    explicit Win32Client(const std::string& serverPath)
    {
        mServerPath = Utf8ToWide(serverPath);
    }

    std::unique_ptr<IConnection> Connect() override
    {
        HANDLE h = ::CreateFileW(
            mServerPath.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

        if (h == INVALID_HANDLE_VALUE)
        {
            LOG_ERROR("CreateFileW failed GLE %d", ::GetLastError());
            return nullptr;
        }

        return std::make_unique<Win32Connection>(h);
    }
private:
    std::wstring mServerPath;
};

std::unique_ptr<IServer> MakeIpcServer()
{
    return std::make_unique<Win32Server>(kPipePathStr);
}

std::unique_ptr<IClient> MakeIpcClient()
{
    return std::make_unique<Win32Client>(kPipePathStr);
}



#endif // _WIN32
