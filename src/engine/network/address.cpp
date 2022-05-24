#include "engine/network/network.h"

#ifndef _WIN32
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
#else
#include <WinSock2.h>
typedef int socklen_t;
#endif

namespace Net {

    Address MakeAddress(char a, char b, char c, char d, uint16_t port) {
        Address address;
        address.addr = htonl(( a << 24 ) | 
                        ( b << 16 ) | 
                        ( c << 8  ) | 
                        d);
        address.port = htons(port);
        return address;
    }
    Address MakeAddress(uint32_t addr, uint16_t port) {
        Address address;
        address.addr = htonl(addr);
        address.port = htons(port);
        return address;
    }

    bool operator==(const Address& a, const Address& b) {
        return (a.addr == b.addr && a.port == b.port);
    }
    bool operator!=(const Address& a, const Address& b) {
        return (a.addr != b.addr || a.port != b.port);
    }


};