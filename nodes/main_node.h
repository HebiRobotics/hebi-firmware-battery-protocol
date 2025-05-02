/* Main_Node.h
    
*/

#pragma once

#include "base_node.h"
#include "nodes/can_driver.h"
#include <map>

namespace hebi::firmware::protocol {

struct child_node_info {
    static constexpr uint8_t ELEC_TYPE_SIZE = 8 + 1; /* 8 char, 1 null */
    static constexpr uint8_t FW_VERSION_SIZE = 16 + 1; /* 16 char, 1 null */
    static constexpr uint64_t T_STALE_MICROS = 3000 * 1000; /* 3000ms */
    static constexpr uint64_t T_INFO_MICROS = 5 * 1000 * 1000; /* 5s */
    static constexpr uint32_t MAX_BOOTLOADER_READ_SIZE = 512;

    uint64_t t_last_update {};
    uint64_t t_last_info {};

    //Ctrl info
    uint64_t guid64 {};
    char elec_type[ELEC_TYPE_SIZE] {};
    char hw_type[ELEC_TYPE_SIZE] {};
    uint8_t fw_version[16] {};

    //Bootloader info
    bool is_bootloader_active { false }; //Is the node in bootloader mode
    boot_action_t bootloader_action { boot_action_t::NONE }; //Host state
    bool bootloader_has_result { false };
    status_t last_action_result { status_t::OK };
    uint32_t partition_length {0};
    uint32_t read_length {0};
    uint8_t read_buffer[MAX_BOOTLOADER_READ_SIZE];

    //Battery data info
    uint8_t battery_state {};
    float voltage {};
    float current {};
    float soc {};
    float temperature {};

    //battery_state_msg_ext_1
    float avg_current {0};
    float standby_current {0};
    uint16_t status_flags {0};
    float avg_power {0};

    //battery_state_msg_ext_2
    uint16_t time_to_empty {0};
    uint16_t time_to_full {0};
    float capacity_remaining {0};
    float capacity_full {0};

    bool needsInfo(uint64_t t_now) const {
        return (t_now - t_last_info) > T_INFO_MICROS;
    }

    bool isStale(uint64_t t_now) const {
        return (t_now - t_last_update) > T_STALE_MICROS;
    }

    bool isBatteryConnected() const {
        return (battery_state & battery_state_msg::BATTERY_CONNECTED_FLAG);
    }

    bool hasBootloaderResult() const {
        return bootloader_has_result;
    }

    status_t getBootloaderResult() {
        if(!bootloader_has_result) return status_t::ERROR;
        
        bootloader_has_result = false;
        return last_action_result;
    }

    bool bootloaderResultIsOk() {
        return getBootloaderResult() == status_t::OK;
    }
};

class Main_Node : public Base_Node {
    enum class DriverState {
        INIT = 0,
        NORMAL,
        ACQUIRE_POLL,   //Collect current ids
        ACQUIRE_SET     //Start setting ids
    };

public:
    Main_Node(protocol::CAN_driver& can_driver);

    void update(bool acquire_enable, bool clear_ids, uint64_t t_micros);

    child_node_info& getNodeFromID(node_id_t node_id);

    const std::map<node_id_t, child_node_info>& childNodes() { return child_nodes_; }
    float soc() { return current_soc_; }


protected:
    //Helper function that updates "t_last_update"
    child_node_info& getNodeFromIDAndUpdate(node_id_t node_id); 

    void startAcquire(bool should_clear_ids);
    void stopAcquire();
    
    void recvd_data_battery_state(battery_state_msg& msg) override;
    void recvd_data_battery_state_ext_1(battery_state_ext_1_msg& msg) override;
    void recvd_data_battery_state_ext_2(battery_state_ext_2_msg& msg) override;
    void recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg& msg) override;

    void recvd_boot_partition_length(boot_partition_length_msg& msg) override;
    void recvd_boot_read_data(boot_read_data_msg& msg) override;
    void recvd_boot_read_end(boot_read_end_msg& msg) override;
    void recvd_boot_write_end(boot_write_end_msg& msg) override;
    void recvd_boot_erase(boot_erase_msg& msg) override;
    
    void recvd_ctrl_guid(ctrl_guid_msg& msg) override;
    void recvd_elec_type(ctrl_elec_type_msg& msg) override;
    void recvd_hw_type(ctrl_hw_type_msg& msg) override;
    void recvd_fw_version(ctrl_fw_version_msg& msg) override;
    void recvd_fw_mode(ctrl_fw_mode_msg& msg) override;
    
    std::map<node_id_t, child_node_info> child_nodes_;

    const uint8_t NODE_ID = 0; //Hardcoded node id to 0
    const uint16_t ACQUIRE_POLL_TIME_MS = 500; 
    const uint16_t ACQUIRE_POLL_PERIOD_MS = 100; 
    const uint16_t ACQUIRE_SET_PERIOD_MS = 100; 
    const uint16_t NORMAL_PERIOD_MS = 1000; 

    uint64_t t_last_update_ {0};
    DriverState state_ {DriverState::INIT};
    uint16_t count_ {0};
    uint8_t max_node_id_seen_ {0};
    float current_soc_ {0.0};

    protocol::CAN_driver& can_driver_;
};

};