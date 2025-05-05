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
        base_msg (node_id, MessageType::CTRL_SET_NODE_ID) {
            len = MSG_LEN_BYTES;
            data8[IND_STAY_IN_BOOT] = stay_in_boot;
    }

    //Raw data to struct
    ctrl_set_stay_in_boot_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_SET_NODE_ID, MSG_LEN_BYTES, data) {
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

struct ctrl_read_info_msg : public base_msg {
    static const uint8_t IND_REQUEST = 0;
    static const uint8_t MSG_LEN_BYTES = 2;

    static const uint16_t READ_GUID         = 0x01 << 0;
    static const uint16_t READ_ELEC_TYPE    = 0x01 << 1;
    static const uint16_t READ_HW_TYPE      = 0x01 << 2;
    static const uint16_t READ_FW_VERSION   = 0x01 << 3;
    static const uint16_t READ_FW_MODE      = 0x01 << 4;

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
    bool read_FW_mode() { return data16[IND_REQUEST] & READ_FW_MODE; }
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

struct ctrl_fw_mode_msg : public base_msg {
    static const uint8_t IND_MODE = 0;
    static const uint8_t FW_MODE_BOOT = 1;
    static const uint8_t FW_MODE_APP = 2;
    static const uint8_t MSG_LEN_BYTES = 1;

    //Struct to raw data
    ctrl_fw_mode_msg(uint8_t node_id, uint8_t mode) :
        base_msg (node_id, MessageType::CTRL_FW_MODE) {
            len = MSG_LEN_BYTES;
            data8[IND_MODE] = mode;
    }

    //Raw data to struct
    ctrl_fw_mode_msg(uint8_t node_id, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, MessageType::CTRL_FW_MODE, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    bool is_bootloader() { return data8[IND_MODE] == FW_MODE_BOOT; }
    bool is_application() { return data8[IND_MODE] == FW_MODE_APP; }
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