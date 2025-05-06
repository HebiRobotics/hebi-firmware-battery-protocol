/* Base_Node.h
    Common message parsing functions
*/

#pragma once

#include "all_msg.h"
#include "LockFreeRingBuffer.h"

namespace hebi::firmware::protocol {

class Base_Node {
public:
    Base_Node() { /* Do Nothing */ }

protected:
    bool tryParseMsg(base_msg& msg);

    virtual void recvd_ctrl_set_node_id(ctrl_set_node_id_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_start_acquisition(ctrl_start_acquisition_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_stop_acquisition(ctrl_stop_acquisition_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_reset(ctrl_reset_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_set_stay_in_boot(ctrl_set_stay_in_boot_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_boot(ctrl_boot_msg& msg) { (void) msg; }

    virtual void recvd_ctrl_read_info(ctrl_read_info_msg& msg) { (void) msg; }
    virtual void recvd_ctrl_guid(ctrl_guid_msg& msg) { (void) msg; }
    virtual void recvd_elec_type(ctrl_elec_type_msg& msg) { (void) msg; }
    virtual void recvd_hw_type(ctrl_hw_type_msg& msg) { (void) msg; }
    virtual void recvd_fw_version(ctrl_fw_version_msg& msg) { (void) msg; }
    virtual void recvd_fw_mode(ctrl_fw_mode_msg& msg) { (void) msg; }
    virtual void recvd_app_fw_hash(ctrl_app_fw_hash_msg& msg) { (void) msg; }
    virtual void recvd_boot_fw_hash(ctrl_boot_fw_hash_msg& msg) { (void) msg; }
    virtual void recvd_serial_num(ctrl_serial_num_msg& msg) { (void) msg; }
    virtual void recvd_hw_rev(ctrl_hw_rev_msg& msg) { (void) msg; }
    virtual void recvd_elec_rev(ctrl_elec_rev_msg& msg) { (void) msg; }

    virtual void recvd_boot_set_key(boot_set_key_msg& msg) { (void) msg; }
    virtual void recvd_boot_partition_length(boot_partition_length_msg& msg) { (void) msg; }
    virtual void recvd_boot_read(boot_read_msg& msg) { (void) msg; }
    virtual void recvd_boot_read_data(boot_read_data_msg& msg) { (void) msg; }
    virtual void recvd_boot_read_end(boot_read_end_msg& msg) { (void) msg; }
    virtual void recvd_boot_write(boot_write_msg& msg) { (void) msg; }
    virtual void recvd_boot_write_data(boot_write_data_msg& msg) { (void) msg; }
    virtual void recvd_boot_write_end(boot_write_end_msg& msg) { (void) msg; }
    virtual void recvd_boot_erase(boot_erase_msg& msg) { (void) msg; }
    virtual void recvd_boot_set_serial_num(boot_set_serial_num_msg& msg) { (void) msg; }

    virtual void recvd_cmd_start_data(cmd_start_data_msg& msg) { (void) msg; }
    virtual void recvd_cmd_set_led(cmd_set_led_msg& msg) { (void) msg; }
    virtual void recvd_cmd_disable_output(cmd_disable_output_msg& msg) { (void) msg; }
    virtual void recvd_cmd_enable_output(cmd_enable_output_msg& msg) { (void) msg; }

    virtual void recvd_data_battery_state(battery_state_msg& msg) { (void) msg; }
    virtual void recvd_data_battery_state_ext_1(battery_state_ext_1_msg& msg) { (void) msg; }
    virtual void recvd_data_battery_state_ext_2(battery_state_ext_2_msg& msg) { (void) msg; }
};

};