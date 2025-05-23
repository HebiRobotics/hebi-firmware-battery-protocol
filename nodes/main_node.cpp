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

void Main_Node::recvd_data_battery_state(battery_state_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.battery_state = msg.state();
    node_info.soc = (float) msg.soc(); //0 - 100 Int
    node_info.voltage = (float) msg.voltage() / 1000.; //Int mV to float V
    node_info.current = (float) msg.current() / 1000.; //Int mA to float A
    node_info.temperature = (float) (msg.temperature() / 10.) - 273.15; //Int 0.1K to float deg C
}

void Main_Node::recvd_data_battery_state_ext_1(battery_state_ext_1_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.avg_current = msg.avg_current() / 1000.; //Int mA to float A
    node_info.standby_current = (float) msg.standby_current() / 1000.; //Int mA to float A
    node_info.status_flags = msg.status_flags();
    node_info.avg_power = (float) msg.avg_power() / 10.; //Int mW to float W
}

void Main_Node::recvd_data_battery_state_ext_2(battery_state_ext_2_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.time_to_empty = msg.time_to_empty(); //Minutes
    node_info.time_to_full = msg.time_to_full(); //Minutes
    node_info.capacity_remaining = (float) msg.capacity_remaining() / 1000.; //Int mAh to float Ah
    node_info.capacity_full = (float) msg.capacity_full() / 1000.; //Int mAh to float Ah
}

void Main_Node::recvd_guid(info_guid_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);
    
    node_info.uid48 = msg.guid();
}

void Main_Node::recvd_elec_type(info_elec_type_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.elec_type_handler_.handleMessage(msg);
}

void Main_Node::recvd_hw_type(info_hw_type_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.hw_type_handler_.handleMessage(msg);
}

void Main_Node::recvd_fw_version(info_fw_version_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.fw_version_handler_.handleMessage(msg);
}

void Main_Node::recvd_app_fw_hash(info_app_fw_hash_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.app_fw_hash_handler_.handleMessage(msg);

}

void Main_Node::recvd_serial_num(info_serial_num_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.serial_number_handler_.handleMessage(msg);
}

void Main_Node::recvd_hw_rev(info_hw_rev_msg& msg){
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.hw_rev_handler_.handleMessage(msg);
}

void Main_Node::recvd_elec_rev(info_elec_rev_msg& msg){
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.elec_rev_handler_.handleMessage(msg);
}

void Main_Node::recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg& msg) {
    if(msg.EID.node_id != 0xFF)
        getNodeFromIDAndUpdate(msg.EID.node_id);
}

void Main_Node::recvd_fw_mode(info_fw_mode_msg& msg) { 
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    node_info.is_bootloader_active = msg.is_bootloader();
}

void Main_Node::recvd_boot_partition_length(boot_partition_length_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    if(node_info.bootloader_action == boot_action_t::PARTITION_LENGTH){
        node_info.partition_length = msg.partition_length();
        node_info.last_action_result = status_t::OK;
        node_info.bootloader_has_result = true;
    }
}

void Main_Node::recvd_boot_read_data(boot_read_data_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    if(node_info.bootloader_action == boot_action_t::READ){
        uint8_t max_index = ((child_node_info::MAX_BOOTLOADER_READ_SIZE) / msg.MSG_LEN_BYTES);

        if(msg.index() < max_index){
            for(uint8_t i = 0; i < msg.MSG_LEN_BYTES; i++)
                node_info.read_buffer[i + (msg.index() * msg.MSG_LEN_BYTES)] = msg.data8[i];
            
            //Assume that the read is padded to a multiple of MSG_LEN_BYTES
            node_info.read_length += msg.MSG_LEN_BYTES;
        }
    }
}

void Main_Node::recvd_boot_read_end(boot_read_end_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);
    
    if(node_info.bootloader_action == boot_action_t::READ){
        if(msg.length() == node_info.read_length){
            node_info.last_action_result = msg.status();
        } else {
            node_info.last_action_result = status_t::ERROR;
        }
        node_info.bootloader_has_result = true;
    }
}

void Main_Node::recvd_boot_write_end(boot_write_end_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    if(node_info.bootloader_action == boot_action_t::WRITE){
        node_info.last_action_result = msg.status();
        node_info.bootloader_has_result = true;
    }
}

void Main_Node::recvd_boot_erase(boot_erase_msg& msg) {
    child_node_info& node_info = getNodeFromIDAndUpdate(msg.EID.node_id);

    if(node_info.bootloader_action == boot_action_t::ERASE){
        node_info.last_action_result = msg.status();
        node_info.bootloader_has_result = true;
    }
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

            //Request info if we haven't in a while and aren't in bootloader mode
            if(node.needsInfo(t_now) && node.bootloader_action == boot_action_t::NONE){
                can_driver_.sendMessage(
                    info_read_msg(pair.first, 
                        info_read_msg::READ_GUID | 
                        info_read_msg::READ_ELEC_TYPE | 
                        info_read_msg::READ_HW_TYPE | 
                        info_read_msg::READ_FW_VERSION | 
                        info_read_msg::READ_FW_MODE | 
                        info_read_msg::READ_APP_FW_HASH | 
                        info_read_msg::READ_SERIAL_NUM | 
                        info_read_msg::READ_HW_REV | 
                        info_read_msg::READ_ELEC_REV
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
            can_driver_.sendMessage(ctrl_poll_node_id_msg(NODE_ID));
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