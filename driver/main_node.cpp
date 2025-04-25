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

    node_info.battery_state = msg.state();
    node_info.soc = (float) msg.soc(); //0 - 100 Int
    node_info.voltage = (float) msg.voltage() / 1000.; //Int mV to float V
    node_info.current = (float) msg.current() / 1000.; //Int mA to float A
    node_info.temperature = (float) (msg.temperature() / 10.) - 273.15; //Int 0.1K to float deg C
}

void Main_Node::recvd_data_battery_state_ext_1(battery_state_ext_1_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.avg_current = msg.avg_current() / 1000.; //Int mA to float A
    node_info.standby_current = (float) msg.standby_current() / 1000.; //Int mA to float A
    node_info.status_flags = msg.status_flags();
    node_info.avg_power = (float) msg.avg_power() / 10.; //Int mW to float W
}

void Main_Node::recvd_data_battery_state_ext_2(battery_state_ext_2_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.time_to_empty = msg.time_to_empty(); //Minutes
    node_info.time_to_full = msg.time_to_full(); //Minutes
    node_info.capacity_remaining = (float) msg.capacity_remaining() / 1000.; //Int mAh to float Ah
    node_info.capacity_full = (float) msg.capacity_full() / 1000.; //Int mAh to float Ah
}

void Main_Node::recvd_ctrl_guid(ctrl_guid_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    if(msg.index() == 0)
        node_info.guid64 = msg.guid();
}

void Main_Node::recvd_elec_type(ctrl_elec_type_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    for(uint8_t i = 0; i < msg.MSG_LEN_BYTES; i++)
        node_info.elec_type[i] = msg.data8[i];
}

void Main_Node::recvd_hw_type(ctrl_hw_type_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    for(uint8_t i = 0; i < msg.MSG_LEN_BYTES; i++)
        node_info.hw_type[i] = msg.data8[i];
}

void Main_Node::recvd_fw_version(ctrl_fw_version_msg msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    uint8_t max_index = ((child_node_info::FW_VERSION_SIZE) / msg.MSG_LEN_BYTES);

    if(msg.index() < max_index)
        for(uint8_t i = 0; i < msg.MSG_LEN_BYTES; i++)
            node_info.fw_version[i + (msg.index() * msg.MSG_LEN_BYTES)] = msg.data8[i];
    
    //TODO: "valid" indicator?
}

void Main_Node::recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg msg) {
    if(msg.EID.node_id != 0xFF)
        child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);
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

    uint8_t count = 0;
    float soc = 0.0;
    for(auto &pair : child_nodes_){
        auto &node = pair.second;
        if(!node.isStale(t_now)){ 
            if(node.isBatteryConnected()){
                soc += node.soc;
                count++;
            }

            //Request info if we haven't in a while
            if(node.needsInfo(t_now)){
                can_driver_.sendMessage(
                    ctrl_read_info_msg(pair.first, 
                        ctrl_read_info_msg::READ_GUID | 
                        ctrl_read_info_msg::READ_ELEC_TYPE | 
                        ctrl_read_info_msg::READ_HW_TYPE | 
                        ctrl_read_info_msg::READ_FW_VERSION
                    ));
                node.t_last_info = t_now;
            }
        }
    }

    if(count != 0)
        current_soc_ = soc / count;
    else
        current_soc_ = 0.0;


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
            can_driver_.sendMessage(ctrl_poll_node_id_msg(NODE_ID));
            can_driver_.sendMessage(ctrl_set_node_id_msg(DEFAULT_NODE_ID, max_node_id_seen_ + 1));
            count_ = 0;
        }
        break;
    default:
        break;
    }

}