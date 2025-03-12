#include "battery_node.h"

using namespace hebi::firmware::protocol;

Battery_Node::Battery_Node() {

}

void Battery_Node::update() {
    Message_Parser::update();
    
}

void Battery_Node::recvd_ctrl_set_node_addr(ctrl_set_node_id_msg msg){
    if(msg.EID.node_id != node_id_) return;

    node_id_ = msg.new_node_id();
    comms_enabled_ = true;
}