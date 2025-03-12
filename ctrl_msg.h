/* ctrl_msg.h
    Control message definitions
*/

#pragma once

#include "base_msg.h"

#include <stdint.h>

namespace hebi::firmware::protocol {

struct ctrl_set_node_id_msg : public base_msg {
    static const uint8_t IND_NODE_ID = 0;
    static const uint8_t MSG_LEN_BYTES = 1;

    //Struct to raw data
    ctrl_set_node_id_msg(
        uint8_t node_id, 
        uint8_t new_node_id) :
        base_msg (node_id, MessageType::CTRL_SET_NODE_ID) {
            len = MSG_LEN_BYTES;
            data8[IND_NODE_ID] = new_node_id;
    }

    //Raw data to struct
    ctrl_set_node_id_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_SET_NODE_ID, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint8_t new_node_id() { return data8[IND_NODE_ID]; }
};


}