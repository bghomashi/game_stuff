#include "engine/network/network.h"

namespace Net {
    void message::reserve(const int size) {
        body.reserve(size);
    }
    size_t message::size() const {
        return sizeof(message_header) + body.size();
    }

    std::ostream& operator<< (std::ostream& os, const message& msg) {
        os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
        return os;
    }


    message& operator<< (message& msg, const char* data) {
        size_t i = msg.body.size();
        size_t len = strlen(data);
        msg.body.resize(msg.body.size() + len + 1);
        std::memcpy(msg.body.data()+i, data, len);
        msg.body[i+len] = len;
        msg.header.size = msg.size();
        return msg;
    }
    message& operator<< (message& msg, const std::string& data) {
        size_t i = msg.body.size();
        msg.body.resize(msg.body.size() + data.length() + 1);
        std::memcpy(msg.body.data()+i, data.c_str(), data.length());
        msg.body[i+data.length()] = data.length();
        msg.header.size = msg.size();
        return msg;
    }
    message& operator>> (message& msg, std::string& data) {
        int size = msg.body.size();
        std::uint8_t len = msg.body[size-1];
        size--;

        data.resize(len);
        for (int i = 0; i < len; i++)
            data[len-1-i] = msg.body[size-1-i];
        size -= len;
        
        msg.body.resize(size);
        msg.header.size = msg.size();
        return msg;
    }
}