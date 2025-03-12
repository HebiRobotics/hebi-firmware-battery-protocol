/* parser.h
    Common message parsing functions
*/

#pragma once

#include "all_msg.h"
#include "LockFreeRingBuffer.h"

namespace hebi::firmware::protocol {

class Message_Parser {
public:
    Message_Parser();

    virtual void update();

    std::optional<base_msg> getTxMessage() { return tx_buffer_.take(); }
    void addRxMessage(base_msg msg) { rx_buffer_.add(msg); }
    void addTxMessage(base_msg msg) { tx_buffer_.add(msg); }

protected:
    bool tryParseMsg(base_msg &msg);

    virtual void recvd_data_battery_state(battery_state_msg msg) {}
    
    util::LF_RingBuffer<protocol::base_msg, 5> rx_buffer_;
    util::LF_RingBuffer<protocol::base_msg, 5> tx_buffer_;
};

};