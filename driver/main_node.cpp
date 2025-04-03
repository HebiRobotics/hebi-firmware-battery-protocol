#include "main_node.h"

using namespace hebi::firmware::protocol;


Main_Node::Main_Node(protocol::CAN_driver& can_driver) : 
    can_driver_(can_driver) {
    state_ = DriverState::NORMAL;
    count_ = 0;
}


child_node_info& Main_Node::getNodeFromID(node_id_t node_id) {
    if(node_id > max_node_id_seen_)
        max_node_id_seen_ = node_id;
    
    return child_nodes_[node_id];
}

child_node_info& Main_Node::getNodeFromIDAndUpdate(node_id_t node_id) {
    child_node_info& node = getNodeFromID(node_id);
    node.t_last_update = t_last_update_;
    
    return node;
}

void Main_Node::recvd_data_battery_state(battery_state_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.voltage = (float) msg.voltage() / 1000.;
    node_info.current = (float) msg.current() / 1000.;
    node_info.capacity = (float) msg.capacity() / 1000.;
    node_info.capacity_max = (float) msg.capacity_max() / 1000.;
}

void Main_Node::startAcquire(bool should_clear_ids){
    if(state_ == DriverState::NORMAL){
        state_ = DriverState::ACQUIRE_POLL;

        if(should_clear_ids){
            child_nodes_.clear();
            max_node_id_seen_ = 0;
        }

        //Send a "start acquisition" message twice when we enter the acquisition state
        can_driver_.sendMessage(ctrl_start_acquisition_msg(NODE_ID, should_clear_ids));
        can_driver_.sendMessage(ctrl_start_acquisition_msg(NODE_ID, should_clear_ids));
    }
}

void Main_Node::stopAcquire(){
    if(state_ == DriverState::ACQUIRE_SET){
        state_ = DriverState::NORMAL;

        //Send a "stop acquisition" message twice when we leave the acquisition state
        can_driver_.sendMessage(ctrl_stop_acquisition_msg(NODE_ID));
        can_driver_.sendMessage(ctrl_stop_acquisition_msg(NODE_ID));
    }
}

void Main_Node::update(bool acquire_enable, bool clear_ids, uint64_t t_now){
    bool has_data = true;
    bool msg_recvd = false;
    t_last_update_ = t_now;

    while(has_data){
        auto msg_opt = can_driver_.getMessage();
        if(msg_opt.has_value()){
            msg_recvd |= tryParseMsg(msg_opt.value());
        } else {
            has_data = false;
        }

    }

    switch(state_){
    case DriverState::NORMAL:
        count_++;

        //Periodically tell all nodes to start transmitting data
        //Serves as a "keep alive" message
        if(count_ == NORMAL_PERIOD_MS){
            can_driver_.sendMessage(cmd_start_data_msg(NODE_ID));
            count_ = 0;
        }
        
        if(acquire_enable){
            startAcquire(clear_ids);
            count_ = 0;
        }
        
        break;
    case DriverState::ACQUIRE_POLL:
        count_++;

        if((count_ % ACQUIRE_POLL_PERIOD_MS) == 0)
            can_driver_.sendMessage(ctrl_poll_node_id_msg(NODE_ID));

        if(count_ >= ACQUIRE_POLL_TIME_MS){
            state_ = DriverState::ACQUIRE_SET;
            count_ = 0;
        }
        
        break;
    case DriverState::ACQUIRE_SET:
        count_++;

        if(!acquire_enable) {
            stopAcquire();
            count_ = 0;
        } else if(count_ == ACQUIRE_SET_PERIOD_MS){
            can_driver_.sendMessage(ctrl_set_node_id_msg(DEFAULT_NODE_ID, max_node_id_seen_ + 1));
            count_ = 0;
        }
        break;
    default:
        break;
    }

}