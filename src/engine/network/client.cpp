#include "engine/network/network.h"

namespace Net {
    bool Client::Start(const std::string& hostname, uint16_t port) {
        if (!_connection.Open())
            return false;
        _addr = MakeAddress(hostname, port);
        return true;
    }
    bool Client::Start(Address addr) {
        _addr = addr;
        return _connection.Open();
    }
    void Client::Stop() {
        _connection.Close();
    }
    void Client::Update() {
        std::vector<Packet::Ptr_t> packets;

        Packet::Ptr_t packet;
        if (!_outgoing_messages.empty()) {
            packet.reset(new Packet);
            packet->addr = _addr;
            packets.push_back(packet);
        }

        // lock the message queue at this point (if threaded)
        while (!_outgoing_messages.empty()) {
            auto& msg = _outgoing_messages.front();
            // if the message can't fit make a new packet
            if (packet->data.size() + msg.size() > Packet::MAX_SIZE) {
                packet.reset(new Packet());
                packet->addr = _addr;
                packets.push_back(packet);
            }
            // now append the message (and pop it off the queue)
            packet->Append(msg);
            _outgoing_messages.pop();
        }
        // we could unlock the message queue now if this were threaded

        for (auto& p : packets)
            _connection.SendTo(*p);

        Recv();
    }
    void Client::Send(const message& msg) {
        _outgoing_messages.push(msg);
    }
    bool Client::Recv() {
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
            std::memcpy(msg.body.data(), byte, msg.body.size());  byte += msg.body.size();

            _incoming_messages.push(msg);
        }
        return true;
    }
    bool Client::messages_waiting() const {
        return !_incoming_messages.empty();
    }
    message Client::pop_incoming_message() {
        message msg = _incoming_messages.front();
        _incoming_messages.pop();
        return msg;
    }
}