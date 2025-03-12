/* data_msg.h
    Data message definitions
*/

#pragma once

#include "base_msg.h"

namespace hebi::firmware::protocol {

struct battery_state_msg : public base_msg {
    static const uint8_t IND_VOLTAGE = 0;
    static const uint8_t IND_CURRENT = 1;
    static const uint8_t IND_CAP = 2;
    static const uint8_t IND_CAP_MAX = 3;
    static const uint8_t MSG_LEN_BYTES = 8;

    //Struct to raw data
    battery_state_msg(
        uint8_t node_id, 
        uint16_t voltage, 
        uint16_t current, 
        uint16_t capacity, 
        uint16_t capacity_max) :
        base_msg (node_id, MessageType::DATA_BATTERY_STATE) {
            len = MSG_LEN_BYTES;
            data16[IND_VOLTAGE] = voltage;
            data16[IND_CURRENT] = current;
            data16[IND_CAP] = capacity;
            data16[IND_CAP_MAX] = capacity_max;
    }

    //Raw data to struct
    battery_state_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::DATA_BATTERY_STATE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint16_t voltage() { return data16[IND_VOLTAGE]; }
    int16_t current() { return data16[IND_CURRENT]; }
    uint16_t capacity() { return data16[IND_CAP]; }
    uint16_t capacity_max() { return data16[IND_CAP_MAX]; }
};


}