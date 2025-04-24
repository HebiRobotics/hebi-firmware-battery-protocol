/* data_msg.h
    Data message definitions
*/

#pragma once

#include "base_msg.h"

namespace hebi::firmware::protocol {

struct battery_state_msg : public base_msg {
    static const uint8_t BATTERY_CONNECTED_FLAG = 0x1;

    static const uint8_t IND_STATE_8 = 0;
    static const uint8_t IND_SOC_8 = 1;
    static const uint8_t IND_VOLTAGE_16 = 1;
    static const uint8_t IND_CURRENT_16 = 2;
    static const uint8_t IND_TEMP_16 = 3;
    static const uint8_t MSG_LEN_BYTES = 8;

    //Struct to raw data
    battery_state_msg(
        uint8_t node_id, 
        uint8_t state,
        uint8_t soc,
        uint16_t voltage, 
        uint16_t current, 
        uint16_t temperature) :
        base_msg (node_id, MessageType::DATA_BATTERY_STATE) {
            len = MSG_LEN_BYTES;
            data8[IND_STATE_8] = state;
            data8[IND_SOC_8] = soc;
            data16[IND_VOLTAGE_16] = voltage;
            data16[IND_CURRENT_16] = current;
            data16[IND_TEMP_16] = temperature;
    }

    //Raw data to struct
    battery_state_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::DATA_BATTERY_STATE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    bool battery_connected() { return data8[IND_STATE_8] & BATTERY_CONNECTED_FLAG; }
    uint8_t state() { return data8[IND_STATE_8]; }
    uint16_t soc() { return data8[IND_SOC_8]; }
    uint16_t voltage() { return data16[IND_VOLTAGE_16]; }
    int16_t current() { return data16[IND_CURRENT_16]; }
    uint16_t temperature() { return data16[IND_TEMP_16]; }
};


}