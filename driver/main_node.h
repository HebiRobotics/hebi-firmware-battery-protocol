/* Main_Node.h
    
*/

#pragma once

#include "base_node.h"
#include "driver/can_driver.h"
#include <map>

namespace hebi::firmware::protocol {

struct child_node_info {
    uint64_t t_last_update {};

    //Battery data info
    uint8_t battery_state {};
    float voltage {};
    float current {};
    float soc {};
    float temperature {};
    float capacity {};
    float capacity_max {};

    bool isStale(uint64_t t_now) const {
        return (t_now - t_last_update) > T_STALE_MICROS;
    }

    bool isBatteryConnected() const {
        return (battery_state & battery_state_msg::BATTERY_CONNECTED_FLAG);
    }

    static constexpr uint64_t T_STALE_MICROS = 3000 * 1000; /* 3000ms */
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
    
    void recvd_data_battery_state(battery_state_msg msg) override;
    void recvd_ctrl_poll_node_id(ctrl_poll_node_id_msg msg) override;
    
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