/* ctrl_msg.h
    Control message definitions
*/

#pragma once

#include "base_msg.h"

namespace hebi::firmware::protocol {

enum class partition_t {
    ALL = 0,
    APPLICATION = 1,
    DATABASE = 2,
    BOOTLOADER = 3,
};

enum class status_t {
    OK = 0,
    ERROR = 1,
    OUT_OF_RANGE = 2,
    FINAL_PACKET = 3,
};

enum class boot_action_t {
    NONE = 0,
    SET_KEY = 1,
    PARTITION_LENGTH = 2,
    READ = 3,
    WRITE = 4,
    ERASE = 5,
};

struct boot_set_key_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //Raw data to struct
    boot_set_key_msg(uint8_t node_id, uint8_t index, const uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_SET_KEY, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint8_t index() { return EID.index; }
};

struct boot_partition_length_msg : public base_msg {
    // length           : 4 bytes
    // partition        : 1 byte
    static const uint8_t IND_LENGTH_32 = 0;
    static const uint8_t IND_PARTITION_8 = 4;
    static const uint8_t MSG_LEN_BYTES = 5;

    //Struct to raw data
    boot_partition_length_msg(
        uint8_t node_id, 
        uint32_t partition_length,
        partition_t partition) :
        base_msg (node_id, MessageType::BOOT_PARTITION_LENGTH) {
            len = MSG_LEN_BYTES;
            data32[IND_LENGTH_32] = partition_length;
            data8[IND_PARTITION_8] = static_cast<uint8_t>(partition);
    }
    
    //Raw data to struct
    boot_partition_length_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_PARTITION_LENGTH, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint32_t partition_length() { return data32[IND_LENGTH_32]; }
    partition_t partition() { return static_cast<partition_t>(data8[IND_PARTITION_8]); }
};

struct boot_read_msg : public base_msg {
    // offset           : 4 bytes
    // length           : 2 bytes
    // partition        : 1 byte
    static const uint8_t IND_OFFSET_32 = 0;
    static const uint8_t IND_LENGTH_16 = 2;
    static const uint8_t IND_PARTITION_8 = 6;
    static const uint8_t MSG_LEN_BYTES = 7;

    //Struct to raw data
    boot_read_msg(
        uint8_t node_id, 
        uint32_t offset,
        uint16_t length,
        partition_t partition) :
        base_msg (node_id, MessageType::BOOT_READ) {
            len = MSG_LEN_BYTES;
            data32[IND_OFFSET_32] = offset;
            data16[IND_LENGTH_16] = length;
            data8[IND_PARTITION_8] = static_cast<uint8_t>(partition);
    }

    //Raw data to struct
    boot_read_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_READ, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint32_t offset() { return data32[IND_OFFSET_32]; }
    uint16_t length() { return data16[IND_LENGTH_16]; }
    partition_t partition() { return static_cast<partition_t>(data8[IND_PARTITION_8]); }
};

struct boot_read_data_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //Raw data to struct
    boot_read_data_msg(uint8_t node_id, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_READ_DATA, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint8_t index() { return EID.index; }
};

struct boot_read_end_msg : public base_msg {
    // offset           : 4 bytes
    // length           : 2 bytes
    // partition        : 1 byte
    // status           : 1 byte
    static const uint8_t IND_OFFSET_32 = 0;
    static const uint8_t IND_LENGTH_16 = 2;
    static const uint8_t IND_PARTITION_8 = 6;
    static const uint8_t IND_STATUS_8 = 7;
    static const uint8_t MSG_LEN_BYTES = 8;

    //Struct to raw data
    boot_read_end_msg(
        uint8_t node_id, 
        uint32_t offset,
        uint16_t length,
        partition_t partition,
        status_t status) :
        base_msg (node_id, MessageType::BOOT_READ_END) {
            len = MSG_LEN_BYTES;
            data32[IND_OFFSET_32] = offset;
            data16[IND_LENGTH_16] = length;
            data8[IND_PARTITION_8] = static_cast<uint8_t>(partition);
            data8[IND_STATUS_8] = static_cast<uint8_t>(status);
    }

    //Raw data to struct
    boot_read_end_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_READ_END, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint32_t offset() { return data16[IND_OFFSET_32]; }
    uint16_t length() { return data16[IND_LENGTH_16]; }
    partition_t partition() { return static_cast<partition_t>(data8[IND_PARTITION_8]); }
    status_t status() { return static_cast<status_t>(data8[IND_STATUS_8]); }
};

struct boot_write_msg : public base_msg {
    // length           : 2 bytes
    // sequence number  : 2 bytes
    // partition        : 1 byte
    static const uint8_t IND_LENGTH_16 = 0;
    static const uint8_t IND_SEQUENCE_16 = 1;
    static const uint8_t IND_PARTITION_8 = 4;
    static const uint8_t MSG_LEN_BYTES = 5;
    
