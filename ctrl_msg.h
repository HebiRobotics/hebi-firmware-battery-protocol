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


struct ctrl_poll_node_id_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 0;

    //Struct to raw data / raw data to struct
    ctrl_poll_node_id_msg(uint8_t node_id) :
        base_msg (node_id, MessageType::CTRL_POLL_NODE_ID) {
            len = MSG_LEN_BYTES;
    }
};

struct ctrl_read_info_msg : public base_msg {
    static const uint8_t IND_REQUEST = 0;
    static const uint8_t MSG_LEN_BYTES = 2;

    static const uint16_t READ_GUID         = 0x01 << 0;
    static const uint16_t READ_ELEC_TYPE    = 0x01 << 1;
    static const uint16_t READ_HW_TYPE      = 0x01 << 2;
    static const uint16_t READ_FW_VERSION   = 0x01 << 3;

    //Struct to raw data / raw data to struct
    ctrl_read_info_msg(uint8_t node_id, uint16_t requested_info) :
        base_msg (node_id, MessageType::CTRL_READ_INFO) {
            len = MSG_LEN_BYTES;
            data16[IND_REQUEST] = requested_info;
    }

    //Raw data to struct
    ctrl_read_info_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_READ_INFO, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    bool read_GUID() { return data16[IND_REQUEST] & READ_GUID; }
    bool read_elec_type() { return data16[IND_REQUEST] & READ_ELEC_TYPE; }
    bool read_HW_type() { return data16[IND_REQUEST] & READ_HW_TYPE; }
    bool read_FW_version() { return data16[IND_REQUEST] & READ_FW_VERSION; }
};

struct ctrl_guid_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //Struct to raw data
    ctrl_guid_msg(uint8_t node_id, uint8_t index, uint64_t guid) :
        base_msg (node_id, MessageType::CTRL_GUID, index) {
            len = MSG_LEN_BYTES;
            data64 = guid;
    }

    //Raw data to struct
    ctrl_guid_msg(uint8_t node_id, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_GUID, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint64_t guid() { return data64; }
    uint8_t index() { return EID.index_crc; }
};

struct ctrl_elec_type_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //String to raw data
    ctrl_elec_type_msg(uint8_t node_id, const char* string) :
        base_msg(node_id, MessageType::CTRL_ELEC_TYPE) {
        len = MSG_LEN_BYTES;

        //Copy string, stop at null terminator
        for(uint8_t i = 0; i < MSG_LEN_BYTES; i++){
            data8[i] = string[i];
            if(string[i] == 0x00) break;
        }
    }

    //Raw data to struct
    ctrl_elec_type_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_ELEC_TYPE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }
};

struct ctrl_hw_type_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //String to raw data
    ctrl_hw_type_msg(uint8_t node_id, const char* string) :
        base_msg(node_id, MessageType::CTRL_HW_TYPE) {
        len = MSG_LEN_BYTES;

        //Copy string, stop at null terminator
        for(uint8_t i = 0; i < MSG_LEN_BYTES; i++){
            data8[i] = string[i];
            if(string[i] == 0x00) break;
        }
    }

    //Raw data to struct
    ctrl_hw_type_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_HW_TYPE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }
};

struct ctrl_fw_version_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //String to raw data
    ctrl_fw_version_msg(uint8_t node_id, uint8_t index, const char* string) :
        base_msg(node_id, MessageType::CTRL_FW_VERSION, index) {
        len = MSG_LEN_BYTES;

        //Copy string, stop at null terminator
        for(uint8_t i = 0; i < MSG_LEN_BYTES; i++){
            data8[i] = string[i];
            if(string[i] == 0x00) break;
        }
    }

    //Raw data to struct
    ctrl_fw_version_msg(uint8_t node_id, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_FW_VERSION, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint8_t index() { return EID.index_crc; }
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