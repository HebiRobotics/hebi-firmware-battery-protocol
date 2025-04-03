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
    float voltage {};
    float current {};
    float capacity {};
    float capacity_max {};
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

    void update(bool acquire_enable, bool clear_ids);

    child_node_info& getNodeFromID(node_id_t node_id);

    const std::map<node_id_t, child_node_info>& childNodes() { return child_nodes_; }


protected:
    void startAcquire(bool should_clear_ids);
    void stopAcquire();
    
    void recvd_data_battery_state(battery_state_msg msg) override;
    
    std::map<node_id_t, child_node_info> child_nodes_;

    const uint8_t NODE_ID = 0; //Hardcoded node id to 0
    const uint16_t ACQUIRE_POLL_TIME_MS = 500; 
    const uint16_t ACQUIRE_POLL_PERIOD_MS = 100; 
    const uint16_t ACQUIRE_SET_PERIOD_MS = 100; 
    const uint16_t NORMAL_PERIOD_MS = 1000; 

    DriverState state_ {DriverState::INIT};
    uint16_t count_ {0};
    uint8_t max_node_id_seen_ {0};

    protocol::CAN_driver& can_driver_;
};

};