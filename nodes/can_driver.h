/* can_driver.h
    Basic can driver interface
*/

#pragma once
#include <optional>
#include "base_msg.h"
#include "LockFreeRingBuffer.h"

namespace hebi::firmware::protocol {

class CAN_driver {
public:
    virtual void sendMessage(base_msg msg) = 0;
    virtual std::optional<base_msg> getMessage() = 0;

    /* Helper function to split a string into smaller messages */
    void sendString(uint8_t node_id, MessageType msg_type, const char* string, uint16_t length){
        uint8_t n_packets = (length / string_msg::MSG_LEN_BYTES) + 1;

        for(uint8_t ind = 0; ind < n_packets; ind++){
            sendMessage(string_msg(
                node_id, msg_type,      // Message specific
                ind, string, length     // Generic
            ));
        }
    }
};

}; //namespace hebi::firmware::protocol