#pragma once

#include <cstddef>
#include <memory>
#include <string>

class IConnection
{
public:
    virtual ~IConnection() = default;

    virtual bool Read(void* buffer, size_t len) = 0;
    virtual bool Write(const void* buffer, size_t len) = 0;
};

class IServer
{
public:
    virtual ~IServer() = default;

    // Call before WaitForClient once
    virtual void Listen() = 0;

    // Blocks until a client connects, returns a connected endpoint
    virtual std::unique_ptr<IConnection> WaitForClient() = 0;
};

class IClient
{
public:
    virtual ~IClient() = default;

    // Connect to endpoint and return a connected endpoint, doesn't block
    virtual std::unique_ptr<IConnection> Connect() = 0;
};
