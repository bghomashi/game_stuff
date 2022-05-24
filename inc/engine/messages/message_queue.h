#pragma once

#include "engine/utility/slot_map.h"
#include <vector>
#include <queue>
#include <memory>
#include <functional>

struct Message {
    typedef std::shared_ptr<Message> Ptr_t;

    virtual void Dispatch() = 0;
};

template <typename T>
class MessageType : public Message {
protected:
    static std::vector<std::function<void(T*)>> s_listeners;
public:

    static void RegisterListener(std::function<void(T*)> f) {
        s_listeners.push_back(f);
    }

    // dispatch this message
    void Dispatch() {
        for (auto& listener : s_listeners)
            listener(static_cast<T*>(this));
    }
};

class MessageQueue {
    static std::queue<Message::Ptr_t> s_msg_queue;
public:
    static void Push(Message* message);
    static void Push(Message::Ptr_t message);
    static void Flush();
};


template <typename T>
std::vector<std::function<void(T*)>> MessageType<T>::s_listeners;