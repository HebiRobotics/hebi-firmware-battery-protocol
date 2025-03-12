#include "main_node.h"

using namespace hebi::firmware::protocol;


Main_Node::Main_Node(){

}


child_node_info& Main_Node::getNodeFromID(node_id_t node_id) {
    return child_nodes_[node_id];
}

void Main_Node::recvd_data_battery_state(battery_state_msg msg) {
    child_node_info& node_info = getNodeFromID(msg.EID.node_id);

    node_info.voltage = (float) msg.voltage() / 1000.;
    node_info.current = (float) msg.current() / 1000.;
    node_info.capacity = (float) msg.capacity() / 1000.;
    node_info.capacity_max = (float) msg.capacity_max() / 1000.;
}

void Main_Node::update(){
    Message_Parser::update();
}