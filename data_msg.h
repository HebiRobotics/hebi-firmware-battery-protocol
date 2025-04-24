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

struct battery_state_ext_1_msg : public base_msg {
    static const uint8_t IND_AVG_CURRENT = 0;
    static const uint8_t IND_STANDBY_CURRENT = 1;
    static const uint8_t IND_FLAGS = 2;
    static const uint8_t IND_AVG_POWER = 3;
    static const uint8_t MSG_LEN_BYTES = 8;
    
    //Struct to raw data
    battery_state_ext_1_msg(
        uint8_t node_id, 
        uint16_t avg_current,
        uint16_t standby_current,
        uint16_t status_flags, 
        uint16_t avg_power) :
        base_msg (node_id, MessageType::DATA_BATTERY_STATE_EXT_1) {
            len = MSG_LEN_BYTES;
            data16[IND_AVG_CURRENT] = avg_current;
            data16[IND_STANDBY_CURRENT] = standby_current;
            data16[IND_FLAGS] = status_flags;
            data16[IND_AVG_POWER] = avg_power;
    }

    //Raw data to struct
    battery_state_ext_1_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::DATA_BATTERY_STATE_EXT_1, MSG_LEN_BYTES, data) {
        //Do Nothing
    }
    
    int16_t avg_current() { return data16[IND_AVG_CURRENT]; }
    int16_t standby_current() { return data16[IND_STANDBY_CURRENT]; }
    uint16_t status_flags() { return data16[IND_FLAGS]; }
    int16_t avg_power() { return data16[IND_AVG_POWER]; }
};

struct battery_state_ext_2_msg : public base_msg {
    static const uint8_t IND_TIME_TO_EMPTY = 0;
    static const uint8_t IND_TIME_TO_FULL = 1;
    static const uint8_t IND_CAPACITY_REMAINING = 2;
    static const uint8_t IND_CAPACITY_FULL = 3;
    static const uint8_t MSG_LEN_BYTES = 8;
    
    //Struct to raw data
    battery_state_ext_2_msg(
        uint8_t node_id, 
        uint16_t time_to_empty,
        uint16_t time_to_full,
        uint16_t capacity_remaining, 
        uint16_t capacity_full) :
        base_msg (node_id, MessageType::DATA_BATTERY_STATE_EXT_2) {
            len = MSG_LEN_BYTES;
            data16[IND_TIME_TO_EMPTY] = time_to_empty;
            data16[IND_TIME_TO_FULL] = time_to_full;
            data16[IND_CAPACITY_REMAINING] = capacity_remaining;
            data16[IND_CAPACITY_FULL] = capacity_full;
    }

    //Raw data to struct
    battery_state_ext_2_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::DATA_BATTERY_STATE_EXT_2, MSG_LEN_BYTES, data) {
        //Do Nothing
    }
    
    uint16_t time_to_empty() { return data16[IND_TIME_TO_EMPTY]; }
    uint16_t time_to_full() { return data16[IND_TIME_TO_FULL]; }
    uint16_t capacity_remaining() { return data16[IND_CAPACITY_REMAINING]; }
    uint16_t capacity_full() { return data16[IND_CAPACITY_FULL]; }
};

}