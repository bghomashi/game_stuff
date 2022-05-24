#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <cstring>
#include <vector>
#include <chrono>
#include <queue>
#include <unordered_map>
#include "engine/utility/UUID.h"

namespace Net {

    struct message_header {
        unsigned id;            // message id
        std::uint32_t size;     // size of message
    };

    
    struct message {
        message_header header;
        std::vector<std::uint8_t> body;

        void reserve(const int size);
        size_t size() const;
    };

    // we can try this as a serializer
    template <typename DT>
    message& operator<< (message& msg, const DT& data) {
        static_assert(std::is_standard_layout<DT>::value, "Data must be POD.");
    
        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DT));
        std::memcpy(msg.body.data()+i,&data, sizeof(DT));
        msg.header.size = msg.size();
        return msg;
    }
    message& operator>> (message& msg, std::string& data);

    template <typename DT>
    message& operator>> (message& msg, DT& data) {
        static_assert(std::is_standard_layout<DT>::value, "Data must be POD.");
    
        size_t i = msg.body.size() - sizeof(DT);
        std::memcpy(&data, msg.body.data()+i, sizeof(DT));
        msg.body.resize(i);
        msg.header.size = msg.size();
        return msg;
    }
    
    std::ostream& operator<< (std::ostream& os, const message& msg);



    struct Address {
        uint32_t addr;
        uint16_t port;
    };

    struct Packet {
        typedef std::shared_ptr<Packet> Ptr_t;
        static const unsigned MAX_SIZE;

        Address addr;
        std::vector<std::uint8_t> data;
    
        
        void Append(const message& msg);
    };

    struct Connection {
        typedef std::chrono::steady_clock Clock_t;
        
        int sock;

        bool Open();
        bool Open(int port);
        void Close();
        
        bool SendTo(const Packet& packet);
        bool RecvFrom(Packet& p, bool block);
        void Update();
    };

    Address MakeAddress(char a, char b, char c, char d, uint16_t port);
    Address MakeAddress(uint32_t address, uint16_t port);
    bool operator==(const Address& a, const Address& b);
    bool operator!=(const Address& a, const Address& b);





    typedef UUID ClientID;

    class Client {
        Address _addr;
        Connection _connection;
        std::queue<message> _message_queue;
    public:
        bool Start(Address addr);
        void Stop();
        void Update();
        void Send(const message& msg);
        bool Recv(std::vector<message>& messages);
    };

    
    class Server {
        struct ClientConnection {
            Address addr;
            std::queue<message> message_queue;
        };

        Connection _connection;

        std::unordered_map<ClientID, ClientConnection, ClientID::hash_fn> _clients;
        

    public:
        void RemoveClientConnection(const ClientID& uuid);

        bool Start(int port);
        void Stop();
        void Update();

        void Send(const message& message, const ClientID& addr);
        bool Recv(std::vector<message>& messages, ClientID& addr);
    };
    

};







