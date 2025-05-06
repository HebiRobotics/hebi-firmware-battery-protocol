/*
 * @file StringMsgHelper.h
 *
 * @brief 
 * An implementation of a FIFO queue as a fixed-size lock-free ring buffer
 * This buffer has n-1 slots, where n is the size in memory
 * Based off of https://ferrous-systems.com/blog/lock-free-ring-buffer/
 *
 * @author Nick Paiva
 * @since 2024
 */

#pragma once

#include "base_msg.h"
#include <cstdint>
#include <array>
#include <optional>

namespace hebi::firmware::util {

template <uint16_t MAX_STRING_SIZE>
class StringMsgHelper {
    static constexpr uint16_t BUFFER_SIZE = MAX_STRING_SIZE + 1;
public:
    StringMsgHelper() { 
        clear();
    }

    /* clear: Zero out the buffer and state variables */
    void clear(){
        memset(buffer_, 0, BUFFER_SIZE);
        has_string_ = false;
        current_string_length_ = 0;
    }

    /* handleMessage: Parse a "string_msg" */
    bool handleMessage(protocol::string_msg& msg){
        static const uint8_t msg_len = protocol::string_msg::MSG_LEN_BYTES;
        uint16_t buffer_index = msg_len * msg.index();

        //Don't accept any messages past the max buffer size
        if(buffer_index >= BUFFER_SIZE) return false;

        //If this is the first packet, index will be zero.
        //Clear out the buffer
        if(msg.index() == 0) clear();

        //Copy at most "msg_len" bytes into the buffer
        uint16_t space_remaining = BUFFER_SIZE - buffer_index;
        space_remaining = (space_remaining > msg_len) ? msg_len : space_remaining;
        strncpy(&buffer_[buffer_index], (const char*) msg.data8, space_remaining);

        //Update current string length
        uint16_t string_length = strnlen((const char*) msg.data8, msg_len);
        current_string_length_ += string_length;

        //If this is the last packet, it will contain a null terminator
        //The message length will be less than the max length of the packet
        if(string_length != msg_len)
            has_string_ = true;

        //Successful parse
        return true;
    }

    /* hasString: Have we received a valid string */
    bool hasString() const {
        return has_string_;
    }

    const char* string() const {
        return buffer_;
    }

    uint16_t stringLength() const {
        if(has_string_)
            return current_string_length_;
        else
            return 0;
    }

    void load(bool success){
        has_string_ = false;
        if(success){
            current_string_length_ = strnlen(buffer_, BUFFER_SIZE);
            has_string_ = (current_string_length_ < BUFFER_SIZE);
        }
    }

    char buffer_[BUFFER_SIZE] {};
private:  
    uint16_t current_string_length_ {0};
    bool has_string_ {false};
};

} /* namespace hebi::firmware::util */