#pragma once
#include "FluxTypes.h"

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
    static Bool                 Initialize();                                                                                   \
    static SocketDescriptor     CreateSocket(ESocketFamily family);                                                             \
    static Bool                 BindSocket(SocketDescriptor const& sock, SocketAddressDescriptor const& address);               \
    static int32                SendMessage(SocketDescriptor const& sock , SocketAddressDescriptor const& address,              \
                                const uint8* message, const int32 messageLen);                                                  \
    static int32                RecvMessage(SocketDescriptor const& sock , SocketAddressDescriptor& address,                    \
                                uint8* message, const int32 messageLen);                                                        \
    static void                 CloseSocket(SocketDescriptor const& socket);                                                    \
    static void                 Shutdown();                                                                                     \
    static Bool                 SetNonBlocking(SocketDescriptor const& sock, Bool enable);                                      \
    static uint16               HostToNetworkShort(uint16 value);                                                               \
    static uint32               HostToNetworkLong(uint32 value);                                                                \
    static uint16               NetworkToHostShort(uint16 value);                                                               \
    static uint32               NetworkToHostLong(uint32 value);                                                                \
    static Bool                 CreateNetworkAddress(SocketDescriptor const& sock,                                              \
                                SocketAddressDescriptor& descriptor, const char* src);
