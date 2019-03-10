#pragma once
#include "FluxTypes.h"

namespace Flux
{
    enum class ESocketFamily
    {
        IPV4,
        IPV6
    };

    struct SocketDescriptor
    {
        ESocketFamily           Family;
        PlatformSocket          Socket;
    };

    struct SocketAddressDescriptor
    {
        ESocketFamily           Family;
        uint16                  Port;
        PlatformSocketAddr      Address;
    };

    struct PacketHeader
    {
    };
}

#define DEFINE_SINGLETON(Module)                                                                                                \
public:                                                                                                                         \
static  void         CreateInstance();                                                                                          \
static  Module*      Instance();                                                                                                \
static  void         DestroyInstance();                                                                                         \
private:                                                                                                                        \
    Module();                                                                                                                   \
    ~Module();                                                                                                                  \
    static  Module*     ms_instance;

#define IMPLEMENT_SINGLETON(Module)                                                                                             \
Module* Module::ms_instance = nullptr;                                                                                          \
void    Module::CreateInstance()                                                                                                \
{                                                                                                                               \
    if(!ms_instance)                                                                                                            \
    {                                                                                                                           \
        ms_instance = new Module();                                                                                             \
    }                                                                                                                           \
}                                                                                                                               \
Module* Module::Instance()                                                                                                      \
{                                                                                                                               \
    return ms_instance;                                                                                                         \
}                                                                                                                               \
void    Module::DestroyInstance()                                                                                               \
{                                                                                                                               \
    if(ms_instance)                                                                                                             \
    {                                                                                                                           \
        delete ms_instance;                                                                                                     \
        ms_instance = nullptr;                                                                                                  \
    }                                                                                                                           \
}

#define DEFINE_NET_MODULE(Module)                                                                                               \
public:                                                                                                                         \
    Bool                Initialize();                                                                                           \
    SocketDescriptor    CreateSocket(ESocketFamily family);                                                                     \
    Bool                BindSocket(SocketDescriptor const& sock, SocketAddressDescriptor const& address);                       \
    int32               SendMessage(SocketDescriptor const& sock , SocketAddressDescriptor const& address,                      \
                        const uint8* message, const int32 messageLen);                                                          \
    int32               RecvMessage(SocketDescriptor const& sock , SocketAddressDescriptor& address,                            \
                        uint8* message, const int32 messageLen);                                                                \
    void                CloseSocket(SocketDescriptor const& socket);                                                            \
    void                Shutdown();                                                                                             \
    Bool                SetNonBlocking(SocketDescriptor const& sock, Bool enable);                                              \
    uint16              HostToNetworkShort(uint16 value);                                                                       \
    uint32              HostToNetworkLong(uint32 value);                                                                        \
    uint16              NetworkToHostShort(uint16 value);                                                                       \
    uint32              NetworkToHostLong(uint32 value);                                                                        \
    Bool                CreateNetworkAddress(SocketAddressDescriptor& descriptor, const char* src);                             \
    DEFINE_SINGLETON(Module)

#define IMPLEMENT_NET_MODULE(Module)                                                                                            \
    IMPLEMENT_SINGLETON(Module)
