/* base_msg.h
    Data message definitions
*/

#pragma once

#include "base_msg.h"

namespace hebi::firmware::protocol {

struct battery_state_msg : public base_msg {
    static const uint8_t IND_VOLTAGE = 0;
    static const uint8_t IND_CURRENT = 1;
    static const uint8_t IND_SOC = 2;
    static const uint8_t IND_SOC_MAX = 3;
    static const uint8_t MSG_LEN_BYTES = 8;

    //Struct to raw data
    battery_state_msg(
        uint8_t node_id, 
        uint16_t voltage, 
        uint16_t current, 
        uint16_t soc, 
        uint16_t soc_max) :
        base_msg (node_id, MessageType::DATA_BATTERY_STATE) {
            len = MSG_LEN_BYTES;
            data16[IND_VOLTAGE] = voltage;
            data16[IND_CURRENT] = current;
            data16[IND_SOC] = soc;
            data16[IND_SOC_MAX] = soc_max;
    }

    //Raw data to struct
    battery_state_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::DATA_BATTERY_STATE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint16_t voltage() { return data16[IND_VOLTAGE]; }
    uint16_t current() { return data16[IND_CURRENT]; }
    uint16_t soc() { return data16[IND_SOC]; }
    uint16_t soc_max() { return data16[IND_SOC_MAX]; }
};


}