    //Struct to raw data
    boot_write_msg(
        uint8_t node_id, 
        uint16_t length,
        uint16_t sequence_number,
        partition_t partition) :
        base_msg (node_id, MessageType::BOOT_WRITE) {
            len = MSG_LEN_BYTES;
            data16[IND_LENGTH_16] = length;
            data16[IND_SEQUENCE_16] = sequence_number;
            data8[IND_PARTITION_8] = static_cast<uint8_t>(partition);
    }

    //Raw data to struct
    boot_write_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_WRITE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint16_t length() { return data16[IND_LENGTH_16]; }
    uint16_t sequence_number() { return data16[IND_SEQUENCE_16]; }
    partition_t partition() { return static_cast<partition_t>(data8[IND_PARTITION_8]); }
};

struct boot_write_data_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //Raw data to struct
    boot_write_data_msg(uint8_t node_id, uint8_t index, const uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_WRITE_DATA, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint8_t index() { return EID.index; }
};

struct boot_write_end_msg : public base_msg {
    // length           : 2 bytes
    // sequence number  : 2 bytes
    // partition        : 1 byte
    // status           : 1 byte
    static const uint8_t IND_LENGTH_16 = 0;
    static const uint8_t IND_SEQUENCE_16 = 1;
    static const uint8_t IND_PARTITION_8 = 4;
    static const uint8_t IND_STATUS_8 = 5;
    static const uint8_t MSG_LEN_BYTES = 6;

    //Struct to raw data
    boot_write_end_msg(
        uint8_t node_id, 
        uint16_t length,
        uint16_t sequence_number,
        partition_t partition,
        status_t status) :
        base_msg (node_id, MessageType::BOOT_WRITE_END) {
            len = MSG_LEN_BYTES;
            data16[IND_LENGTH_16] = length;
            data16[IND_SEQUENCE_16] = sequence_number;
            data8[IND_PARTITION_8] = static_cast<uint8_t>(partition);
            data8[IND_STATUS_8] = static_cast<uint8_t>(status);
    }

    //Raw data to struct
    boot_write_end_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_WRITE_END, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    uint16_t length() { return data16[IND_LENGTH_16]; }
    uint16_t sequence_number() { return data16[IND_SEQUENCE_16]; }
    partition_t partition() { return static_cast<partition_t>(data8[IND_PARTITION_8]); }
    status_t status() { return static_cast<status_t>(data8[IND_STATUS_8]); }
};

struct boot_erase_msg : public base_msg {
    // partition        : 1 byte
    // status           : 1 byte
    static const uint8_t IND_PARTITION_8 = 0;
    static const uint8_t IND_STATUS_8 = 1;
    static const uint8_t MSG_LEN_BYTES = 2;

    //Struct to raw data
    boot_erase_msg(
        uint8_t node_id, 
        partition_t partition,
        status_t status) :
        base_msg (node_id, MessageType::BOOT_ERASE) {
            len = MSG_LEN_BYTES;
            data8[IND_PARTITION_8] = static_cast<uint8_t>(partition);
            data8[IND_STATUS_8] = static_cast<uint8_t>(status);
    }

    //Raw data to struct
    boot_erase_msg(uint8_t node_id, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_ERASE, MSG_LEN_BYTES, data) {
        //Do Nothing
    }

    partition_t partition() { return static_cast<partition_t>(data8[IND_PARTITION_8]); }
    status_t status() { return static_cast<status_t>(data8[IND_STATUS_8]); }
};

struct boot_set_serial_num_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //String to raw data
    boot_set_serial_num_msg(uint8_t node_id, uint8_t index, const char* string) :
        base_msg(node_id, MessageType::BOOT_SET_SERIAL_NUM, index) {
        len = MSG_LEN_BYTES;

        //Copy string, stop at null terminator
        for(uint8_t i = 0; i < MSG_LEN_BYTES; i++){
            data8[i] = string[i];
            if(string[i] == 0x00) break;
        }
    }

    //Raw data to struct
    boot_set_serial_num_msg(uint8_t node_id, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, MessageType::BOOT_SET_SERIAL_NUM, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint8_t index() { return EID.index; }
};

} //hebi::firmware::protocol