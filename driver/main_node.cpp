#include "main_node.h"

using namespace hebi::firmware::protocol;


Main_Node::Main_Node(){
    state_ = DriverState::ACQUIRE;
    count_ = 0;
}


child_node_info& Main_Node::getNodeFromID(node_id_t node_id) {
    if(node_id > max_node_id_seen_)
        max_node_id_seen_ = node_id;
    
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
    Base_Node::update();

    switch(state_){
    case DriverState::ACQUIRE:
        count_++;

        if((count_ % ACQUIRE_PERIOD_MS) == 0)
            addTxMessage(ctrl_set_node_id_msg(DEFAULT_NODE_ID, max_node_id_seen_ + 1));

        if((count_ % ACQUIRE_PERIOD_MS) == ACQUIRE_PERIOD_MS/2)
            addTxMessage(ctrl_poll_node_id_msg(NODE_ID));
        
        if(count_ >= ACQUIRE_TIME_MS){
            state_ = DriverState::NORMAL;
            count_ = 0;
        }
        break;
    case DriverState::NORMAL:
        count_++;
        count_ %= NORMAL_PERIOD_MS;

        //Periodically tell all nodes to start transmitting data
        if(count_ == 0)
            addTxMessage(cmd_start_data_msg(NODE_ID));
        
        break;
    default:
        break;
    }

}