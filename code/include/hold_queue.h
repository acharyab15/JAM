//
// Created by acharyab on 4/11/16.
//

#ifndef JAM_HOLD_QUEUE_H
#define JAM_HOLD_QUEUE_H

#include <queue>
#include "payload.h"
#include "stream_communicator.h"

class HoldQueue {

public:                    // begin public section
    HoldQueue();
    ~HoldQueue();

    void AddMessageToQueue(Payload payload);
    void Process(Payload payload);

    void GetPayloadInHistory(int32_t value);

private:
    std::queue<Payload> history_queue_;
    std::vector<Payload> delivery_queue_;


    int user_handler_pipe_;

};

#endif //JAM_HOLD_QUEUE_H
