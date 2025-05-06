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


struct ctrl_reset_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    ctrl_reset_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CTRL_RESET) {
            len = MSG_LEN_BYTES;
    }
};

struct ctrl_set_stay_in_boot_msg : public base_msg {
    static const uint8_t IND_STAY_IN_BOOT = 0;
    static const uint8_t MSG_LEN_BYTES = 1;

    //Struct to raw data
    ctrl_set_stay_in_boot_msg(
        uint8_t node_id, 
        bool stay_in_boot) :
        base_msg (node_id, MessageType::CTRL_SET_STAY_IN_BOOT) {
            len = MSG_LEN_BYTES;
            data8[IND_STAY_IN_BOOT] = stay_in_boot;
    }

    //Raw data to struct
    ctrl_set_stay_in_boot_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_SET_STAY_IN_BOOT, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    bool stay_in_bootloader() { return data8[IND_STAY_IN_BOOT]; }
};

struct ctrl_boot_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    ctrl_boot_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CTRL_BOOT) {
            len = MSG_LEN_BYTES;
    }
};

struct ctrl_poll_node_id_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    ctrl_poll_node_id_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CTRL_POLL_NODE_ID) {
            len = MSG_LEN_BYTES;
    }
};

struct ctrl_start_acquisition_msg : public base_msg {
    static const uint8_t IND_CLEAR_ID = 0;
    static const uint8_t MSG_LEN_BYTES = 1;

    //Struct to raw data
    ctrl_start_acquisition_msg(
        uint8_t node_id, 
        bool should_clear_id) :
        base_msg (node_id, MessageType::CTRL_START_ACQUISITION) {
            len = MSG_LEN_BYTES;
            data8[IND_CLEAR_ID] = should_clear_id;
    }

    //Raw data to struct
    ctrl_start_acquisition_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_START_ACQUISITION, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    /* This flag indicates whether a node that already has a valid
    node id should clear it upon recipt of this message. */
    bool should_clear_id() { return data8[IND_CLEAR_ID]; }
};

struct ctrl_stop_acquisition_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data
    ctrl_stop_acquisition_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CTRL_STOP_ACQUISITION) {
            len = MSG_LEN_BYTES;
    }
};

} //hebi::firmware::protocol