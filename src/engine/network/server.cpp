#include "engine/network/network.h"

namespace Net {
    bool Server::Start(int port) {
        return _connection.Open(port);
    }
    void Server::Stop() {
        _connection.Close();
    }
    void Server::Update() {
        std::vector<Packet::Ptr_t> packets; 
        for (auto& element : _clients) {
            auto& uuid = element.first;
            auto& cc = element.second;

            Packet::Ptr_t packet;
            if (!cc.message_queue.empty()) {
                packet.reset(new Packet);
                packet->addr = cc.addr;
                packets.push_back(packet);
            }

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
        for (auto& p : packets)
            _connection.SendTo(*p);
    }

    void Server::Send(const message& msg, const ClientID& uuid) {
        auto it = _clients.find(uuid);
        if (it == _clients.end()) return;

        it->second.message_queue.push(msg);
    }
    bool Server::Recv(std::vector<message>& messages, ClientID& uuid) {
        Packet packet;
        if (!_connection.RecvFrom(packet, false))
            return false;

        uuid = ClientID::INVALID;
        for (auto& ele : _clients)
            if (ele.second.addr == packet.addr)
                uuid = ele.first;

        if (uuid == ClientID::INVALID) {             // add client
            uuid = ClientID::get();
            _clients.insert(std::make_pair(uuid,ClientConnection{packet.addr}));
        }

        message msg;
        std::uint8_t* byte = packet.data.data();

        while (byte - packet.data.data() < packet.data.size()) {
            std::memcpy(&msg.header, byte, sizeof(msg.header)); byte += sizeof(msg.header);
            msg.body.resize(msg.header.size - sizeof(msg.header));
            std::memcpy(msg.body.data(), byte, msg.body.size());  byte += msg.header.size;

            messages.push_back(msg);
        }
        return true;
    }
    void Server::RemoveClientConnection(const ClientID& uuid) {
        _clients.erase(uuid);
    }
}