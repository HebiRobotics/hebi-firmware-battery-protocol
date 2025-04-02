/* Base_Node.h
    Common message parsing functions
*/

#pragma once

#include "all_msg.h"
#include "LockFreeRingBuffer.h"

namespace hebi::firmware::protocol {

class Base_Node {
public:
    Base_Node() { /* Do Nothing */ }

protected:
    bool tryParseMsg(base_msg &msg);

    virtual void recvd_ctrl_set_node_id(ctrl_set_node_id_msg msg) { (void) msg; }
    virtual void recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg msg) { (void) msg; }
    virtual void recvd_ctrl_start_acquisition(ctrl_start_acquisition_msg msg) { (void) msg; }
    virtual void recvd_ctrl_stop_acquisition(ctrl_stop_acquisition_msg msg) { (void) msg; }

    virtual void recvd_cmd_start_data(cmd_start_data_msg msg) { (void) msg; }
    virtual void recvd_cmd_set_led(cmd_set_led_msg msg) { (void) msg; }
    virtual void recvd_cmd_disable_output(cmd_disable_output_msg msg) { (void) msg; }
    virtual void recvd_cmd_enable_output(cmd_enable_output_msg msg) { (void) msg; }

    virtual void recvd_data_battery_state(battery_state_msg msg) { (void) msg; }
};

};