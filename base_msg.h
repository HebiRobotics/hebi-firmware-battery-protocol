/* base_msg.h
    Basic message structre for a simple protocol
*/

#pragma once

#include <stdint.h>
#include <cstring>


namespace hebi::firmware::protocol {

using node_id_t = uint8_t;
constexpr uint8_t DEFAULT_NODE_ID = 0xFF;

enum class MessageType {
    MSG_INVALID = 0x000,
    
    /* Highest Priority: Control messages */
    CTRL_SET_NODE_ID = 0x001,
    CTRL_POLL_NODE_ID = 0x002,
    CTRL_START_ACQUISITION = 0x003, //Tell nodes to enter acquisition mode
    CTRL_STOP_ACQUISITION = 0x004,  //Tell nodes to exit acquisition mode
    CTRL_RESET = 0x005,             //Tell a node to reset
    CTRL_SET_STAY_IN_BOOT = 0x006,  //Tell a node to stay in bootloader on next reset
    CTRL_BOOT = 0x007,              //Tell a node to boot (if the app is valid)

    //Info Messages
    CTRL_READ_INFO = 0x010,         //Request info from a node
    CTRL_GUID = 0x011,              //Response: STM32 UID
    CTRL_ELEC_TYPE = 0x012,         //Response: 8-character ASCII encoded Electrical Type
    CTRL_HW_TYPE = 0x013,           //Response: 8-character ASCII encoded Hardware Type
    CTRL_FW_VERSION = 0x014,        //Response: 8 bytes of FW version string, position determined by "index" field
    CTRL_FW_MODE = 0x015,           //Response: FW mode (boot or application)
    CTRL_APP_FW_HASH = 0x016,       //Response: 8 bytes of app FW hash, position determined by "index" field
    CTRL_BOOT_FW_HASH = 0x017,      //Response: 8 bytes of boot FW hash, position determined by "index" field
    CTRL_SERIAL_NUM = 0x018,        //Response: 8 bytes of serial number string, position determined by "index" field
    
    /* Command Messages */
    CMD_START_DATA = 0x100,         //TODO
    CMD_DISABLE_OUTPUT = 0x101,     //Takes precedence over enable output
    CMD_ENABLE_OUTPUT = 0x102,      
    CMD_SET_LED = 0x103,

    /* Data Messages */
    DATA_BATTERY_STATE = 0x200,
    DATA_BATTERY_STATE_EXT_1 = 0x201,
    DATA_BATTERY_STATE_EXT_2 = 0x202,
    
    /* Bootloader Messages */
    BOOT_SET_KEY = 0x301,           //Command: Set key for firmware decryption
    BOOT_PARTITION_LENGTH = 0x302,  //Request / Response: flash partition length
    BOOT_READ = 0x303,              //Request to read flash data from a node, max 2048 bytes at a time
    BOOT_READ_DATA = 0x304,         //Response: 8 bytes of requested data, position determined by "index"
    BOOT_READ_END = 0x305,
    BOOT_WRITE = 0x306,             //Start a write operation, max 2048 bytes at a time
    BOOT_WRITE_DATA = 0x307,        //Indexed data, 8 bytes
    BOOT_WRITE_END = 0x308,         //Command / Response: Finish write, respond with status
    BOOT_ERASE = 0x309,             //Command / Response: Erase a partition
    BOOT_SET_SERIAL_NUM = 0x30A,    //Command: Set the serial number string, indexed data
};

inline uint8_t node_id_from_eid(uint32_t EID){
    return ( EID & 0xFF );
}

inline uint8_t index_crc_from_eid(uint32_t EID){
    return ( (EID & (0xFF << 20)) >> 20 );
}

inline MessageType msg_type_from_eid(uint32_t EID){
    return static_cast<MessageType>((EID & (0xFFF << 8)) >> 8);
}

struct base_msg {
    union{
        uint32_t raw;
        struct {
            uint32_t node_id:8;     //Logical UID for this node
            uint32_t msg_type:12;   //Defined with "MessageType" enum
            uint32_t index:8;   //Byte index
            uint32_t rsvd:1;        //TODO: Assign these
            uint32_t invalid:3;     //Pad to 32 bits
        };
    } EID;
    uint8_t len; //Size of data
    union {
        uint8_t data8[8];
        uint16_t data16[4];
        uint32_t data32[2];
        uint64_t data64;
    };

    base_msg () { 
        EID.raw = 0;
        len = 0;
        data32[0] = 0;
        data32[1] = 0;
    }

    base_msg (uint8_t node_id_, MessageType msg_type_, uint8_t index = 0){
        EID.rsvd = 0;
        EID.node_id = node_id_;
        EID.msg_type = static_cast<uint16_t>(msg_type_);
        EID.index = index;
    }

    base_msg (uint8_t node_id_, MessageType msg_type_, uint8_t len_, const uint8_t data[8], uint8_t index = 0) :
        base_msg(node_id_, msg_type_, index){
        len = len_;
        
        //TODO: Make more efficient? MEMCPY?
        for(uint8_t i = 0; i < len; i++){
            data8[i] = data[i];
        }
    }

    base_msg (uint32_t EID_, uint8_t len_, const uint8_t data_[8]) { 
        EID.raw = EID_;
        len = len_;
        
        //TODO: Make more efficient? MEMCPY?
        for(uint8_t i = 0; i < len; i++){
            data8[i] = data_[i];
        }
    }
};

struct string_msg : public base_msg {
    static const uint8_t MSG_LEN_BYTES = 8;

    //String to raw data
    string_msg(uint8_t node_id, MessageType msg_type, 
        uint8_t index, const char* string, uint16_t str_len) :
        base_msg(node_id, msg_type, index) {
        len = MSG_LEN_BYTES;

        uint16_t string_index = (index * MSG_LEN_BYTES);

        if(string_index < str_len){
            // Copy MSG_LEN_BYTES of string, stopping at null terminator in string
            strncpy((char*) data8, &string[string_index], MSG_LEN_BYTES);
        }
        //else: data is default initialized to all zeros by base_msg constructor
    }

    //Raw data to struct
    string_msg(uint8_t node_id, MessageType msg_type, uint8_t index, uint8_t data[8]) :
        base_msg(node_id, msg_type, MSG_LEN_BYTES, data, index) {
        //Do Nothing
    }

    uint8_t index() { return EID.index; }
};

}

