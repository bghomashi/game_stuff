#include "engine/network/network.h"

#define CLIENT_TIMEOUT  5.f                         // 5 seconds

namespace Net {
    bool Server::Start(int port) {
        bytes_sent = 0;
        bytes_recv = 0;
        return _connection.Open(port);
    }
    void Server::Stop() {
        _connection.Close();
    }
    void Server::Flush(ClientID client_id) {
        std::vector<Packet::Ptr_t> packets; 
        Packet::Ptr_t packet;
        auto cc = _clients[client_id];
        if (!cc.message_queue.empty()) {
            packet.reset(new Packet);
            packet->addr = cc.addr;
            packets.push_back(packet);
        }
        // loop through all the messages in the queue and append to packet
        while (!cc.message_queue.empty()) {
            auto& msg = cc.message_queue.front();
            // if the message can't fit make a new packet
            if (packet->data.size() + msg.size() > Packet::MAX_SIZE) {
                packet.reset(new Packet());
                packet->addr = cc.addr;
                packets.push_back(packet);
            }
            // now append the message (and pop it off the queue)
            packet->Append(msg);
            cc.message_queue.pop();
        }
        // ship all the packets
        for (auto& p : packets) {
            _connection.SendTo(*p);
            bytes_sent += p->data.size();
        }
    }
    void Server::Update(float dt) {
        std::vector<Packet::Ptr_t> packets; 
        // check all the timeouts and disconnect
        for (auto it = _clients.begin(); it != _clients.end(); ) {
            if (it->second.timeout >= CLIENT_TIMEOUT) {
                if (client_timeout_callback)
                    client_timeout_callback(it->first);
                it = _clients.erase(it);
            } else {
                it++;
            }
        }
        // for each of the clients
        for (auto& element : _clients) {
            auto& uuid = element.first;
            auto& cc = element.second;
            // tick the timeout counter
            cc.timeout += dt;

            // create a packet from this client
            Packet::Ptr_t packet;
            if (!cc.message_queue.empty()) {
                packet.reset(new Packet);
                packet->addr = cc.addr;
                packets.push_back(packet);
            }
            // loop through all the messages in the queue and append to packet
            while (!cc.message_queue.empty()) {
                auto& msg = cc.message_queue.front();
                // if the message can't fit make a new packet
                if (packet->data.size() + msg.size() > Packet::MAX_SIZE) {
                    packet.reset(new Packet());
                    packet->addr = cc.addr;
                    packets.push_back(packet);
                }
                // now append the message (and pop it off the queue)
                packet->Append(msg);
                cc.message_queue.pop();
            }
        }
        // ship all the packets
        for (auto& p : packets) {
            _connection.SendTo(*p);
            bytes_sent += p->data.size();
        }
    }

    void Server::Send(const message& msg, const ClientID& uuid) {
        auto it = _clients.find(uuid);
        if (it == _clients.end()) return;

        it->second.message_queue.push(msg);
    }
    bool Server::Recv(std::vector<message>& messages, ClientID& uuid) {
        /// recv the packet
        Packet packet;
        if (!_connection.RecvFrom(packet, false))
            return false;
        bytes_recv += packet.data.size();

        // find this client if they exist!
        uuid = ClientID::INVALID;
        for (auto& ele : _clients) {
            if (ele.second.addr == packet.addr) {
                uuid = ele.first;
                ele.second.timeout = 0.f;           // reset timer
                break;
            }
        }

        if (uuid == ClientID::INVALID) {             // add client
            uuid = ClientID::get();
            _clients.insert(std::make_pair(uuid,ClientConnection{packet.addr}));
            _clients[uuid].timeout = 0.f;
        }

        message msg;
        std::uint8_t* byte = packet.data.data();

        while (byte - packet.data.data() < packet.data.size()) {
            std::memcpy(&msg.header, byte, sizeof(msg.header)); byte += sizeof(msg.header);
            msg.body.resize(msg.header.size - sizeof(msg.header));
            std::memcpy(msg.body.data(), byte, msg.body.size());  byte += msg.body.size();

            messages.push_back(msg);
        }
        return true;
    }
    void Server::RemoveClientConnection(const ClientID& uuid) {
        _clients.erase(uuid);
    }
}