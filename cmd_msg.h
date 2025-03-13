/* ctrl_msg.h
    Control message definitions
*/

#pragma once

#include "base_msg.h"

#include <stdint.h>

namespace hebi::firmware::protocol {

struct cmd_set_led_msg : public base_msg {
    static const uint8_t IND_ENABLED = 0;
    static const uint8_t IND_R = 1;
    static const uint8_t IND_G = 2;
    static const uint8_t IND_B = 3;
    static const uint8_t MSG_LEN_BYTES = 4;

    //Struct to raw data
    cmd_set_led_msg(
        uint8_t node_id, 
        uint8_t enabled,
        uint8_t R,
        uint8_t G,
        uint8_t B) :
        base_msg (node_id, MessageType::CMD_SET_LED) {
            len = MSG_LEN_BYTES;
            data8[IND_ENABLED] = enabled;
            data8[IND_R] = R;
            data8[IND_G] = G;
            data8[IND_B] = B;
    }

    //Raw data to struct
    cmd_set_led_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CMD_SET_LED, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint8_t enabled() { return data8[IND_ENABLED]; }
    uint8_t R() { return data8[IND_R]; }
    uint8_t G() { return data8[IND_G]; }
    uint8_t B() { return data8[IND_B]; }
};

struct cmd_start_data_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    cmd_start_data_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CMD_START_DATA) {
            len = MSG_LEN_BYTES;
    }
};

struct cmd_disable_output_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    cmd_disable_output_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CMD_DISABLE_OUTPUT) {
            len = MSG_LEN_BYTES;
    }
};

struct cmd_enable_output_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    cmd_enable_output_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CMD_ENABLE_OUTPUT) {
            len = MSG_LEN_BYTES;
    }
};


}