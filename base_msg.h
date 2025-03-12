/* base_msg.h
    Basic message structre for a simple protocol
*/

#pragma once

#include <stdint.h>


namespace hebi::firmware::protocol {

using node_id_t = uint8_t;

enum class MessageType {
    MSG_INVALID = 0x000,
    
    /* Highest Priority: Control messages */
    CTRL_SET_NODE_ADDR = 0x001,
    CTRL_READ_GUID = 0x002,
    CTRL_READ_FW_VERSION = 0x003,

    /* Command Messages */
    CMD_START_DATA = 0x100,
    CMD_ENABLE_OUTPUT = 0x101,
    CMD_DISABLE_OUTPUT = 0x102,

    /* Data Messages */
    DATA_BATTERY_STATE = 0x200,
};

inline uint8_t node_id_from_eid(uint32_t EID){
    return ( EID & 0xFF );
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
            uint32_t rsvd:9;        //TODO: Assign these
            uint32_t invalid:3;     //Pad to 32 bits
        };
    } EID;
    uint8_t len; //Size of data
    union {
        uint8_t data8[8];
        uint16_t data16[4];
        uint32_t data32[2];
    };

    base_msg () { 
        EID.raw = 0;
        len = 0;
        data32[0] = 0;
        data32[1] = 0;
    }

    base_msg (uint8_t node_id_, MessageType msg_type_){
        EID.rsvd = 0;
        EID.node_id = node_id_;
        EID.msg_type = static_cast<uint16_t>(msg_type_);
    }

    base_msg (uint8_t node_id_, MessageType msg_type_, uint8_t len_, uint8_t data[8]) :
        base_msg(node_id_, msg_type_){
        len = len_;
        
        //TODO: Make more efficient? MEMCPY?
        for(uint8_t i = 0; i < len; i++){
            data8[i] = data[i];
        }
    }

    base_msg (uint32_t EID_, uint8_t len_, uint8_t data_[8]) { 
        EID.raw = EID_;
        len = len_;
        
        //TODO: Make more efficient? MEMCPY?
        for(uint8_t i = 0; i < len; i++){
            data8[i] = data_[i];
        }
    }
};

}

