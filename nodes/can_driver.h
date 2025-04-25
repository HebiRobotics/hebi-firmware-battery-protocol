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
};

}; //namespace hebi::firmware::protocol