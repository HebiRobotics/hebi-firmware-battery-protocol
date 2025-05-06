#include "base_node.h"

using namespace hebi::firmware::protocol;

bool Base_Node::tryParseMsg(base_msg &msg){
    auto msg_type = msg_type_from_eid(msg.EID.raw);
    auto node_id = node_id_from_eid(msg.EID.raw);
    auto index = index_crc_from_eid(msg.EID.raw);
    
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
        case MessageType::CTRL_RESET: {
            if(msg.len != ctrl_reset_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_reset_msg(node_id);
            recvd_ctrl_reset(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CTRL_SET_STAY_IN_BOOT: {
            if(msg.len != ctrl_set_stay_in_boot_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_set_stay_in_boot_msg(node_id, msg.data8);
            recvd_ctrl_set_stay_in_boot(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::CTRL_BOOT: {
            if(msg.len != ctrl_boot_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_boot_msg(node_id);
            recvd_ctrl_boot(parsed); //Trigger event

            return true; //Successful
        }

        case MessageType::INFO_READ: {
            if(msg.len != ctrl_read_info_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_read_info_msg(node_id, msg.data8);
            recvd_ctrl_read_info(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_GUID: {
            if(msg.len != ctrl_guid_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_guid_msg(node_id, msg.data8);
            recvd_ctrl_guid(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_ELEC_TYPE: {
            if(msg.len != ctrl_elec_type_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_elec_type_msg(node_id, index, msg.data8);
            recvd_elec_type(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_HW_TYPE: {
            if(msg.len != ctrl_hw_type_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_hw_type_msg(node_id, index, msg.data8);
            recvd_hw_type(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_FW_VERSION: {
            if(msg.len != ctrl_fw_version_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_fw_version_msg(node_id, index, msg.data8);
            recvd_fw_version(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_FW_MODE: {
            if(msg.len != ctrl_fw_mode_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_fw_mode_msg(node_id, index, msg.data8);
            recvd_fw_mode(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_APP_FW_HASH: {
            if(msg.len != ctrl_app_fw_hash_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_app_fw_hash_msg(node_id, index, msg.data8);
            recvd_app_fw_hash(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_BOOT_FW_HASH: {
            if(msg.len != ctrl_boot_fw_hash_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_boot_fw_hash_msg(node_id, index, msg.data8);
            recvd_boot_fw_hash(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_SERIAL_NUM: {
            if(msg.len != ctrl_serial_num_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_serial_num_msg(node_id, index, msg.data8);
            recvd_serial_num(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_HW_REV: {
            if(msg.len != ctrl_hw_rev_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_hw_rev_msg(node_id, index, msg.data8);
            recvd_hw_rev(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::INFO_ELEC_REV: {
            if(msg.len != ctrl_elec_rev_msg::MSG_LEN_BYTES) return false;

            auto parsed = ctrl_elec_rev_msg(node_id, index, msg.data8);
            recvd_elec_rev(parsed); //Trigger event

            return true; //Successful
        }

        case MessageType::BOOT_SET_KEY: {
            if(msg.len != boot_set_key_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_set_key_msg(node_id, index, msg.data8);
            recvd_boot_set_key(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_PARTITION_LENGTH: {
            if(msg.len != boot_partition_length_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_partition_length_msg(node_id, msg.data8);
            recvd_boot_partition_length(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_READ: {
            if(msg.len != boot_read_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_read_msg(node_id, msg.data8);
            recvd_boot_read(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_READ_DATA: {
            if(msg.len != boot_read_data_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_read_data_msg(node_id, index, msg.data8);
            recvd_boot_read_data(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_READ_END: {
            if(msg.len != boot_read_end_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_read_end_msg(node_id, msg.data8);
            recvd_boot_read_end(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_WRITE: {
            if(msg.len != boot_write_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_write_msg(node_id, msg.data8);
            recvd_boot_write(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_WRITE_DATA: {
            if(msg.len != boot_write_data_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_write_data_msg(node_id, index, msg.data8);
            recvd_boot_write_data(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_WRITE_END: {
            if(msg.len != boot_write_end_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_write_end_msg(node_id, msg.data8);
            recvd_boot_write_end(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_ERASE: {
            if(msg.len != boot_erase_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_erase_msg(node_id, msg.data8);
            recvd_boot_erase(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_SET_SERIAL_NUM: {
            if(msg.len != boot_set_serial_num_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_set_serial_num_msg(node_id, index, msg.data8);
            recvd_boot_set_serial_num(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_SET_HW_TYPE: {
            if(msg.len != boot_set_hw_type_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_set_hw_type_msg(node_id, index, msg.data8);
            recvd_boot_set_hw_type(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_SET_HW_REV: {
            if(msg.len != boot_set_hw_rev_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_set_hw_rev_msg(node_id, index, msg.data8);
            recvd_boot_set_hw_rev(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::BOOT_SET_ELEC_REV: {
            if(msg.len != boot_set_elec_rev_msg::MSG_LEN_BYTES) return false;

            auto parsed = boot_set_elec_rev_msg(node_id, index, msg.data8);
            recvd_boot_set_elec_rev(parsed); //Trigger event

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
        case MessageType::DATA_BATTERY_STATE_EXT_1: {
            if(msg.len != battery_state_ext_1_msg::MSG_LEN_BYTES) return false;

            auto parsed = battery_state_ext_1_msg(node_id, msg.data8);
            recvd_data_battery_state_ext_1(parsed); //Trigger event

            return true; //Successful
        }
        case MessageType::DATA_BATTERY_STATE_EXT_2: {
            if(msg.len != battery_state_ext_2_msg::MSG_LEN_BYTES) return false;

            auto parsed = battery_state_ext_2_msg(node_id, msg.data8);
            recvd_data_battery_state_ext_2(parsed); //Trigger event

            return true; //Successful
        }
        default:
            //Invalid Message!
            return false;
    }
}