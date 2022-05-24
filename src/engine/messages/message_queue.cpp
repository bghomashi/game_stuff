#include "engine/messages/message_queue.h"

std::queue<Message::Ptr_t> MessageQueue::s_msg_queue;

void MessageQueue::Push(Message* message) {
    s_msg_queue.push(Message::Ptr_t(message));
}
void MessageQueue::Push(Message::Ptr_t message) {
    s_msg_queue.push(message);
}
void MessageQueue::Flush() {
    int length = s_msg_queue.size();               

    // any message generated during dispath are 
    // held till the next frame
    for (int i = 0; i < length; i++) {
        s_msg_queue.front()->Dispatch();
        s_msg_queue.pop();
    }
}