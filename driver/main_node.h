/* Main_Node.h
    
*/

#pragma once

#include "base_node.h"
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
        ACQUIRE,
        NORMAL
    };

public:
    Main_Node();

    void update();

    child_node_info& getNodeFromID(node_id_t node_id);

    const std::map<node_id_t, child_node_info>& childNodes() { return child_nodes_; }

protected:
    void recvd_data_battery_state(battery_state_msg msg) override;
    
    std::map<node_id_t, child_node_info> child_nodes_;

    const uint8_t NODE_ID = 0; //Hardcoded node id to 0
    const uint16_t ACQUIRE_PERIOD_MS = 200; 
    const uint16_t ACQUIRE_TIME_MS = 2000; 
    const uint16_t NORMAL_PERIOD_MS = 1000; 

    DriverState state_ {DriverState::INIT};
    uint16_t count_ {0};
    uint8_t max_node_id_seen_ {0};
};

};