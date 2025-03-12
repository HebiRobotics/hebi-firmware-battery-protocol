/* Battery_Node.h
    
*/

#pragma once

#include "parser.h"
#include <map>

namespace hebi::firmware::protocol {

class Battery_Node : public Message_Parser {
public:
    Battery_Node();

    void update() override;
    
    void addTxMessage(base_msg msg) override {
        if(!comms_enabled_) return;

        Message_Parser::addTxMessage(msg);
    }

protected:
    void recvd_ctrl_set_node_addr(ctrl_set_node_id_msg msg) override;
    
    bool comms_enabled_ { false };
    uint8_t node_id_ { DEFAULT_NODE_ID };
    
    // uint8_t counter_ {0};
};

};