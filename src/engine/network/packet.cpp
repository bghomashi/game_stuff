#include "engine/network/network.h"

namespace Net {
    void Packet::Append(const message& msg) {
        int i = data.size(); 
        data.resize(data.size() + msg.size());
        std::memcpy(data.data()+i, &(msg.header), sizeof(msg.header));
        std::memcpy(data.data()+i+sizeof(msg.header), msg.body.data(), msg.body.size());
    }
}