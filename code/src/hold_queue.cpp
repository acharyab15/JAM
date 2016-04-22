//
// Created by acharyab on 4/11/16.
//

#include "../include/hold_queue.h"

HoldQueue::HoldQueue(CentralQueues *queues) :
        queues_(queues), user_handler_pipe_(-1), expected_order_(DEFAULT_FIRST_ORDER) {
}

HoldQueue::~HoldQueue() {
}

void HoldQueue::AddMessageToQueue(Payload payload) {

    if (payload.GetType() == CHAT_MSG) {
        delivery_queue_.push_back(payload);
        std::sort(delivery_queue_.begin(), delivery_queue_.end());
        ProcessPayloads();
    }
}

void HoldQueue::ProcessPayloads() {

    while(delivery_queue_.size() > 0 && delivery_queue_[0].GetOrder() == expected_order_) {
        Payload payload = delivery_queue_[0];

        StreamCommunicator::SendMessage(user_handler_pipe_,
                                        payload.GetUsername(),
                                        payload.GetMessage());
        history_queue_.push(payload); //add sent payload to history queue
        delivery_queue_.erase(delivery_queue_.begin()); //remove sent message from delivery queue
        expected_order_ += 1; //increase the expected order of the next message

    }
}

bool HoldQueue::GetPayloadInHistory(int32_t value, Payload* payload) {

    if (value < history_queue_.size()) {
        payload = history_queue_.
    }
    return false;
}

void HoldQueue::SetUserHandlerPipe(int pipeId) {
    user_handler_pipe_ = pipeId;
}

int HoldQueue::GetUserHandlerPipe(){
    return user_handler_pipe_;
}



