/* Base_Node.h
    Common message parsing functions
*/

#pragma once

#include "all_msg.h"
#include "LockFreeRingBuffer.h"

namespace hebi::firmware::protocol {

class Base_Node {
public:
    Base_Node();

    virtual void update();

    bool hasTxMessage() { return tx_buffer_.length() > 0; }
    std::optional<base_msg> getTxMessage() { return tx_buffer_.take(); }
    virtual void addRxMessage(base_msg msg) { rx_buffer_.add(msg); }
    virtual void addTxMessage(base_msg msg) { tx_buffer_.add(msg); }

protected:
    bool tryParseMsg(base_msg &msg);

    virtual void recvd_ctrl_set_node_id(ctrl_set_node_id_msg msg) { (void) msg; }
    virtual void recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg msg) { (void) msg; }

    virtual void recvd_cmd_start_data(cmd_start_data_msg msg) { (void) msg; }
    virtual void recvd_cmd_set_led(cmd_set_led_msg msg) { (void) msg; }
    virtual void recvd_cmd_disable_output(cmd_disable_output_msg msg) { (void) msg; }
    virtual void recvd_cmd_enable_output(cmd_enable_output_msg msg) { (void) msg; }

    virtual void recvd_data_battery_state(battery_state_msg msg) { (void) msg; }
    
    util::LF_RingBuffer<protocol::base_msg, 5> rx_buffer_;
    util::LF_RingBuffer<protocol::base_msg, 5> tx_buffer_;
};

};