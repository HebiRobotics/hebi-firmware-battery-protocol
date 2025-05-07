/* info_msg.h
    Info message definitions
*/

#pragma once

#include "base_msg.h"

#include <stdint.h>

namespace hebi::firmware::protocol {

struct info_read_msg : public base_msg {
    static const uint8_t IND_REQUEST = 0;
    static const uint8_t MSG_LEN_BYTES = 2;

    static const uint16_t READ_GUID         = 0x01 << 0;
    static const uint16_t READ_ELEC_TYPE    = 0x01 << 1;
    static const uint16_t READ_HW_TYPE      = 0x01 << 2;
    static const uint16_t READ_FW_VERSION   = 0x01 << 3;
    static const uint16_t READ_FW_MODE      = 0x01 << 4;
    static const uint16_t READ_APP_FW_HASH  = 0x01 << 5;
    static const uint16_t READ_BOOT_FW_HASH = 0x01 << 6;
    static const uint16_t READ_SERIAL_NUM   = 0x01 << 7;
    static const uint16_t READ_HW_REV       = 0x01 << 8;
    static const uint16_t READ_ELEC_REV     = 0x01 << 9;

    //Struct to raw data / raw data to struct
    info_read_msg(uint8_t node_id, uint16_t requested_info) :
        base_msg (node_id, MessageType::INFO_READ) {
            len = MSG_LEN_BYTES;
            data16[IND_REQUEST] = requested_info;
    }

    //Raw data to struct
    info_read_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::INFO_READ, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    bool read_GUID() { return data16[IND_REQUEST] & READ_GUID; }
    bool read_elec_type() { return data16[IND_REQUEST] & READ_ELEC_TYPE; }
    bool read_HW_type() { return data16[IND_REQUEST] & READ_HW_TYPE; }
    bool read_FW_version() { return data16[IND_REQUEST] & READ_FW_VERSION; }
    bool read_FW_mode() { return data16[IND_REQUEST] & READ_FW_MODE; }
    bool read_APP_FW_hash() { return data16[IND_REQUEST] & READ_APP_FW_HASH; }
    bool read_BOOT_FW_hash() { return data16[IND_REQUEST] & READ_BOOT_FW_HASH; }
    bool read_serial_number() { return data16[IND_REQUEST] & READ_SERIAL_NUM; }
    bool read_HW_revision() { return data16[IND_REQUEST] & READ_HW_REV; }
    bool read_ELEC_revision() { return data16[IND_REQUEST] & READ_ELEC_REV; }
};

struct info_guid_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //Struct to raw data
    info_guid_msg(uint8_t node_id, uint64_t guid) :
        base_msg (node_id, MessageType::INFO_GUID) {
            len = MSG_LEN_BYTES;
            data64 = guid;
    }

    //Raw data to struct
    info_guid_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::INFO_GUID, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint64_t guid() { return data64; }
};

struct info_fw_mode_msg : public base_msg {
    static const uint8_t IND_MODE = 0;
    static const uint8_t FW_MODE_BOOT = 1;
    static const uint8_t FW_MODE_APP = 2;
    static const uint8_t MSG_LEN_BYTES = 1;

    //Struct to raw data
    info_fw_mode_msg(uint8_t node_id, uint8_t mode) :
        base_msg (node_id, MessageType::INFO_FW_MODE) {
            len = MSG_LEN_BYTES;
            data8[IND_MODE] = mode;
    }

    //Raw data to struct
    info_fw_mode_msg(uint8_t node_id, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, MessageType::INFO_FW_MODE, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    bool is_bootloader() { return data8[IND_MODE] == FW_MODE_BOOT; }
    bool is_application() { return data8[IND_MODE] == FW_MODE_APP; }
};

/* Message types that use the generic string_msg interface */
using info_elec_type_msg    = string_msg_template<MessageType::INFO_ELEC_TYPE>;
using info_hw_type_msg      = string_msg_template<MessageType::INFO_HW_TYPE>;
using info_fw_version_msg   = string_msg_template<MessageType::INFO_FW_VERSION>;
using info_app_fw_hash_msg  = string_msg_template<MessageType::INFO_APP_FW_HASH>;
using info_boot_fw_hash_msg = string_msg_template<MessageType::INFO_BOOT_FW_HASH>;
using info_serial_num_msg   = string_msg_template<MessageType::INFO_SERIAL_NUM>;
using info_hw_rev_msg       = string_msg_template<MessageType::INFO_HW_REV>;
using info_elec_rev_msg     = string_msg_template<MessageType::INFO_ELEC_REV>;

} //hebi::firmware::protocol