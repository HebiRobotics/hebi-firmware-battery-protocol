/*
 * @file LockFreeRingBuffer.h
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

#include <cstdint>
#include <array>
#include <optional>

namespace hebi::firmware::util {

template <typename T, uint16_t N_SLOTS>
class LF_RingBuffer {
    static constexpr uint16_t SIZE = N_SLOTS + 1;
public:
    LF_RingBuffer() { 
        clear();
    }

    /* Zero out the buffer 
    */
    void clear(){
        start_ = 0;
        end_ = 0;
        std::fill(buffer_.begin(), buffer_.end(), T());
    }

    /* Add a sample to the end of the buffer if there is space
    */
    void add(const T& data){
        uint16_t ind = end_ + 1;
        if(ind == SIZE) ind = 0;

        if(ind != start_){
            buffer_[end_] = data;
            end_ = ind;
        } 
    }

    /* Get the value of the nth value in the buffer without removing it
    */
    std::optional<T> peek(uint16_t ind){
        if(ind >= length())
            return {};
        
        uint16_t ind_ = (start_ + ind) % SIZE;
        return {buffer_[ind_]};
    }

    /* Get the value of the len - nth value in the buffer without removing it
    */
    std::optional<T> peekReverse(uint16_t ind){
        uint16_t length_ = length();
        if(ind >= length_)
            return {};
        
        return peek(length_ - ind - 1);
    }

    /* Take the first sample out of the buffer
    */
    std::optional<T> take() {
        if(start_ == end_) return {};
        
        uint16_t ind = start_;
        T sample_ = buffer_[ind];

        ind = ind + 1;
        if(ind == SIZE) ind = 0;

        start_ = ind;

        return {sample_};
    }

    /* Count the number of samples in the buffer */
    uint16_t length() {
        if(start_ == end_) return 0;
        else if (start_ < end_) return end_ - start_;
        else return SIZE - start_ + end_;
    }

    bool full(){
        return length() == SIZE - 1;
    }

private:  
    std::array<T, SIZE> buffer_;
    uint16_t start_ {0}; //Location of the first (least recent) value
    uint16_t end_ {0}; //Location of the last (most recent) value
};

} /* namespace hebi::firmware::util */