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
typedef int socklen_t;
#endif

namespace Net {
    static std::uint32_t hostname_to_ip(const char* hostname)
    {
        struct hostent *he;
        struct in_addr **addr_list;
        int i;

        if ( (he = gethostbyname( hostname ) ) == NULL) {
            // get the host info
            herror("gethostbyname");
            return 1;
        }

        addr_list = (struct in_addr **) he->h_addr_list;

        for(i = 0; addr_list[i] != NULL; i++) {
            // LOG_CRITICAL(inet_ntoa(*addr_list[i]));
            return (std::uint32_t)(addr_list[i]->s_addr);
        }

        return 0;
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