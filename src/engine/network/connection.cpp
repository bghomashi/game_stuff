#include "engine/network/network.h"
#include "engine/utility/logger.h"
#include <iostream>
#include <algorithm>
#include <string.h> 

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
    bool Connection::Open(int port) {
    #ifdef _WIN32
        int iResult;
        WSADATA wsadata;
        if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0) {
            LOG_CRITICAL("WSAStartup failed: " + std::to_string(iResult));
            return false;
        }
    #endif

        sockaddr_in address = {};
	    memset(&address, 0, sizeof(address));
        // Filling server information 
        address.sin_family      = AF_INET; // IPv4 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port        = htons(port); 

        // create socket
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
            LOG_CRITICAL("socket creation failed"); 
            return false;
        } 
        // Bind the socket with the server address 
        if (bind(sock, (const struct sockaddr*)&address, sizeof(address)) < 0) { 
            LOG_CRITICAL("bind failed"); 
            return false;
        } 
        return true;
    }
    bool Connection::Open() {
    #ifdef _WIN32
        int iResult;
        WSADATA wsadata;
        if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsadata)) != 0) {
            LOG_CRITICAL("WSAStartup failed: " + std::to_string(iResult));
            return false;
        }
    #endif


        sockaddr_in address = {};
	    memset(&address, 0, sizeof(address));
        // Filling client information 
        address.sin_family      = AF_INET; // IPv4 
        address.sin_addr.s_addr = INADDR_ANY; 
        address.sin_port        = htons(0);         // we dont care about port

        // create socket
        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
            LOG_CRITICAL("socket creation failed"); 
            return false;
        } 
        // Bind the socket with the client address 
        if (bind(sock, (const struct sockaddr*)&address, sizeof(address)) < 0) { 
            LOG_CRITICAL("bind failed"); 
            return false;
        } 
        return true;
    }
        
    void Connection::Close() {
    #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
    #else
        close(sock);
    #endif
    }
    bool Connection::SendTo(const Packet& packet) {
        sockaddr_in address = {};
 
        address.sin_family      = AF_INET; // IPv4 
        address.sin_addr.s_addr = packet.addr.addr; 
        address.sin_port        = packet.addr.port; 

        return sendto(sock, 
                &packet.data[0], packet.data.size(), 
                0, (sockaddr*)&address, sizeof(address)) >= 0;
    }
    bool Connection::RecvFrom(Packet& p, bool block) {
        sockaddr_in address;
        char buffer[Packet::MAX_SIZE];
        int flags = 0, len = sizeof(address);

    #ifdef WIN32
        u_long iMode = 0;
        if (!block) {
            int iResults = ioctlsocket(sock, FIONBIO, &iMode);
            if (iResults != NO_ERROR)
                LOG_CRITICAL("ioctlsocket failed with error: " + std::to_string(iResults));
        }
    #else
        flags |= (block ? 0 : MSG_DONTWAIT);
    #endif

        memset(&address, 0, sizeof(address)); 
        int n = recvfrom(sock, 
                    (char *)buffer, Packet::MAX_SIZE,  
                    flags, 
                    (sockaddr*)&address, (socklen_t*)&len); 

        if (n > 0) {                // packet recv'ed
            p.addr.port = address.sin_port;
            p.addr.addr = address.sin_addr.s_addr;
            p.data.resize(n);
            std::copy(buffer, buffer + n, p.data.begin());
            
            return true;
        }

        return false;
    }


    
    const unsigned Packet::MAX_SIZE = 1024;
}

