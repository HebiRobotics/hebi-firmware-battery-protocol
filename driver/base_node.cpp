#include "base_node.h"

using namespace hebi::firmware::protocol;

bool Base_Node::tryParseMsg(base_msg &msg){
    auto msg_type = msg_type_from_eid(msg.EID.raw);
    auto node_id = node_id_from_eid(msg.EID.raw);
    
    switch(msg_type){
        case MessageType::CTRL_SET_NODE_ID: {
            if(msg.len != ctrl_set_node_id_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_set_node_id_msg(node_id, msg.data8);
            recvd_ctrl_set_node_id(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CTRL_POLL_NODE_ID: {
            if(msg.len != ctrl_poll_node_id_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_poll_node_id_msg(node_id);
            recvd_ctrl_poll_node_id(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CTRL_START_ACQUISITION: {
            if(msg.len != ctrl_start_acquisition_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_start_acquisition_msg(node_id, msg.data8);
            recvd_ctrl_start_acquisition(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CTRL_STOP_ACQUISITION: {
            if(msg.len != ctrl_stop_acquisition_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_stop_acquisition_msg(node_id);
            recvd_ctrl_stop_acquisition(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CMD_SET_LED: {
            if(msg.len != cmd_set_led_msg::MSG_LEN_BYTES) return false;

            auto parsed = cmd_set_led_msg(node_id, msg.data8);
            recvd_cmd_set_led(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CMD_START_DATA: {
            if(msg.len != cmd_start_data_msg::MSG_LEN_BYTES) return false;

            auto parsed = cmd_start_data_msg(node_id);
            recvd_cmd_start_data(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CMD_DISABLE_OUTPUT: {
            if(msg.len != ctrl_set_node_id_msg::MSG_LEN_BYTES) return false;

            auto parsed = cmd_disable_output_msg(node_id);
            recvd_cmd_disable_output(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CMD_ENABLE_OUTPUT: {
            if(msg.len != ctrl_set_node_id_msg::MSG_LEN_BYTES) return false;

            auto parsed = cmd_enable_output_msg(node_id);
            recvd_cmd_enable_output(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::DATA_BATTERY_STATE: {
            if(msg.len != battery_state_msg::MSG_LEN_BYTES) return false;

            auto parsed = battery_state_msg(node_id, msg.data8);
            recvd_data_battery_state(parsed); //Trigger event

            return true; //Successful
        }
        default:
            //Invalid Message!
            return false;
    }
}