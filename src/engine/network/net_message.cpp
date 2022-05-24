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
    message& operator>> (message& msg, std::string& data) {
        int size = msg.body.size();
        while (msg.body[size-1] == '\0') size--;            // eliminate any trailing '\0'
        int start = size-1;
        while (start > 0 && msg.body[start] != '\0') start--;            // go back until we hit another string or the end
        
        for (int i = start + (msg.body[start] == '\0' ? 1 : 0); i < size; i++)
            data += msg.body[i];
        
        msg.body.resize(start);
        msg.header.size = msg.size();
        return msg;
    }
}