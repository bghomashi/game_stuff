#include "engine/network/network.h"

namespace Net {
    bool Client::Start(Address addr) {
        _addr = addr;
        return _connection.Open();
    }
    void Client::Stop() {
        _connection.Close();
    }
    void Client::Update() {
        Packet::Ptr_t packet(new Packet);
        std::vector<Packet::Ptr_t> packets;
        packet->addr = _addr;
        packets.push_back(packet);

        // lock the message queue at this point (if threaded)
        while (!_message_queue.empty()) {
            auto& msg = _message_queue.front();
            // if the message can't fit make a new packet
            if (packet->data.size() + msg.size() > Packet::MAX_SIZE) {
                packet.reset(new Packet());
                packet->addr = _addr;
                packets.push_back(packet);
            }
            // now append the message (and pop it off the queue)
            packet->Append(msg);
            _message_queue.pop();
        }
        // we coudl unlock the message queue now if this were threaded

        for (auto& p : packets)
            _connection.SendTo(*p);


    }
    void Client::Send(const message& msg) {
        _message_queue.push(msg);
    }
    bool Client::Recv(std::vector<message>& messages) {
        Packet packet;
        if (!_connection.RecvFrom(packet, false))
            return false;

        if (_addr != packet.addr)        
            return false;

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
}