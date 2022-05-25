#include "engine/network/network.h"
#include "engine/utility/logger.h"
#include <cstdint>

#ifndef _WIN32
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  
#else
#include <WinSock2.h>
#include <WS2tcpip.h>
typedef int socklen_t;
#endif


namespace Net {
    static std::uint32_t hostname_to_ip(const char* hostname) {
        struct addrinfo *ai;
        struct addrinfo hints;
 
        memset(&hints, 0, sizeof(hints));
        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = 0;
 
        int ret = getaddrinfo(hostname, NULL, &hints, &ai);
        if (ret != 0)
            return 0;       // error

        auto result = ((sockaddr_in*)(ai->ai_addr))->sin_addr.s_addr;               // save address
        LOG_CRITICAL(inet_ntoa(((sockaddr_in*)(ai->ai_addr))->sin_addr));

        freeaddrinfo(ai);       // free whatever was alloced

        return result;
    }
    Address MakeAddress(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d, uint16_t port) {
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
    Address MakeAddress(const std::string& url, uint16_t port) {
        Address address;
        address.addr = hostname_to_ip(url.c_str());
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